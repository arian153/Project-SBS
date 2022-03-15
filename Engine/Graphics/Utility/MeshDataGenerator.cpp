#include "MeshDataGenerator.hpp"

#include "../../Math/Utility/VectorDef.hpp"

namespace Engine
{
    MeshData MeshDataGenerator::CreateBox(Real width, Real height, Real depth, Uint32 num_subdivisions)
    {
        MeshData mesh_data;

        Real w2 = 0.5f * width;
        Real h2 = 0.5f * height;
        Real d2 = 0.5f * depth;

        mesh_data.vertices.resize(24);

        // Fill in the front face vertex data.
        mesh_data.vertices[0] = SkinnedVertex(-w2, -h2, -d2, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[1] = SkinnedVertex(-w2, +h2, -d2, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[2] = SkinnedVertex(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[3] = SkinnedVertex(+w2, -h2, -d2, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        // Fill in the back face vertex data.
        mesh_data.vertices[4] = SkinnedVertex(-w2, -h2, +d2, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        mesh_data.vertices[5] = SkinnedVertex(+w2, -h2, +d2, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        mesh_data.vertices[6] = SkinnedVertex(+w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        mesh_data.vertices[7] = SkinnedVertex(-w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f);
        // Fill in the top face vertex data.
        mesh_data.vertices[8]  = SkinnedVertex(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[9]  = SkinnedVertex(-w2, +h2, +d2, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[10] = SkinnedVertex(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[11] = SkinnedVertex(+w2, +h2, -d2, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        // Fill in the bottom face vertex data.
        mesh_data.vertices[12] = SkinnedVertex(-w2, -h2, -d2, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        mesh_data.vertices[13] = SkinnedVertex(+w2, -h2, -d2, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        mesh_data.vertices[14] = SkinnedVertex(+w2, -h2, +d2, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        mesh_data.vertices[15] = SkinnedVertex(-w2, -h2, +d2, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f);
        // Fill in the left face vertex data.
        mesh_data.vertices[16] = SkinnedVertex(-w2, -h2, +d2, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        mesh_data.vertices[17] = SkinnedVertex(-w2, +h2, +d2, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        mesh_data.vertices[18] = SkinnedVertex(-w2, +h2, -d2, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        mesh_data.vertices[19] = SkinnedVertex(-w2, -h2, -d2, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f);
        // Fill in the right face vertex data.
        mesh_data.vertices[20] = SkinnedVertex(+w2, -h2, -d2, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mesh_data.vertices[21] = SkinnedVertex(+w2, +h2, -d2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mesh_data.vertices[22] = SkinnedVertex(+w2, +h2, +d2, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        mesh_data.vertices[23] = SkinnedVertex(+w2, -h2, +d2, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        // Create the indices.

        mesh_data.indices.resize(36);

        // Fill in the front face index data
        mesh_data.indices[0] = 0;
        mesh_data.indices[1] = 1;
        mesh_data.indices[2] = 2;
        mesh_data.indices[3] = 0;
        mesh_data.indices[4] = 2;
        mesh_data.indices[5] = 3;

        // Fill in the back face index data
        mesh_data.indices[6]  = 4;
        mesh_data.indices[7]  = 5;
        mesh_data.indices[8]  = 6;
        mesh_data.indices[9]  = 4;
        mesh_data.indices[10] = 6;
        mesh_data.indices[11] = 7;

        // Fill in the top face index data
        mesh_data.indices[12] = 8;
        mesh_data.indices[13] = 9;
        mesh_data.indices[14] = 10;
        mesh_data.indices[15] = 8;
        mesh_data.indices[16] = 10;
        mesh_data.indices[17] = 11;

        // Fill in the bottom face index data
        mesh_data.indices[18] = 12;
        mesh_data.indices[19] = 13;
        mesh_data.indices[20] = 14;
        mesh_data.indices[21] = 12;
        mesh_data.indices[22] = 14;
        mesh_data.indices[23] = 15;

        // Fill in the left face index data
        mesh_data.indices[24] = 16;
        mesh_data.indices[25] = 17;
        mesh_data.indices[26] = 18;
        mesh_data.indices[27] = 16;
        mesh_data.indices[28] = 18;
        mesh_data.indices[29] = 19;

        // Fill in the right face index data
        mesh_data.indices[30] = 20;
        mesh_data.indices[31] = 21;
        mesh_data.indices[32] = 22;
        mesh_data.indices[33] = 20;
        mesh_data.indices[34] = 22;
        mesh_data.indices[35] = 23;

        mesh_data.faces.resize(12);
        mesh_data.faces[0]  = Face(0, 1, 2);
        mesh_data.faces[1]  = Face(0, 2, 3);
        mesh_data.faces[2]  = Face(4, 5, 6);
        mesh_data.faces[3]  = Face(4, 6, 7);
        mesh_data.faces[4]  = Face(8, 9, 10);
        mesh_data.faces[5]  = Face(8, 10, 11);
        mesh_data.faces[6]  = Face(12, 13, 14);
        mesh_data.faces[7]  = Face(12, 14, 15);
        mesh_data.faces[8]  = Face(16, 17, 18);
        mesh_data.faces[9]  = Face(16, 18, 19);
        mesh_data.faces[10] = Face(20, 21, 22);
        mesh_data.faces[11] = Face(20, 22, 23);

        Uint32 subdivisions = std::min<Uint32>(num_subdivisions, 6u);
        for (Uint32 j = 0; j < subdivisions; ++j)
        {
            Subdivide(mesh_data);
        }

        return mesh_data;
    }

    MeshData MeshDataGenerator::CreateSphere(Real radius, Uint32 slice_count, Uint32 stack_count)
    {
        MeshData      mesh_data;
        SkinnedVertex top_vertex(0.0f, +radius, 0.0f, 0.0f, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        SkinnedVertex bottom_vertex(0.0f, -radius, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices.push_back(top_vertex);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        for (Uint32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;

                SkinnedVertex vertex;
                vertex.pos = Vector3(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta));
                vertex.tex = Vector2(theta / Math::TWO_PI, phi / Math::PI);
                vertex.n   = vertex.pos.Normalize();
                vertex.t   = Vector3(-radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta)).Normalize();

                mesh_data.vertices.push_back(vertex);
            }
        }
        mesh_data.vertices.push_back(bottom_vertex);
        // Compute indices for top stack.  The top stack was written first to the vertex buffer
        // and connects the top pole to the first ring.
        for (Uint32 i = 1; i <= slice_count; ++i)
        {
            mesh_data.indices.push_back(0);
            mesh_data.indices.push_back(i + 1);
            mesh_data.indices.push_back(i);

            mesh_data.faces.emplace_back(0, i + 1, i);
        }
        // Compute indices for inner stacks (not connected to poles).
        // Offset the indices to the index of the first vertex in the first ring.
        // This is just skipping the top pole vertex.
        Uint32 base_index        = 1;
        Uint32 ring_vertex_count = slice_count + 1;
        for (Uint32 i = 0; i < stack_count - 2; ++i)
        {
            for (Uint32 j = 0; j < slice_count; ++j)
            {
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);

                mesh_data.faces.emplace_back(
                                             base_index + i * ring_vertex_count + j,
                                             base_index + i * ring_vertex_count + j + 1,
                                             base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.faces.emplace_back(
                                             base_index + (i + 1) * ring_vertex_count + j,
                                             base_index + i * ring_vertex_count + j + 1,
                                             base_index + (i + 1) * ring_vertex_count + j + 1);
            }
        }
        // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
        // and connects the bottom pole to the bottom ring.
        // South pole vertex was added last.
        Uint32 south_pole_index = static_cast<Uint32>(mesh_data.vertices.size()) - 1;
        // Offset the indices to the index of the first vertex in the last ring.
        base_index = south_pole_index - ring_vertex_count;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            mesh_data.indices.push_back(south_pole_index);
            mesh_data.indices.push_back(base_index + i);
            mesh_data.indices.push_back(base_index + i + 1);
            mesh_data.faces.emplace_back(south_pole_index, base_index + i, base_index + i + 1);
        }
        return mesh_data;
    }

    MeshData MeshDataGenerator::CreateGeodesicSphere(Real radius, Uint32 subdivisions)
    {
        MeshData mesh_data;
        // Put a cap on the number of subdivisions.
        Uint32 max_subdivisions = std::min<Uint32>(subdivisions, 5);
        // Approximate a sphere by tessellating an icosahedron.
        constexpr Real x            = 0.525731f;
        constexpr Real z            = 0.850651f;
        Vector3        position[12] =
        {
            Vector3(-x, 0.0f, z),
            Vector3(x, 0.0f, z),
            Vector3(-x, 0.0f, -z),
            Vector3(x, 0.0f, -z),
            Vector3(0.0f, z, x),
            Vector3(0.0f, z, -x),
            Vector3(0.0f, -z, x),
            Vector3(0.0f, -z, -x),
            Vector3(z, x, 0.0f),
            Vector3(-z, x, 0.0f),
            Vector3(z, -x, 0.0f),
            Vector3(-z, -x, 0.0f)
        };

        Uint32 k[60] =
        {
            1, 4, 0, 4, 9, 0,
            4, 5, 9, 8, 5, 4,
            1, 8, 4, 1, 10, 8,
            10, 3, 8, 8, 3, 5,
            3, 2, 5, 3, 7, 2,
            3, 10, 7, 10, 6, 7,
            6, 11, 7, 6, 0, 11,
            6, 1, 0, 10, 1, 6,
            11, 0, 9, 2, 11, 9,
            5, 2, 9, 11, 2, 7
        };

        mesh_data.vertices.resize(12);
        mesh_data.indices.assign(&k[0], &k[60]);
        for (Uint32 i = 0; i < 12; ++i)
        {
            mesh_data.vertices[i].pos = (position[i]);
        }
        for (Uint32 i = 0; i < max_subdivisions; ++i)
        {
            Subdivide(mesh_data);
        }
        // Project vertices onto sphere and scale.
        for (auto& vertex : mesh_data.vertices)
        {
            vertex.n   = vertex.pos.Normalize();
            vertex.pos = vertex.n.Scale(radius);

            Real theta = atan2f(vertex.pos.z, vertex.pos.x);
            theta      = theta < 0.0f ? theta + Math::TWO_PI : theta;
            Real phi   = acosf(vertex.pos.y / radius);

            vertex.tex = Vector2(theta / Math::TWO_PI, phi / Math::PI);
            vertex.t   = Vector3(-radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta)).Normalize();
        }
        return mesh_data;
    }

    MeshData MeshDataGenerator::CreateCylinder(Real bottom_radius, Real top_radius, Real height, Uint32 slice_count, Uint32 stack_count)
    {
        MeshData mesh_data;
        Real     stack_height = height / stack_count;
        // Amount to increment radius as we move up each stack level from bottom to top.
        Real   radius_step = (top_radius - bottom_radius) / stack_count;
        Uint32 ring_count  = stack_count + 1;
        // Compute vertices for each stack ring starting at the bottom and moving up.
        for (Uint32 i = 0; i < ring_count; ++i)
        {
            Real y = -0.5f * height + i * stack_height;
            Real r = bottom_radius + i * radius_step;
            // vertices of ring
            Real d_theta = Math::TWO_PI / slice_count;
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real    c         = cosf(j * d_theta);
                Real    s         = sinf(j * d_theta);
                Real    dr        = bottom_radius - top_radius;
                Vector3 bitangent = Vector3(dr * c, -height, dr * s).Normalize();

                SkinnedVertex vertex;
                vertex.pos = Vector3(r * c, y, r * s);
                vertex.tex = Vector2(static_cast<Real>(j) / slice_count, 1.0f - static_cast<Real>(i) / stack_count);
                vertex.t   = Vector3(-s, 0.0f, c).Normalize();
                vertex.n   = CrossProduct(vertex.t, bitangent).Normalize();

                mesh_data.vertices.push_back(vertex);
            }
        }
        // Add one because we duplicate the first and last vertex per ring
        // since the texture coordinates are different.
        Uint32 ring_vertex_count = slice_count + 1;
        // Compute indices for each stack.
        for (Uint32 i = 0; i < stack_count; ++i)
        {
            for (Uint32 j = 0; j < slice_count; ++j)
            {
                mesh_data.indices.push_back(i * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(i * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(i * ring_vertex_count + j + 1);

                mesh_data.faces.emplace_back(
                                             i * ring_vertex_count + j,
                                             (i + 1) * ring_vertex_count + j,
                                             (i + 1) * ring_vertex_count + j + 1);
                mesh_data.faces.emplace_back(
                                             i * ring_vertex_count + j,
                                             (i + 1) * ring_vertex_count + j + 1,
                                             i * ring_vertex_count + j + 1);
            }
        }
        BuildCylinderTopCap(top_radius, height, slice_count, mesh_data);
        BuildCylinderBottomCap(bottom_radius, height, slice_count, mesh_data);

        return mesh_data;
    }

    MeshData MeshDataGenerator::CreateCapsule(Real radius, Real height, Uint32 slice_count, Uint32 stack_count)
    {
        MeshData mesh_data;
        Real     stack_height = height / stack_count;
        // Amount to increment radius as we move up each stack level from bottom to top.
        Uint32 ring_count = stack_count + 1;
        // Compute vertices for each stack ring starting at the bottom and moving up.
        for (Uint32 i = 0; i < ring_count; ++i)
        {
            Real y = -0.5f * height + i * stack_height;
            // vertices of ring
            Real d_theta = Math::TWO_PI / slice_count;
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real    c         = cosf(j * d_theta);
                Real    s         = sinf(j * d_theta);
                Vector3 bitangent = Vector3(0.0f, -height, 0.0f).Normalize();

                SkinnedVertex vertex;
                vertex.pos = Vector3(radius * c, y, radius * s);
                vertex.tex = Vector2(static_cast<Real>(j) / slice_count, 1.0f - static_cast<Real>(i) / stack_count);
                vertex.t   = Vector3(-s, 0.0f, c).Normalize();
                vertex.n   = CrossProduct(vertex.t, bitangent).Normalize();

                mesh_data.vertices.push_back(vertex);
            }
        }
        // Add one because we duplicate the first and last vertex per ring
        // since the texture coordinates are different.
        Uint32 ring_vertex_count = slice_count + 1;
        // Compute indices for each stack.
        for (Uint32 i = 0; i < stack_count; ++i)
        {
            for (Uint32 j = 0; j < slice_count; ++j)
            {
                mesh_data.indices.push_back(i * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(i * ring_vertex_count + j);
                mesh_data.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(i * ring_vertex_count + j + 1);

                mesh_data.faces.emplace_back(
                                             i * ring_vertex_count + j,
                                             (i + 1) * ring_vertex_count + j,
                                             (i + 1) * ring_vertex_count + j + 1);
                mesh_data.faces.emplace_back(
                                             i * ring_vertex_count + j,
                                             (i + 1) * ring_vertex_count + j + 1,
                                             i * ring_vertex_count + j + 1);
            }
        }

        BuildCapsuleSphere(radius, height, slice_count, mesh_data);

        return mesh_data;
    }

    MeshData MeshDataGenerator::CreateGrid(Real width, Real depth, Uint32 m, Uint32 n)
    {
        MeshData mesh_data;

        Uint32 vertex_count = m * n;
        Uint32 face_count   = (m - 1) * (n - 1) * 2;
        Real   half_width   = 0.5f * width;
        Real   half_depth   = 0.5f * depth;
        Real   dx           = width / (n - 1);
        Real   dz           = depth / (m - 1);
        Real   du           = 1.0f / (n - 1);
        Real   dv           = 1.0f / (m - 1);
        mesh_data.vertices.resize(vertex_count);

        for (size_t i = 0; i < m; ++i)
        {
            Real z = half_depth - i * dz;
            for (size_t j = 0; j < n; ++j)
            {
                Real x = -half_width + j * dx;
                // Stretch texture over grid.
                mesh_data.vertices[i * n + j].pos = Vector3(x, 0.0f, z);
                mesh_data.vertices[i * n + j].tex = Vector2(j * du, i * dv);
                mesh_data.vertices[i * n + j].n   = Math::Vector3::Y_AXIS;
                mesh_data.vertices[i * n + j].t   = Math::Vector3::X_AXIS;
            }
        }

        size_t count = face_count;
        mesh_data.indices.resize(count * 3); // 3 indices per face
        mesh_data.faces.resize(count);

        size_t k = 0;
        size_t f = 0;

        for (Uint32 i = 0; i < m - 1; ++i)
        {
            for (Uint32 j = 0; j < n - 1; ++j)
            {
                mesh_data.indices[k]     = i * n + j;
                mesh_data.indices[k + 1] = i * n + j + 1;
                mesh_data.indices[k + 2] = (i + 1) * n + j;

                mesh_data.indices[k + 3] = (i + 1) * n + j;
                mesh_data.indices[k + 4] = i * n + j + 1;
                mesh_data.indices[k + 5] = (i + 1) * n + j + 1;

                mesh_data.faces[f]     = Face(mesh_data.indices[k], mesh_data.indices[k + 1], mesh_data.indices[k + 2]);
                mesh_data.faces[f + 1] = Face(mesh_data.indices[k + 3], mesh_data.indices[k + 4], mesh_data.indices[k + 5]);

                f += 2;
                k += 6; // next quad
            }
        }

        return mesh_data;
    }

    MeshData MeshDataGenerator::CreateQuad(Real x, Real y, Real w, Real h, Real depth)
    {
        MeshData mesh_data;
        mesh_data.vertices.resize(4);
        mesh_data.indices.resize(6);
        mesh_data.faces.resize(2);
        // Position coordinates specified in NDC space.
        mesh_data.vertices[0] = SkinnedVertex(
                                              x, y - h, depth,
                                              0.0f, 1.0f,
                                              0.0f, 0.0f, -1.0f,
                                              1.0f, 0.0f, 0.0f
                                             );
        mesh_data.vertices[1] = SkinnedVertex(
                                              x, y, depth,
                                              0.0f, 0.0f,
                                              0.0f, 0.0f, -1.0f,
                                              1.0f, 0.0f, 0.0f
                                             );
        mesh_data.vertices[2] = SkinnedVertex(
                                              x + w, y, depth,
                                              1.0f, 0.0f,
                                              0.0f, 0.0f, -1.0f,
                                              1.0f, 0.0f, 0.0f
                                             );
        mesh_data.vertices[3] = SkinnedVertex(
                                              x + w, y - h, depth,
                                              1.0f, 1.0f,
                                              0.0f, 0.0f, -1.0f,
                                              1.0f, 0.0f, 0.0f
                                             );
        mesh_data.indices[0] = 0;
        mesh_data.indices[1] = 1;
        mesh_data.indices[2] = 2;
        mesh_data.indices[3] = 0;
        mesh_data.indices[4] = 2;
        mesh_data.indices[5] = 3;

        mesh_data.faces[0] = Face(0, 1, 2);
        mesh_data.faces[1] = Face(0, 2, 3);
        return mesh_data;
    }

    MeshData MeshDataGenerator::CreateRectangle(Real w, Real h)
    {
        MeshData mesh_data;
        mesh_data.vertices.resize(4);
        mesh_data.indices.resize(6);
        mesh_data.faces.resize(2);

        Real w2 = 0.5f * w;
        Real h2 = 0.5f * h;

        mesh_data.vertices[0] = SkinnedVertex(-w2, -h2, 0, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[1] = SkinnedVertex(-w2, +h2, 0, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[2] = SkinnedVertex(+w2, +h2, 0, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);
        mesh_data.vertices[3] = SkinnedVertex(+w2, -h2, 0, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f);

        mesh_data.indices[0] = 0; mesh_data.indices[1] = 1; mesh_data.indices[2] = 2;
        mesh_data.indices[3] = 0; mesh_data.indices[4] = 2; mesh_data.indices[5] = 3;

        mesh_data.faces[0] = Face(0, 1, 2);
        mesh_data.faces[1] = Face(0, 2, 3);

        return mesh_data;
    }

    SkinnedVertex MeshDataGenerator::MidPoint(const SkinnedVertex& v0, const SkinnedVertex& v1)
    {
        SkinnedVertex mid_vertex;
        mid_vertex.pos = (v0.pos + v1.pos).Half();
        mid_vertex.tex = (v0.tex + v1.tex).Half();
        mid_vertex.n   = (v0.n + v1.n).Half().Normalize();
        mid_vertex.t   = (v0.t + v1.t).Half().Normalize();

        return mid_vertex;
    }

    void MeshDataGenerator::Subdivide(MeshData& mesh_data)
    {
        // Save a copy of the input geometry.
        std::vector<SkinnedVertex> vertices = mesh_data.vertices;
        std::vector<Uint32>        indices  = mesh_data.indices;

        mesh_data.vertices.clear();
        mesh_data.indices.clear();
        mesh_data.faces.clear();
        Uint32 tri_angle_count = static_cast<Uint32>(indices.size()) / 3;

        for (Uint32 i = 0; i < tri_angle_count; ++i)
        {
            SkinnedVertex v0 = vertices[indices[i * 3 + 0]];
            SkinnedVertex v1 = vertices[indices[i * 3 + 1]];
            SkinnedVertex v2 = vertices[indices[i * 3 + 2]];
            SkinnedVertex m0 = MidPoint(v0, v1);
            SkinnedVertex m1 = MidPoint(v1, v2);
            SkinnedVertex m2 = MidPoint(v0, v2);

            //push vertices
            mesh_data.vertices.push_back(v0); // 0
            mesh_data.vertices.push_back(v1); // 1
            mesh_data.vertices.push_back(v2); // 2
            mesh_data.vertices.push_back(m0); // 3
            mesh_data.vertices.push_back(m1); // 4
            mesh_data.vertices.push_back(m2); // 5
            //push indices
            mesh_data.indices.push_back(i * 6 + 0);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 4);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 5);
            mesh_data.indices.push_back(i * 6 + 4);
            mesh_data.indices.push_back(i * 6 + 2);
            mesh_data.indices.push_back(i * 6 + 3);
            mesh_data.indices.push_back(i * 6 + 1);
            mesh_data.indices.push_back(i * 6 + 4);
            //push faces
            mesh_data.faces.push_back(Face(i * 6 + 0, i * 6 + 3, i * 6 + 5));
            mesh_data.faces.push_back(Face(i * 6 + 3, i * 6 + 4, i * 6 + 5));
            mesh_data.faces.push_back(Face(i * 6 + 5, i * 6 + 4, i * 6 + 2));
            mesh_data.faces.push_back(Face(i * 6 + 3, i * 6 + 1, i * 6 + 4));
        }
    }

    void MeshDataGenerator::BuildCylinderTopCap(Real top_radius, Real height, Uint32 slice_count, MeshData& mesh_data)
    {
        Uint32 base_index = static_cast<Uint32>(mesh_data.vertices.size());
        Real   y          = 0.5f * height;
        Real   dtheta     = Math::TWO_PI / slice_count;
        // Duplicate cap ring vertices because the texture coordinates and normals differ.
        for (Uint32 i = 0; i <= slice_count; ++i)
        {
            Real x = top_radius * cosf(i * dtheta);
            Real z = top_radius * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.
            Real u = x / height + 0.5f;
            Real v = z / height + 0.5f;
            mesh_data.vertices.push_back(SkinnedVertex(x, y, z, u, v, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
        }
        // Cap center vertex.
        mesh_data.vertices.push_back(SkinnedVertex(0.0f, y, 0.0f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
        // Index of center vertex.
        Uint32 center_index = static_cast<Uint32>(mesh_data.vertices.size()) - 1;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            mesh_data.indices.push_back(center_index);
            mesh_data.indices.push_back(base_index + i + 1);
            mesh_data.indices.push_back(base_index + i);
            mesh_data.faces.emplace_back(center_index, base_index + i + 1, base_index + i);
        }
    }

    void MeshDataGenerator::BuildCylinderBottomCap(Real bottom_radius, Real height, Uint32 slice_count, MeshData& mesh_data)
    {
        Uint32 base_index = static_cast<Uint32>(mesh_data.vertices.size());
        Real   y          = -0.5f * height;
        // vertices of ring
        Real dtheta = Math::TWO_PI / slice_count;
        for (Uint32 i = 0; i <= slice_count; ++i)
        {
            Real x = bottom_radius * cosf(i * dtheta);
            Real z = bottom_radius * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.
            Real u = x / height + 0.5f;
            Real v = z / height + 0.5f;
            mesh_data.vertices.push_back(SkinnedVertex(x, y, z, u, v, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
        }
        // Cap center vertex.
        mesh_data.vertices.push_back(SkinnedVertex(0.0f, y, 0.0f, 0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f));
        // Cache the index of center vertex.
        Uint32 center_index = static_cast<Uint32>(mesh_data.vertices.size()) - 1;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            mesh_data.indices.push_back(center_index);
            mesh_data.indices.push_back(base_index + i);
            mesh_data.indices.push_back(base_index + i + 1);
            mesh_data.faces.emplace_back(center_index, base_index + i, base_index + i + 1);
        }
    }

    void MeshDataGenerator::BuildCapsuleSphere(Real radius, Real height, Uint32 slice_count, MeshData& mesh_data)
    {
        Uint32 base_index = static_cast<Uint32>(mesh_data.vertices.size());

        Real half_height = height * 0.5f;

        SkinnedVertex top_vertex(0.0f, half_height + radius, 0.0f, 0.0f, 0.0f, 0.0f, +1.0f, 0.0f, 1.0f, 0.0f, 0.0f);
        SkinnedVertex bottom_vertex(0.0f, -half_height - radius, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f);

        mesh_data.vertices.push_back(top_vertex);
        Real phi_step   = Math::PI / 15;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        for (Uint32 i = 1; i <= 8; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;

                SkinnedVertex vertex;
                vertex.pos = Vector3(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta));
                vertex.tex = Vector2(theta / Math::TWO_PI, phi / Math::PI);
                vertex.n   = vertex.pos.Normalize();
                vertex.t   = Vector3(-radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta)).Normalize();

                vertex.pos.y += half_height;

                mesh_data.vertices.push_back(vertex);
            }
        }

        for (Uint32 i = 7; i <= 14; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;

                SkinnedVertex vertex;
                vertex.pos = Vector3(radius * sinf(phi) * cosf(theta), radius * cosf(phi), radius * sinf(phi) * sinf(theta));
                vertex.tex = Vector2(theta / Math::TWO_PI, phi / Math::PI);
                vertex.n   = vertex.pos.Normalize();
                vertex.t   = Vector3(-radius * sinf(phi) * sinf(theta), 0.0f, radius * sinf(phi) * cosf(theta)).Normalize();

                vertex.pos.y -= half_height;

                mesh_data.vertices.push_back(vertex);
            }
        }

        mesh_data.vertices.push_back(bottom_vertex);
        // Compute indices for top stack.  The top stack was written first to the vertex buffer
        // and connects the top pole to the first ring.
        for (Uint32 i = 1; i <= slice_count; ++i)
        {
            mesh_data.indices.push_back(base_index);
            mesh_data.indices.push_back(base_index + i + 1);
            mesh_data.indices.push_back(base_index + i);

            mesh_data.faces.emplace_back(base_index, base_index + i + 1, base_index + i);
        }
        // Compute indices for inner stacks (not connected to poles).
        // Offset the indices to the index of the first vertex in the first ring.
        // This is just skipping the top pole vertex.
        base_index += 1;
        Uint32 ring_vertex_count = slice_count + 1;
        for (Uint32 i = 0; i < 7; ++i)
        {
            for (Uint32 j = 0; j < slice_count; ++j)
            {
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);

                mesh_data.faces.emplace_back(
                                             base_index + i * ring_vertex_count + j,
                                             base_index + i * ring_vertex_count + j + 1,
                                             base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.faces.emplace_back(
                                             base_index + (i + 1) * ring_vertex_count + j,
                                             base_index + i * ring_vertex_count + j + 1,
                                             base_index + (i + 1) * ring_vertex_count + j + 1);
            }
        }

        for (Uint32 i = 8; i < 15; ++i)
        {
            for (Uint32 j = 0; j < slice_count; ++j)
            {
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                mesh_data.indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);

                mesh_data.faces.emplace_back(
                                             base_index + i * ring_vertex_count + j,
                                             base_index + i * ring_vertex_count + j + 1,
                                             base_index + (i + 1) * ring_vertex_count + j);
                mesh_data.faces.emplace_back(
                                             base_index + (i + 1) * ring_vertex_count + j,
                                             base_index + i * ring_vertex_count + j + 1,
                                             base_index + (i + 1) * ring_vertex_count + j + 1);
            }
        }

        // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
        // and connects the bottom pole to the bottom ring.
        // South pole vertex was added last.
        Uint32 south_pole_index = static_cast<Uint32>(mesh_data.vertices.size()) - 1;
        // Offset the indices to the index of the first vertex in the last ring.
        base_index = south_pole_index - ring_vertex_count;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            mesh_data.indices.push_back(south_pole_index);
            mesh_data.indices.push_back(base_index + i);
            mesh_data.indices.push_back(base_index + i + 1);
            mesh_data.faces.emplace_back(south_pole_index, base_index + i, base_index + i + 1);
        }
    }
}
