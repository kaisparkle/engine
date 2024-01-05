#pragma once

#include <render/framebuffer.h>

namespace Render {
    class FramebufferGL : public IFramebuffer {
    public:
        void build(int windowWidth, int windowHeight);
        void bind();
    };
}