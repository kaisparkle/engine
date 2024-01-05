#pragma once

#include <render/framebuffer.h>

namespace Render {
    class IRenderer {
    public:
        virtual void init() = 0;
        virtual void cleanup() = 0;

        virtual IFramebuffer* get_framebuffer() = 0;
        virtual void resize_viewport(int width, int height) = 0;
        virtual void tick() = 0;
    };
}