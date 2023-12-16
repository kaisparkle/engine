#include <cassert>
#include <optick.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <core/window.h>
#include <ui/imgui.h>

namespace UI {
    // create the IMGUI singleton
    IMGUI* IMGUI::create_instance() {
        assert(instance == nullptr);
        instance = new IMGUI;
        return instance;
    }

    // destroy the IMGUI singleton
    void IMGUI::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    // return the IMGUI singleton
    IMGUI* IMGUI::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    // initialize IMGUI
    void IMGUI::init() {
        // set up dear imgui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        // initialize dear imgui
        ImGui_ImplSDL2_InitForOpenGL(Core::Window::get_instance()->get_handle(), Core::Window::get_instance()->get_glcontext());
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    // clean up IMGUI
    void IMGUI::cleanup() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    // process SDL2 events
    void IMGUI::process_event(SDL_Event* event) {
        OPTICK_EVENT();
        ImGui_ImplSDL2_ProcessEvent(event);
    }

    // draw the next frame
    void IMGUI::tick() {
        OPTICK_EVENT();
        // start frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // show the demo window for now
        ImGui::ShowDemoWindow();

        // render the frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}