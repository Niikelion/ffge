#include "material.hpp"

namespace ffge
{
    void Material::use() const
    {
        if (program != Program::current())
            program->use();
        for (const auto& i:sources)
        {
            i -> bindUniform();
        }
    }
}
