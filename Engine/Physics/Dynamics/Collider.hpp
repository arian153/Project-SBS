#pragma once
#include <vector>

#include "MassData.hpp"
#include "../ResolutionPhase/Contact/RigidContactPoint.hpp"

namespace Engine
{
    class RigidBody;
    enum class ePrimitiveType;
    class Primitive;

    class Collider
    {
    public:
        Collider();
        ~Collider();

        void Shutdown();

        Primitive* AddPrimitive(ePrimitiveType type);
        MassData   GetMassData(Real density) const;

        RigidBody* GetRigidBody() const;
        Vector3   LocalToWorldPoint(const Vector3& point) const;

    private:
        friend class RigidContactManifold;

    private:
        RigidBody*              m_rigid_body = nullptr;
        std::vector<Primitive*> m_primitives;
    };
}
