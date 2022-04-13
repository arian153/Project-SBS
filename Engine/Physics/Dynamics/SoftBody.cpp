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

        /*   m_bvh.Update();*/
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
        m_mesh_type = 0;
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

                // link.local_q_a = m_local_positions[link.a];
                //   link.local_q_b = m_local_positions[link.b];
                m_links.push_back(link);

                link.b = (i + 1) * w_count + j;
                //   link.local_q_b = m_local_positions[link.b];
                m_links.push_back(link);
            }
        }

        for (size_t j = 0; j < w_count - 1; ++j)
        {
            link.a = (h_count - 1) * w_count + j;
            link.b = (h_count - 1) * w_count + j + 1;
            //      link.local_q_a = m_local_positions[link.a];
            //     link.local_q_b = m_local_positions[link.b];
            m_links.push_back(link);
        }

        for (size_t i = 0; i < h_count - 1; ++i)
        {
            link.a = i * w_count + (w_count - 1);
            link.b = (i + 1) * w_count + (w_count - 1);
            //       link.local_q_a = m_local_positions[link.a];
            //        link.local_q_b = m_local_positions[link.b];
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

        m_mesh_vertex_count = m_rigid_bodies.size();

        UpdateMeshData();
        UpdateCentroid();
    }

    void SoftBody::CreateSampleSphere(bool is_center_fixed)
    {
        m_mesh_type = 1;

        auto [vertex_type, vertices, indices, faces]
                = MeshDataGenerator::CreateGeodesicSphere(10.0f, 2);

        size_t                     vertex_count = vertices.size();
        size_t                     face_count   = faces.size();
        std::vector<AdjacentFaces> adj_faces_per_vertex;
        adj_faces_per_vertex.resize(vertex_count);

        for (size_t i = 0; i < face_count; ++i)
        {
            auto& face = faces[i];

            adj_faces_per_vertex[face.a].faces.push_back(face);
            adj_faces_per_vertex[face.b].faces.push_back(face);
            adj_faces_per_vertex[face.c].faces.push_back(face);
        }

        std::vector<Vector3> positions;
        m_mesh_data.vertices.clear();
        for (size_t i = 0; i < vertex_count; ++i)
        {
            Vector3 pos = vertices[i].pos;

            auto found = std::find(positions.begin(), positions.end(), pos);

            if (found == positions.end())
            {
                positions.push_back(pos);
                m_mesh_data.vertices.push_back(vertices[i]);
            }
        }

        size_t new_vertex_count = positions.size();
        m_adj_faces_per_vertex.resize(new_vertex_count);

        m_mesh_data.faces.clear();
        m_mesh_data.indices.clear();

        for (size_t i = 0; i < vertex_count; ++i)
        {
            for (auto& face : adj_faces_per_vertex[i].faces)
            {
                Vector3 pos_a = vertices[face.a].pos;
                Vector3 pos_b = vertices[face.b].pos;
                Vector3 pos_c = vertices[face.c].pos;

                auto   found_a = std::find(positions.begin(), positions.end(), pos_a);
                size_t idx_a   = found_a - positions.begin();
                auto   found_b = std::find(positions.begin(), positions.end(), pos_b);
                size_t idx_b   = found_b - positions.begin();
                auto   found_c = std::find(positions.begin(), positions.end(), pos_c);
                size_t idx_c   = found_c - positions.begin();

                Face new_face;
                new_face.a = static_cast<Uint32>(idx_a);
                new_face.b = static_cast<Uint32>(idx_b);
                new_face.c = static_cast<Uint32>(idx_c);

                m_adj_faces_per_vertex[idx_a].faces.push_back(new_face);
                m_adj_faces_per_vertex[idx_b].faces.push_back(new_face);
                m_adj_faces_per_vertex[idx_c].faces.push_back(new_face);
                m_mesh_data.faces.push_back(new_face);
                m_mesh_data.indices.push_back(new_face.a);
                m_mesh_data.indices.push_back(new_face.b);
                m_mesh_data.indices.push_back(new_face.c);
            }
        }

        m_mesh_vertex_count = 0;
        vertex_count        = m_mesh_data.vertices.size();
        face_count          = m_mesh_data.faces.size();

        m_rigid_bodies.resize(vertex_count + 1);
        m_local_positions.resize(vertex_count + 1);

        for (size_t i = 0; i < vertex_count; ++i)
        {
            Vector3 local_pos    = m_mesh_data.vertices[i].pos;
            m_local_positions[i] = local_pos;
            m_rigid_bodies[i].SetPosition(m_transform.LocalToWorldPoint(local_pos));
            m_rigid_bodies[i].GetVqs().scale = 0.025f;

            if (local_pos.y < -1.0f)
                m_rigid_bodies[i].SetMassInfinite();
        }

        m_local_positions[vertex_count] = Vector3();
        m_rigid_bodies[vertex_count].SetPosition(m_transform.LocalToWorldPoint(Vector3()));
        m_rigid_bodies[vertex_count].GetVqs().scale = 0.025f;
        m_rigid_bodies[vertex_count].SetMassInfinite();

        size_t k = 0;
        m_links.resize(face_count * 3);

        for (size_t i = 0; i < face_count; ++i)
        {
            m_links[k].a     = m_mesh_data.faces[i].a;
            m_links[k].b     = m_mesh_data.faces[i].b;
            m_links[k + 1].a = m_mesh_data.faces[i].b;
            m_links[k + 1].b = m_mesh_data.faces[i].c;
            m_links[k + 2].a = m_mesh_data.faces[i].c;
            m_links[k + 2].b = m_mesh_data.faces[i].a;

            m_links[k].local_q_a     = m_local_positions[m_links[k].b];
            m_links[k].local_q_b     = m_local_positions[m_links[k].a];
            m_links[k + 1].local_q_a = m_local_positions[m_links[k + 1].b];
            m_links[k + 1].local_q_b = m_local_positions[m_links[k + 1].a];
            m_links[k + 2].local_q_a = m_local_positions[m_links[k + 2].b];
            m_links[k + 2].local_q_b = m_local_positions[m_links[k + 2].a];
            k += 3;
        }

        if (is_center_fixed)
        {
            Link link;
            link.a = vertex_count;
            for (size_t i = 0; i < vertex_count; ++i)
            {
                link.b         = i;
                link.local_q_a = m_local_positions[i];
                link.local_q_b = -m_local_positions[i];
                m_links.push_back(link);
            }
        }

        m_mesh_vertex_count = vertex_count;

        UpdateMeshData();
        UpdateCentroid();

        GenerateBVH();

        m_damper_constant = 1.125f;
        m_spring_constant = 3.875f;
    }

    void SoftBody::CreateSampleBox()
    {
        m_mesh_type = 2;

        RigidBody body;

        Matrix33 inertia;
        inertia.SetDiagonal(150.0f, 150.0f, 150.0f);
        body.SetInertia(inertia);

        int width  = 8;
        int height = 7;
        int depth  = 5;

        m_box_w = width;
        m_box_h = height;
        m_box_d = depth;

        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                for (int k = 0; k < depth; ++k)
                {
                    Vector3 local_pos;
                    local_pos.x = static_cast<Real>(i);
                    local_pos.y = static_cast<Real>(j);
                    local_pos.z = static_cast<Real>(k);

                    m_local_positions.push_back(local_pos);
                    body.SetPosition(m_transform.LocalToWorldPoint(local_pos));
                    m_rigid_bodies.push_back(body);
                }
            }
        }

        m_rigid_bodies[0].SetMassInfinite(); //000
        m_rigid_bodies[static_cast<size_t>(depth - 1)].SetMassInfinite(); //001

        m_rigid_bodies[static_cast<size_t>((width - 1) * height * depth)].SetMassInfinite(); //100
        m_rigid_bodies[static_cast<size_t>((width - 1) * height * depth + depth - 1)].SetMassInfinite(); //101

        m_rigid_bodies[static_cast<size_t>((height - 1) * depth)].SetMassInfinite(); //010
        m_rigid_bodies[static_cast<size_t>((height - 1) * depth + depth - 1)].SetMassInfinite(); //011

        m_rigid_bodies[static_cast<size_t>((width - 1) * height * depth + (height - 1) * depth)].SetMassInfinite(); //110
        m_rigid_bodies[static_cast<size_t>((width - 1) * height * depth + (height - 1) * depth + depth - 1)].SetMassInfinite(); //111

        Link link;

        for (int i = 0; i < width; ++i)
        {
            int i_base = i * height * depth;
            int i_next = (i + 1) * height * depth;
            int i_prev = (i - 1) * height * depth;

            for (int j = 0; j < height; ++j)
            {
                int j_base = j * depth;
                int j_next = (j + 1) * depth;
                int j_prev = (j - 1) * depth;

                for (int k = 0; k < depth; ++k)
                {
                    int k_base = k;
                    int k_next = k + 1;
                    int k_prev = k - 1;

                    //next
                    {
                        if (k_next < depth)
                            AddLink(
                                    static_cast<size_t>(i_base + j_base + k_base),
                                    static_cast<size_t>(i_base + j_base + k_next));

                        if (j_next < height)
                            AddLink(
                                    static_cast<size_t>(i_base + j_base + k_base),
                                    static_cast<size_t>(i_base + j_next + k_base));

                        if (i_next < width)
                            AddLink(
                                    static_cast<size_t>(i_base + j_base + k_base),
                                    static_cast<size_t>(i_next + j_base + k_base));

                        /*     if (j_next < height && k_next < depth)
                                 AddLink(
                                         static_cast<size_t>(i_base + j_base + k_base),
                                         static_cast<size_t>(i_base + j_next + k_next));
     
                             if (i_next < width && j_next < height)
                                 AddLink(
                                         static_cast<size_t>(i_base + j_base + k_base),
                                         static_cast<size_t>(i_next + j_next + k_base));
     
                             if (i_next < width && k_next < depth)
                                 AddLink(
                                         static_cast<size_t>(i_base + j_base + k_base),
                                         static_cast<size_t>(i_next + j_base + k_next));*/
                    }

                    //prev
                    {
                        if (k_prev >= 0)
                            AddLink(
                                    static_cast<size_t>(i_base + j_base + k_base),
                                    static_cast<size_t>(i_base + j_base + k_prev));

                        if (j_prev >= 0)
                            AddLink(
                                    static_cast<size_t>(i_base + j_base + k_base),
                                    static_cast<size_t>(i_base + j_prev + k_base));

                        if (i_prev >= 0)
                            AddLink(
                                    static_cast<size_t>(i_base + j_base + k_base),
                                    static_cast<size_t>(i_prev + j_base + k_base));

                        /*            if (i_prev >= 0 && j_prev >= 0)
                                        AddLink(
                                                static_cast<size_t>(i_base + j_base + k_base),
                                                static_cast<size_t>(i_prev + j_prev + k_base));
            
                                    if (i_prev >= 0 && k_prev >= 0)
                                        AddLink(
                                                static_cast<size_t>(i_base + j_base + k_base),
                                                static_cast<size_t>(i_prev + j_base + k_prev));
            
                                    if (k_prev >= 0 && j_prev >= 0)
                                        AddLink(
                                                static_cast<size_t>(i_base + j_base + k_base),
                                                static_cast<size_t>(i_base + j_prev + k_prev));*/
                    }

                    ////prev + next
                    //{
                    //    if (k_prev >= 0 && j_next < height)
                    //        AddLink(
                    //                static_cast<size_t>(i_base + j_base + k_base),
                    //                static_cast<size_t>(i_base + j_next + k_prev));

                    //    if (j_prev >= 0 && k_next < depth)
                    //        AddLink(
                    //                static_cast<size_t>(i_base + j_base + k_base),
                    //                static_cast<size_t>(i_base + j_prev + k_next));

                    //    if (i_prev >= 0 && k_next < depth)
                    //        AddLink(
                    //                static_cast<size_t>(i_base + j_base + k_base),
                    //                static_cast<size_t>(i_prev + j_base + k_next));

                    //    if (k_prev >= 0 && i_next < width)
                    //        AddLink(
                    //                static_cast<size_t>(i_base + j_base + k_base),
                    //                static_cast<size_t>(i_next + j_base + k_prev));

                    //    if (i_prev >= 0 && j_next < height)
                    //        AddLink(
                    //                static_cast<size_t>(i_base + j_base + k_base),
                    //                static_cast<size_t>(i_prev + j_next + k_base));

                    //    if (j_prev >= 0 && i_next < width)
                    //        AddLink(
                    //                static_cast<size_t>(i_base + j_base + k_base),
                    //                static_cast<size_t>(i_next + j_prev + k_base));
                    //}
                }
            }
        }

        m_rigid_bodies[0].SetMassInfinite(); //000
        m_rigid_bodies[static_cast<size_t>(depth - 1)].SetMassInfinite(); //001

        m_rigid_bodies[static_cast<size_t>((width - 1) * height * depth)].SetMassInfinite(); //100
        m_rigid_bodies[static_cast<size_t>((width - 1) * height * depth + depth - 1)].SetMassInfinite(); //101

        m_rigid_bodies[static_cast<size_t>((height - 1) * depth)].SetMassInfinite(); //010
        m_rigid_bodies[static_cast<size_t>((height - 1) * depth + depth - 1)].SetMassInfinite(); //011

        m_rigid_bodies[static_cast<size_t>((width - 1) * height * depth + (height - 1) * depth)].SetMassInfinite(); //110
        m_rigid_bodies[static_cast<size_t>((width - 1) * height * depth + (height - 1) * depth + depth - 1)].SetMassInfinite(); //111

        int side_count  = height * depth;
        int top_count   = width * depth;
        int front_count = width * height;

        m_mesh_data.vertices.resize(2 * static_cast<size_t>(side_count + top_count + front_count));

        Real du = 1.0f / (width - 1);
        Real dv = 1.0f / (height - 1);
        Real dw = 1.0f / (depth - 1);

        //(width - 1) * height * depth + (height - 1) * depth + depth - 1

        for (int i = 0; i < width; ++i)
        {
            for (int j = 0; j < height; ++j)
            {
                int i_base = i * height * depth;
                int j_base = j * depth;
                int k_base = depth - 1;

                int body_1 = i_base + j_base;
                int body_2 = body_1 + k_base;
                int vert_1 = i * height + j;
                int vert_2 = vert_1 + front_count;

                m_mesh_data.vertices[vert_1].pos = m_rigid_bodies[body_1].GetPosition();
                m_mesh_data.vertices[vert_1].tex = Vector2(i * du, j * dv);

                m_mesh_data.vertices[vert_2].pos = m_rigid_bodies[body_2].GetPosition();
                m_mesh_data.vertices[vert_2].tex = Vector2(i * du, j * dv);
            }
        }

        for (int i = 0; i < width; ++i)
        {
            for (int k = 0; k < depth; ++k)
            {
                int i_base = i * height * depth;
                int j_base = (height - 1) * depth;
                int k_base = k;

                int body_1 = i_base + k_base;
                int body_2 = body_1 + j_base;

                int vert_1 = front_count + front_count + i * depth + k;
                int vert_2 = vert_1 + top_count;

                m_mesh_data.vertices[vert_1].pos = m_rigid_bodies[body_1].GetPosition();
                m_mesh_data.vertices[vert_1].tex = Vector2(i * du, k * dw);

                m_mesh_data.vertices[vert_2].pos = m_rigid_bodies[body_2].GetPosition();
                m_mesh_data.vertices[vert_2].tex = Vector2(i * du, k * dw);
            }
        }

        for (int j = 0; j < height; ++j)
        {
            for (int k = 0; k < depth; ++k)
            {
                int i_base = (width - 1) * height * depth;
                int j_base = j * depth;
                int k_base = k;

                int body_1 = j_base + k_base;
                int body_2 = i_base + body_1;
                int vert_1 = front_count + front_count + top_count + top_count + j * depth + k;
                int vert_2 = vert_1 + side_count;

                m_mesh_data.vertices[vert_1].pos = m_rigid_bodies[body_1].GetPosition();
                m_mesh_data.vertices[vert_1].tex = Vector2(j * dv, k * dw);

                m_mesh_data.vertices[vert_2].pos = m_rigid_bodies[body_2].GetPosition();
                m_mesh_data.vertices[vert_2].tex = Vector2(j * dv, k * dw);
            }
        }

        size_t count = static_cast<size_t>((width - 1) * (height - 1) + (width - 1) * (depth - 1) + (depth - 1) * (height - 1)) * 2;
        m_mesh_data.indices.resize(count * 6); // 3 indices per face
        m_mesh_data.faces.resize(count * 2);

        size_t k = 0;
        size_t f = 0;

        int prev_count = 0;

        for (int i = 0; i < width - 1; ++i)
        {
            for (int j = 0; j < height - 1; ++j)
            {
                m_mesh_data.indices[k]     = static_cast<Uint32>(i * height + j);
                m_mesh_data.indices[k + 1] = static_cast<Uint32>(i * height + j + 1);
                m_mesh_data.indices[k + 2] = static_cast<Uint32>((i + 1) * height + j);

                m_mesh_data.indices[k + 3] = static_cast<Uint32>((i + 1) * height + j);
                m_mesh_data.indices[k + 4] = static_cast<Uint32>(i * height + j + 1);
                m_mesh_data.indices[k + 5] = static_cast<Uint32>((i + 1) * height + j + 1);

                m_mesh_data.faces[f]     = Face(m_mesh_data.indices[k], m_mesh_data.indices[k + 1], m_mesh_data.indices[k + 2]);
                m_mesh_data.faces[f + 1] = Face(m_mesh_data.indices[k + 3], m_mesh_data.indices[k + 4], m_mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }
        prev_count = front_count;

        for (int i = 0; i < width - 1; ++i)
        {
            for (int j = 0; j < height - 1; ++j)
            {
                m_mesh_data.indices[k]     = static_cast<Uint32>(i * height + j + prev_count);
                m_mesh_data.indices[k + 2] = static_cast<Uint32>(i * height + j + 1 + prev_count);
                m_mesh_data.indices[k + 1] = static_cast<Uint32>((i + 1) * height + j + prev_count);

                m_mesh_data.indices[k + 3] = static_cast<Uint32>((i + 1) * height + j + prev_count);
                m_mesh_data.indices[k + 5] = static_cast<Uint32>(i * height + j + 1 + prev_count);
                m_mesh_data.indices[k + 4] = static_cast<Uint32>((i + 1) * height + j + 1 + prev_count);

                m_mesh_data.faces[f]     = Face(m_mesh_data.indices[k], m_mesh_data.indices[k + 1], m_mesh_data.indices[k + 2]);
                m_mesh_data.faces[f + 1] = Face(m_mesh_data.indices[k + 3], m_mesh_data.indices[k + 4], m_mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }

        ///////////////////
        ///
        ///

         prev_count = front_count + front_count;
        for (int i = 0; i < width - 1; ++i)
        {
            for (int j = 0; j < depth - 1; ++j)
            {
                m_mesh_data.indices[k]     = static_cast<Uint32>(i * depth + j + prev_count);
                m_mesh_data.indices[k + 2] = static_cast<Uint32>(i * depth + j + 1 + prev_count);
                m_mesh_data.indices[k + 1] = static_cast<Uint32>((i + 1) * depth + j + prev_count);

                m_mesh_data.indices[k + 3] = static_cast<Uint32>((i + 1) * depth + j + prev_count);
                m_mesh_data.indices[k + 5] = static_cast<Uint32>(i * depth + j + 1 + prev_count);
                m_mesh_data.indices[k + 4] = static_cast<Uint32>((i + 1) * depth + j + 1 + prev_count);

                m_mesh_data.faces[f]     = Face(m_mesh_data.indices[k], m_mesh_data.indices[k + 1], m_mesh_data.indices[k + 2]);
                m_mesh_data.faces[f + 1] = Face(m_mesh_data.indices[k + 3], m_mesh_data.indices[k + 4], m_mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }

        prev_count = front_count + front_count + top_count;

        for (int i = 0; i < width - 1; ++i)
        {
            for (int j = 0; j < depth - 1; ++j)
            {
                m_mesh_data.indices[k]     = static_cast<Uint32>(i * depth + j + prev_count);
                m_mesh_data.indices[k + 1] = static_cast<Uint32>(i * depth + j + 1 + prev_count);
                m_mesh_data.indices[k + 2] = static_cast<Uint32>((i + 1) * depth + j + prev_count);

                m_mesh_data.indices[k + 3] = static_cast<Uint32>((i + 1) * depth + j + prev_count);
                m_mesh_data.indices[k + 4] = static_cast<Uint32>(i * depth + j + 1 + prev_count);
                m_mesh_data.indices[k + 5] = static_cast<Uint32>((i + 1) * depth + j + 1 + prev_count);

                m_mesh_data.faces[f]     = Face(m_mesh_data.indices[k], m_mesh_data.indices[k + 1], m_mesh_data.indices[k + 2]);
                m_mesh_data.faces[f + 1] = Face(m_mesh_data.indices[k + 3], m_mesh_data.indices[k + 4], m_mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }

        /////////////////////////////////

        prev_count = front_count + front_count + top_count + top_count;

        for (int i = 0; i < height - 1; ++i)
        {
            for (int j = 0; j < depth - 1; ++j)
            {
                m_mesh_data.indices[k]     = static_cast<Uint32>(i * depth + j + prev_count);
                m_mesh_data.indices[k + 1] = static_cast<Uint32>(i * depth + j + 1 + prev_count);
                m_mesh_data.indices[k + 2] = static_cast<Uint32>((i + 1) * depth + j + prev_count);

                m_mesh_data.indices[k + 3] = static_cast<Uint32>((i + 1) * depth + j + prev_count);
                m_mesh_data.indices[k + 4] = static_cast<Uint32>(i * depth + j + 1 + prev_count);
                m_mesh_data.indices[k + 5] = static_cast<Uint32>((i + 1) * depth + j + 1 + prev_count);

                m_mesh_data.faces[f]     = Face(m_mesh_data.indices[k], m_mesh_data.indices[k + 1], m_mesh_data.indices[k + 2]);
                m_mesh_data.faces[f + 1] = Face(m_mesh_data.indices[k + 3], m_mesh_data.indices[k + 4], m_mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }

        prev_count = front_count + front_count + top_count + top_count + side_count;

        for (int i = 0; i < height - 1; ++i)
        {
            for (int j = 0; j < depth - 1; ++j)
            {
                m_mesh_data.indices[k]     = static_cast<Uint32>(i * depth + j + prev_count);
                m_mesh_data.indices[k + 2] = static_cast<Uint32>(i * depth + j + 1 + prev_count);
                m_mesh_data.indices[k + 1] = static_cast<Uint32>((i + 1) * depth + j + prev_count);

                m_mesh_data.indices[k + 3] = static_cast<Uint32>((i + 1) * depth + j + prev_count);
                m_mesh_data.indices[k + 5] = static_cast<Uint32>(i * depth + j + 1 + prev_count);
                m_mesh_data.indices[k + 4] = static_cast<Uint32>((i + 1) * depth + j + 1 + prev_count);

                m_mesh_data.faces[f]     = Face(m_mesh_data.indices[k], m_mesh_data.indices[k + 1], m_mesh_data.indices[k + 2]);
                m_mesh_data.faces[f + 1] = Face(m_mesh_data.indices[k + 3], m_mesh_data.indices[k + 4], m_mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }

        size_t face_count = m_mesh_data.faces.size();
        m_adj_faces_per_vertex.resize(m_mesh_data.vertices.size());

        for (size_t i = 0; i < face_count; ++i)
        {
            auto& face = m_mesh_data.faces[i];

            m_adj_faces_per_vertex[face.a].faces.push_back(face);
            m_adj_faces_per_vertex[face.b].faces.push_back(face);
            m_adj_faces_per_vertex[face.c].faces.push_back(face);
        }

        m_mesh_vertex_count = m_mesh_data.vertices.size();
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
        if (m_rigid_bodies.empty())
            return;

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

    void SoftBody::AddLink(size_t a, size_t b)
    {
        Link link;
        link.a = a;
        link.b = b;
        /*  link.local_q_a = -m_local_positions[a];
          link.local_q_b = -m_local_positions[b];*/

        bool has_link = false;
        for (auto& [prev_a, prev_b, local_q_a, local_q_b] : m_links)
        {
            if (prev_a == a && prev_b == b)
            {
                has_link = true;
                break;
            }

            if (prev_a == b && prev_b == a)
            {
                has_link = true;
                break;
            }
        }

        if (has_link == false)
        {
            m_links.push_back(link);
        }
    }

    void SoftBody::UpdateMeshData()
    {
        size_t size = m_mesh_vertex_count;
        if (m_mesh_vertex_count == 0)
            return;

        if (m_mesh_type == 0)
        {
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
        else if (m_mesh_type == 1)
        {
            for (size_t i = 0; i < size; ++i)
            {
                Vector3 pos                 = m_transform.WorldToLocalPoint(m_rigid_bodies[i].GetPosition());
                m_mesh_data.vertices[i].pos = pos;
            }
            std::vector<Vector3> normals;
            Vector3              accumulated_normal;

            for (size_t i = 0; i < size; ++i)
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
        else if (m_mesh_type == 2)
        {
            int width  = m_box_w;
            int height = m_box_h;
            int depth  = m_box_d;

            int side_count  = height * depth;
            int top_count   = width * depth;
            int front_count = width * height;

            for (int i = 0; i < width; ++i)
            {
                for (int j = 0; j < height; ++j)
                {
                    int i_base = i * height * depth;
                    int j_base = j * depth;
                    int k_base = depth - 1;

                    int body_1 = i_base + j_base;
                    int body_2 = body_1 + k_base;
                    int vert_1 = i * height + j;
                    int vert_2 = vert_1 + front_count;

                    m_mesh_data.vertices[vert_1].pos = m_transform.WorldToLocalPoint(m_rigid_bodies[body_1].GetPosition());
                    m_mesh_data.vertices[vert_2].pos = m_transform.WorldToLocalPoint(m_rigid_bodies[body_2].GetPosition());
                }
            }

            for (int i = 0; i < width; ++i)
            {
                for (int k = 0; k < depth; ++k)
                {
                    int i_base = i * height * depth;
                    int j_base = (height - 1) * depth;
                    int k_base = k;

                    int body_1 = i_base + k_base;
                    int body_2 = body_1 + j_base;

                    int vert_1 = front_count + front_count + i * depth + k;
                    int vert_2 = vert_1 + top_count;

                    m_mesh_data.vertices[vert_1].pos = m_transform.WorldToLocalPoint(m_rigid_bodies[body_1].GetPosition());
                    m_mesh_data.vertices[vert_2].pos = m_transform.WorldToLocalPoint(m_rigid_bodies[body_2].GetPosition());
                }
            }

            for (int j = 0; j < height; ++j)
            {
                for (int k = 0; k < depth; ++k)
                {
                    int i_base = (width - 1) * height * depth;
                    int j_base = j * depth;
                    int k_base = k;

                    int body_1 = j_base + k_base;
                    int body_2 = i_base + body_1;
                    int vert_1 = front_count + front_count + top_count + top_count + j * depth + k;
                    int vert_2 = vert_1 + side_count;

                    m_mesh_data.vertices[vert_1].pos = m_transform.WorldToLocalPoint(m_rigid_bodies[body_1].GetPosition());
                    m_mesh_data.vertices[vert_2].pos = m_transform.WorldToLocalPoint(m_rigid_bodies[body_2].GetPosition());
                }
            }

            std::vector<Vector3> normals;
            Vector3              accumulated_normal;

            size_t adj_vert_count = m_adj_faces_per_vertex.size();

            for (size_t i = 0; i < adj_vert_count; ++i)
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

    void SoftBody::GenerateBVH()
    {
        /* m_bvh.Initialize();
 
         size_t size = m_rigid_bodies.size();
         m_bounding_boxes.resize(size);
 
         for (size_t i = 0; i < size; ++i)
         {
             m_rigid_bodies[i].GetVqs().scale = 0.025f;
             m_bounding_boxes[i].Set(&m_rigid_bodies[i], eBoundingObjectType::RigidBody);
             m_bvh.Add(&m_bounding_boxes[i]);
         }*/
    }

    void SoftBody::RenderBVH()
    {
        //m_bvh.Render();
        //m_bvh.Render();
    }

    void SoftBody::ShutdownBVH()
    {
        //m_bvh.Shutdown();
    }
}
