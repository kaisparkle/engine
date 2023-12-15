#pragma once

#include <render/renderer.h>

namespace Render {
    class RendererGL: public IRenderer {
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
    };
}