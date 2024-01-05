#include <iostream>
#include <glad/glad.h>
#include <render/gl/apiobjects.h>
#include <render/gl/framebuffer.h>

namespace Render {
    void FramebufferGL::build(int windowWidth, int windowHeight) {
        // free previous objects
        free(apiObjects);
        free(texture);

        // create framebuffer
        auto* framebufferObjects = (FramebufferObjectsGL*)malloc(sizeof(FramebufferObjectsGL));
        glGenFramebuffers(1, &framebufferObjects->FBO);
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferObjects->FBO);

        // create target texture
        auto* tex = new Core::Texture;
        auto* textureObjects = (TextureObjectsGL*)malloc(sizeof(TextureObjectsGL));
        glGenTextures(1, &textureObjects->id);
        glBindTexture(GL_TEXTURE_2D, textureObjects->id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);

        // attach target as a color attachment
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureObjects->id, 0);

        // create renderbuffer
        glGenRenderbuffers(1, &framebufferObjects->RBO);
        glBindRenderbuffer(GL_RENDERBUFFER, framebufferObjects->RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, windowWidth, windowHeight);
        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        // attach renderbuffer as depth and stencil attachments
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, framebufferObjects->RBO);

        // check
        if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Failed to build framebuffer!" << std::endl;
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        apiObjects = (void*)framebufferObjects;
        tex->apiObjects = (void*)textureObjects;
        texture = tex;
        width = windowWidth;
        height = windowHeight;
    }

    void FramebufferGL::bind() {
        auto* objects = (FramebufferObjectsGL*)apiObjects;
        glBindFramebuffer(GL_FRAMEBUFFER, objects->FBO);
    }
}