#ifndef OBJECT_H
#define OBJECT_H

#include <ffge/basics.hpp>
#include <vector>
#include <memory>

namespace ffge
{
    class Object;

    class Transform
    {
    private:
        glm::vec3 position_,scale_;
        glm::quat rotation_;
        mutable bool dispersed_;
        mutable glm::mat4 transforms_;
        std::weak_ptr<Transform> parent_;
        std::vector<std::shared_ptr<Transform>> children;
        Object* object;
    public:
        Object& getObject();
        bool hasObject() const noexcept;
        std::weak_ptr<Transform> getParent() const;
        void setParent(const std::shared_ptr<Transform>& parent);

        glm::vec3 getPosition() const;
        glm::vec3 getScale() const;
        glm::quat getRotation() const;

        glm::mat4 getMatrix() const;
        glm::mat4 getLocalMatrix() const;

        void setPosition(const glm::vec3& p);
        void setScale(const glm::vec3& s);
        void setRotation(const glm::quat& r);

        void translate(const glm::vec3& t);
        void scale(const glm::vec3& s);
        void rotate(const glm::quat& r);

        void scaleRelativeTo(const glm::vec3& p,const glm::vec3& s);
        void rotateAround(const glm::vec3& p,const glm::quat& r);

        Transform() : position_() ,scale_(1) ,rotation_() ,dispersed_(false) ,transforms_(1), parent_(), object(nullptr) {}
        Transform(const Transform&) = default;
        Transform(Transform&&) noexcept = default;
    };

    class Object
    {
    private:
    public:
        std::weak_ptr<Object> getParent() const;

        Transform& transforms;

        std::shared_ptr<Drawable> drawable;
    };
}

#endif // OBJECT_H
