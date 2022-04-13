#pragma once
#include "../../../Math/Math.hpp"
#include "RigidContactPoint.hpp"
#include "../../Utility/PhysicsDef.hpp"

namespace Engine
{
    class Collider;
    class RigidContactPoint;
    class RigidBody;

    class RigidContactManifold
    {
    public:
        RigidContactManifold() = delete;
        explicit RigidContactManifold(Collider* a, Collider* b);
        ~RigidContactManifold();
        RigidContactManifold(const RigidContactManifold& rhs);
        RigidContactManifold& operator=(const RigidContactManifold& rhs);

        void   Set(const RigidContactManifold& manifold);
        void   UpdateInvalidContact();
        void   UpdateCurrentManifold(const RigidContactPoint& new_contact);
        void   CutDownManifold();
        size_t ContactsCount() const;
        void   ClearContacts();

    private:
        Real DistanceFromPoint(const RigidContactPoint& contact, RigidContactPoint* p0);
        Real DistanceFromLineSegment(const RigidContactPoint& contact, RigidContactPoint* p0, RigidContactPoint* p1);
        Real DistanceFromTriangle(const RigidContactPoint& contact, RigidContactPoint* p0, RigidContactPoint* p1, RigidContactPoint* p2);
        bool OnTriangle(RigidContactPoint* point, RigidContactPoint* p0, RigidContactPoint* p1, RigidContactPoint* p2);

    private:
        friend class Resolution;
        friend class NarrowPhase;
        friend class ContactConstraint;
        friend class ManifoldTable;

    private:
        bool is_collide = false;

        //data
        Collider* m_set_a = nullptr;
        Collider* m_set_b = nullptr;

        std::vector<RigidContactPoint> contacts;
    };
}
