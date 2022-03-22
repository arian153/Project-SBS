#pragma once

#include "MassData.hpp"
#include "RigidBody.hpp"
#include "../../Graphics/Data/MeshData.hpp"
#include "../../Math/Primitive/ConvexHull3D/Sphere.hpp"
#include "../../Math/Structure/VecQuatScale.hpp"

namespace Engine
{
    class Transform;

    struct Link
    {
        size_t a = 0;
        size_t b = 0;

        Vector3 local_q_a;
        Vector3 local_q_b;
    };

    class AdjacentFaces
    {
    public:
        AdjacentFaces()  = default;
        ~AdjacentFaces() = default;

    public:
        std::vector<Face> faces;
    };

    class SoftBody
    {
    public:
        SoftBody();
        ~SoftBody();

        void Integrate(Real dt);
        void SolveSpringDamper();

        void CreateSampleCloth(size_t w_count, size_t h_count, bool is_fixed);

        VecQuatScale&       GetVqs();
        const VecQuatScale& GetVqs() const;

        void ApplyForce(const Vector3& force, const Vector3& at);
        void ApplyForceCentroid(const Vector3& force);

        void UpdateCentroid();
        void SetTransform(const Transform& transform);

        void Clear();

        void Draw(SPtr<PrimitiveRenderer> renderer);

        void UpdateMeshData();
        void UpdatePosition();

    private:
        friend class PhysicsSubsystem;
        friend class SoftBodyCompo;

    private:
        Sphere m_sphere;

        VecQuatScale m_transform;
        Vector3      m_centroid;

        Real m_spring_constant = 5.0f;
        Real m_damper_constant = 5.0f;

        std::vector<Link>          m_links;
        std::vector<RigidBody>     m_rigid_bodies;
        std::vector<AdjacentFaces> m_adj_faces_per_vertex;
        std::vector<Vector3>       m_local_positions;

        MeshData m_mesh_data;
    };
}
