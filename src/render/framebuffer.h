#pragma once

#include <core/texture.h>

namespace Render {
    class IFramebuffer {
    public:
        virtual void build(int windowWidth, int windowHeight) = 0;
        virtual void bind() = 0;

        Core::Texture* texture = nullptr;
        void* apiObjects = nullptr;

        int width;
        int height;
    };
}