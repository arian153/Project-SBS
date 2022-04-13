#pragma once
#include "../../../EngineDefine.hpp"
#include "../../../Math/Math.hpp"

namespace Engine
{
    class Color;
    class PrimitiveRenderer;

    class Constraint
    {
    public:
        Constraint();
        virtual ~Constraint();

        virtual void Shutdown() = 0;

        virtual void GenerateVelocityConstraints(Real dt) = 0;
        virtual void SolveVelocityConstraints(Real dt) = 0;
        virtual void ApplyVelocityConstraints() = 0;

        virtual void GeneratePositionConstraints(Real dt) = 0;
        virtual void SolvePositionConstraints(Real dt) = 0;
        virtual void ApplyPositionConstraints() = 0;

        virtual void Render(PrimitiveRenderer* primitive_renderer, const Color& color) const = 0;
    private:
    };
}
