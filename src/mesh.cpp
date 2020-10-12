#include "mesh.hpp"

namespace ffge
{
    void SimpleMesh::draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const
    {
        binding.bind();
        if (material && Program::current() != nullptr)
            material -> use();
        glDrawElements(primitive,count,indices.subtype,(void*)0);
    }

    void ComplexMesh::draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const
    {
        for (const auto& i: _bindings)
        {
            i.binding.bind();
            if (i.material)
                i.material -> use();
            glDrawElements(i.primitive,i.count,i.indices.subtype,(void*)0);
        }
    }
}
