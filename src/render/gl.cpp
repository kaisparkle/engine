#include <cassert>
#include <glad/glad.h>
#include <SDL.h>
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

    void RendererGL::viewport_clear(int width, int height) {
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}