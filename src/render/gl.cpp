#include <glad/glad.h>
#include <SDL.h>
#include <render/gl.h>

namespace Render::GL {
    // acquire OpenGL function pointers
    void get_function_pointers() {
        gladLoadGLLoader(SDL_GL_GetProcAddress);
    }

    // update the viewport and clear buffers
    void viewport_clear(int width, int height) {
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}