#ifndef RENDERER_H
#define RENDERER_H

namespace ffge
{
    class RenderTarget
    {
    public:
        virtual void activate() const = 0;
    };

    class BasicRenderTarget: public RenderTarget
    {
    public:
        virtual void activate() const;
    };

    extern BasicRenderTarget DefaultRenderTarget;
}

#endif // RENDERER_H
