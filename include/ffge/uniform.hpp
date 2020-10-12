#ifndef UNIFORMS_H
#define UNIFORMS_H

#include <ffge/ff_glew.hpp>
#include <ffge/buffer.hpp>
#include <string>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <map>
#include <memory>

namespace ffge
{
    class Uniform
    {
    public:

        Uniform& operator = (int a);
        Uniform& operator = (unsigned int a);
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

        inline bool isValid() const
        {
            return slot >= 0;
        }

        Uniform& operator = (const Uniform&) = default;

        Uniform() : slot(-1) {}

        Uniform(unsigned slot);

        Uniform(const Uniform&) = default;
    private:
        int slot;
    };

    class UniformBlock: public DataBuffer
    {
    public:
        //TODO: store in shared ptr and store binded slot
        struct Layout
        {
            unsigned id;
            size_t size;
            std::map<std::string, unsigned> offsets;

            inline int getProgramId() const
            {
                return program;
            }

            inline void bindToSlot(unsigned slot)
            {
                this->slot = slot;
                glUniformBlockBinding(program, id, slot);
            }

            inline int getSlot()
            {
                return slot;
            }

            Layout& operator = (const Layout&) = default;

            Layout() : id(0), size(0), program(0), slot(-1) {}
            Layout(int p, int i, size_t siz, int s = 0) : id(i), size(siz), program(p), slot(s)
            {
                if (slot >= 0)
                {
                    bindToSlot(s);
                }
            }
        private:
            int program;
            int slot;
        };

        void loadLayout(const std::shared_ptr<Layout>& layout);
        std::shared_ptr<Layout> getLayout() const;

        void bindUniform();

        void set(const std::string& name,unsigned size,void* var);
        template<typename T>inline void set(const std::string& name,const T& var)
        {
            set(name,sizeof(T),const_cast<void*>(reinterpret_cast<const void*>(&var)));
        }

        UniformBlock();
        UniformBlock(const std::shared_ptr<Layout>& l) : UniformBlock()
        {
            layout = l;
        }

        UniformBlock(const UniformBlock&) = delete;
        UniformBlock(UniformBlock&&) noexcept = default;

        ~UniformBlock();

    protected:
        std::shared_ptr<Layout> layout;
    };
}

#endif // UNIFORMS_H
