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
        update();
        for (const auto& object : handles)
        {
            Object* pointer = std::get<0>(object)->second;
            pointer->drawable->draw(pointer->transforms.getMatrix(),v,p);
        }
    }

    void Scene::draw(const glm::mat4& v,const glm::mat4& p) const
    {
        for (const auto& layer: layers)
        {
            layer.draw(v,p);
        }
    }

    /*void Scene::linkCamera(Camera& c)
    {
        camera.reset(&c,[](Camera*){});
    }
    void Scene::linkCamera(Camera* c)
    {
        camera.reset(c);
    }
    void Scene::linkCamera()
    {
        camera.reset();
    }

    void Scene::draw(UIRect winrect)
    {
        glm::mat4 proj_m(1) ,view_m(1);
        viewbox.apply(winrect);
        if (perspective)
        {
            proj_m = glm::perspective(fov,static_cast<float>(viewbox.view.width)/viewbox.view.height,0.1f,100.0f);
        }
        else
        {
            proj_m = glm::ortho(0.0f,static_cast<float>(viewbox.view.width),static_cast<float>(viewbox.view.height),0.0f);
        }

        if (camera)
        {
            view_m = camera.get() -> getMatrix();
        }
        else
        {
            //view_m = glm::mat4(1);
        }
        if (Program::current() != nullptr)
        {
            UniformBlock::shared.bind(Program::current());
        }
        UniformBlock::shared.subdata(0,4*4*sizeof(float),glm::value_ptr(proj_m));
        UniformBlock::shared.subdata(4*4*sizeof(float),4*4*sizeof(float),glm::value_ptr(view_m));

        for (auto &i:objects)
        {
            i.first->draw(view_m*i.second,glm::mat4(1));
        }
    }

    Scene::Scene()
    {
        perspective = false;
        fov = 45.0f;
    }*/

    void Frame::draw(const UIRect& winrect)
    {
        Program* current = Program::current();
        if (scene && camera && current != nullptr)
        {
            viewbox.apply(winrect);
            glm::mat4 p = camera->getProjection(viewbox.view.width,viewbox.view.height);
            glm::mat4 v = camera->getView();

            glm::mat4 pv = p * v;

            UniformBlock::shared.bind(Program::current());
            UniformBlock::shared.set("projectionView",pv);

            scene->draw(v,p);
        }
    }
}
