#include <cassert>
#include <vector>
#include <glad/glad.h>
#include <SDL.h>
#include <entities/entitymanager.h>
#include <components/mesh.h>
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
    }

    void RendererGL::cleanup() {
        // do cleanup tasks
    }

    void RendererGL::resize_viewport(int width, int height) {
        glViewport(0, 0, width, height);
    }

    void RendererGL::tick() {
        // clear viewport
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        // check which entities have mesh components
        // TODO: this is probably piss slow
        std::vector<uint32_t> entityIds = Entity::EntityManager::get_instance()->get_active_ids();
        for(auto id : entityIds) {
            Entity::Entity* ent = Entity::EntityManager::get_instance()->get_entity(id);
            if(ent->get_component<Component::Mesh>()) {
                // check if api-specific objects created
                // draw mesh
            }
        }
    }
}