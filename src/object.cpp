#include "object.hpp"
#include <glm/gtc/matrix_transform.hpp>

glm::quat safeQuatLookAt(
    glm::vec3 const& lookFrom,
    glm::vec3 const& lookTo,
    glm::vec3 const& up,
    glm::vec3 const& alternativeUp)
{
    glm::vec3  direction = lookTo - lookFrom;
    float      directionLength = glm::length(direction);

    if (!(directionLength > 0.0001))
        return glm::quat(1, 0, 0, 0);

    direction /= directionLength;

    if (glm::abs(glm::dot(direction, up)) > .9999f) {
        return glm::quatLookAt(direction, alternativeUp);
    }
    else {
        return glm::quatLookAt(direction, up);
    }
}

namespace ffge
{
    Transform* Transform::getParent()
    {
        return parent_;
    }

    void Transform::setParent(Transform* parent)
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
		if (parent_ != nullptr)
		{
			return parent_ -> getMatrix() * getLocalMatrix();
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
        position_ = p + glm::mat3_cast(-r)*(position_ - p);
        rotation_ *= r;
    }

    void Transform::lookAt(const glm::vec3& position, const glm::vec3& up, const glm::vec3& alternativeUp)
    {
        setRotation(safeQuatLookAt(getPosition(),position,up,alternativeUp));
    }

	void Object::bareSetParent(const std::weak_ptr<Object>& parent)
	{
		this->parent = parent;
		transforms.setParent(&parent.lock()->transforms);
	}

	void Object::bareAppendChild(const std::shared_ptr<Object>& child)
	{
		children.emplace_back(child);
	}

	std::shared_ptr<Object> Object::getParent() const
	{
		return parent.lock();
	}

	void Object::setParent(const std::shared_ptr<Object>& parent)
	{
		if (parent == this->parent.lock())
		{
			bareSetParent(parent);
			parent -> bareAppendChild(shared_from_this());
		}
	}

	void Object::appendChild(const std::shared_ptr<Object>& child)
	{
		for (const auto& i : children)
			if (child == i)
				return;
		child->bareSetParent(std::weak_ptr<Object>(shared_from_this()));
		bareAppendChild(child);
	}

	void Object::removeChild(const std::shared_ptr<Object>& child)
	{
		for (auto it = children.begin(); it != children.end(); ++it)
		{
			if (*it == child)
			{
				children.erase(it);
				return;
			}
		}
	}

	/*void Object::draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const
    {
        if (!__flag && model != nullptr)
        {
            __flag = true;
            model -> draw(m*transforms,v,p);
            __flag = false;
        }
    }

    void Object::unlock() noexcept
    {
        __flag = false;
    }

    Object::Object()
    {
        model = nullptr;
        transforms = glm::mat4(1);
        __flag = false;
    }

    void ObjectGroup::draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const
    {
        if (!__flag)
        {
            __flag = true;
            for (const auto &i:objects)
                i.first.get()->draw(m*i.second,v,p);
            __flag = false;
        }
    }

    void ObjectGroup::add(const std::shared_ptr<Drawable> & target,const glm::mat4& t)
    {
        objects.emplace_back(target,t);
    }

    void ObjectGroup::add(Drawable & target,const glm::mat4& t)
    {
        objects.emplace_back(std::shared_ptr<Drawable>(const_cast<Drawable*>(&target),[](Drawable*){}),t);
    }

    void ObjectGroup::add(Drawable * target,const glm::mat4& t)
    {
        objects.emplace_back(std::shared_ptr<Drawable>(const_cast<Drawable*>(target)),t);
    }

    ObjectGroup::iterator ObjectGroup::begin() const noexcept
    {
        return const_cast<ObjectGroup*>(this)->objects.begin();
    }

    ObjectGroup::iterator ObjectGroup::end() const noexcept
    {
        return const_cast<ObjectGroup*>(this)->objects.end();
    }

    ObjectGroup::const_iterator ObjectGroup::cbegin() const noexcept
    {
        return objects.cbegin();
    }

    ObjectGroup::const_iterator ObjectGroup::cend() const noexcept
    {
        return objects.cend();
    }

    ObjectGroup::reverse_iterator ObjectGroup::rbegin() const noexcept
    {
        return const_cast<ObjectGroup*>(this)->objects.rbegin();
    }

    ObjectGroup::reverse_iterator ObjectGroup::rend() const noexcept
    {
        return const_cast<ObjectGroup*>(this)->objects.rend();
    }

    ObjectGroup::const_reverse_iterator ObjectGroup::crbegin() const noexcept
    {
        return objects.crbegin();
    }

    ObjectGroup::const_reverse_iterator ObjectGroup::crend() const noexcept
    {
        return objects.crend();
    }

    ObjectGroup::container_type::value_type& ObjectGroup::operator[](size_t off)
    {
        return objects[off];
    }

    ObjectGroup::iterator ObjectGroup::find(const Drawable& target)
    {
        return std::find_if(
                            begin(),
                            end(),
                            [&target](const container_type::value_type& a)
                            {
                                return a.first.get() == &target;
                            }
                            );
    }

    ObjectGroup::reverse_iterator ObjectGroup::rfind(const Drawable& target)
    {
        return std::find_if(
                            rbegin(),
                            rend(),
                            [&target](const container_type::value_type& a)
                            {
                                return a.first.get() == &target;
                            }
                            );
    }*/
}
