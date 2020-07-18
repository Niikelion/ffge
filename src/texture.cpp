#include "texture.hpp"

#include <climits>
#include <cmath>
#include <freeimage.h>

namespace ffge
{
    //frees bitmap
    std::tuple<int,int,dataTypes::DATATYPES,Texture2D::formats::FORMATS> LoadBitmap2DToBuffer(FIBITMAP* bitmap, std::vector<BYTE>& bytes)
    {
        if (bitmap)
        {
            const FREE_IMAGE_COLOR_TYPE colType = FreeImage_GetColorType(bitmap);
            switch (colType)
            {
            case FREE_IMAGE_COLOR_TYPE::FIC_RGBALPHA:
                break;
            case FREE_IMAGE_COLOR_TYPE::FIC_MINISBLACK:
            {
                FIBITMAP* tmp = FreeImage_ConvertToStandardType(bitmap);
                FreeImage_Unload(bitmap);
                bitmap = tmp;
                break;
            }
            default:
            {
                FIBITMAP* tmp = FreeImage_ConvertTo32Bits(bitmap);
                FreeImage_Unload(bitmap);
                bitmap = tmp;
                break;
            }
            }

            if (bitmap)
            {
                int w = FreeImage_GetWidth(bitmap);
                int h = FreeImage_GetHeight(bitmap);
                int bpp = FreeImage_GetBPP(bitmap);
                FREE_IMAGE_TYPE imageType = FreeImage_GetImageType(bitmap);

                Texture2D::formats::FORMATS format;
                dataTypes::DATATYPES datatype;
                int c = 0;
                switch (imageType)
                {
                case FREE_IMAGE_TYPE::FIT_BITMAP:
                {
                    datatype = dataTypes::Ubyte;
                    c = bpp / 8;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_FLOAT:
                {
                    datatype = dataTypes::Float;
                    c = 1;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_INT16:
                {
                    datatype = dataTypes::Short;
                    c = 1;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_UINT16:
                {
                    datatype = dataTypes::Ushort;
                    c = 1;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_INT32:
                {
                    datatype = dataTypes::Int;
                    c = 1;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_UINT32:
                {
                    datatype = dataTypes::Uint;
                    c = 1;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_RGB16:
                {
                    datatype = dataTypes::Ushort;
                    c = 3;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_RGBA16:
                {
                    datatype = dataTypes::Ushort;
                    c = 4;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_RGBF:
                {
                    datatype = dataTypes::Float;
                    c = 3;
                    break;
                }
                case FREE_IMAGE_TYPE::FIT_RGBAF:
                {
                    datatype = dataTypes::Float;
                    c = 4;
                    break;
                }
                }

                if (c > 0)
                {
                    switch (c)
                    {
                    case 1:
                    {
                        format = Texture2D::formats::r;
                        break;
                    }
                    case 2:
                    {
                        format = Texture2D::formats::rg;
                        break;
                    }
                    case 3:
                    {
                        format = Texture2D::formats::bgr;
                    }
                    case 4:
                    {
                        format = Texture2D::formats::bgra;
                    }
                    }

                    int scanWidth = w * bpp;

                    bytes.resize(h * scanWidth);

                    FreeImage_ConvertToRawBits(bytes.data(), bitmap, scanWidth, bpp, FI_RGBA_RED_MASK, FI_RGBA_GREEN_MASK, FI_RGBA_BLUE_MASK, TRUE);
                    FreeImage_Unload(bitmap);
                    return std::make_tuple(w,h,datatype,format);
                }
                FreeImage_Unload(bitmap);
            }
        }
        return std::make_tuple(0,0,dataTypes::Ubyte,Texture2D::formats::rgba);
    }

    unsigned int Texture::getId() const noexcept
    {
        return id;
    }

    void Texture1D::bind(unsigned _id)
    {
        glBindTexture(GL_TEXTURE_1D,_id);
    }
    void Texture1D::bind() const noexcept
    {
        bind(id);
    }
    void Texture1D::copy(const Texture1D& target)
    {
        target.bind();
        GLint w = 0, format = 0;
        glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_1D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);

        if (w > 0)
        {
            FrameBuffer fbo;
            fbo.bindAs(GL_READ_FRAMEBUFFER);
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glFramebufferTexture1D(GL_READ_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_1D,target.id,0);
            bind();
            glTexImage1D(GL_TEXTURE_1D,0,format,w,0,format,dataTypes::Ubyte,nullptr);
            glCopyTexSubImage1D(GL_TEXTURE_1D,0,0,0,0,w);
            Texture1D::bind(0);
        }
    }

    void Texture1D::createFromPixels(int format,unsigned width,unsigned type,void * data,int internalFormat)
    {
        if (id)
        {
            bind();
            glTexImage1D(GL_TEXTURE_1D,0,internalFormat,width,0,format,type,data);
        }
    }

    void Texture1D::updateSubRegion(int format,unsigned x,unsigned width,unsigned type,void * data)
    {
        if (id)
        {
            bind();
            glTexSubImage1D(GL_TEXTURE_1D,0,x,width,format,type,data);
        }
    }

    Texture1D::Texture1D()
    {
        id = 0;
        glGenTextures(1,&id);
        bind();
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        bind(0);
    }

    Texture1D::Texture1D(const Texture1D& t)
    {
        copy(t);
    }

    Texture1D::Texture1D(Texture1D && t) noexcept
    {
        id = t.id;
        t.id=0;
    }

    Texture1D::~Texture1D()
    {
        if (id)
        {
            glDeleteTextures(1, &id);
        }
    }

    void Texture2D::bind(unsigned _id)
    {
        glBindTexture(GL_TEXTURE_2D,_id);
    }

    void Texture2D::bind() const noexcept
    {
        bind(id);
    }

    void Texture2D::copy(const Texture2D& target)
    {
        ///\todo perform GPU-side copy
        target.bind();
        GLint w = 0,h = 0, format = 0;
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &format);
        if (w > 0 && h > 0)
        {
            FrameBuffer fbo;
            fbo.bindAs(GL_READ_FRAMEBUFFER);
            glReadBuffer(GL_COLOR_ATTACHMENT0);
            glFramebufferTexture2D(GL_READ_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,target.id,0);
            bind();
            glTexImage2D(GL_TEXTURE_2D,0,format,w,h,0,format,dataTypes::Ubyte,nullptr);
            glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,w,h);
            Texture2D::bind(0);
        }
    }

    void Texture2D::createFromPixels(int format,unsigned width,unsigned height,unsigned type,void * data,int internalFormat)
    {
        if (id)
        {
            bind();
            glTexImage2D(GL_TEXTURE_2D,0,internalFormat,width,height,0,format,type,data);
        }
    }

    void Texture2D::updateSubRegion(int format,unsigned x,unsigned y,unsigned width,unsigned height,unsigned type,void * data)
    {
        if (id)
        {
            bind();
            glTexSubImage2D(GL_TEXTURE_2D,0,x,y,width,height,format,type,data);
        }
    }

    void Texture2D::loadFromFile(const std::string& name)
    {
        FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(name.c_str()), name.c_str());
        if (bitmap)
        {
            std::vector<BYTE> bytes;
            auto res = LoadBitmap2DToBuffer(bitmap,bytes);
            createFromPixels(std::get<3>(res), std::get<0>(res), std::get<1>(res), std::get<2>(res), bytes.data());
        }
    }
    void Texture2D::updateSubRegionFromFile(const std::string& name,unsigned x,unsigned y)
    {
        FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(name.c_str()), name.c_str());
        if (bitmap)
        {
            std::vector<BYTE> bytes;
            auto res = LoadBitmap2DToBuffer(bitmap, bytes);
            updateSubRegion(std::get<3>(res), x, y, std::get<0>(res), std::get<1>(res), std::get<2>(res), bytes.data());
        }
    }

    Texture2D::Texture2D()
    {
        id = 0;
        glGenTextures(1,&id);
        bind();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        bind(0);
    }

    Texture2D::Texture2D(const Texture2D& t)
    {
        copy(t);
    }

    Texture2D::Texture2D(Texture2D && t) noexcept
    {
        id = t.id;
        t.id=0;
    }

    Texture2D::~Texture2D()
    {
        if (id)
        {
            glDeleteTextures(1, &id);
        }
    }

    void TextureAtlas2D::createFromPixels(int format,unsigned width,unsigned height,unsigned type,void * data,int internalFormat)
    {
        size = std::max(width,height);
        Texture2D::createFromPixels(format,size,size,type,data,internalFormat);
    }

    void TextureAtlas2D::prepareForRects(std::vector<UIRect>& _rects)
    {
        if (_rects.size())
        {
            if (!id)
            {
                glGenTextures(1,&id);
            }
            unsigned siz = size;
            std::vector<unsigned> order;
            order.resize(_rects.size());
            for (unsigned i=0; i<order.size(); ++i)
            {
                order[i] = i;
            }
            std::sort(order.begin(),order.end(),[&_rects](unsigned a,unsigned b)
                      {
                          return _rects[a].height > _rects[b].height;
                      });
            if (size == 0)
            {
                unsigned area = 0;
                unsigned maxWidth = 0;
                unsigned maxHeight = 0;
                for (const auto& rect:_rects)
                {
                    area += rect.width * rect.height;
                    maxWidth = std::max(maxWidth, rect.width);
                    maxHeight = std::max(maxHeight, rect.height);
                }
                unsigned s = std::max({static_cast<unsigned>(std::ceil(std::sqrt(area))), maxWidth,maxHeight});
                spaces.emplace(0,0,s,s);
                siz = s;
            }
            for(auto i:order)
            {
                if (_rects[i].width == 0 || _rects[i].height == 0)
                {
                    _rects[i] = UIRect(0,0,0,0);
                    continue;
                }
                if (spaces.size() == 0 || spaces.rbegin()->height < _rects[i].height)//highest block is to short or no blocks
                {
                    //no available space for rect, note, atlas expansion and add additional blocks

                    //calculate minimal size to add new blocks
                    unsigned nw = siz + _rects[i].width;
                    unsigned nh = std::max(siz,_rects[i].height);
                    unsigned tsiz = std::max(nw,nh);
                    //create additional blocks
                    UIRect  b1(siz,_rects[i].height,tsiz - siz,tsiz - _rects[i].height),
                            b2(0,siz,siz,tsiz - siz);
                    //place rect in newly allocated region
                    _rects[i].x = siz;
                    _rects[i].y = 0;

                    siz = tsiz;
                    //insert b1
                    if (b1.height > 0)
                    {
                        spaces.emplace(b1);
                    }
                    //insert b2
                    spaces.emplace(b2);
                }
                else //find fitting block, note, that there can be none due to not enough width
                {
                    bool t=true;
                    for (auto j = spaces.lower_bound(UIRect(0,0,0,_rects[i].height)); j!=spaces.end(); ++j)
                    {
                        if (j -> width >= _rects[i].width) //first spot that fits with least height
                        {
                            if (j -> height == _rects[i].height && j -> width == _rects[i].width) //remove block
                            {
                                //
                            }
                            else if (j -> height == _rects[i].height) //shrink horizontally
                            {
                                UIRect t(j->x+_rects[i].width,j->y,j->width-_rects[i].width,j->height);
                                spaces.emplace(t);
                            }
                            else if (j -> width == _rects[i].width) //shrink vertically
                            {
                                UIRect t(j->x,j->y+_rects[i].height,j->width,j->height-_rects[i].height);
                                spaces.emplace(t);
                            }
                            else
                            {
                                UIRect t(j->x,j->y+_rects[i].height,j->width,j->height - _rects[i].height);
                                spaces.emplace(j->x+_rects[i].width,j->y,j->width-_rects[i].width,_rects[i].height);
                                spaces.emplace(t);
                            }
                            _rects[i].x = j->x;
                            _rects[i].y = j->y;

                            spaces.erase(j);
                            t = false;
                            break;
                        }
                    }
                    if (t)
                    {
                        //no available space for rect, note, atlas expansion and add additional blocks

                        //calculate minimal size to add new blocks
                        unsigned nw = siz + _rects[i].width;
                        unsigned nh = std::max(siz,_rects[i].height);
                        unsigned tsiz = std::max(nw,nh);
                        //create additional blocks
                        UIRect  b1(siz,_rects[i].height,tsiz - siz,tsiz - _rects[i].height),
                                b2(0,siz,siz,tsiz - siz);
                        //place rect in newly allocated region

                        _rects[i].x = siz;
                        _rects[i].y = 0;

                        siz = tsiz;
                        //insert b1
                        if (b1.height > 0)
                        {
                            spaces.emplace(b1);
                        }
                        //insert b2
                        spaces.emplace(b2);
                    }
                }
            }
            if (siz > size)
            {
                if (size > 0)
                {
                    unsigned tmpTex = 0;
                    glGenTextures(1,&tmpTex);

                    {
                        FrameBuffer fbo;
                        fbo.bindAs(GL_READ_FRAMEBUFFER);
                        glReadBuffer(GL_COLOR_ATTACHMENT0);
                        glFramebufferTexture2D(GL_READ_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,id,0);
                        Texture2D::bind(tmpTex);
                        glTexImage2D(GL_TEXTURE_2D,0,format,siz,siz,0,format,dataTypes::Ubyte,nullptr);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                        glCopyTexSubImage2D(GL_TEXTURE_2D,0,0,0,0,0,size,size);
                        Texture2D::bind(0);
                    }

                    glDeleteTextures(1,&id);
                    id = tmpTex;
                }
                else
                {
                    bind();
                    glTexImage2D(GL_TEXTURE_2D,0,format,siz,siz,0,format,dataTypes::Ubyte,nullptr);
                }
                size = siz;
            }
        }
    }

    void TextureAtlas2D::setFormat(int f)
    {
        format = f;
    }
    int TextureAtlas2D::getFormat() const noexcept
    {
        return format;
    }
    unsigned TextureAtlas2D::getSize() const noexcept
    {
        return size;
    }

    void TextureRenderer::setTarget(Texture2D & t,unsigned slot)
    {
        if (renderId)
        {
            target = &t;

            int w,h;

            target->bind();
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_WIDTH,&w);
            glGetTexLevelParameteriv(GL_TEXTURE_2D,0,GL_TEXTURE_HEIGHT,&h);

            glBindRenderbuffer(GL_RENDERBUFFER,depthId);
            glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,w,h);

            glBindFramebuffer(GL_FRAMEBUFFER,renderId);
            glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depthId);
            glFramebufferTexture(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+slot,t.getId(),0);
            glDrawBuffer(GL_COLOR_ATTACHMENT0+slot);
        }
    }

    void TextureRenderer::releaseTarget()
    {
        target = nullptr;
    }

    void TextureRenderer::activate() const
    {
        if (target != nullptr)
        {
            glBindFramebuffer(GL_FRAMEBUFFER,renderId);
        }
    }

    TextureRenderer::TextureRenderer()
    {
        renderId=0;
        depthId=0;
        target=nullptr;

        glGenFramebuffers(1,&renderId);
        glGenRenderbuffers(1,&depthId);
    }

    TextureRenderer::TextureRenderer(Texture2D& t,unsigned slot): TextureRenderer()
    {
        setTarget(t,slot);
    }

    TextureRenderer::TextureRenderer(TextureRenderer&& t) noexcept
    {
        renderId = t.renderId;
        depthId = t.depthId;
        target=t.target;

        t.renderId=0;
        t.depthId=0;
        t.target=nullptr;
    }

    TextureRenderer::~TextureRenderer()
    {
        target = nullptr;
        if (renderId)
        {
            glDeleteFramebuffers(1,&renderId);
            glDeleteRenderbuffers(1,&depthId);
            renderId=0;
            depthId=0;
        }
    }

    bool _FreeImageInitializer::initialized = false;
    _FreeImageInitializer _FreeImageInitializer::Guard;
    _FreeImageInitializer::_FreeImageInitializer()
    {
        if (!initialized)
        {
            FreeImage_Initialise();
            initialized = true;
        }
    }
    _FreeImageInitializer::~_FreeImageInitializer()
    {
        if (initialized)
        {
            FreeImage_DeInitialise();
        }
    }
}
