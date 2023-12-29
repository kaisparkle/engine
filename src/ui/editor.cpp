#include <cassert>
#include <optick.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <imgui_impl_sdl2.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <core/window.h>
#include <core/deltatime.h>
#include <tools/scrollingbuffer.h>
#include <entity/entitymanager.h>
#include <component/transform.h>
#include <component/rigidbody.h>
#include <render/gl/apiobjects.h>
#include <player/playermanager.h>
#include <physics/physicsmanager.h>
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

    void Editor::init(Render::IFramebuffer* fb) {
        framebuffer = fb;
        // set up dear imgui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        // initialize dear imgui
        ImGui_ImplSDL2_InitForOpenGL(Core::Window::get_instance()->get_handle(), Core::Window::get_instance()->get_glcontext());
        ImGui_ImplOpenGL3_Init("#version 450");

        activeSelection = nullptr;
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
        ImGuiID dockspace = ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

        if(firstTick) {
            firstTick = false;
            init_docking(dockspace);
        }

        if(activeSelection) {
            update_selection_local();
        }

        menu_bar();
        viewport();
        frametime_plot();
        hierarchy_pane();
        inspector_pane();

        // render the frame
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        Physics::PhysicsManager::get_instance()->set_enabled(physicsEnabled);
    }

    void Editor::init_docking(ImGuiID dockspace) {
        // clear existing nodes and create a new one
        ImGui::DockBuilderRemoveNode(dockspace);
        ImGui::DockBuilderAddNode(dockspace, ImGuiDockNodeFlags_DockSpace);
        // use the whole window
        ImGui::DockBuilderSetNodeSize(dockspace, ImGui::GetWindowSize());
        // center node for the viewport
        main = dockspace;
        // split the center node to create a left dock node for hierarchy
        leftTop = ImGui::DockBuilderSplitNode(main, ImGuiDir_Left, 0.75f, nullptr, &main);
        // split the left panel node to dock the inspector underneath the hierarchy
        leftBottom = ImGui::DockBuilderSplitNode(leftTop, ImGuiDir_Down, 0.5f, nullptr, &leftTop);
        // dock all the windows and finish
        ImGui::DockBuilderDockWindow("Viewport", main);
        ImGui::DockBuilderDockWindow("Hierarchy", leftTop);
        ImGui::DockBuilderDockWindow("Inspector", leftBottom);
        ImGui::DockBuilderFinish(dockspace);
    }

    void Editor::menu_bar() {
        if(ImGui::BeginMainMenuBar()) {
            if(ImGui::BeginMenu("Systems")) {
                ImGui::Checkbox("Physics", &physicsEnabled);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void Editor::viewport() {
        ImGui::Begin("Viewport");
        ImVec2 currentSize = ImGui::GetContentRegionAvail();
        if(currentSize.x != viewportWidth || currentSize.y != viewportHeight) {
            viewportWidth = currentSize.x;
            viewportHeight = currentSize.y;
            Player::PlayerManager::get_instance()->get_player_camera()->update_projection(90.0f, viewportWidth / viewportHeight, 0.1f, 10000.0f);
            framebuffer->build(viewportWidth, viewportHeight);
        }

        auto* textureObjects = (Render::TextureObjectsGL*)framebuffer->texture->apiObjects;
        ImGui::Image((void*)(intptr_t)textureObjects->id, currentSize, ImVec2(0.0f, 1.0f), ImVec2(1.0f, 0.0f));

        ImGui::End();
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
        // lock to the top left corner of the viewport
        ImVec2 frametimeWindowPos = {ImGui::DockBuilderGetNode(main)->Pos.x + 8.0f, ImGui::DockBuilderGetNode(main)->Pos.y + 27.0f};
        ImGui::SetNextWindowPos(frametimeWindowPos);
        ImGui::Begin("Frametime Plot", nullptr, windowFlags);
        ImGui::PushItemWidth(500);
        ImGui::SliderFloat("##History", &history, 1, 15, "%.1f s");

        if (ImPlot::BeginPlot("##Frametime Plot", ImVec2(500, 150))) {
            ImPlot::SetupAxes(nullptr, nullptr);
            ImPlot::SetupAxisLimits(ImAxis_X1, t - history, t, ImGuiCond_Always);
            ImPlot::SetupAxisLimits(ImAxis_Y1, 0, 32);
            ImPlot::SetNextFillStyle(IMPLOT_AUTO_COL, 0.5f);
            ImPlot::PlotLine("Frametime (ms)", &sdata.Data[0].x, &sdata.Data[0].y, sdata.Data.size(), 0, sdata.Offset,
                             2 * sizeof(float));
            ImPlot::EndPlot();
        }
        ImGui::End();
    }

    void Editor::hierarchy_pane() {
        std::vector<uint32_t> ids = Entity::EntityManager::get_instance()->get_active_ids();
        std::vector<Entity::Entity*> entities;
        for(uint32_t id : ids) {
            entities.push_back(Entity::EntityManager::get_instance()->get_entity(id));
        }

        ImGui::Begin("Hierarchy", nullptr);
        for (Entity::Entity* entity : entities) {
            std::string name = entity->get_name() + " (" + std::to_string(entity->get_id()) + ")";
            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth |
                    ImGuiTreeNodeFlags_NoTreePushOnOpen |
                    ImGuiTreeNodeFlags_Leaf |
                    (entity == activeSelection ? ImGuiTreeNodeFlags_Selected : ImGuiTreeNodeFlags_None);
            ImGui::TreeNodeEx(name.c_str(), flags);
            if(ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                if(ImGui::IsItemHovered()) {
                    activeSelection = entity;
                    update_selection_local();
                }
            }
        }
        ImGui::End();
    }

    void Editor::inspector_pane() {
        ImGui::Begin("Inspector");
        if(activeSelection) {
            std::string name = activeSelection->get_name() + " (" + std::to_string(activeSelection->get_id()) + ")";
            ImGui::Text("%s", name.c_str());
            auto* transform = activeSelection->get_component<Component::Transform>();
            if(transform) {
                const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_SpanAvailWidth;
                if(ImGui::TreeNodeEx("Components", flags)) {
                    if (ImGui::TreeNodeEx("Transform", flags | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
                        ImGui::DragFloat3("Position", selectionPosition, 1.0f, 0.0f, 0.0f, "%.1f");
                        update_selection_position();
                        ImGui::DragFloat3("Rotation", selectionRotation, 1.0f, -360.0f, 360.0f, "%.1f deg");
                        update_selection_rotation();
                        ImGui::DragFloat3("Scale", selectionScale, 1.0f, 0.0f, 0.0f, "%.1f");
                        update_selection_scale();
                    }
                    ImGui::TreePop();
                }
            }
        }
        ImGui::End();
    }

    void Editor::update_selection_position() {
        glm::vec3 newPosition = glm::vec3(selectionPosition[0], selectionPosition[1], selectionPosition[2]);
        activeSelection->get_component<Component::Transform>()->position = newPosition;
        if(activeSelection->get_component<Component::Rigidbody>()) {
            activeSelection->get_component<Component::Rigidbody>()->update_position();
        }
    }

    void Editor::update_selection_rotation() {
        glm::quat newRotation = glm::quat(glm::radians(glm::vec3(selectionRotation[0], selectionRotation[1], selectionRotation[2])));
        activeSelection->get_component<Component::Transform>()->rotation = newRotation;
        if(activeSelection->get_component<Component::Rigidbody>()) {
            activeSelection->get_component<Component::Rigidbody>()->update_rotation();
        }
    }

    void Editor::update_selection_scale() {
        glm::vec3 newScale = glm::vec3(selectionScale[0], selectionScale[1], selectionScale[2]);
        activeSelection->get_component<Component::Transform>()->scale = newScale;
        if(activeSelection->get_component<Component::Rigidbody>()) {
            activeSelection->get_component<Component::Rigidbody>()->update_scale();
        }
    }

    void Editor::update_selection_local() {
        // check if position needs to be updated
        glm::vec3 localPosition = glm::vec3(selectionPosition[0], selectionPosition[1], selectionPosition[2]);
        glm::vec3 currentPosition = activeSelection->get_component<Component::Transform>()->position;
        if(localPosition != currentPosition) {
            selectionPosition[0] = currentPosition.x;
            selectionPosition[1] = currentPosition.y;
            selectionPosition[2] = currentPosition.z;
        }

        // check if rotation needs to be updated - must compare quat to quat, not euler to euler
        glm::quat localRotation = glm::quat(glm::radians(glm::vec3(selectionRotation[0], selectionRotation[1], selectionRotation[2])));
        glm::quat currentRotation = activeSelection->get_component<Component::Transform>()->rotation;
        if(localRotation != currentRotation) {
            glm::vec3 rotation = glm::degrees(glm::eulerAngles(activeSelection->get_component<Component::Transform>()->rotation));
            selectionRotation[0] = rotation.x;
            selectionRotation[1] = rotation.y;
            selectionRotation[2] = rotation.z;
        }

        // check if scale needs to be updated
        glm::vec3 localScale = glm::vec3(selectionScale[0], selectionScale[1], selectionScale[2]);
        glm::vec3 currentScale = activeSelection->get_component<Component::Transform>()->scale;
        if(localScale != currentScale) {
            selectionScale[0] = currentScale.x;
            selectionScale[1] = currentScale.y;
            selectionScale[2] = currentScale.z;
        }
    }
}