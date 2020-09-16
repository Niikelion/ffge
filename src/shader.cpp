#include "shader.hpp"

#include "ff_glew.hpp"
#include <iostream>

namespace ffge
{
    unsigned Shader::mapTypeToGL(unsigned type)
    {
        switch (type)
        {
            case Types::Fragment:               return GL_FRAGMENT_SHADER;
            case Types::Vertex:                 return GL_VERTEX_SHADER;
            case Types::Geometry:               return GL_GEOMETRY_SHADER;
            case Types::Compute:                return GL_COMPUTE_SHADER;
            case Types::TessellactionEvaluation:return GL_TESS_EVALUATION_SHADER;
            case Types::TessellationControll:   return GL_TESS_CONTROL_SHADER;
            default:                            return 0;
        }
    }

    Shader::Shader()
    {
        _sid = 0;
        _type = 0;
    }

    Shader::~Shader()
    {
        if (_sid != 0)
        {
            destroy();
        }
    }

    void Shader::fromString(unsigned type,const std::string& source)
    {
        type = mapTypeToGL(type);
        if (_sid)
            destroy();
        unsigned shader = glCreateShader (type);

        const char * ptmp = source.c_str();
        glShaderSource (shader, 1, &ptmp, NULL);

        int status;
        glCompileShader (shader);
        glGetShaderiv (shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            int infoLogLength;
            glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infoLogLength);
            char *infoLog = new GLchar[infoLogLength];
            glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog);

            //error log
            //cerr<<"Compile log: "<<infoLog<<endl;


            delete [] infoLog;
        }
        _sid = shader;
        _type = type;
    }

    void Shader::fromFile(unsigned type, const std::string& filename)
    {
        std::ifstream fp;
        fp.open(filename.c_str(), std::ios::in);
        if(fp.is_open())
        {
            std::string line, buffer;
            while(getline(fp, line))
            {
                buffer.append(line);
                buffer.append("\r\n");
            }
            fromString(type, buffer);
        }
        else
        {
            //error
            //cerr<<"Blad wczytywania shadera: "<<filename<<endl;
        }
    }

    unsigned Shader::getID() const noexcept
    {
        return _sid;
    }

    void Shader::destroy()
    {
        glDeleteShader(_sid);
        _sid = 0;
    }

    void Program::create()
    {
        if (!_program)
        {
            _program = glCreateProgram();
        }
    }

    void Program::attach(Shader& s)
    {
        auto i = std::find_if(shaders.begin(),shaders.end(),[&s](const std::shared_ptr<Shader>& a){return s.getID() == a->getID();});
        if (i != shaders.end())
            return;
        shaders.emplace_back(&s,[](Shader*){/**/});
        glAttachShader(_program,s.getID());
    }

    void Program::attach(Shader* s)
    {
        auto i = std::find_if(shaders.begin(),shaders.end(),[s](const std::shared_ptr<Shader>& a){return s->getID() == a->getID();});
        if (i != shaders.end())
            return;
        shaders.emplace_back(s,[](Shader*){/**/});
        glAttachShader(_program,s->getID());
    }

    void Program::detach(Shader* s)
    {
        glDetachShader(_program, s->getID());
    }

    #define type_mask(mac) std::make_pair(mac,#mac)

    void Program::link()
    {
        /*
        std::map<unsigned,std::string>type_masks = {
            type_mask(GL_FLOAT),
            type_mask(GL_FLOAT_VEC2),
            type_mask(GL_FLOAT_VEC3),
            type_mask(GL_FLOAT_VEC4),
            type_mask(GL_DOUBLE),
            type_mask(GL_DOUBLE_VEC2),
            type_mask(GL_DOUBLE_VEC3),
            type_mask(GL_DOUBLE_VEC4),
            type_mask(GL_INT),
            type_mask(GL_INT_VEC2),
            type_mask(GL_INT_VEC3),
            type_mask(GL_INT_VEC4),
            type_mask(GL_UNSIGNED_INT),
            type_mask(GL_UNSIGNED_INT_VEC2),
            type_mask(GL_UNSIGNED_INT_VEC3),
            type_mask(GL_UNSIGNED_INT_VEC4),
            type_mask(GL_BOOL),
            type_mask(GL_BOOL_VEC2),
            type_mask(GL_BOOL_VEC3),
            type_mask(GL_BOOL_VEC4),
            type_mask(GL_FLOAT_MAT2),
            type_mask(GL_FLOAT_MAT3),
            type_mask(GL_FLOAT_MAT4),
            type_mask(GL_FLOAT_MAT2x3),
            type_mask(GL_FLOAT_MAT2x4),
            type_mask(GL_FLOAT_MAT3x2),
            type_mask(GL_FLOAT_MAT3x4),
            type_mask(GL_FLOAT_MAT4x2),
            type_mask(GL_FLOAT_MAT4x3),
            type_mask(GL_DOUBLE_MAT2),
            type_mask(GL_DOUBLE_MAT3),
            type_mask(GL_DOUBLE_MAT4),
            type_mask(GL_DOUBLE_MAT2x3),
            type_mask(GL_DOUBLE_MAT2x4),
            type_mask(GL_DOUBLE_MAT3x2),
            type_mask(GL_DOUBLE_MAT3x4),
            type_mask(GL_DOUBLE_MAT4x2),
            type_mask(GL_DOUBLE_MAT4x3),
            type_mask(GL_SAMPLER_1D),
            type_mask(GL_SAMPLER_2D),
            type_mask(GL_SAMPLER_3D),
            type_mask(GL_SAMPLER_CUBE),
            type_mask(GL_SAMPLER_1D_SHADOW),
            type_mask(GL_SAMPLER_2D_SHADOW),
            type_mask(GL_SAMPLER_1D_ARRAY),
            type_mask(GL_SAMPLER_2D_ARRAY),
            type_mask(GL_SAMPLER_1D_ARRAY_SHADOW),
            type_mask(GL_SAMPLER_2D_ARRAY_SHADOW),
            type_mask(GL_SAMPLER_2D_MULTISAMPLE),
            type_mask(GL_SAMPLER_2D_MULTISAMPLE_ARRAY),
            type_mask(GL_SAMPLER_CUBE_SHADOW),
            type_mask(GL_SAMPLER_BUFFER),
            type_mask(GL_SAMPLER_2D_RECT),
            type_mask(GL_SAMPLER_2D_RECT_SHADOW),
            type_mask(GL_INT_SAMPLER_1D),
            type_mask(GL_INT_SAMPLER_2D),
            type_mask(GL_INT_SAMPLER_3D),
            type_mask(GL_INT_SAMPLER_CUBE),
            type_mask(GL_INT_SAMPLER_1D_ARRAY),
            type_mask(GL_INT_SAMPLER_2D_ARRAY),
            type_mask(GL_INT_SAMPLER_2D_MULTISAMPLE),
            type_mask(GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY),
            type_mask(GL_INT_SAMPLER_BUFFER),
            type_mask(GL_INT_SAMPLER_2D_RECT),
            type_mask(GL_UNSIGNED_INT_SAMPLER_1D),
            type_mask(GL_UNSIGNED_INT_SAMPLER_2D),
            type_mask(GL_UNSIGNED_INT_SAMPLER_3D),
            type_mask(GL_UNSIGNED_INT_SAMPLER_CUBE),
            type_mask(GL_UNSIGNED_INT_SAMPLER_1D_ARRAY),
            type_mask(GL_UNSIGNED_INT_SAMPLER_2D_ARRAY),
            type_mask(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE),
            type_mask(GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY),
            type_mask(GL_UNSIGNED_INT_SAMPLER_BUFFER),
            type_mask(GL_UNSIGNED_INT_SAMPLER_2D_RECT)
        };
        */
        int status;
        glLinkProgram (_program);
        glGetProgramiv (_program, GL_LINK_STATUS, &status);
        if (status == GL_FALSE)
        {
            int infoLogLength;

            glGetProgramiv (_program, GL_INFO_LOG_LENGTH, &infoLogLength);
            char *infoLog = new GLchar[infoLogLength];
            glGetProgramInfoLog (_program, infoLogLength, NULL, infoLog);

            delete [] infoLog;
        }
        //
        int count = 0,size = 0;
        unsigned type;

        constexpr GLsizei bfs = 32;

        char name[bfs];

        GLsizei length;
        //get attributes
        glGetProgramiv(_program,GL_ACTIVE_ATTRIBUTES,&count);
        for (int i=0; i<count; ++i)
        {
            glGetActiveAttrib(_program,static_cast<unsigned>(i),bfs,&length,&size,&type,name);
            _attributeList[std::string(name)] = glGetAttribLocation(_program,const_cast<const char*>(name));
        }
        glGetProgramiv(_program,GL_ACTIVE_UNIFORMS,&count);
        for (int i=0; i<count; ++i)
        {
            glGetActiveUniform(_program,static_cast<unsigned>(i),bfs,&length,&size,&type,name);
            int ii = glGetUniformLocation(_program,name);
            if (ii >= 0)
                _uniformLocationList[std::string(name)] = ii;
        }
        glGetProgramiv(_program,GL_ACTIVE_UNIFORM_BLOCKS,&count);
        for (int i=0; i<count; ++i)
        {
            UniformLayout l;
            glGetActiveUniformBlockName(_program,static_cast<unsigned>(i),bfs,&length,name);
            l.id = glGetUniformBlockIndex(_program,const_cast<const char*>(name));

            int res;
            glGetActiveUniformBlockiv(_program,l.id,GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,&res);

            std::vector<int> vars;
            vars.resize(res);
            glGetActiveUniformBlockiv(_program,l.id,GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,vars.data());

            char name2[bfs];

            std::vector<int>offsets;
            offsets.resize(res);
            //int types[res];
            //glGetActiveUniformsiv(_program,res,reinterpret_cast<unsigned*>(vars),GL_UNIFORM_TYPE,types);
            glGetActiveUniformsiv(_program,res,reinterpret_cast<unsigned*>(vars.data()),GL_UNIFORM_OFFSET,offsets.data());
            for (unsigned j = 0; j<static_cast<unsigned>(res); ++j)
            {

                glGetActiveUniformName(_program,static_cast<unsigned>(j),bfs,&length,name2);
                l.offsets[std::string(name2)] = offsets[j];
                //std::cout << "  " << name2 << " : " << type_masks[types[j]] << " offset: " << offsets[j] << std::endl;
            }
            _uniformblockList[std::string(name)] = l;
        }
    }

    void Program::destroy()
    {
        glDeleteProgram(_program);
        _program = 0;
    }

    void Program::use(unsigned p)
    {
        glUseProgram(p);
    }

    void Program::use() const
    {
        _current = const_cast<Program*>(this);
        glUseProgram(_program);
    }

    unsigned Program::getID() const noexcept
    {
        return _program;
    }

    void Program::bindAttributeName(unsigned program,unsigned id,const std::string& name)
    {
        glBindAttribLocation(program,id,name.c_str());
    }
    void Program::bindAttributeName(unsigned id,const std::string& name)
    {
        bindAttributeName(_program,id,name);
    }

    Program* Program::current()
    {
        return _current;
    }

    int Program::getUniform(const std::string& uniform) const
    {
        auto it = _uniformLocationList.find(uniform);
        if (it != _uniformLocationList.end())
            return it -> second;
        return -1;
    }
    int Program::getAttribute(const std::string& attribute) const
    {
        auto it = _attributeList.find(attribute);
        if (it != _attributeList.end())
            return it -> second;
        return -1;
    }
    int Program::getUniformBlock(const std::string& c) const
    {
        auto it = _uniformblockList.find(c);
        if (it != _uniformblockList.end())
            return it -> second.id;
        return -1;
    }

    Program::UniformLayout Program::getUniformBlockLayout(const std::string& block) const
    {
        auto it = _uniformblockList.find(block);
        if (it != _uniformblockList.end())
            return it -> second;
        return UniformLayout();
    }

    Program::Program()
    {
        _program = 0;
    }

    Program* Program::_current = nullptr;
}
