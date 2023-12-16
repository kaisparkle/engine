#include <cassert>
#include <optick.h>
#include <imgui.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <core/window.h>
#include <core/deltatime.h>
#include <tools/scrollingbuffer.h>
#include <entities/entitymanager.h>
#include <components/transform.h>
#include <ui/editor.h>

namespace UI {
    Editor* Editor::create_instance() {
        assert(instance == nullptr);
        instance = new Editor;
        return instance;
    }

    void Editor::destroy_instance() {
        assert(instance != nullptr);
        delete instance;
        instance = nullptr;
    }

    Editor* Editor::get_instance() {
        assert(instance != nullptr);
        return instance;
    }

    void Editor::init() {
        // set up dear imgui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGui::StyleColorsDark();

        // initialize dear imgui
        ImGui_ImplSDL2_InitForOpenGL(Core::Window::get_instance()->get_handle(), Core::Window::get_instance()->get_glcontext());
        ImGui_ImplOpenGL3_Init("#version 450");
    }

    void Editor::cleanup() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }

    void Editor::process_event(SDL_Event* event) {
        OPTICK_EVENT();
        ImGui_ImplSDL2_ProcessEvent(event);
    }

    void Editor::tick() {
        OPTICK_EVENT();

        // start frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        frametime_plot();
        scene_pane();

        // render the frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Editor::frametime_plot() {
        static ScrollingBuffer sdata;
        static float t = 0;
        t += ImGui::GetIO().DeltaTime;
        sdata.AddPoint(t, Core::DeltaTime::get_instance()->get_delta_time());
        static float history = 5.0f;

        ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize |
                                       ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing |
                                       ImGuiWindowFlags_NoNav;
        ImVec2 frametimeWindowPos = {0.0f + 10.0f, 0.0f + 10.0f};
        ImGui::SetNextWindowPos(frametimeWindowPos);
        ImGui::Begin("Frametime Plot", nullptr, windowFlags);
        ImGui::PushItemWidth(500);
        ImGui::SliderFloat("##History", &history, 1, 15, "%.1f s");

        if (ImPlot::BeginPlot("##Frametime Plot", ImVec2(500, 150))) {
            ImPlot::SetupAxes(nullptr, nullptr);
            ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 5, 15);
            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
            ImPlot::PlotLine("Frametime (ms)", &sdata.Data[0].x, &sdata.Data[0].y, sdata.Data.size(), 0, sdata.Offset,
                             2 * sizeof(float));
            ImPlot::EndPlot();
        }
        ImGui::End();
    }

    void Editor::scene_pane() {
        std::vector<uint32_t> ids = Entity::EntityManager::get_instance()->get_active_ids();
        std::vector<Entity::Entity*> entities;
        for(uint32_t id : ids) {
            entities.push_back(Entity::EntityManager::get_instance()->get_entity(id));
        }

        ImVec2 sceneWindowPos = {(float)Core::Window::get_instance()->get_window_width(), 0.0f};
        ImVec2 sceneWindowPivot = {1.0f, 0.0f};
        ImVec2 sceneWindowSize = {-1, ImGui::GetIO().DisplaySize.y};
        ImGui::SetNextWindowPos(sceneWindowPos, 0, sceneWindowPivot);
        ImGui::SetNextWindowSize(sceneWindowSize);
        ImGui::Begin("Scene", nullptr);
        for (Entity::Entity* entity : entities) {
            std::string name = entity->get_name() + " (" + std::to_string(entity->get_id()) + ")";
            auto* transform = entity->get_component<Component::Transform>();
            if (ImGui::TreeNode(name.c_str())) {
                ImGui::DragFloat3("Position", transform->position, 1.0f, 0.0f, 0.0f, "%.1f");
                ImGui::DragFloat3("Rotation", transform->rotation, 1.0f, -360.0f, 360.0f, "%.1f deg");
                ImGui::DragFloat3("Scale", transform->scale, 1.0f, 0.0f, 0.0f, "%.1f");
                ImGui::TreePop();
            }
        }
        ImGui::End();
    }
}