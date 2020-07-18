#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <ffge/buffer.hpp>
#include <ffge/shader.hpp>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace ffge
{
    class Uniform
    {
    protected:
        Uniform(unsigned p,const std::string& name);
        unsigned _p;
    public:
        std::string name;
        unsigned slot;

        Uniform& operator = (float);
        Uniform& operator = (const std::initializer_list<float>&);
        Uniform& operator = (const std::vector<float>&);

        Uniform& operator = (const glm::vec2&);
        Uniform& operator = (const glm::vec3&);
        Uniform& operator = (const glm::vec4&);

        Uniform& operator = (const std::initializer_list<glm::vec2>&);
        Uniform& operator = (const std::initializer_list<glm::vec3>&);
        Uniform& operator = (const std::initializer_list<glm::vec4>&);

        Uniform& operator = (const std::vector<glm::vec2>&);
        Uniform& operator = (const std::vector<glm::vec3>&);
        Uniform& operator = (const std::vector<glm::vec4>&);

        Uniform& operator = (const glm::mat3&);
        Uniform& operator = (const glm::mat4&);

        Uniform& operator = (const std::initializer_list<glm::mat3>&);
        Uniform& operator = (const std::initializer_list<glm::mat4>&);

        Uniform& operator = (const std::vector<glm::mat3>&);
        Uniform& operator = (const std::vector<glm::mat4>&);

        unsigned getSlotByName(unsigned p,const std::string& a) const;

        Uniform();
        Uniform(unsigned p);

        Uniform(const Uniform&) = default;
        Uniform(Uniform&&) noexcept = default;
    };

    class UniformBlock: public DataBuffer
    {
    protected:
        unsigned _binding;
        std::map<std::string,unsigned> offsets;
    public:
        static UniformBlock shared;

        unsigned slot;
        std::string name;

        static void bind(unsigned p,unsigned i,unsigned slot);
        static void bind(const Program& p, const std::string& name, unsigned slot);
        void bind(const Program& p);
        void bind(Program* p);

        void loadFromProgram(const Program& p);
        void layout(const std::vector<std::pair<std::string,unsigned>>& _offsets);

        void sync();

        void set(const std::string& name,unsigned size,void* var);
        template<typename T>inline void set(const std::string& name,T& var)
        {
            set(name,sizeof(T),const_cast<void*>(reinterpret_cast<const void*>(&var)));
        }

        static void init();

        UniformBlock();
        UniformBlock(unsigned sl,const std::string& n);
        UniformBlock(unsigned sl,const std::string& n,bool);
        UniformBlock(const UniformBlock&) = delete;
        UniformBlock(UniformBlock&&) noexcept = default;

        ~UniformBlock();
    };
}

#endif // UNIFORMS_H
