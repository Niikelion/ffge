#include "mesh.hpp"

namespace ffge
{
    void SimpleMesh::draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const
    {
        UniformBlock::shared.set("model",m);

        binding.bind();
        if (material && Program::current() != nullptr)
            material -> use(*Program::current());
        glDrawElements(primitive,count,indices.subtype,(void*)0);
    }

    void ComplexMesh::draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const
    {
        UniformBlock::shared.set("model",m);

        Program* pp = Program::current();
        for (const auto& i: _bindings)
        {
            i.binding.bind();
            if (i.material && pp != nullptr)
                i.material -> use(*pp);
            glDrawElements(i.primitive,i.count,i.indices.subtype,(void*)0);
        }
    }
}
