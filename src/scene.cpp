#include "scene.hpp"

namespace ffge
{
    void Layer::update() const
    {
        return;
        for (auto& object: handles)
        {
            auto& iterator = std::get<0>(object);
            Object* pointer = iterator->second;
            float& value = std::get<1>(object);

            float v = 0;
            if (sortingType == SortingType::BY_AXIS)
            {
                v = glm::dot(pointer->transforms.getPosition(), sortAxis);
            }
            else if (sortingType == SortingType::BY_MATHERIAL)
            {
                v = 0;
            }
            if (abs(v - value) > 0.0000001f)
            {
                sorted.erase(iterator);
                iterator = sorted.emplace(v,pointer);
                value = v;
            }
        }
    }

    void Layer::draw(const glm::mat4& v,const glm::mat4& p) const
    {
        glm::mat4 vp = p * v;
        update();
        for (const auto& object : handles)
        {
            Object* pointer = std::get<0>(object)->second;
            if (pointer->drawable)
            {
                Program::current()->getUniform("mvp") = vp * pointer->transforms.getMatrix();
                pointer->drawable->draw(pointer->transforms.getMatrix(), v, p);
            }
        }
    }

    void Scene::draw(const glm::mat4& v,const glm::mat4& p) const
    {
        for (const auto& layer: layers)
        {
            layer.draw(v,p);
        }
    }

    void Frame::draw(const UIRect& winrect)
    {
        Program* current = Program::current();
        if (scene && camera && current != nullptr)
        {
            viewbox.apply(winrect);
            glm::mat4 p = camera->getProjection(viewbox.view.width,viewbox.view.height);
            glm::mat4 v = camera->getView();

            scene->draw(v,p);
        }
    }
}
