#ifndef TEXTURE_H
#define TEXTURE_H

#include <ffge/basics.hpp>
#include <ffge/renderer.hpp>
#include <ffge/buffer.hpp>

#include <string>
#include <set>
#include <vector>

namespace ffge
{
    template <typename T>T _rtnp2(T v)
    {
        v--;
        for (size_t i = 1; i < sizeof(v) * CHAR_BIT; i <<= 1)
        {
            v |= v >> i;
        }
        return ++v;
    }

    class Texture
    {
    protected:
        unsigned int id;
    public:
        struct formats
        {
            enum FORMATS
            {
                r = GL_RED,
                r_i = GL_RED_INTEGER,

                g = GL_GREEN,
                g_i = GL_GREEN_INTEGER,

                b = GL_BLUE,
                b_i = GL_BLUE_INTEGER,

                rg = GL_RG,
                rg_i = GL_RG_INTEGER,

                rgb = GL_RGB,
                rgb_i = GL_RGB_INTEGER,

                rgba = GL_RGBA,
                rgba_i = GL_RGBA_INTEGER,

                bgr = GL_BGR,
                bgr_i = GL_BGR_INTEGER,

                bgra = GL_BGRA,
                bgra_i = GL_BGRA_INTEGER
            };
        };

        unsigned int getId() const noexcept;
        virtual void bind() const noexcept = 0;
    };

    class Texture1D: public Texture
    {
    public:
        virtual void createFromPixels(int format,unsigned width,unsigned type,void * data,int internalFormat);
        inline void createFromPixels(int format,unsigned width,unsigned type,void * data)
        {
            createFromPixels(format,width,type,data,format);
        }

        virtual void updateSubRegion(int format,unsigned x,unsigned width,unsigned type,void * data);

        static void bind(unsigned _id);
        virtual void bind() const noexcept;
        virtual void copy(const Texture1D& target);

        Texture1D();
        explicit Texture1D(const Texture1D &);
        Texture1D(Texture1D &&) noexcept;

        virtual ~Texture1D();
    };

    class Texture2D: public Texture
    {
    public:
        virtual void createFromPixels(int format,unsigned width,unsigned height,unsigned type,void * data,int internalFormat);
        inline void createFromPixels(int format,unsigned width,unsigned height,unsigned type,void * data)
        {
            createFromPixels(format,width,height,type,data,format);
        }

        virtual void updateSubRegion(int format,unsigned x,unsigned y,unsigned width,unsigned height,unsigned type,void * data);
        inline void updateSubRegion(int format,UIRect rect,unsigned type,void * data)
        {
            updateSubRegion(format,rect.x,rect.y,rect.width,rect.height,type,data);
        }

        virtual void loadFromFile(const std::string& name);
        virtual void updateSubRegionFromFile(const std::string& name,unsigned x,unsigned y);

        static void bind(unsigned _id);
        virtual void bind() const noexcept;
        virtual void copy(const Texture2D& target);

        Texture2D();
        explicit Texture2D(const Texture2D &);
        Texture2D(Texture2D &&) noexcept;

        virtual ~Texture2D();
    };

    class TextureAtlas2D: public Texture2D
    {
    protected:
        struct _cmpRectByHeight
        {
            bool operator() (const UIRect& a, const UIRect& b) const
            {
                return a.height < b.height;
            }
        };

        unsigned size;
        int format;
        std::multiset<UIRect,_cmpRectByHeight> spaces;
    public:
        virtual void prepareForRects(std::vector<UIRect>& _rects);

        virtual void createFromPixels(int format,unsigned width,unsigned height,unsigned type,void * data,int internalFormat);

        void setFormat(int f);
        int getFormat() const noexcept;
        unsigned getSize() const noexcept;

        TextureAtlas2D():Texture2D()
        {
            size = 0;
            format = 0;
        }
        TextureAtlas2D(const TextureAtlas2D&) = default;
        TextureAtlas2D(TextureAtlas2D&& t) noexcept: Texture2D(t)
        {
            size = t.size;
            t.size = 0;
            format = t.format;
        }
    };

    class TextureRenderer: public RenderTarget
    {
    protected:
        unsigned int renderId,depthId;
        Texture2D* target;
    public:
        virtual void setTarget(Texture2D &,unsigned slot=0);
        virtual void releaseTarget();
        virtual void activate() const;

        TextureRenderer();
        TextureRenderer(Texture2D &,unsigned slot = 0);
        TextureRenderer(const TextureRenderer &) = delete;
        TextureRenderer(TextureRenderer &&) noexcept;

        ~TextureRenderer();
    };

    class RenderTexture2D: public RenderTarget, public Texture2D
    {
    protected:
        unsigned int renderId;
    public:
        virtual void activate() const;

        RenderTexture2D();
        RenderTexture2D(RenderTexture2D&&) noexcept;

        ~RenderTexture2D();
    };

    class _FreeImageInitializer
    {
        _FreeImageInitializer();
    public:
        static bool initialized;
        static _FreeImageInitializer Guard;
        ~_FreeImageInitializer();
    };
}

#endif // TEXTURE_H
