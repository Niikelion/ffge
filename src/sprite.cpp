#include "sprite.hpp"

#include "uniform.hpp"

namespace ffge
{
    void Sprite::draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const
    {
        UniformBlock::shared.set("model",m);
        if (texture)
            texture->bind();
        vao.bind();
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        VertexArray::bind(0);
    }

    void Sprite::create(unsigned w,unsigned h,FRect uv)
    {
        struct PosUvPair
        {
            glm::vec2 pos;
            glm::vec2 uv;
        };
        PosUvPair vertices[4];
        vertices[0] = {glm::vec2(0,0),glm::vec2(uv.x,uv.y)};
        vertices[1] = {glm::vec2(w,0),glm::vec2(uv.width,uv.y)};
        vertices[2] = {glm::vec2(0,h),glm::vec2(uv.x,uv.height)};
        vertices[3] = {glm::vec2(w,h),glm::vec2(uv.width,uv.height)};

        vao.bind();

        buffer.data(4*sizeof(PosUvPair),vertices,GL_STATIC_DRAW);
        buffer.attrib(Attribs::position,2,dataTypes::Float,false,sizeof(PosUvPair),0);
        buffer.attrib(Attribs::texcoord,2,dataTypes::Float,false,sizeof(PosUvPair),2*sizeof(float));

        vao.unbind();
    }

    void Sprite::attachTexture(const std::shared_ptr<Texture2D>& tex)
    {
        texture = tex;
    }
}
