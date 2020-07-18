#ifndef MATERIAL_H
#define MATERIAL_H

#include <ffge/uniform.hpp>
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
        virtual void use(const Program& p) const;
    };
}

#endif // MATERIAL_H
