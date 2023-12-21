#define STB_IMAGE_IMPLEMENTATION

#include <cassert>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <optick.h>
#include <glad/glad.h>
#include <SDL.h>
#include <stb_image.h>
#include <entities/entitymanager.h>
#include <player/playermanager.h>
#include <asset/assetmanager.h>
#include <asset/model.h>
#include <components/model.h>
#include <components/transform.h>
#include <components/camera.h>
#include <render/gl.h>

namespace Render {
    RendererGL* RendererGL::create_instance() {
        assert(instance == nullptr);
        instance = new RendererGL;
        return instance;
    }

    void RendererGL::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    RendererGL* RendererGL::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    void RendererGL::init() {
        // acquire OpenGL function pointers
        gladLoadGLLoader(SDL_GL_GetProcAddress);
        defaultShader = new ShaderGL("../shaders/default.vert", "../shaders/default.frag");

        // set up debug context if enabled
        int flags;
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
        if (flags & GL_CONTEXT_FLAG_DEBUG_BIT) {
            glEnable(GL_DEBUG_OUTPUT);
            glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
            glDebugMessageCallback(debug_output, nullptr);
            glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
        }

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void RendererGL::cleanup() {
        // do cleanup tasks
    }

    void RendererGL::resize_viewport(int width, int height) {
        glViewport(0, 0, width, height);
    }

    void RendererGL::tick() {
        OPTICK_EVENT();
        // clear viewport
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);

        // get view and projection matrices
        glm::mat4 view = Player::PlayerManager::get_instance()->get_player_camera()->get_view_matrix();
        glm::mat4 proj = Player::PlayerManager::get_instance()->get_player_camera()->get_projection_matrix();

        // iterate all entities in the scene
        // TODO: this is probably piss slow
        std::vector<uint32_t> entityIds = Entity::EntityManager::get_instance()->get_active_ids();
        for(auto id : entityIds) {
            Entity::Entity* ent = Entity::EntityManager::get_instance()->get_entity(id);
            if(ent->get_component<Component::Model>()) {
                uint32_t modelId = ent->get_component<Component::Model>()->get_asset();
                auto* modelAsset = Asset::AssetManager::get_instance()->get_asset<Asset::Model>(modelId);
                for(auto &mesh : modelAsset->meshes) {
                    // check if api-specific objects created
                    generate_mesh_objects(mesh);
                    generate_texture_objects(mesh);

                    // TODO: if the gfx API ever switches mid-execution you will not see the light of heaven
                    auto* meshObjects = (MeshObjectsGL*)mesh->apiObjects;
                    // shader binds
                    defaultShader->bind();
                    if(!mesh->textures.empty()) {
                        glActiveTexture(GL_TEXTURE0);
                        defaultShader->set_int("texture_diffuse", 0);
                        auto* textureObjects = (TextureObjectsGL*)mesh->textures[0]->apiObjects;
                        glBindTexture(GL_TEXTURE_2D, textureObjects->id);
                    }
                    // get model matrix and calculate MVP matrix
                    glm::mat4 model = ent->get_component<Component::Transform>()->get_matrix();
                    defaultShader->set_mat4("matrix_mvp", proj * view * model);

                    // draw
                    glBindVertexArray(meshObjects->VAO);
                    glDrawElements(GL_TRIANGLES, mesh->indices.size(), GL_UNSIGNED_INT, nullptr);
                    glBindVertexArray(0);
                }
            }
        }
    }

    void RendererGL::generate_mesh_objects(Core::Mesh* mesh) {
        if(!mesh->apiObjects) {
            auto *objects = (MeshObjectsGL *) malloc(sizeof(MeshObjectsGL));

            // generate IDs
            glGenVertexArrays(1, &objects->VAO);
            glGenBuffers(1, &objects->VBO);
            glGenBuffers(1, &objects->EBO);

            // copy vertices
            glBindVertexArray(objects->VAO);
            glBindBuffer(GL_ARRAY_BUFFER, objects->VBO);
            glBufferData(GL_ARRAY_BUFFER, mesh->vertices.size() * sizeof(Core::Vertex), &mesh->vertices[0],
                         GL_STATIC_DRAW);

            // copy indices
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, objects->EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->indices.size() * sizeof(unsigned int),
                         &mesh->indices[0], GL_STATIC_DRAW);

            // set up attributes
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex), (void *) 0);
            // normals
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex),
                                  (void *) offsetof(Core::Vertex, normal));
            // uvs
            glEnableVertexAttribArray(2);
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex),
                                  (void *) offsetof(Core::Vertex, uv));
            // tangents
            glEnableVertexAttribArray(3);
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex),
                                  (void *) offsetof(Core::Vertex, tangent));
            // bitangents
            glEnableVertexAttribArray(4);
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Core::Vertex),
                                  (void *) offsetof(Core::Vertex, bitangent));

            glBindVertexArray(0);
            mesh->apiObjects = (void*)objects;
        }
    }

    void RendererGL::generate_texture_objects(Core::Mesh *mesh) {
        for(Asset::Texture* texture : mesh->textures) {
            if(!texture->apiObjects) {
                // load image data
                int texWidth, texHeight, texChannels;
                stbi_uc *pixels = stbi_load(texture->filePath.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
                if (!pixels) {
                    std::cout << "Failed to load texture " << texture->filePath.c_str() << std::endl;
                    return;
                }

                // generate and set params
                auto* objects = (TextureObjectsGL*)malloc(sizeof(TextureObjectsGL));
                GLenum format = GL_RGBA;
                glGenTextures(1, &objects->id);
                glBindTexture(GL_TEXTURE_2D, objects->id);
                glTexImage2D(GL_TEXTURE_2D, 0, (GLint) format, texWidth, texHeight, 0, format, GL_UNSIGNED_BYTE, pixels);
                glGenerateMipmap(GL_TEXTURE_2D);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16.0f);

                stbi_image_free(pixels);
                texture->apiObjects = (void*)objects;
            }
        }
    }

    void debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                                  const char *message, const void *userParam) {
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;
        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " << message << std::endl;

        switch (source) {
            case GL_DEBUG_SOURCE_API:
                std::cout << "Source: API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                std::cout << "Source: Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                std::cout << "Source: Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                std::cout << "Source: Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                std::cout << "Source: Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                std::cout << "Source: Other";
                break;
            default:
                std::cout << "Source: Unknown";
                break;
        }
        std::cout << std::endl;

        switch (type) {
            case GL_DEBUG_TYPE_ERROR:
                std::cout << "Type: Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cout << "Type: Deprecated Behaviour";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cout << "Type: Undefined Behaviour";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cout << "Type: Portability";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                std::cout << "Type: Performance";
                break;
            case GL_DEBUG_TYPE_MARKER:
                std::cout << "Type: Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                std::cout << "Type: Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                std::cout << "Type: Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cout << "Type: Other";
                break;
            default:
                std::cout << "Type: Unknown";
                break;
        }
        std::cout << std::endl;

        switch (severity) {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cout << "Severity: high";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cout << "Severity: medium";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                std::cout << "Severity: low";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cout << "Severity: notification";
                break;
            default:
                std::cout << "Severity: unknown";
                break;
        }
        std::cout << std::endl;
        std::cout << "---------------" << std::endl;
    }

    ShaderGL::ShaderGL(const std::string &vertPath, const std::string &fragPath, const std::string &geomPath) {
        uint32_t vertexShader = 0;
        uint32_t fragShader = 0;
        uint32_t geomShader = 0;
        if (!load_shader_file(vertPath, vertexShader, GL_VERTEX_SHADER)) {
            std::cout << "Failed to load vertex shader" << std::endl;
        }
        if (!load_shader_file(fragPath, fragShader, GL_FRAGMENT_SHADER)) {
            std::cout << "Failed to load fragment shader" << std::endl;
        }
        if (!geomPath.empty()) {
            if (!load_shader_file(geomPath, geomShader, GL_GEOMETRY_SHADER)) {
                std::cout << "Failed to load geometry shader" << std::endl;
            }
        }

        // link the shaders
        programID = glCreateProgram();
        glAttachShader(programID, vertexShader);
        glAttachShader(programID, fragShader);
        if (!geomPath.empty()) {
            glAttachShader(programID, geomShader);
        }
        glLinkProgram(programID);

        // check
        int result = 0;
        glGetProgramiv(programID, GL_LINK_STATUS, &result);
        if (!result) {
            char infoLog[512];
            glGetProgramInfoLog(programID, 512, nullptr, infoLog);
            std::cout << "Shader linking failed\n" << infoLog << std::endl;
        }

        // delete once final program is linked
        glDeleteShader(vertexShader);
        glDeleteShader(fragShader);
        if (!geomPath.empty()) {
            glDeleteShader(geomShader);
        }
    }

    void ShaderGL::bind() const {
        glUseProgram(programID);
    }

    void ShaderGL::set_bool(const std::string &name, bool value) const {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), (int) value);
    }

    void ShaderGL::set_int(const std::string &name, int value) const {
        glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
    }

    void ShaderGL::set_float(const std::string &name, float value) const {
        glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
    }

    void ShaderGL::set_mat4(const std::string &name, const glm::mat4 &mat) const {
        glUniformMatrix4fv(glGetUniformLocation(programID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }

    void ShaderGL::set_glm_vec3(const std::string &name, const glm::vec3 &value) const {
        glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
    }

    void ShaderGL::set_float_vec3(const std::string &name, float x, float y, float z) const {
        glUniform3f(glGetUniformLocation(programID, name.c_str()), x, y, z);
    }

    bool ShaderGL::load_shader_file(const std::string &filePath, uint32_t &id, uint32_t type) {
        // load shader source from file
        std::ifstream shaderFile;
        shaderFile.open(filePath);
        if(!shaderFile) {
            std::cout << "Failed to load shader source" << std::endl;
            return false;
        }

        // read in shader source
        std::string shaderCode;
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();

        // convert to char for compiling
        shaderCode = shaderStream.str();
        const char* shaderCodeChar = shaderCode.c_str();

        // compile
        id = glCreateShader(type);
        glShaderSource(id, 1, &shaderCodeChar, nullptr);
        glCompileShader(id);

        // check
        int result = 0;
        glGetShaderiv(id, GL_COMPILE_STATUS, &result);
        if (!result) {
            char infoLog[512];
            glGetShaderInfoLog(id, 512, nullptr, infoLog);
            std::cout << "Shader loading failed\n" << infoLog << std::endl;
        }

        return result;
    }
}