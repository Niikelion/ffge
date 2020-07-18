#include "buffer.hpp"

#include "ff_glew.hpp"
#include <cstdint>

using namespace std;

namespace ffge
{
    DataBuffer& DataBuffer::operator = (DataBuffer&& t) noexcept
    {
        if (id)
            glDeleteBuffers(1,&id);
        id = t.id;
        t.id = 0;
        return *this;
    }
    void DataBuffer::data(size_t size,const void* d,unsigned short hint)
    {
        if (id)
        {
            bind();
            glBufferData(type,size,d,hint);
        }
    }

    void DataBuffer::subdata(size_t offset,size_t size,const void* d)
    {
        if (id)
        {
            bind();
            glBufferSubData(type,offset,size,d);
        }
    }

    void DataBuffer::bind() const
    {
        glBindBuffer(type,id);
    }

    void DataBuffer::bindAs(unsigned t) const
    {
        glBindBuffer(t,id);
    }

    void DataBuffer::bind(unsigned _id,unsigned _type)
    {
        glBindBuffer(_type,_id);
    }

    void DataBuffer::unbind() const
    {
        glBindBuffer(type,0);
    }
    void DataBuffer::unbind(unsigned t)
    {
        glBindBuffer(t,0);
    }

    DataBuffer::DataBuffer(DataBuffer&& t ) noexcept
    {
        id = t.id;
        type = t.type;
        t.id = 0;
    }

    VertexBuffer& VertexBuffer::operator= (VertexBuffer&& t) noexcept
    {
        DataBuffer::operator= (std::move(t));
        return *this;
    }

    void VertexBuffer::attrib(unsigned location,unsigned elements,unsigned type,bool normalized,unsigned stride,uint64_t offset)
    {
        if (id)
        {
            bind();
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location,elements,type,normalized,stride,(void*)offset);
            unbind();
        }
    }

    void VertexBuffer::disable(unsigned location)
    {
        glDisableVertexAttribArray(location);
    }

    VertexBuffer::VertexBuffer()
    {
        type = GL_ARRAY_BUFFER;
        glGenBuffers(1,&id);
    }

    VertexBuffer::~VertexBuffer()
    {
        if (id)
            glDeleteBuffers(1,&id);
    }

    IndicesBuffer& IndicesBuffer::operator= (IndicesBuffer&& t) noexcept
    {
        DataBuffer::operator= (std::move(t));
        subtype = t.subtype;
        return *this;
    }

    IndicesBuffer::IndicesBuffer()
    {
        type = GL_ELEMENT_ARRAY_BUFFER;
        subtype = 0;
        glGenBuffers(1,&id);
    }

    IndicesBuffer::~IndicesBuffer()
    {
        if (id)
            glDeleteBuffers(1,&id);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(id);
    }
    void VertexArray::bind(unsigned int target)
    {
        glBindVertexArray(target);
    }
    void VertexArray::unbind()
    {
        glBindVertexArray(0);
    }

    VertexArray::VertexArray()
    {
        glGenVertexArrays(1,&id);
    }
    VertexArray::VertexArray(VertexArray&& t) noexcept
    {
        id = t.id;
        t.id = 0;
    }
    VertexArray::~VertexArray()
    {
        if (id)
            glDeleteVertexArrays(1,&id);
    }

    void FrameBuffer::bind(unsigned _id)
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,_id);
    }
    void FrameBuffer::bind() const
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,id);
    }
    void FrameBuffer::bindAs(unsigned target) const
    {
        glBindFramebuffer(target,id);
    }
    void FrameBuffer::unbind(unsigned t)
    {
        glBindFramebuffer(t,0);
    }
    FrameBuffer::FrameBuffer()
    {
        id = 0;
        glGenFramebuffers(1,&id);
    }
    FrameBuffer::FrameBuffer(FrameBuffer&& t) noexcept
    {
        id = t.id;
        t.id = 0;
    }
    FrameBuffer::~FrameBuffer()
    {
        if (id)
            glDeleteFramebuffers(1,&id);
    }
}
