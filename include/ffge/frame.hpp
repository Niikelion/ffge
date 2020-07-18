#ifndef FRAME_H
#define FRAME_H

#include <ffge/basics.hpp>

namespace ffge
{
    class Viewbox
    {
    public:
        UIRect view,frame;

        void apply(const UIRect& dFrame) const;

        Viewbox& operator = (const Viewbox&) noexcept = default;
        Viewbox& operator = (Viewbox&&) noexcept = default;

        Viewbox() = default;
        Viewbox(const Viewbox&) noexcept = default;
        Viewbox(Viewbox&&) noexcept = default;

        Viewbox(const UIRect& _view,const UIRect& _frame);
        Viewbox(const UIRect& rect);

        virtual ~Viewbox() = default;
    };
}

#endif // FRAME_H
