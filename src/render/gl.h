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

        // update the viewport and clear buffers
        void viewport_clear(int width, int height);
    private:
        inline static RendererGL* instance;
    };
}