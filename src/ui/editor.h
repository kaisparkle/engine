#pragma once

#include <vector>
#include <imgui.h>
#include <SDL.h>
#include <entity/entity.h>
#include <render/framebuffer.h>

namespace UI {
    class Editor {
    public:
        // create the Editor singleton
        static Editor* create_instance();
        // destroy the Editor singleton
        static void destroy_instance();
        // return the Editor singleton
        static Editor* get_instance();

        // initialize the Editor
        void init(Render::IFramebuffer* fb);
        // clean up the Editor
        void cleanup();

        // process SDL2 events
        void process_event(SDL_Event* event);
        // draw the next frame
        void tick();
    private:
        inline static Editor* instance;

        Entity::Entity* activeSelection;
        float selectionPosition[3] = {0.0f, 0.0f, 0.0f};
        float selectionRotation[3] = {0.0f, 0.0f, 0.0f};
        float selectionScale[3] = {0.0f, 0.0f, 0.0f};

        Render::IFramebuffer* framebuffer;

        float viewportWidth;
        float viewportHeight;

        bool firstTick = true;

        bool physicsEnabled = false;

        ImGuiID main, leftTop, leftBottom;

        // initialize default docking layout
        void init_docking(ImGuiID dockspace);

        // menu bar
        void menu_bar();
        // viewport
        void viewport();
        // frametime plot
        void frametime_plot();
        // hierarchy pane
        void hierarchy_pane();
        // inspector pane
        void inspector_pane();

        // update the selected item's transform position, and update the physics engine if applicable
        void update_selection_position();
        // update the selected item's transform rotation, and update the physics engine if applicable
        void update_selection_rotation();
        // update the selected item's transform scale, and update the physics engine if applicable
        void update_selection_scale();

        // update the local position/rotation/scale values if needed
        void update_selection_local();
    };
}