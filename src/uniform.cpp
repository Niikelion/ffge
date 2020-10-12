#include "uniform.hpp"

namespace ffge
{
    unsigned Uniform::getSlotByName(unsigned p,const std::string& a) const
    {
        return glGetUniformLocation(p,a.c_str());
    }
    Uniform::Uniform(unsigned slot)
    {
        this->slot = slot;
    }
    Uniform& Uniform::operator=(int a)
    {
        if (isValid())
            glUniform1i(slot, a);
        return *this;
    }
    Uniform& Uniform::operator=(unsigned int a)
    {
        if (isValid())
            glUniform1ui(slot, a);
        return *this;
    }
    Uniform& Uniform::operator = (float a)
    {
        if (isValid())
            glUniform1f(slot,a);
        return *this;
    }
    Uniform& Uniform::operator = (const std::initializer_list<float>& a)
    {
        if (isValid())
            glUniform1fv(slot,static_cast<unsigned>(a.size()),a.begin());
        return *this;
    }
    Uniform& Uniform::operator = (const std::vector<float>& a)
    {
        if (isValid())
            glUniform1fv(slot,static_cast<unsigned>(a.size()),a.data());
        return *this;
    }

    Uniform& Uniform::operator = (const glm::vec2& a)
    {
        if (isValid())
            glUniform1fv(slot,2,glm::value_ptr(a));
        return *this;
    }
    Uniform& Uniform::operator = (const glm::vec3& a)
    {
        if (isValid())
            glUniform3fv(slot,1,glm::value_ptr(a));
        return *this;
    }
    Uniform& Uniform::operator = (const glm::vec4& a)
    {
        if (isValid())
            glUniform4fv(slot,1,glm::value_ptr(a));
        return *this;
    }

    Uniform& Uniform::operator = (const std::initializer_list<glm::vec2>& a)
    {
        if (isValid())
            if (a.size())
                glUniform2fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.begin()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::initializer_list<glm::vec3>& a)
    {
        if (isValid() && a.size() > 0)
            glUniform3fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.begin()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::initializer_list<glm::vec4>& a)
    {
        if (isValid() && a.size() > 0)
            glUniform4fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.begin()));
        return *this;
    }

    Uniform& Uniform::operator = (const std::vector<glm::vec2>& a)
    {
        if (isValid() && a.size() > 0)
            glUniform2fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.data()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::vector<glm::vec3>& a)
    {
        if (isValid() && a.size() > 0)
            glUniform3fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.data()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::vector<glm::vec4>& a)
    {
        if (isValid() && a.size() > 0)
            glUniform4fv(slot,static_cast<unsigned>(a.size()),glm::value_ptr(*a.data()));
        return *this;
    }

    Uniform& Uniform::operator = (const glm::mat3& a)
    {
        if (isValid())
            glUniformMatrix3fv(slot,1,false,glm::value_ptr(a));
        return *this;
    }
    Uniform& Uniform::operator = (const glm::mat4& a)
    {
        if (isValid())
            glUniformMatrix4fv(slot,1,false,glm::value_ptr(a));
        return *this;
    }

    Uniform& Uniform::operator = (const std::initializer_list<glm::mat3>& a)
    {
        if (isValid() && a.size() > 0)
            glUniformMatrix3fv(slot,static_cast<unsigned>(a.size()),false,glm::value_ptr(*a.begin()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::initializer_list<glm::mat4>& a)
    {
        if (isValid() && a.size() > 0)
            glUniformMatrix4fv(slot,static_cast<unsigned>(a.size()),false,glm::value_ptr(*a.begin()));
        return *this;
    }

    Uniform& Uniform::operator = (const std::vector<glm::mat3>& a)
    {
        if (isValid() && a.size() > 0)
            glUniformMatrix3fv(slot,static_cast<unsigned>(a.size()),false,glm::value_ptr(*a.data()));
        return *this;
    }
    Uniform& Uniform::operator = (const std::vector<glm::mat4>& a)
    {
        if (isValid() && a.size() > 0)
            glUniformMatrix4fv(slot,static_cast<unsigned>(a.size()),false,glm::value_ptr(*a.data()));
        return *this;
    }

    void UniformBlock::loadLayout(const std::shared_ptr<Layout>& layout)
    {
        this->layout = layout;
        data(layout->size, nullptr, GL_DYNAMIC_DRAW);
    }

    std::shared_ptr<UniformBlock::Layout> UniformBlock::getLayout() const
    {
        return std::shared_ptr<Layout>();
    }

    void UniformBlock::bindUniform()
    {
        if (layout)
           glBindBufferBase(GL_UNIFORM_BUFFER,layout->getSlot(),id);
    }

    void UniformBlock::set(const std::string& name,unsigned size,void* var)
    {
        if (layout)
        {
            auto it = layout->offsets.find(name);
            if (it != layout->offsets.end())
            {
                subdata(it->second, size, var);
            }
        }
    }

    UniformBlock::UniformBlock()
    {
        type = GL_UNIFORM_BUFFER;
        glGenBuffers(1,&id);
    }

    UniformBlock::~UniformBlock()
    {
        if (id)
            glDeleteBuffers(1,&id);
    }
}
