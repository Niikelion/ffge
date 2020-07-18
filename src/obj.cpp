#include "obj.hpp"

#include <iostream>

namespace ffge
{
    std::vector<std::string>split(std::string s,const std::string& delimiter)
    {
        std::vector<std::string>ret;
        size_t pos = 0;
        std::string token;
        while ((pos = s.find(delimiter)) != std::string::npos)
        {
            token = s.substr(0, pos);
            ret.push_back(token);
            s.erase(0, pos + delimiter.length());
        }
        ret.push_back(s);
        return ret;
    }

    std::map<std::string,std::shared_ptr<Material>> loadMtl(const std::string& filename)
    {
        std::ifstream in;
        in.open(filename.c_str());

        std::map<std::string,std::shared_ptr<Material>> ret;

        std::string name,line;

        while (std::getline(in,line))
        {
            if (line.size())
            {
                std::vector<std::string>data = split(line," ");
                if (data.size())
                {
                    if (data[0] == "newmtl")
                    {
                        if (data.size() == 2)
                        {
                            name = data[1];
                            ret.emplace(name,std::shared_ptr<Material>(new Material));
                        }
                    }
                    else if (data[0] == "map_Kd")
                    {
                        if (data.size() == 2 && name.size())
                        {
                            ret[name] -> textures.emplace_back(std::shared_ptr<Texture2D>(new Texture2D()));
                            ret[name] -> textures.back()->loadFromFile(data[1]);
                        }
                    }
                    else if (data[0] == "Kd")
                    {
                        if (data.size() == 4 && name.size())
                        {
                            //create uniform and set data block
                        }
                    }
                }
            }
        }

        return ret;
    }

    ObjData loadObj(const std::string& filename)
    {
        std::ifstream in;
        in.open(filename.c_str());
        std::string line;

        ObjData ret;

        std::vector<glm::vec3>points;
        std::vector<glm::vec3>normals;
        std::vector<glm::vec2>uv;

        std::vector<ObjData::__mesh_data> meshes;

        std::string name;
        VertexArray::unbind();
        while (std::getline(in,line))
        {
            if (line.size())
            {
                std::vector<std::string>data = split(line," ");
                if (data.size())
                {
                    if (data[0] == "v")
                    {
                        glm::vec3 pos(0,0,0);
                        if (data.size() > 1)
                        {
                            pos.x = std::stof(data[1]);
                            if (data.size() > 2)
                            {
                                pos.y = std::stof(data[2]);
                                if (data.size() > 3)
                                    pos.z = std::stof(data[3]);
                            }
                        }
                        points.push_back(pos);
                    }
                    else if (data[0] == "vt")
                    {
                        glm::vec2 pos(0,0);
                        if (data.size() > 1)
                        {
                            pos.x = std::stof(data[1]);
                            if (data.size() > 2)
                            {
                                pos.y = std::stof(data[2]);
                            }
                        }
                        uv.push_back(pos);
                    }
                    else if (data[0] == "vn")
                    {
                        glm::vec3 pos(0,0,0);
                        if (data.size() > 1)
                        {
                            pos.x = std::stof(data[1]);
                            if (data.size() > 2)
                            {
                                pos.y = std::stof(data[2]);
                                if (data.size() > 3)
                                    pos.z = std::stof(data[3]);
                            }
                        }
                        normals.push_back(pos);
                    }
                    else if (data[0] == "f")
                    {
                        if (meshes.size() && data.size() == 4)
                        {
                            if (meshes.back().submeshes.size() == 0)
                            {
                                meshes.back().submeshes.emplace_back();
                                meshes.back().submeshes.back().material = name;
                            }
                            meshes.back().submeshes.back().faces.emplace_back();
                            for (unsigned i=1; i<data.size(); ++i)
                            {
                                std::vector<std::string>splitted = split(data[i],"/");
                                if (splitted.size() == 3)
                                {
                                    std::tuple<unsigned, unsigned, unsigned>tmp(
                                        std::stoi(splitted[0]),
                                        std::stoi(splitted[1]),
                                        std::stoi(splitted[2])
                                    );
                                    unsigned num=0;
                                    for (unsigned i=0; i<meshes.back().vertices.size(); ++i)
                                    {
                                        if (meshes.back().vertices[i] == tmp)
                                        {
                                            num = i+1;
                                            break;
                                        }
                                    }
                                    if (num == 0)
                                    {
                                        meshes.back().vertices.push_back(tmp);
                                        num = static_cast<unsigned>(meshes.back().vertices.size());
                                    }
                                    --num;
                                    meshes.back().submeshes.back().faces.back().push_back(num);
                                }
                            }
                        }
                    }
                    else if (data[0] == "o")
                    {
                        if (data.size() > 1)
                        {
                            meshes.push_back(ObjData::__mesh_data());
                            meshes.back().name = data[1];
                        }
                    }
                    else if (data[0] == "usemtl")
                    {
                        if (data.size() == 2 && meshes.size())
                        {
                            meshes.back().submeshes.emplace_back();
                            meshes.back().submeshes.back().material = data[1];
                            name = data[1];
                        }
                    }
                    else if (data[0] == "mtllib")
                    {
                        if (data.size() == 2)
                        {
                            //load external file
                            std::string fn = filename;
                            size_t d = fn.rfind("/");
                            if (d == std::string::npos)
                                fn.clear();
                            else
                                fn = fn.substr(0,d);
                            fn += "/";
                            fn += data[1];
                            std::map<std::string,std::shared_ptr<Material>>tmp = loadMtl(fn);
                            for (const auto& k:tmp)
                                ret.materials[k.first] = k.second;
                        }
                    }
                }
            }
        }
        std::vector<glm::vec3>pos,norm;
        std::vector<glm::vec2>uvs;
        std::vector<unsigned>indices;
        for (const auto& i:meshes)
        {
            auto it = ret.meshes.emplace(i.name,std::shared_ptr<ComplexMesh>(new ComplexMesh())).first;
            pos.clear();
            norm.clear();
            uvs.clear();
            for (const auto& j:i.vertices)
            {
                pos.emplace_back(points[std::get<0>(j)-1]);
                uvs.emplace_back(uv[std::get<1>(j)-1]);
                norm.emplace_back(normals[std::get<2>(j)-1]);
            }

            it -> second -> _buffers.emplace_back();
            it -> second -> _buffers.back().data(3*sizeof(float)*pos.size(),&pos[0],GL_STATIC_DRAW);

            it -> second -> _buffers.emplace_back();
            it -> second -> _buffers.back().data(2*sizeof(float)*uvs.size(),&uvs[0],GL_STATIC_DRAW);

            it -> second -> _buffers.emplace_back();
            it -> second -> _buffers.back().data(3*sizeof(float)*norm.size(),&norm[0],GL_STATIC_DRAW);

            for (const auto& j:i.submeshes)
            {
                it -> second -> _bindings.emplace_back();
                it -> second -> _bindings.back().binding.bind();

                //prepare indices for buffer and binding
                indices.clear();
                for (const auto& k:j.faces)
                {
                    for (unsigned z=0; z<k.size(); ++z)
                    {
                        indices.push_back(k[z]);
                    }
                }

                it -> second -> _bindings.back().count = static_cast<unsigned>(indices.size());
                it -> second -> _bindings.back().primitive = GL_TRIANGLES;

                it -> second -> _bindings.back().indices.data(sizeof(unsigned)*indices.size(),indices.data(),GL_STATIC_DRAW);
                it -> second -> _bindings.back().indices.subtype = (unsigned)dataTypes::Uint;

                it -> second -> _buffers[0].attrib((unsigned)Attribs::position,3,(unsigned)dataTypes::Float,false,3*sizeof(float),0);
                it -> second -> _buffers[1].attrib((unsigned)Attribs::texcoord,2,(unsigned)dataTypes::Float,false,2*sizeof(float),0);
                it -> second -> _buffers[2].attrib((unsigned)Attribs::normal,3,(unsigned)dataTypes::Float,false,3*sizeof(float),0);
            }
        }
        return ret;
    }
}
