#pragma once

#include <glad/glad.h>
#include <string>
#include <glm/glm.hpp>
#include <render/renderer.h>

namespace Render {
    class ShaderGL {
    public:
        unsigned int programID;

        ShaderGL(const std::string &vertPath, const std::string &fragPath, const std::string &geomPath = "");

        void bind() const;
        void set_bool(const std::string &name, bool value) const;
        void set_int(const std::string &name, int value) const;
        void set_float(const std::string &name, float value) const;
        void set_mat4(const std::string &name, const glm::mat4 &mat) const;
        void set_glm_vec3(const std::string &name, const glm::vec3 &value) const;
        void set_float_vec3(const std::string &name, float x, float y, float z) const;
    private:
        static bool load_shader_file(const std::string &filePath, uint32_t &id, uint32_t type);
    };

    class RendererGL : public IRenderer {
    public:
        // create the RendererGL singleton
        static RendererGL* create_instance();
        // destroy the RendererGL singleton
        static void destroy_instance();
        // return the RendererGL singleton
        static RendererGL* get_instance();

        // initialize the renderer
        void init();
        // clean up the renderer
        void cleanup();

        // update the viewport
        void resize_viewport(int width, int height);
        void tick();
    private:
        inline static RendererGL* instance;

        ShaderGL* defaultShader;
    };

    void APIENTRY debug_output(GLenum source, GLenum type, unsigned int id, GLenum severity, GLsizei length,
                               const char *message, const void *userParam);

    struct MeshObjectsGL {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
    };
}