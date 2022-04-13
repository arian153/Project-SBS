// ReSharper disable CppFunctionIsNotImplemented
#pragma once
#include <unordered_map>

namespace Engine
{
    enum class eCollisionState : int
    {
        None = 0,
        Start,
        Persist,
        End,
        Invalid
    };

    class RigidContactManifold;
    class Collider;

    class CollisionStateData
    {
    public:
        Collider*    set_a = nullptr;
        Collider*    set_b = nullptr;
        eCollisionState state = eCollisionState::None;
    };

    class ManifoldTable
    {
    public:
        ManifoldTable();
        ~ManifoldTable();

        void Initialize();
        void Shutdown();

        void SendHasCollision(Collider* a, Collider* b, bool was_collision);
        void SendNotCollision(Collider* a, Collider* b, bool was_collision);
        void SendInvalidCollision(Collider* a, Collider* b);

        size_t GenerateKey(Collider* a, Collider* b);
        size_t GenerateKey(ColliderPrimitive* a, ColliderPrimitive* b);
        size_t GenerateKey(RigidContactManifold* manifold);
        auto   FindAssociatedPairs(Collider* a);
        auto   FindCollisionData(Collider* a, Collider* b, size_t at = 0) const;
        auto   FindCollisionState(Collider* a, Collider* b);

        RigidContactManifold* FindManifold(Collider* a, Collider* b);
        RigidContactManifold* CreateManifold(Collider* a, Collider* b);
        bool                  HasManifold(Collider* a, Collider* b) const;
        void                  FilteringManifolds();
        size_t                RegisterKey(Collider* a, Collider* b);
        void                  DeRegisterKey(Collider* a, Collider* b);

    public:
        std::unordered_multimap<size_t, RigidContactManifold> m_manifold_table;
        std::unordered_multimap<size_t, CollisionStateData>   m_state_table;
        std::unordered_multimap<Collider*, Collider*>   m_key_table;
    };
}
