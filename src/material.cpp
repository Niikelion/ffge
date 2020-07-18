#include "material.hpp"

namespace ffge
{
    void Material::use(const Program& p) const
    {
        for (const auto& i:sources)
        {
            i -> bind(p);
        }
    }
}
