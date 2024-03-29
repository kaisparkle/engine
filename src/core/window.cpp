#include <cassert>
#include <optick.h>
#include <SDL.h>
#include <ui/editor.h>
#include <core/engine.h>
#include <player/playermanager.h>
#include <core/window.h>

namespace Core {
    // create the Window singleton
    Window* Window::create_instance() {
        assert(instance == nullptr);
        instance = new Window;
        return instance;
    }

    // destroy the Window singleton
    void Window::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    // return the Window singleton
    Window* Window::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    // initialize the Window
    void Window::init() {
        assert(handle == nullptr);

        // initialize SDL and subsystems
        if(SDL_Init(SDL_INIT_VIDEO) != 0) {
            printf("failed to init SDL");
            exit(EXIT_FAILURE);
        }

        // set our flags
        // TODO: should load defaults from a config
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        auto flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);

        // create our window and GL context
        handle = SDL_CreateWindow("engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_width, window_height, flags);
        assert(handle != nullptr);
        glContext = SDL_GL_CreateContext(handle);
        assert(glContext != nullptr);

        // associate the context and window, and enable vsync
        SDL_GL_MakeCurrent(handle, glContext);
        SDL_GL_SetSwapInterval(1);

        SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
        SDL_SetRelativeMouseMode(SDL_TRUE);
    }

    // clean up the Window
    void Window::cleanup() {
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(handle);
        SDL_Quit();
    }

    // return the SDL_Window handle
    SDL_Window *Window::get_handle() {
        return handle;
    }

    // return the SDL_GLContext
    SDL_GLContext Window::get_glcontext() {
        return glContext;
    }

    // return current window width
    int Window::get_window_width() {
        return window_width;
    }

    // return current window height
    int Window::get_window_height() {
        return window_height;
    }

    // poll SDL events
    void Window::poll_events() {
        OPTICK_EVENT();
        // disable relative mouse if using UI currently
        if(enableUI) {
            SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "0", SDL_HINT_OVERRIDE);
            SDL_SetRelativeMouseMode(SDL_FALSE);
        } else {
            SDL_SetHintWithPriority(SDL_HINT_MOUSE_RELATIVE_MODE_WARP, "1", SDL_HINT_OVERRIDE);
            SDL_SetRelativeMouseMode(SDL_TRUE);
        }

        SDL_Event event;
        while(SDL_PollEvent(&event)) {
            if(enableUI) {
                UI::Editor::get_instance()->process_event(&event);
            }
            switch(event.type) {
                case SDL_QUIT:
                    Core::Engine::get_instance()->stop();
                    break;
                case SDL_MOUSEMOTION:
                    if(!enableUI) {
                        Player::PlayerManager::get_instance()->get_player_controller()->process_mouse(
                                event.motion.xrel, event.motion.yrel);
                    }
                    break;
                case SDL_KEYDOWN:
                        if(event.key.keysym.sym == SDLK_TAB) enableUI = !enableUI;
                    break;
            }
        }
        if(!enableUI) {
            Player::PlayerManager::get_instance()->get_player_controller()->process_keyboard(
                    DeltaTime::get_instance()->get_delta_time(), const_cast<uint8_t *>(SDL_GetKeyboardState(nullptr)));
        }
    }

    // swap the buffers
    void Window::swap_buffers() {
        OPTICK_EVENT();
        SDL_GL_SwapWindow(handle);
    }
}