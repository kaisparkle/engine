#pragma once

namespace Render {
    struct FramebufferObjectsGL {
        unsigned int FBO;
        unsigned int RBO;
    };

    struct MeshObjectsGL {
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;
    };

    struct TextureObjectsGL {
        unsigned int id;
    };
}