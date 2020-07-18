#include "object.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <stdexcept>

namespace ffge
{
    Object& Transform::getObject()
    {
        if (object == nullptr)
            throw std::logic_error("object does not exist");
        return *object;
    }
    bool Transform::hasObject() const noexcept
    {
        return object!=nullptr;
    }
    std::weak_ptr<Transform> Transform::getParent() const
    {
        return parent_;
    }

    void Transform::setParent(const std::shared_ptr<Transform>& parent)
    {
        parent_ = parent;
    }

    glm::vec3 Transform::getPosition() const
    {
        return position_;
    }
    glm::vec3 Transform::getScale() const
    {
        return scale_;
    }
    glm::quat Transform::getRotation() const
    {
        return rotation_;
    }

    glm::mat4 Transform::getMatrix() const
    {
        if (auto ptr = parent_.lock())
        {
            return ptr->getMatrix() * getLocalMatrix();
        }
        return getLocalMatrix();
    }

    glm::mat4 Transform::getLocalMatrix() const
    {
        if (dispersed_)
        {
            transforms_ = glm::scale(glm::mat4(1),scale_);
            transforms_ = glm::translate(transforms_,position_);
            transforms_ *= glm::mat4_cast(rotation_);
            dispersed_ = false;
        }
        return transforms_;
    }

    void Transform::setPosition(const glm::vec3& p)
    {
        dispersed_ = true;
        position_ = p;
    }
    void Transform::setScale(const glm::vec3& s)
    {
        dispersed_ = true;
        scale_ = s;
    }
    void Transform::setRotation(const glm::quat& r)
    {
        dispersed_ = true;
        rotation_ = r;
    }

    void Transform::translate(const glm::vec3& t)
    {
        dispersed_ = true;
        position_ += t;
    }
    void Transform::scale(const glm::vec3& s)
    {
        dispersed_ = true;
        scale_ *= s;
    }
    void Transform::rotate(const glm::quat& r)
    {
        dispersed_ = true;
        rotation_ *= r;
    }

    void Transform::scaleRelativeTo(const glm::vec3& p,const glm::vec3& s)
    {
        dispersed_ = true;
        position_ = p + (position_ - p) * s;
        scale_ *= s;
    }
    void Transform::rotateAround(const glm::vec3& p,const glm::quat& r)
    {
        dispersed_ = true;
        position_ = p + glm::mat3_cast(r)*(position_ - p);
        rotation_ *= r;
    }


    std::weak_ptr<Object> Object::getParent() const
    {
        return std::weak_ptr<Object>();
    }
}
