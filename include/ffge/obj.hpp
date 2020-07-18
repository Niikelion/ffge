#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <ffge/mesh.hpp>
#include <map>
#include <fstream>
#include <string>
#include <glm/glm.hpp>

namespace ffge
{
    struct ObjData
    {
        struct __material_data
        {
            //
        };
        struct __submesh_data
        {
            std::string material;
            std::vector<std::vector<unsigned>>faces;
        };
        struct __mesh_data
        {
            std::string name;
            std::vector<std::tuple<unsigned,unsigned,unsigned>>vertices;
            std::vector<__submesh_data> submeshes;
        };

        std::map<std::string,std::shared_ptr<Material> >materials;
        std::map<std::string,std::shared_ptr<ComplexMesh> >meshes;
    };
    ObjData loadObj(const std::string& filename);
    std::map<std::string,std::shared_ptr<Material>> loadMtl(const std::string& filename);
}

#endif // MESHLOADER_H
