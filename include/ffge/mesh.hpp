#ifndef MESH_H
#define MESH_H

#include <map>
#include <vector>
#include <ffge/basics.hpp>
#include <ffge/buffer.hpp>
#include <ffge/uniform.hpp>
#include <ffge/material.hpp>
#include <ffge/shader.hpp>

namespace ffge
{
    class SimpleMesh: public Drawable
    {
    public:
        std::vector<VertexBuffer> buffers;
        IndicesBuffer indices;
        VertexArray binding;

        GLenum primitive;

        unsigned int count;

        std::shared_ptr<Material> material;

        virtual void draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const override;
    };

    class ComplexMesh:public Drawable
    {
    public:
        struct _binding_data
        {
            VertexArray binding;
            IndicesBuffer indices;
            unsigned int count;
            unsigned primitive;
            std::shared_ptr<Material> material;
        };
        std::vector<VertexBuffer> _buffers;
        std::vector<_binding_data> _bindings;
        virtual void draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const override;
    };

    typedef ComplexMesh MeshCollection;

    class InstancedSimpleMesh: public SimpleMesh
    {
    public:
        //
    };

    class InstancedComplexMesh: public ComplexMesh
    {
    public:
        //
    };
}

#endif // MESH_H
