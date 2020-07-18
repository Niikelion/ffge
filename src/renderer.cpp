#include "renderer.hpp"

#include "ff_glew.hpp"

namespace ffge
{
    void BasicRenderTarget::activate() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDrawBuffer(GL_BACK);
    }

    BasicRenderTarget DefaultRenderTarget;
}
