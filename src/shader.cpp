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


            std::vector<char> infoLog(infoLogLength);
            glGetShaderInfoLog (shader, infoLogLength, NULL, infoLog.data());

            std::string err(infoLog.data());
            
            throw std::logic_error(err);
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
            throw std::logic_error("Could not open file "+filename);
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
                _uniformLocationList[std::string(name)] = Uniform(ii);
        }
        glGetProgramiv(_program,GL_ACTIVE_UNIFORM_BLOCKS,&count);
        for (int i=0; i<count; ++i)
        {
            glGetActiveUniformBlockName(_program, static_cast<unsigned>(i), bfs, &length, name);
            int id = glGetUniformBlockIndex(_program, const_cast<const char*>(name));
            int size = 0;
            glGetActiveUniformBlockiv(_program, id, GL_UNIFORM_BLOCK_DATA_SIZE, &size);
            UniformBlock::Layout l(
                _program,
                id,
                size
            );
            int res;
            glGetActiveUniformBlockiv(_program,l.id,GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS,&res);

            std::vector<int> vars;
            vars.resize(res);
            glGetActiveUniformBlockiv(_program,l.id,GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES,vars.data());

            char name2[bfs];

            std::vector<int>offsets;
            offsets.resize(res);
            glGetActiveUniformsiv(_program,res,reinterpret_cast<unsigned*>(vars.data()),GL_UNIFORM_OFFSET,offsets.data());
            for (unsigned j = 0; j<static_cast<unsigned>(res); ++j)
            {

                glGetActiveUniformName(_program,static_cast<unsigned>(vars[j]),bfs,&length,name2);
                l.offsets[std::string(name2)] = offsets[j];
            }
            _uniformblockList[std::string(name)] = std::make_shared<UniformBlock::Layout>(l);
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

    Uniform Program::getUniform(const std::string& uniform) const
    {
        auto it = _uniformLocationList.find(uniform);
        if (it != _uniformLocationList.end())
            return it -> second;
        return Uniform();
    }
    int Program::getAttribute(const std::string& attribute) const
    {
        auto it = _attributeList.find(attribute);
        if (it != _attributeList.end())
            return it -> second;
        return -1;
    }

    std::shared_ptr<UniformBlock::Layout> Program::getUniformBlockLayout(const std::string& block) const
    {
        auto it = _uniformblockList.find(block);
        if (it != _uniformblockList.end())
            return it -> second;
        return std::shared_ptr<UniformBlock::Layout>();
    }

    Program::Program()
    {
        _program = 0;
    }

    Program* Program::_current = nullptr;
}
