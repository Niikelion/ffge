#ifndef FONT_H
#define FONT_H

#include <string>
#include <map>

#include <ffge/ftguard.hpp>

#include <ffge/basics.hpp>
#include <ffge/buffer.hpp>
#include <ffge/texture.hpp>

namespace ffge
{
    class Font
    {
    public:
        class Access
        {
        public:
            struct Glyph
            {
                unsigned index,advance;
                Glyph();
                Glyph(unsigned i,unsigned a);
                Glyph(const Glyph&) = default;
                Glyph(Glyph&&) noexcept = default;
            };
            struct GlyphData
            {
                UIRect uvRect;
                glm::ivec2 offset;

                GlyphData() = default;
                GlyphData(UIRect uv,glm::ivec2 off);
                GlyphData(const GlyphData&) = default;
            };
            struct PerVertexData
            {
                glm::ivec2 offset;
                glm::uvec2 uv;

                PerVertexData() = default;
                PerVertexData(glm::ivec2 off,glm::uvec2 tex);
                PerVertexData(const PerVertexData&) = default;

                static void fromGlyphData(const GlyphData& data,PerVertexData* ret);
            };

            VertexBuffer buffer;
            IndicesBuffer indices;
            std::map<char32_t,Glyph> glyphs;
        protected:
            VertexArray vao;
            TextureAtlas2D atlas;
            FT_Face face;
        public:
            const unsigned size;

            unsigned getHeight()    const noexcept;
            unsigned getWidth()     const noexcept;

            Glyph& getGlyph(char32_t c);
            Glyph* findGlyph(char32_t c) const noexcept;

            void prepareGlyph(char32_t c);
            void prepareGlyphs(const std::vector<char32_t>& cs);

            void bindTexture();
            void bindBuffer();
            unsigned getOffsetOf(char32_t c) const noexcept;

            Access() = delete;
            Access(const Access & target) = delete;

            Access(unsigned _size,FT_Face _face);
            Access(Access&&) noexcept = default;

            virtual ~Access() = default;
        };

    protected:
        std::map<int,Access> sizes;
        FT_Face face;

    public:
        bool loadFromFile(const std::string& filename);

        Texture2D renderText(unsigned size, const std::string& text);

        Access & getAccess(unsigned size);

        Font();
        Font(const Font & target) = delete;
        Font(Font&&) noexcept = default;

        ~Font();
    };
    class _FreeTypeInitializer
    {
        _FreeTypeInitializer();
    public:
        static bool initialized;
        static FT_Library freetype;
        static _FreeTypeInitializer Guard;
        ~_FreeTypeInitializer();
    };
}

#endif // FONT_H
