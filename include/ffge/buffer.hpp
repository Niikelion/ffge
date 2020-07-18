#ifndef BUFFERS_H
#define BUFFERS_H

#include <utility>
#include <cstdint>

namespace ffge
{
    class DataBuffer
    {
    protected:
        unsigned type;
    public:
        unsigned id;

        DataBuffer& operator = (const DataBuffer&) = default;
        DataBuffer& operator = (DataBuffer&&) noexcept;

        void data(size_t size,const void* d,unsigned short hint);

        void subdata(size_t offset,size_t size,const void* d);

        static void bind(unsigned _id,unsigned _type);
        static void unbind(unsigned t);

        void bind() const;
        void unbind() const;
        void bindAs(unsigned t) const;

        unsigned getType() const noexcept;

        DataBuffer() = default;
        DataBuffer(const DataBuffer&) = delete;
        DataBuffer(DataBuffer&&) noexcept;
        ~DataBuffer() = default;
    };

    class VertexBuffer final: public DataBuffer
    {
    public:
        VertexBuffer& operator= (VertexBuffer&& t) noexcept;

        void attrib(unsigned location,unsigned elements,unsigned type,bool normalized = false,unsigned stride = 0,std::uint64_t offset = 0);

        static void disable(unsigned location);

        VertexBuffer();
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer(VertexBuffer&& t) noexcept:DataBuffer(std::move(t)) {};
        ~VertexBuffer();
    };

    class IndicesBuffer final: public DataBuffer
    {
    public:
        IndicesBuffer& operator= (IndicesBuffer&& t) noexcept;
        unsigned short subtype;

        IndicesBuffer();
        IndicesBuffer(const IndicesBuffer&) = delete;
        IndicesBuffer(IndicesBuffer&& t) noexcept: DataBuffer(std::move(t)), subtype(0) {};
        ~IndicesBuffer();
    };

    class VertexArray final
    {
    public:
        unsigned id;

        void bind() const;
        static void bind(unsigned id);
        static void unbind();

        VertexArray();
        VertexArray(const VertexArray&) = delete;
        VertexArray(VertexArray&&) noexcept;
        ~VertexArray();
    };

    class FrameBuffer
    {
    public:
        unsigned id;

        void bind() const;
        void bindAs(unsigned target) const;
        static void bind(unsigned _id);
        static void unbind(unsigned t);

        FrameBuffer();
        FrameBuffer(const FrameBuffer&) = delete;
        FrameBuffer(FrameBuffer&& t) noexcept;
        ~FrameBuffer();
    };
}

#endif // BUFFERS_H
