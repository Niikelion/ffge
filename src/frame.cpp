#include "frame.hpp"

#include "ff_glew.hpp"

namespace ffge
{
    void Viewbox::apply(const UIRect& rect) const
    {
        unsigned hw = rect.height * view.height / frame.height;
        glViewport(view.x * rect.width / frame.width,
                   rect.height - (view.y * rect.height / frame.height) - hw,
                   rect.width * view.width / frame.width,
                   hw);
    }

    Viewbox::Viewbox(const UIRect& _view,const UIRect& _frame)
    {
        view = _view;
        frame = _frame;
    }
    Viewbox::Viewbox(const UIRect& rect)
    {
        view = rect;
        frame = rect;
    }
}
