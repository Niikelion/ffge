#ifndef CAMERA_H
#define CAMERA_H

#include <ffge/object.hpp>

namespace ffge
{
    class Camera
    {
    public:
        Transform transforms;
        float fov,near,far;
        bool perspective;

        glm::mat4 getProjection(unsigned width, unsigned height) const;
        glm::mat4 getView() const;

        Camera() : fov(35.f), near(0.1f), far(100.f), perspective(true) {}
    };



    /*class Camera
    {
    protected:
        glm::mat4 _m;
    public:

        void reset();

        Camera& moveTo(const glm::vec3&);
        Camera& moveBy(const glm::vec3&);

        Camera& lookAt(const glm::vec3&,const glm::vec3& = glm::vec3(0,1,0));

        Camera& rotateBy(const glm::quat&);
        Camera& rotateBy(const glm::vec3&,float);
        Camera& scaleBy(const glm::vec3&);

        Camera movedBy(const glm::vec3&);
        Camera rotatedBy(const glm::quat&);

        Camera& fromMatrix(const glm::mat4&);

        const glm::mat4& getMatrix() const noexcept;
        void setMatrix(const glm::mat4&) noexcept;

        Camera();
        Camera(const Camera&) = default;
        Camera(Camera&&) noexcept = default;

        Camera(const glm::mat4&);

        virtual ~Camera() = default;
    };*/
}
#endif // CAMERA_H
