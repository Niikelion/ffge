#ifndef SPRITE_H
#define SPRITE_H

#include <ffge/basics.hpp>
#include <ffge/buffer.hpp>
#include <ffge/texture.hpp>
#include <memory>

namespace ffge
{
    class Sprite: public Drawable
    {
    protected:
        VertexBuffer buffer;
        VertexArray vao;
        std::shared_ptr<Texture2D> texture;
    public:
        virtual void draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const override;

        void create(unsigned width,unsigned height,FRect uv = FRect(0,0,1,1));

        void attachTexture(const std::shared_ptr<Texture2D>& tex);
    };
}

#endif // SPRITE_H
