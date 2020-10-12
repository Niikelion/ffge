#ifndef MATERIAL_H
#define MATERIAL_H

#include <ffge/uniform.hpp>
#include <ffge/shader.hpp>
#include <ffge/texture.hpp>
#include <memory>
#include <vector>

namespace ffge
{
    class Material
    {
    public:
        std::vector<std::shared_ptr<UniformBlock>> sources;
        std::vector<std::shared_ptr<Texture2D>> textures;
        Program* program;
        virtual void use() const;
    };
}

#endif // MATERIAL_H
