#include "SoftBody.hpp"

#include "../../Graphics/Utility/MeshDataGenerator.hpp"
#include "../../Graphics/Utility/PrimitiveRenderer.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../../Math/Utility/Utility.hpp"

namespace Engine
{
    SoftBody::SoftBody()
    {
        m_sphere.radius = 0.25f;
    }

    SoftBody::~SoftBody()
    {
    }

    void SoftBody::Integrate(Real dt)
    {
        for (auto& body : m_rigid_bodies)
        {
            body.IntegrateEuler(dt);
        }

        UpdateLocal();
        UpdateCentroid();
        UpdateMeshData();
    }

    void SoftBody::SolveSpringDamper()
    {
        for (auto& [a, b, local_q_a, local_q_b] : m_links)
        {
            Vector3 v_a = m_rigid_bodies[a].GetLinearVelocity();
            Vector3 v_b = m_rigid_bodies[b].GetLinearVelocity();

            //spring's end point which is connected to body.
            Vector3 q_a = m_rigid_bodies[a].GetVqs().LocalToWorldPoint(local_q_a);
            Vector3 q_b = m_rigid_bodies[b].GetVqs().LocalToWorldPoint(local_q_b);

            Vector3 f_a = m_spring_constant * (q_b - q_a) + m_damper_constant * (v_b - v_a);
            Vector3 f_b = -f_a;

            m_rigid_bodies[a].ApplyForce(f_a, q_a);
            m_rigid_bodies[b].ApplyForce(f_b, q_b);
        }
    }

    void SoftBody::CreateSampleCloth(size_t w_count, size_t h_count, bool is_fixed)
    {
        RigidBody body;

        Matrix33 inertia;
        inertia.SetDiagonal(150.0f, 150.0f, 150.0f);
        body.SetInertia(inertia);

        Real start_w = -static_cast<Real>(w_count) * 0.5f;
        Real start_h = -static_cast<Real>(h_count) * 0.5f;

        Real end_w = start_w + static_cast<Real>(w_count - 1);
        Real end_h = start_h + static_cast<Real>(h_count - 1);

        Real w_scale = 1.0f / (end_w - start_w);
        Real h_scale = 1.0f / (end_h - start_h);

        for (size_t i = 0; i < h_count; ++i)
        {
            for (size_t j = 0; j < w_count; ++j)
            {
                Vector3 local_pos;
                local_pos.x = (start_w + static_cast<Real>(j)) * w_scale;
                local_pos.y = (start_h + static_cast<Real>(i)) * h_scale;
                m_local_positions.push_back(local_pos);
                body.SetPosition(m_transform.LocalToWorldPoint(local_pos));
                m_rigid_bodies.push_back(body);
            }
        }

        Link link;

        for (size_t i = 0; i < h_count - 1; ++i)
        {
            for (size_t j = 0; j < w_count - 1; ++j)
            {
                link.a = i * w_count + j;
                link.b = i * w_count + j + 1;
                //link.local_q_a = m_rigid_bodies[link.a].GetPosition();
                //link.local_q_b = m_rigid_bodies[link.b].GetPosition();
                m_links.push_back(link);

                link.b = (i + 1) * w_count + j;
                //link.local_q_b = m_rigid_bodies[link.b].GetPosition();
                m_links.push_back(link);
            }
        }

        for (size_t j = 0; j < w_count - 1; ++j)
        {
            link.a = (h_count - 1) * w_count + j;
            link.b = (h_count - 1) * w_count + j + 1;

            //link.local_q_a = m_rigid_bodies[link.a].GetPosition();
            //link.local_q_b = m_rigid_bodies[link.b].GetPosition();
            m_links.push_back(link);
        }

        for (size_t i = 0; i < h_count - 1; ++i)
        {
            link.a = i * w_count + (w_count - 1);
            link.b = (i + 1) * w_count + (w_count - 1);

            //link.local_q_a = m_rigid_bodies[link.a].GetPosition();
            //link.local_q_b = m_rigid_bodies[link.b].GetPosition();
            m_links.push_back(link);
        }

        if (is_fixed)
        {
            m_rigid_bodies[0].SetMassInfinite();
            m_rigid_bodies[w_count - 1].SetMassInfinite();
            m_rigid_bodies[(h_count - 1) * w_count].SetMassInfinite();
            m_rigid_bodies[(h_count - 1) * w_count + (w_count - 1)].SetMassInfinite();

            /* for (size_t i = 0; i < h_count; ++i)
             {
                 link.a = i * w_count + 0;
                 link.b = i * w_count + (w_count - 1);
 
                 m_rigid_bodies[link.a].SetMassInfinite();
                 m_rigid_bodies[link.b].SetMassInfinite();
             }*/

            /*for (size_t j = 0; j < w_count; ++j)
            {
                link.a = j;
                link.b = (h_count - 1) * w_count + j;

                m_rigid_bodies[link.a].SetMassInfinite();
                m_rigid_bodies[link.b].SetMassInfinite();
            }*/
        }

        size_t vertex_count = m_rigid_bodies.size();
        m_mesh_data.vertices.resize(vertex_count * 2);

        Real du = 1.0f / (w_count - 1);
        Real dv = 1.0f / (h_count - 1);

        for (size_t i = 0; i < h_count; ++i)
        {
            for (size_t j = 0; j < w_count; ++j)
            {
                size_t idx = i * w_count + j;

                m_mesh_data.vertices[idx].pos = m_rigid_bodies[idx].GetPosition();
                m_mesh_data.vertices[idx].tex = Vector2(j * du, i * dv);
            }
        }

        for (size_t i = 0; i < h_count; ++i)
        {
            for (size_t j = 0; j < w_count; ++j)
            {
                size_t idx = i * w_count + j;

                m_mesh_data.vertices[vertex_count + idx].pos = m_rigid_bodies[idx].GetPosition();
                m_mesh_data.vertices[vertex_count + idx].tex = Vector2(j * du, i * dv);
            }
        }

        size_t count = (h_count - 1) * (w_count - 1) * 2;
        m_mesh_data.indices.resize(count * 6); // 3 indices per face
        m_mesh_data.faces.resize(count * 2);

        size_t k = 0;
        size_t f = 0;

        for (size_t i = 0; i < h_count - 1; ++i)
        {
            for (size_t j = 0; j < w_count - 1; ++j)
            {
                m_mesh_data.indices[k]     = static_cast<Uint32>(i * w_count + j);
                m_mesh_data.indices[k + 2] = static_cast<Uint32>(i * w_count + j + 1);
                m_mesh_data.indices[k + 1] = static_cast<Uint32>((i + 1) * w_count + j);

                m_mesh_data.indices[k + 3] = static_cast<Uint32>((i + 1) * w_count + j);
                m_mesh_data.indices[k + 5] = static_cast<Uint32>(i * w_count + j + 1);
                m_mesh_data.indices[k + 4] = static_cast<Uint32>((i + 1) * w_count + j + 1);

                m_mesh_data.faces[f]     = Face(m_mesh_data.indices[k], m_mesh_data.indices[k + 1], m_mesh_data.indices[k + 2]);
                m_mesh_data.faces[f + 1] = Face(m_mesh_data.indices[k + 3], m_mesh_data.indices[k + 4], m_mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }

        for (size_t i = 0; i < h_count - 1; ++i)
        {
            for (size_t j = 0; j < w_count - 1; ++j)
            {
                m_mesh_data.indices[k]     = static_cast<Uint32>(i * w_count + j + vertex_count);
                m_mesh_data.indices[k + 1] = static_cast<Uint32>(i * w_count + j + 1 + vertex_count);
                m_mesh_data.indices[k + 2] = static_cast<Uint32>((i + 1) * w_count + j + vertex_count);

                m_mesh_data.indices[k + 3] = static_cast<Uint32>((i + 1) * w_count + j + vertex_count);
                m_mesh_data.indices[k + 4] = static_cast<Uint32>(i * w_count + j + 1 + vertex_count);
                m_mesh_data.indices[k + 5] = static_cast<Uint32>((i + 1) * w_count + j + 1 + vertex_count);

                m_mesh_data.faces[f]     = Face(m_mesh_data.indices[k], m_mesh_data.indices[k + 1], m_mesh_data.indices[k + 2]);
                m_mesh_data.faces[f + 1] = Face(m_mesh_data.indices[k + 3], m_mesh_data.indices[k + 4], m_mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }

        size_t face_count = m_mesh_data.faces.size();
        m_adj_faces_per_vertex.resize(vertex_count * 2);

        for (size_t i = 0; i < face_count; ++i)
        {
            auto& face = m_mesh_data.faces[i];

            m_adj_faces_per_vertex[face.a].faces.push_back(face);
            m_adj_faces_per_vertex[face.b].faces.push_back(face);
            m_adj_faces_per_vertex[face.c].faces.push_back(face);
        }

        UpdateMeshData();
        UpdateCentroid();
    }

    void SoftBody::CreateSampleSphere()
    {
        m_mesh_data = MeshDataGenerator::CreateGeodesicSphere(0.5f, 1);

        size_t vertex_count = m_mesh_data.vertices.size();

        m_rigid_bodies.resize(vertex_count);

        for (size_t i = 0; i < vertex_count; ++i)
        {
            Vector3 local_pos = m_mesh_data.vertices[i].pos;
            m_rigid_bodies[i].SetPosition(m_transform.LocalToWorldPoint(local_pos));
        }
    }

    VecQuatScale& SoftBody::GetVqs()
    {
        return m_transform;
    }

    const VecQuatScale& SoftBody::GetVqs() const
    {
        return m_transform;
    }

    void SoftBody::ApplyForce(const Vector3& force, const Vector3& at)
    {
        Vector3 pos = m_transform.WorldToLocalPoint(at) - m_centroid;

        for (auto& body : m_rigid_bodies)
        {
            body.ApplyForce(force, pos);
        }
    }

    void SoftBody::ApplyForceCentroid(const Vector3& force)
    {
        for (auto& body : m_rigid_bodies)
        {
            body.ApplyForce(force, -m_centroid);
        }
    }

    void SoftBody::UpdateCentroid()
    {
        Real    total_mass = 0.0f;
        Vector3 total_pos;

        for (auto& body : m_rigid_bodies)
        {
            total_mass += body.Mass();
            total_pos += body.Mass() * body.GetCentroid();
        }

        m_centroid = total_pos / total_mass;
    }

    void SoftBody::SetTransform(const Transform& transform)
    {
        m_transform.position = transform.position;
        m_transform.scale    = transform.scale.Largest();
        m_transform.rotation = transform.orientation;
    }

    void SoftBody::Clear()
    {
        for (auto& body : m_rigid_bodies)
        {
            body.SetAngularVelocity(Vector3());
            body.SetLinearVelocity(Vector3());
        }
    }

    void SoftBody::Draw(SPtr<PrimitiveRenderer> renderer)
    {
        for (auto& body : m_rigid_bodies)
        {
            Transform tf;
            tf.position    = body.GetPosition();
            tf.orientation = body.GetOrientation();
            renderer->DrawPrimitive(m_sphere, tf, Color(), eRenderingMode::Lighting);
        }

        for (auto& [a, b, local_q_a, local_q_b] : m_links)
        {
            renderer->DrawLine(m_rigid_bodies[a].GetPosition(), m_rigid_bodies[b].GetPosition());
        }
    }

    void SoftBody::UpdateMeshData()
    {
        size_t size = m_rigid_bodies.size();

        for (size_t i = 0; i < size; ++i)
        {
            Vector3 pos                        = m_transform.WorldToLocalPoint(m_rigid_bodies[i].GetPosition());
            m_mesh_data.vertices[i].pos        = pos;
            m_mesh_data.vertices[i + size].pos = pos;
        }

        std::vector<Vector3> normals;
        Vector3              accumulated_normal;

        for (size_t i = 0; i < size * 2; ++i)
        {
            auto& [faces] = m_adj_faces_per_vertex[i];
            normals.clear();
            accumulated_normal.SetZero();
            for (auto& face : faces)
            {
                Vector3 normal = m_mesh_data.GetFaceNormal(face.a, face.b, face.c);
                auto    found  = std::find(normals.begin(), normals.end(), normal);
                if (found == normals.end())
                {
                    accumulated_normal += normal;
                    normals.push_back(normal);
                }
            }

            Vector3 n                 = accumulated_normal.Normalize();
            m_mesh_data.vertices[i].n = n;
            m_mesh_data.vertices[i].t = Math::GetTangentFast(n);
        }
    }

    void SoftBody::UpdateWorld()
    {
        size_t size = m_rigid_bodies.size();

        for (size_t i = 0; i < size; ++i)
        {
            m_rigid_bodies[i].SetPosition(m_transform.LocalToWorldPoint(m_local_positions[i]));
        }
    }

    void SoftBody::UpdateLocal()
    {
        size_t size = m_rigid_bodies.size();

        for (size_t i = 0; i < size; ++i)
        {
            m_local_positions[i] = m_transform.WorldToLocalPoint(m_rigid_bodies[i].GetPosition());
        }
    }
}
