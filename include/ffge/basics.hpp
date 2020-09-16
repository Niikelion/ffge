#ifndef BASICS_H
#define BASICS_H

#include <algorithm>
#include <functional>
#include <initializer_list>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <ffge/ff_glew.hpp>

namespace ffge
{
    struct dataTypes
    {
        enum DATATYPES
        {
            Byte = GL_BYTE,
            Ubyte = GL_UNSIGNED_BYTE,
            Short = GL_SHORT,
            Ushort = GL_UNSIGNED_SHORT,
            Int = GL_INT,
            Uint = GL_UNSIGNED_INT,
            Fixed = GL_FIXED,
            Float = GL_FLOAT,
            HalfFloat = GL_HALF_FLOAT,
            Double = GL_DOUBLE
        };
    };

    struct Drawable   //abstract base for drawable objects
    {
        virtual void draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const = 0;
        virtual ~Drawable() = default;
    };

    struct Attributes
    {
        enum ATTRIBUTES
        {
            position = 0,
            normal = 1,
            texcoord = 2,
            color = 3,
            user = 4
        };
    };

    typedef Attributes Attribs;

    template<typename T,typename __T = typename std::enable_if<std::is_arithmetic<T>::value>::type > class Rect
    {
    public:
        T x,y,width,height;

        template<typename T1> bool intersects(const Rect<T1>& t) const noexcept
        {
            return  ( (x <= t.x && x+width >= t.x)  || (x <= t.x+t.width && x+width >= t.x+t.width) ) &&
                    ( (y <= t.y && y+height >= t.y) || (y <= t.y+t.height && y+height >= t.y+t.height) );
        }

        template<typename T1> Rect<T>& operator = (const Rect<T1>& t) noexcept
        {
            x = t.x;
            y = t.y;
            width = t.width;
            height = t.height;
        }

        Rect& operator = (const Rect&) noexcept = default;
        Rect& operator = (Rect&& t) noexcept = default;

        Rect():  x(0),
                    y(0),
                    width(0),
                    height(0)
                    {/**/}

        template<typename T1>Rect(const Rect<T1>& t) noexcept:   x(t.x),
                                                                    y(t.y),
                                                                    width(t.width),
                                                                    height(t.height)
                                                                    {/**/}
        Rect(const Rect&) noexcept = default;
        Rect(Rect &&) noexcept = default;
        virtual ~Rect() = default;

        Rect(T _x,T _y,T _width,T _height):x(_x),y(_y),width(_width),height(_height){/**/}
    };
    typedef Rect<short>             SRect;
    typedef Rect<unsigned short>    SIRect;
    typedef Rect<int>               IRect;
    typedef Rect<unsigned>          UIRect;

    typedef Rect<float>             FRect;
    typedef Rect<double>            DRect;
}

#endif // BASICS_H
