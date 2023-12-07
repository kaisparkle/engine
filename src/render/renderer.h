#pragma once

namespace Render {
    class IRenderer {
    public:
        virtual void init() = 0;
        virtual void cleanup() = 0;

        virtual void viewport_clear(int width, int height) = 0;
    };
}