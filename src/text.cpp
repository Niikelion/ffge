#include "text.hpp"
namespace ffge
{
    void Text::prepareCache(const std::string& t,unsigned s)
    {
        if (s == size && t == text)
            return;
        if (s != size)
        {
            access = &font -> getAccess(s);
        }

        offsets.resize(t.size());
        for (unsigned i=0; i<t.size(); ++i)
        {
            auto g = access -> getGlyph(t[i]);
            if (i == 0)
            {
                offsets[0].first = g.advance;
                offsets[0].second = g.index;
            }
            else
            {
                offsets[i].first = offsets[i-1].first + g.advance;
                offsets[i].second = g.index;
            }
        }
        for (size_t i=t.size()-1; i>0; --i)
            offsets[i].first = offsets[i-1].first;
        offsets[0].first = 0;

        s = size;
        text = t;
    }

    void Text::setFont(const Font& f)
    {
        font = const_cast<Font*>(&f);
    }
    void Text::setText(const std::string& t)
    {
        text = t;
    }
    void Text::setSize(unsigned s)
    {
        size = s;
    }

    void Text::prepare(const std::string& t,unsigned s)
    {
        prepareCache(t,s);
    }
    ///

    void Text::draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const
    {
        if (size && access != nullptr)
        {
            access -> bindTexture();
            access -> bindBuffer();

            if (Program::current() != nullptr)
            {
                Program* c = Program::current();

                Uniform offset = c->getUniform("offset"),
                    fontWidth = c->getUniform("fontWidth"),
                    fontHeight = c->getUniform("fontHeight");

                for (const auto& i : offsets)
                {
                    offset = i.first;
                    fontWidth = access->getWidth();
                    fontHeight = access->getHeight();

                    glDrawElementsBaseVertex(GL_TRIANGLE_STRIP, 4, dataTypes::Uint, (void*)0, i.second * 4);
                }
            }
        }
    }


    /*void GLText::setFont(GLFont & target)
    {
        font=&target;
    }
    void GLText::draw() const noexcept
    {
        transforms.start();
        glEnable(GL_TEXTURE_2D);

        double pos = 0;

        color.use();

        GLFont::GLGlyph glyph;

        GLFont::Access* faccess = &(font->getRendered(size));

        faccess->Texture2D.bind();
        glTexParameteri(GL_TEXTURE_2D, GL_Texture2D_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_Texture2D_WRAP_T, GL_CLAMP_TO_EDGE);

        glTexParameteri(GL_TEXTURE_2D, GL_Texture2D_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_Texture2D_MAG_FILTER, GL_LINEAR);

        for (const auto &i:message)
        {
            glyph = faccess->glyphs[i - std::numeric_limits<char>::min()];

            glBegin(GL_POLYGON);
            glTexCoord2d(glyph.tx,glyph.ty);

            GLPoint help(pos,glyph.y,0);
            help.x -= origin.x;
            help.y -= origin.y;
            help.z -= origin.z;
            help.use();

            glTexCoord2d(glyph.tx,glyph.ty+glyph.th);

            help=GLPoint(pos,glyph.y-glyph.height);
            help.x-=origin.x;
            help.y-=origin.y;
            help.z-=origin.z;
            help.use();

            glTexCoord2d(glyph.tx+glyph.tw,glyph.ty+glyph.th);

            help=GLPoint(pos+glyph.width,glyph.y-glyph.height);
            help.x-=origin.x;
            help.y-=origin.y;
            help.z-=origin.z;
            help.use();

            glTexCoord2d(glyph.tx+glyph.tw,glyph.ty);

            help=GLPoint(pos+glyph.width,glyph.y);
            help.x-=origin.x;
            help.y-=origin.y;
            help.z-=origin.z;
            help.use();

            glEnd();
            pos+=glyph.advance;
        }
        GLTexture2D::bind(0);

        transforms.finish();

        glDisable(GL_TEXTURE_2D);
    }

    void GLText::drawEx(const GLVertexTransforms& Transforms) const noexcept
    {
        if (transforms.size()||Transforms.size())
            glPushMatrix();

        Transforms.use();
        transforms.use();

        draw();

        if (transforms.size()||Transforms.size())
            glPopMatrix();
    }

    GLRect GLText::getLocalBounds()
    {
        GLFont::Access* faccess=&(font->getRendered(size));
        GLFont::GLGlyph glyph;
        GLRect result(0,0,0,0);
        int min_back=0;
        GLPoint pos,siz;
        for (unsigned int i=0; i<message.size(); ++i)
        {
            glyph=faccess->glyphs[message[i]-std::numeric_limits<char>::min()];
            if (i==0)
                min_back=glyph.y;
            if (min_back>glyph.y)
                min_back=glyph.y;
        }

        for (unsigned int i=0; i<message.size(); ++i)
        {
            glyph=faccess->glyphs[message[i]-std::numeric_limits<char>::min()];
            if (int(result.height)<glyph.height-min_back+glyph.x)
                result.height=glyph.height;
            if (message[i]==' ')
                glyph.width=size/4;
            result.width+=glyph.width+glyph.x;
        }
        siz.x=result.width;
        siz.y=result.height;
        for (unsigned int i=0; i<transforms.size(); ++i)
        {
            transforms[i].use(pos);
            transforms[i].use(siz);
        }

        GLPoint res=siz.v3()-pos.v3();
        result.width=res.x<0?(-res.x):(res.x);
        result.height=res.y<0?(-res.y):(res.y);
        result.x=pos.x>res.x?res.x:pos.x;
        result.y=pos.y>res.y?res.y:pos.y;
        return result;
    }

    GLRect GLText::getGlobalBounds(const GLVertexTransforms& Transforms,const GLView& view,const GLRect& frame) const
    {
        GLFont::Access* faccess=&(font->getRendered(size));
        GLFont::GLGlyph glyph;
        GLRect result(0,0,0,0);
        int min_back=0;
        GLPoint pos,siz;
        for (size_t i=0; i<message.size(); ++i)
        {
            glyph=faccess->glyphs[message[i]-std::numeric_limits<char>::min()];
            if (i==0)
                min_back=glyph.y;
            if (min_back>glyph.y)
                min_back=glyph.y;
        }

        for (size_t i=0; i<message.size(); ++i)
        {
            glyph=faccess->glyphs[message[i]-std::numeric_limits<char>::min()];
            if (int(result.height)<glyph.height-min_back+glyph.x)
                result.height=glyph.height;
            if (message[i]==' ')
                glyph.width=size/4;
            result.width+=glyph.width+glyph.x;
        }

        result.width*=frame.width;
        result.width/=view.baseview.width;
        result.height*=frame.height;
        result.height/=view.baseview.height;

        siz.x=result.width;
        siz.y=result.height;
        for (size_t i=0; i<transforms.size(); ++i)
        {
            transforms[i].use(pos);
            transforms[i].use(siz);
        }
        for (size_t i=0; i<Transforms.size(); ++i)
        {
            Transforms[i].use(pos);
            Transforms[i].use(siz);
        }
        GLPoint res=siz-pos;

        result.x=((pos.x>res.x?res.x:pos.x)-origin.x)*frame.width/view.baseview.width;
        result.y=((pos.y>res.y?res.y:pos.y)-origin.y)*frame.height/view.baseview.height;
        result.width=res.x<0?(-res.x):(res.x);
        result.height=res.y<0?(-res.y):(res.y);
        return result;
    }

    //FT_GlyphSlotRec_.metrics.

    GLText::GLText()
    {
        font = nullptr;
        size=1;
    }*/
}
