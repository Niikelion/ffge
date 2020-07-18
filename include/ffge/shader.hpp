#ifndef SHADER_H
#define SHADER_H

#include <map>
#include <vector>
#include <fstream>
#include <string>
#include <memory>
#include <algorithm>

namespace ffge
{
    class Shader
    {
    protected:
        unsigned _sid, _type;
        static unsigned mapTypeToGL(unsigned type);
    public:
        enum Types
        {
            Unknown = 0,
            Vertex,
            Fragment,
            Geometry,
            Compute,
            TessellationControll,
            TessellactionEvaluation
        };

        void fromString(unsigned type, const std::string& source);
        void fromFile(unsigned type, const std::string& filename);

        void destroy();

        unsigned getID() const noexcept;

        Shader();
        Shader(const Shader&) = delete;
        Shader(Shader&&) noexcept = default;
        ~Shader();
    };

    class Program
    {
    public:
        struct UniformLayout
        {
            unsigned id;
            std::map<std::string,unsigned> offsets;
            UniformLayout(): id(0) {}
        };
    protected:
        unsigned _program;
        std::vector<std::shared_ptr<Shader> > shaders;
        std::map<std::string,unsigned> _attributeList;
        std::map<std::string,unsigned> _uniformLocationList;
        std::map<std::string,UniformLayout> _uniformblockList;

        static Program* _current;

    public:
        void create();
        void attach(Shader&);
        void attach(Shader*);
        void detach(Shader*);
        void link();
        void destroy();

        static void use(unsigned p);
        void use() const;

        static void bindAttributeName(unsigned program,unsigned id,const std::string& name);
        void bindAttributeName(unsigned id,const std::string& name);

        static Program* current();

        unsigned getID() const noexcept;

        int getUniform(const std::string& uniform) const;
        int getAttribute(const std::string& attribute) const;
        int getUniformBlock(const std::string& block) const;
        UniformLayout getUniformBlockLayout(const std::string& block) const;

        Program();
    };
}

#endif // SHADER_H
