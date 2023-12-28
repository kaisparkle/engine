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
        Render::IFramebuffer* framebuffer;

        float viewportWidth;
        float viewportHeight;

        bool firstTick = true;

        ImGuiID main, leftTop, leftBottom;

        // initialize default docking layout
        void init_docking(ImGuiID dockspace);

        // viewport
        void viewport();
        // frametime plot
        void frametime_plot();
        // hierarchy pane
        void hierarchy_pane();
        // inspector pane
        void inspector_pane();

        void check_position_update();
        void check_rotation_update();
        void check_scale_update();
    };
}