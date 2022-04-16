#pragma once

#include "MassData.hpp"
#include "RigidBody.hpp"
#include "../../Graphics/Data/MeshData.hpp"
#include "../../Math/Primitive/ConvexHull3D/Box.hpp"
#include "../../Math/Primitive/ConvexHull3D/Sphere.hpp"
#include "../../Math/Structure/VecQuatScale.hpp"
#include "../BroadPhase/BroadPhase.hpp"

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

        void IntegrateEuler(Real dt);
        void IntegrateVerlet(Real dt);
        void SolveSpringDamper();

        void CreateSampleCloth(size_t w_count, size_t h_count, bool is_fixed);
        void CreateSampleSphere(bool is_center_fixed);
        void CreateSampleBox();

        VecQuatScale&       GetVqs();
        const VecQuatScale& GetVqs() const;

        void ApplyForce(const Vector3& force, const Vector3& at);
        void ApplyForceCentroid(const Vector3& force);

        void UpdateCentroid();
        void SetTransform(const Transform& transform);

        void Clear();

        void Draw(SPtr<PrimitiveRenderer> renderer, Real global_radius);

        void AddLink(size_t a, size_t b);

        void UpdateMeshData();
        void UpdateWorld();
        void UpdateLocal();

        void GenerateBVH();
        void RenderBVH();
        void ShutdownBVH();

    private:
        friend class PhysicsSubsystem;
        friend class SoftBodyCompo;

    private:
        Sphere m_sphere;

        VecQuatScale m_transform;
        Vector3      m_centroid;

        Real m_spring_constant = 0.01f;
        Real m_damper_constant = 0.01f;
        int m_mesh_type = 0;

        std::vector<Link>          m_links;
        std::vector<RigidBody>     m_rigid_bodies;
        std::vector<AdjacentFaces> m_adj_faces_per_vertex;
        std::vector<Vector3>       m_local_positions;

        Vector3 m_lower_bound;
        Vector3 m_upper_bound;

        MeshData m_mesh_data;
        size_t   m_mesh_vertex_count = 0;


        int m_box_w = 0;
        int m_box_h = 0;
        int m_box_d = 0;
        //BroadPhase m_bvh;

        //std::vector<BoundingBox> m_bounding_boxes;
    };
}
