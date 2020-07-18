#ifndef TEXT_H
#define TEXT_H

#include <ffge/basics.hpp>
#include <ffge/buffer.hpp>
#include <ffge/font.hpp>
#include <ffge/shader.hpp>
#include <ffge/uniform.hpp>

#include <string>

namespace ffge
{
    class Text: public Drawable
    {
    protected:
        Font* font;

        Font::Access* access;
        unsigned size;
        std::string text;
        std::vector<std::pair<unsigned,unsigned>> offsets;

        void prepareCache(const std::string& t,unsigned s);
    public:

        void setText(const std::string& t);
        void setSize(unsigned s);
        void prepare(const std::string& t,unsigned s);
        void setFont(const Font& f);

        virtual void draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const override;
    };
}

#endif // TEXT_H
