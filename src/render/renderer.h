#pragma once

namespace Render {
    class IRenderer {
    public:
        virtual void init() = 0;
        virtual void cleanup() = 0;

        virtual void resize_viewport(int width, int height) = 0;
        virtual void tick() = 0;
    };
}