#ifndef SCENE_H
#define SCENE_H

#include <ffge/object.hpp>
#include <ffge/frame.hpp>
#include <ffge/camera.hpp>
#include <ffge/uniform.hpp>
#include <ffge/shader.hpp>
#include <memory>
#include <map>
#include <glm/gtc/matrix_transform.hpp>


namespace ffge
{
    class Layer
    {
    private:
        mutable std::multimap<float, Object*> sorted;
        mutable std::vector<std::tuple<std::multimap<float,Object*>::iterator, float>> handles;
        void update() const;
    public:
        enum SortingType
        {
            BY_AXIS,
            BY_MATHERIAL
        };

        glm::vec3 sortAxis = glm::vec3(0,0,1);
        SortingType sortingType;

        void draw(const glm::mat4& v,const glm::mat4& p) const;
    };

    class Scene
    {
    public:
        std::vector<Layer> layers;
        std::vector<std::shared_ptr<Object>> objects;
        void draw(const glm::mat4& v,const glm::mat4& p) const;
    };

    class Frame
    {
    public:
        Viewbox viewbox;
        std::shared_ptr<Scene> scene;
        std::shared_ptr<Camera> camera;

        void draw(const UIRect& winrect);
    };
}

#endif // SCENE_H
