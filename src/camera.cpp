#include "camera.hpp"

#include <utility>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace ffge
{
    using namespace glm;


    glm::mat4 Camera::getProjection(unsigned width, unsigned height) const
    {
        if (perspective)
        {
            return glm::perspective(fov, static_cast<float>(width) / height, near, far);
        }
        return glm::ortho(0.f,static_cast<float>(width),static_cast<float>(height),0.f);
    }

    glm::mat4 Camera::getView() const
    {
        return glm::inverse(transforms.getMatrix());
    }

    /*void Camera::reset()
    {
        _m = std::move(mat4(1));
    }

    Camera& Camera::moveTo(const vec3& pos)
    {
        _m[3] = std::move(vec4(pos,1));
        return *this;
    }
    Camera& Camera::moveBy(const vec3& v)
    {
        _m = std::move(translate(_m,-v));
        return *this;
    }

    Camera& Camera::lookAt(const vec3& pos,const vec3& up)
    {
        _m = std::move(glm::lookAt(vec3(_m[3]),pos,up));
        return *this;
    }

    Camera& Camera::rotateBy(const quat& rot)
    {
        _m = std::move(rotate(_m,angle(rot),axis(rot)));
        return *this;
    }

    Camera& Camera::rotateBy(const vec3& axis,float angle)
    {
        _m = std::move(rotate(_m,angle,axis));
        return *this;
    }

    Camera& Camera::scaleBy(const vec3& s)
    {
        _m = std::move(scale(_m,s));
        return *this;
    }

    Camera Camera::movedBy(const vec3& v)
    {
        return std::move(Camera(translate(_m,-v)));
    }
    Camera Camera::rotatedBy(const quat& rot)
    {
        return std::move(Camera(rotate(_m,angle(rot),axis(rot))));
    }

    Camera& Camera::fromMatrix(const mat4& m)
    {
        _m = m;
        return *this;
    }

    const mat4& Camera::getMatrix() const noexcept
    {
        return _m;
    }

    Camera::Camera()
    {
        _m = mat4(1);
    }

    Camera::Camera(const mat4& m)
    {
        _m = m;
    }*/
}
