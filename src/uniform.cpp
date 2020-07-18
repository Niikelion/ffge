#include "uniform.hpp"

#include "ff_glew.hpp"

namespace ffge
{
    unsigned Uniform::getSlotByName(unsigned p,const std::string& a) const
    {
        return glGetUniformLocation(p,a.c_str());
    }
    Uniform& Uniform::operator = (float a)
    {
        if (slot)
            glUniform1f(slot,a);
        else
            glUniform1f(getSlotByName(_p,name),a);
        return *this;
    }
    Uniform& Uniform::operator = (const std::initializer_list<float>& a)
    {
        glUniform1fv(slot,static_cast<unsigned>(a.size()),a.begin());
        return *this;
    }
    Uniform& Uniform::operator = (const std::vector<float>& a)
    {
        glUniform1fv(slot,static_cast<unsigned>(a.size()),a.data());
        return *this;
    }

    Uniform& Uniform::operator = (const glm::vec2& a)
    {
        glUniform1fv(slot,2,glm::value_ptr(a));
        return *this;
    }
    Uniform& Uniform::operator = (const glm::vec3& a)
    {
        glUniform1fv(slot,3,glm::value_ptr(a));
        return *this;
    }
    Uniform& Uniform::operator = (const glm::vec4& a)
    {
        glUniform1fv(slot,4,glm::value_ptr(a));
        return *this;
    }

    Uniform& Uniform::operator = (const std::initializer_list<glm::vec2>& a)
    {
        if (a.size())
            glUniform2fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.begin()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::initializer_list<glm::vec3>& a)
    {
        if (a.size())
            glUniform3fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.begin()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::initializer_list<glm::vec4>& a)
    {
        if (a.size())
            glUniform4fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.begin()));
        return *this;
    }

    Uniform& Uniform::operator = (const std::vector<glm::vec2>& a)
    {
        if (a.size())
            glUniform2fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.data()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::vector<glm::vec3>& a)
    {
        if (a.size())
            glUniform3fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.data()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::vector<glm::vec4>& a)
    {
        if (a.size())
            glUniform4fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.data()));
        return *this;
    }

    Uniform& Uniform::operator = (const glm::mat3& a)
    {
        glUniformMatrix3fv(slot,1,false,glm::value_ptr(a));
        return *this;
    }
    Uniform& Uniform::operator = (const glm::mat4& a)
    {
        glUniformMatrix4fv(slot,1,false,glm::value_ptr(a));
        return *this;
    }

    Uniform& Uniform::operator = (const std::initializer_list<glm::mat3>& a)
    {
        if (a.size())
            glUniformMatrix3fv(slot,static_cast<unsigned>(a.size()),false,glm::value_ptr(*a.begin()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::initializer_list<glm::mat4>& a)
    {
        if (a.size())
            glUniformMatrix4fv(slot,static_cast<unsigned>(a.size()),false,glm::value_ptr(*a.begin()));
        return *this;
    }

    Uniform& Uniform::operator = (const std::vector<glm::mat3>& a)
    {
        if (a.size())
            glUniformMatrix3fv(slot,static_cast<unsigned>(a.size()),false,glm::value_ptr(*a.data()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::vector<glm::mat4>& a)
    {
        if (a.size())
            glUniformMatrix4fv(slot,static_cast<unsigned>(a.size()),false,glm::value_ptr(*a.data()));
        return *this;
    }

    void UniformBlock::bind(unsigned p,unsigned i,unsigned slot)
    {
        glUniformBlockBinding(p,i,slot);
    }

    void UniformBlock::bind(const Program& p,const std::string& name,unsigned slot)
    {
        glUniformBlockBinding(p.getID(),p.getUniformBlock(name),slot);
    }

    void UniformBlock::bind(const Program& p)
    {
        DataBuffer::bind();
        bind(p.getID(),p.getUniformBlock(name),slot);
    }
    void UniformBlock::bind(Program* p)
    {
        if (p != nullptr)
        {
            DataBuffer::bind();
            bind(p->getID(),p->getUniformBlock(name),slot);
        }
    }

    void UniformBlock::loadFromProgram(const Program& p)
    {
        Program::UniformLayout l = p.getUniformBlockLayout(name);
        slot = l.id;
        offsets = l.offsets;
    }
    void UniformBlock::layout(const std::vector<std::pair<std::string,unsigned>>& _offsets)
    {
        for(const auto& i:_offsets)
        {
            offsets[i.first] = i.second;
        }
    }


    void UniformBlock::sync()
    {
        glBindBufferBase(GL_UNIFORM_BUFFER,slot,id);
    }

    void UniformBlock::set(const std::string& name,unsigned size,void* var)
    {
        auto it = offsets.find(name);
        if (it != offsets.end())
        {
            subdata(it -> second,size,var);
        }
    }

    UniformBlock::UniformBlock()
    {
        type=GL_UNIFORM_BUFFER;
        slot=0;
        glGenBuffers(1,&id);
    }
    UniformBlock::UniformBlock(unsigned sl,const std::string& n)
    {
        slot = sl;
        name = n;
        type=GL_UNIFORM_BUFFER;
        glGenBuffers(1,&id);
    }

    UniformBlock::UniformBlock(unsigned sl,const std::string& n,bool)
    {
        slot = sl;
        name = n;
        type=GL_UNIFORM_BUFFER;
        id=0;
    }

    UniformBlock::~UniformBlock()
    {
        if (id)
            glDeleteBuffers(1,&id);
    }

    void UniformBlock::init()
    {
        glGenBuffers(1,&shared.id);
        shared.data(2*16*sizeof(float),nullptr,GL_DYNAMIC_DRAW);
        shared.sync();
    }

    UniformBlock UniformBlock::shared(0,"World",true);
}
