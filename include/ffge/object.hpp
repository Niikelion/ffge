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
        Transform* parent_;
    public:
        Transform* getParent();
        void setParent(Transform* parent);

        glm::vec3 getPosition() const;
        glm::vec3 getScale() const;
        glm::quat getRotation() const;

        glm::mat4 getMatrix() const;
        glm::mat4 getLocalMatrix() const;

        void setPosition(const glm::vec3& position);
        void setScale(const glm::vec3& scale);
        void setRotation(const glm::quat& rotation);

        void translate(const glm::vec3& translation);
        void scale(const glm::vec3& scale);
        void rotate(const glm::quat& rotation);

        void scaleRelativeTo(const glm::vec3& position,const glm::vec3& scale);
        void rotateAround(const glm::vec3& position,const glm::quat& rotation);

        void lookAt(const glm::vec3& position, const glm::vec3& up, const glm::vec3& alternativeUp);
        inline void lookAt(const glm::vec3& position, const glm::vec3& up)
        {
            lookAt(position, up, up);
        }

        Transform() : position_() ,scale_(1) ,rotation_(glm::identity<glm::quat>()) ,dispersed_(false) ,transforms_(1), parent_() {}
        Transform(const Transform&) = default;
        Transform(Transform&&) noexcept = default;
    };

    class Object: public std::enable_shared_from_this<Object>
    {
    private:
		std::weak_ptr<Object> parent;
		std::vector<std::shared_ptr<Object>> children;
		void bareSetParent(const std::weak_ptr<Object>& parent);
		void bareAppendChild(const std::shared_ptr<Object>& child);
    public:
        std::shared_ptr<Object> getParent() const;
		void setParent(const std::shared_ptr<Object>& parent);
		void appendChild(const std::shared_ptr<Object>& child);
		void removeChild(const std::shared_ptr<Object>& child);

        Transform& transforms;

        std::shared_ptr<Drawable> drawable;
    };
    /*class Object: public Drawable
    {
    protected:
        mutable bool __flag;
    public:
        Drawable* model;
        glm::mat4 transforms;

        virtual void draw(const glm::mat4& m, const glm::mat4& v, const glm::mat4& p) const override;

        virtual void unlock() noexcept;

        Object();
        Object(const Object&) = default;
        Object(Object&&) noexcept = default;
        virtual ~Object() = default;
    };

    class ObjectGroup: public Drawable
    {
    public:
        typedef std::vector<std::pair<std::shared_ptr<Drawable>,glm::mat4> > container_type;

        virtual void draw(const glm::mat4& m,const glm::mat4& v, const glm::mat4& p) const override;

        typedef container_type::iterator iterator;
        typedef container_type::const_iterator const_iterator;
        typedef container_type::reverse_iterator reverse_iterator;
        typedef container_type::const_reverse_iterator const_reverse_iterator;

        container_type& container() noexcept;

        void add(const std::shared_ptr<Drawable> & target,const glm::mat4& = glm::mat4(1));
        void add(Drawable & target,const glm::mat4& = glm::mat4(1));
        void add(Drawable * target,const glm::mat4& = glm::mat4(1));

        iterator begin() const noexcept;
        iterator end() const noexcept;

        const_iterator cbegin() const noexcept;
        const_iterator cend() const noexcept;

        reverse_iterator rbegin() const noexcept;
        reverse_iterator rend() const noexcept;

        const_reverse_iterator crbegin() const noexcept;
        const_reverse_iterator crend() const noexcept;

        container_type::value_type & operator [] (size_t off);
        iterator find(const Drawable& target);
        reverse_iterator rfind(const Drawable& target);

        ObjectGroup() = default;
        ObjectGroup(const ObjectGroup&) = default;
        ObjectGroup(ObjectGroup&&) noexcept = default;
        virtual ~ObjectGroup() = default;
    protected:
        container_type objects;
        mutable bool __flag;
    };*/
}

#endif // OBJECT_H
