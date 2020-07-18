#include "font.hpp"

namespace ffge
{
    using namespace glm;
    Font::Access::Glyph::Glyph():index(0),advance(0) {/**/}

    Font::Access::Glyph::Glyph(unsigned i,unsigned a):index(i),advance(a) {/**/}

    Font::Access::GlyphData::GlyphData(UIRect uv,ivec2 off)
    {
        uvRect = uv;
        offset = off;
    }

    Font::Access::PerVertexData::PerVertexData(ivec2 off,uvec2 tex)
    {
        offset = off;
        uv = tex;
    }

    void Font::Access::PerVertexData::fromGlyphData(const GlyphData& data,PerVertexData* ret)
    {
        ret[0] = PerVertexData(ivec2(   data.offset.x,
                                        0-data.offset.y),
                               uvec2(   data.uvRect.x,
                                        data.uvRect.y)
                               );
        ret[1] = PerVertexData(ivec2(   data.offset.x+data.uvRect.width,
                                        0-data.offset.y),
                               uvec2(   data.uvRect.x+data.uvRect.width,
                                        data.uvRect.y)
                               );
        ret[2] = PerVertexData(ivec2(   data.offset.x,
                                        0-data.offset.y+data.uvRect.height),
                               uvec2(   data.uvRect.x,
                                        data.uvRect.y+data.uvRect.height)
                               );
        ret[3] = PerVertexData(ivec2(   data.offset.x+data.uvRect.width,
                                        0-data.offset.y+data.uvRect.height),
                               uvec2(   data.uvRect.x+data.uvRect.width,
                                        data.uvRect.y+data.uvRect.height)
                               );
    }

    unsigned Font::Access::getWidth() const noexcept
    {
        return atlas.getSize();
    }
    unsigned Font::Access::getHeight() const noexcept
    {
        return atlas.getSize();
    }

    Font::Access::Glyph& Font::Access::getGlyph(char32_t c)
    {
        auto it = glyphs.find(c);
        if (it == glyphs.end())
        {
            prepareGlyph(c);
            it = glyphs.find(c);
        }
        return it -> second;
    }

    Font::Access::Glyph* Font::Access::findGlyph(char32_t c) const noexcept
    {
        auto it = glyphs.find(c);
        if (it == glyphs.end())
            return nullptr;
        return const_cast<Font::Access::Glyph*>(&(it -> second));
    }

    void Font::Access::prepareGlyph(char32_t c) //TODO: add glyph to vertex array
    {
        if (glyphs.find(c) == glyphs.end())
        {
            VertexArray::unbind();
            GlyphData data;
            PerVertexData v[4];
            unsigned in[4];

            FT_Set_Pixel_Sizes(face,0,size);

            if(FT_Load_Char(face,c, FT_LOAD_RENDER) == 0)
            {
                data.offset = ivec2(    face -> glyph -> bitmap_left,
                                        face -> glyph -> bitmap_top);

                data.uvRect = UIRect(   0,
                                        0,
                                        face -> glyph -> bitmap.width,
                                        face -> glyph -> bitmap.rows);
                if (data.uvRect.width > 0 && data.uvRect.height > 0)
                {
                    data.uvRect.width   += 2;
                    data.uvRect.height  += 2;

                    std::vector<UIRect>tmp = {data.uvRect};
                    atlas.prepareForRects(tmp);

                    data.uvRect = tmp[0];

                    atlas.bind();
                    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
                    data.uvRect = UIRect(   data.uvRect.x + 1,
                                            data.uvRect.y + 1,
                                            data.uvRect.width - 2,
                                            data.uvRect.height - 2);
                    atlas.updateSubRegion(Texture::formats::r,data.uvRect,dataTypes::Ubyte,face -> glyph -> bitmap.buffer);
                }
                else
                {
                    data.uvRect = UIRect(0,0,0,0);
                }
                //add GlyphData to vertex buffers

                PerVertexData::fromGlyphData(data,v);
                for (unsigned i=0; i<4; ++i)
                    in[i] = static_cast<unsigned int>(glyphs.size()*4+i);

                if (glyphs.size())
                {
                    vao.bind();
                    {
                        VertexBuffer tmpb;
                        buffer.bindAs(GL_COPY_READ_BUFFER);
                        tmpb.data((glyphs.size()+1)*sizeof(PerVertexData)*4,nullptr,GL_DYNAMIC_DRAW);
                        tmpb.bindAs(GL_COPY_WRITE_BUFFER);

                        glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,0,glyphs.size()*sizeof(PerVertexData)*4);

                        DataBuffer::unbind(GL_COPY_READ_BUFFER);
                        DataBuffer::unbind(GL_COPY_WRITE_BUFFER);

                        buffer.bind();
                        buffer.disable(Attributes::position);
                        buffer.disable(Attributes::texcoord);
                        buffer.unbind();
                        buffer = std::move(tmpb);
                        buffer.bind();

                        buffer.attrib(Attributes::position,2,dataTypes::Int,false,sizeof(PerVertexData),0);
                        buffer.attrib(Attributes::texcoord,2,dataTypes::Uint,false,sizeof(PerVertexData),2*sizeof(int));
                    }
                    {
                        IndicesBuffer tmpb;
                        indices.bindAs(GL_COPY_READ_BUFFER);
                        tmpb.data((glyphs.size()+1)*sizeof(unsigned)*4,nullptr,GL_DYNAMIC_DRAW);
                        tmpb.bindAs(GL_COPY_WRITE_BUFFER);

                        glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,0,glyphs.size()*sizeof(unsigned)*4);

                        DataBuffer::unbind(GL_COPY_READ_BUFFER);
                        DataBuffer::unbind(GL_COPY_WRITE_BUFFER);

                        indices.unbind();
                        indices = std::move(tmpb);
                        indices.bind();
                    }
                    buffer.subdata(glyphs.size()*sizeof(PerVertexData)*4,sizeof(PerVertexData)*4,v);
                    indices.subdata(glyphs.size()*sizeof(unsigned)*4,sizeof(unsigned)*4,in);
                    vao.unbind();
                }
                else
                {
                    buffer.data(sizeof(PerVertexData)*4,v,GL_DYNAMIC_DRAW);
                    indices.data(sizeof(unsigned)*4,in,GL_DYNAMIC_DRAW);
                }
                glyphs.emplace(c,Glyph(static_cast<unsigned int>(glyphs.size()),face -> glyph -> advance.x >> 6));
            }
            vao.unbind();
        }
    }

    void Font::Access::prepareGlyphs(const std::vector<char32_t>& cs)
    {
        std::vector<std::pair<char32_t,GlyphData>> tmp;
        std::vector<UIRect>rects;
        tmp.reserve(cs.size());
        rects.reserve(cs.size());

        FT_Set_Pixel_Sizes(face,0,size);
        PerVertexData v[4];
        unsigned in[4];

        for (auto c:cs)
        {
            if (glyphs.find(c) == glyphs.end())
            {
                GlyphData data;
                if(FT_Load_Char(face,c, FT_LOAD_NO_BITMAP) == 0)
                {
                    FT_BBox box;

                    FT_Glyph glyph;

                    FT_Get_Glyph( face->glyph, &glyph );
                    FT_Glyph_Get_CBox(glyph,FT_GLYPH_BBOX_PIXELS,&box);
                    FT_Done_Glyph(glyph);

                    data.offset = ivec2(    box.xMin,
                                            box.yMax);
                    data.uvRect = UIRect(   0,
                                            0,
                                            (box.xMax-box.xMin),
                                            (box.yMax-box.yMin));


                    if (data.uvRect.width > 0 && data.uvRect.height > 0)
                    {
                        data.uvRect.width += 2;
                        data.uvRect.height += 2;
                    }

                    tmp.emplace_back(c,data);
                    rects.emplace_back(data.uvRect);
                }
            }
        }

        unsigned off = static_cast<unsigned int>(glyphs.size());

        VertexArray::unbind();

        atlas.prepareForRects(rects);
        //resize vertex buffer and copy old contents
        if (off)
        {
            vao.bind();
            {
                VertexBuffer tmpb;
                buffer.bindAs(GL_COPY_READ_BUFFER);
                tmpb.data((off+tmp.size())*sizeof(PerVertexData)*4,nullptr,GL_DYNAMIC_DRAW);
                tmpb.bindAs(GL_COPY_WRITE_BUFFER);

                glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,0,off*sizeof(PerVertexData)*4);

                DataBuffer::unbind(GL_COPY_READ_BUFFER);
                DataBuffer::unbind(GL_COPY_WRITE_BUFFER);

                buffer.bind();
                buffer.disable(Attributes::position);
                buffer.disable(Attributes::texcoord);
                buffer.unbind();
                buffer = std::move(tmpb);
                buffer.bind();

                buffer.attrib(Attributes::position,2,dataTypes::Int,false,sizeof(PerVertexData),0);
                buffer.attrib(Attributes::texcoord,2,dataTypes::Uint,false,sizeof(PerVertexData),2*sizeof(int));
            }
            {
                IndicesBuffer tmpb;
                indices.bindAs(GL_COPY_READ_BUFFER);
                tmpb.data((off+tmp.size())*sizeof(unsigned)*4,nullptr,GL_DYNAMIC_DRAW);
                tmpb.bindAs(GL_COPY_WRITE_BUFFER);

                glCopyBufferSubData(GL_COPY_READ_BUFFER,GL_COPY_WRITE_BUFFER,0,0,off*sizeof(unsigned)*4);

                DataBuffer::unbind(GL_COPY_READ_BUFFER);
                DataBuffer::unbind(GL_COPY_WRITE_BUFFER);

                indices.unbind();
                indices = std::move(tmpb);
                indices.bind();
            }
            vao.unbind();
        }
        else
        {
            buffer.data(tmp.size()*sizeof(PerVertexData)*4,nullptr,GL_DYNAMIC_DRAW);
            indices.data(tmp.size()*sizeof(unsigned)*4,nullptr,GL_DYNAMIC_DRAW);
        }
        atlas.bind();
        glPixelStorei(GL_UNPACK_ALIGNMENT,1);
        for (unsigned i=0; i<tmp.size(); ++i)
        {
            tmp[i].second.uvRect = rects[i];

            if (rects[i].width > 0 && rects[i].height > 0)
            {
                tmp[i].second.uvRect = UIRect(  rects[i].x+1,
                                                rects[i].y+1,
                                                rects[i].width - 2,
                                                rects[i].height - 2);
            }
            else
            {
                tmp[i].second.uvRect = UIRect(0,0,0,0);
            }

            if(FT_Load_Char(face,tmp[i].first, FT_LOAD_RENDER) == 0 && rects[i].width > 0 && rects[i].height > 0)
            {
                atlas.updateSubRegion(Texture::formats::r,tmp[i].second.uvRect,dataTypes::Ubyte,face -> glyph -> bitmap.buffer);
            }
            //write data to vertex buffer
            PerVertexData::fromGlyphData(tmp[i].second,v);
            buffer.subdata((off+i)*sizeof(PerVertexData)*4,sizeof(PerVertexData)*4,v);

            for (unsigned j=0; j<4; ++j)
                in[j] = (off+i)*4+j;
            indices.subdata((off+i)*sizeof(unsigned)*4,sizeof(unsigned)*4,in);

            glyphs.emplace(tmp[i].first,Glyph(off + i,face -> glyph -> advance.x >> 6));
        }
    }

    void Font::Access::bindTexture()
    {
        atlas.bind();
    }
    void Font::Access::bindBuffer()
    {
        vao.bind();
    }

    Font::Access::Access(unsigned _size,FT_Face _face):face(_face),size(_size)
    {
        atlas.setFormat(Texture::formats::r);
        vao.bind();
        buffer.bind();
        buffer.attrib(Attributes::position,2,dataTypes::Int,false,sizeof(PerVertexData),0);
        buffer.attrib(Attributes::texcoord,2,dataTypes::Uint,false,sizeof(PerVertexData),2*sizeof(unsigned));
        indices.bind();
        vao.unbind();
    };

    bool Font::loadFromFile(const std::string& filename)
    {
        if(FT_New_Face(_FreeTypeInitializer::freetype,filename.c_str(),0,&face))
        {
            return false;
        }
        return true;
    }

    Font::Access& Font::getAccess(unsigned int size)
    {
        auto it = sizes.find(size);
        if (it == sizes.end())
        {
            return sizes.emplace(std::piecewise_construct,
                                 std::forward_as_tuple(size),
                                 std::forward_as_tuple(size,face)
                                ).first -> second;
        }
        return it -> second;
    }
    /*
    Texture2D Font::renderText(unsigned size,const std::string& text)
    {
        if (size == 0)
        {
            size = 1;
        }

        //create new access
        Texture2D ftex;
        unsigned width = 0,height = 0;

        Glyph glyphs[text.size()];

        FT_Set_Pixel_Sizes(face,0,size);
        for (unsigned i = 0; i < text.size(); ++i)
        {
            if(FT_Load_Char(face,text[i], FT_LOAD_RENDER) == 0)
            {
                //ok
                glyphs[i].height = face->glyph->bitmap.rows;
                glyphs[i].width = face->glyph->bitmap.width;

                glyphs[i].x = face->glyph->metrics.horiBearingX;// + face->glyph->advance.x;
                glyphs[i].y = -face->glyph->bitmap_top;// + face->glyph->advance.y;

                glyphs[i].tx = width + face->glyph->bitmap_left;
                glyphs[i].ty = face->glyph->bitmap_top;

                if (height < static_cast<unsigned>(glyphs[i].height + glyphs[i].y ))
                {
                    height = glyphs[i].height;
                }

                width += glyphs[i].width + glyphs[i].x;
            }
            else
            {
                glyphs[i].height = 0;
                glyphs[i].width = 0;
                glyphs[i].x = 0;
                glyphs[i].y = 0;
                glyphs[i].tx = width;
                glyphs[i].ty = 0;
            }
        }
        GLubyte *pixels = new GLubyte[4*sizeof(GLubyte)*width*height];
        for (unsigned long i = 0; i < 4*sizeof(GLubyte)*width*height; ++i)
        {
            pixels[i] = 0;
        }
        for (unsigned i = 0; i<text.size(); ++i)
        {
            if(!FT_Load_Char(face,text[i], FT_LOAD_RENDER))
            {
                for (unsigned x = 0; x < glyphs[i].width; ++x)
                {
                    for (unsigned y = 0; y<glyphs[i].height; ++y)
                    {
                        pixels[(width*y+static_cast<unsigned>(glyphs[i].tx) + x)*4 + 0] = face->glyph->bitmap.buffer[face->glyph->bitmap.width*y+x];
                        pixels[(width*y+static_cast<unsigned>(glyphs[i].tx) + x)*4 + 1] = face->glyph->bitmap.buffer[face->glyph->bitmap.width*y+x];
                        pixels[(width*y+static_cast<unsigned>(glyphs[i].tx) + x)*4 + 2] = face->glyph->bitmap.buffer[face->glyph->bitmap.width*y+x];
                        pixels[(width*y+static_cast<unsigned>(glyphs[i].tx) + x)*4 + 3] = 255;
                    }
                }
            }
            glyphs[i].tx /= width;
        }
        ftex.createFromPixels(GL_RGBA,width,height,GL_UNSIGNED_BYTE,pixels);
        delete[] pixels;
        return std::move(ftex);
    }
*/
    Font::Font()
    {
        face = nullptr;
    }

    Font::~Font()
    {
        if (face != nullptr)
            FT_Done_Face( face );
    }

    bool _FreeTypeInitializer::initialized = false;
    _FreeTypeInitializer _FreeTypeInitializer::Guard;
    FT_Library _FreeTypeInitializer::freetype = nullptr;
    _FreeTypeInitializer::_FreeTypeInitializer()
    {
        if (!initialized)
            if(!FT_Init_FreeType(&freetype))
                initialized = true;
    }
    _FreeTypeInitializer::~_FreeTypeInitializer()
    {
        if (initialized)
        {
            FT_Done_FreeType(freetype);
        }
    }
}
