#include "Color.hpp"

namespace Engine
{
    Color::Color()
    {
    }

    Color::Color(Real red, Real green, Real blue, Real alpha)
        : r(red), g(green), b(blue), a(alpha)
    {
    }

    Real* Color::Data()
    {
        return &r;
    }

    const Real* Color::Data() const
    {
        return &r;
    }
}
