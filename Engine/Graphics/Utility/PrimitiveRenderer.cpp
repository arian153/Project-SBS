#include "PrimitiveRenderer.hpp"

namespace Engine
{
    PrimitiveRenderer::PrimitiveRenderer()
    {
    }

    PrimitiveRenderer::~PrimitiveRenderer()
    {
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Circle& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Ellipse& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Rectangle& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Triangle& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Box& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Capsule& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }

        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Cone& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Cylinder& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Dome& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Ellipsoid& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Sphere& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }
        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const Tetrahedron& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;
        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }

        return result;
    }

    PrimitiveSubMesh PrimitiveRenderer::GenSubMesh(const TruncatedCone& primitive, eRenderingMode mode)
    {
        PrimitiveSubMesh result;

        if (mode == eRenderingMode::Line)
        {
        }
        else if (mode == eRenderingMode::Face)
        {
        }

        return result;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Circle& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Ellipse& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Rectangle& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Triangle& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Box& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Capsule& primitive)
    {
        FaceSubMesh sub_mesh;

        Uint32 slice_count = 50;
        Uint32 stack_count = 25;

        Uint32 base_index = static_cast<Uint32>(sub_mesh.vertices.size());

        Real half_height = primitive.height * 0.5f;

        NormalVertex top_vertex;
        top_vertex.pos = Vector3(0.0f, half_height + primitive.radius.y, 0.0f);
        top_vertex.n   = Vector3(0, 1, 0);
        NormalVertex bottom_vertex;
        bottom_vertex.pos = Vector3(0.0f, -half_height - primitive.radius.y, 0.0f);
        bottom_vertex.n   = Vector3(0, -1, 0);

        sub_mesh.vertices.push_back(top_vertex);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        Uint32 half_count = (stack_count - 1) / 2 + 1;
        for (Uint32 i = 1; i <= stack_count - 1; ++i)
        {
            Real phi = i * phi_step;
            // Vertices of ring.
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real theta = j * theta_step;

                NormalVertex vertex;
                vertex.pos = Vector3(
                                     primitive.radius.x * sinf(phi) * cosf(theta),
                                     primitive.radius.y * cosf(phi),
                                     primitive.radius.z * sinf(phi) * sinf(theta));
                vertex.n = vertex.pos.Normalize();
                vertex.pos.y += (i < half_count ? half_height : -half_height);
                sub_mesh.vertices.push_back(vertex);
            }
        }

        sub_mesh.vertices.push_back(bottom_vertex);
        // Compute indices for top stack.  The top stack was written first to the vertex buffer
        // and connects the top pole to the first ring.
        for (Uint32 i = 1; i <= slice_count; ++i)
        {
            sub_mesh.indices.push_back(base_index);
            sub_mesh.indices.push_back(base_index + i + 1);
            sub_mesh.indices.push_back(base_index + i);
        }
        // Compute indices for inner stacks (not connected to poles).
        // Offset the indices to the index of the first vertex in the first ring.
        // This is just skipping the top pole vertex.

        Uint32 ring_vertex_count = slice_count + 1;
        base_index += 1;
        for (Uint32 i = 0; i < stack_count - 2; ++i)
        {
            for (Uint32 j = 0; j < slice_count; ++j)
            {
                sub_mesh.indices.push_back(base_index + i * ring_vertex_count + j);
                sub_mesh.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                sub_mesh.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                sub_mesh.indices.push_back(base_index + (i + 1) * ring_vertex_count + j);
                sub_mesh.indices.push_back(base_index + i * ring_vertex_count + j + 1);
                sub_mesh.indices.push_back(base_index + (i + 1) * ring_vertex_count + j + 1);
            }
        }

        // Compute indices for bottom stack.  The bottom stack was written last to the vertex buffer
        // and connects the bottom pole to the bottom ring.
        // South pole vertex was added last.
        Uint32 south_pole_index = static_cast<Uint32>(sub_mesh.vertices.size()) - 1;
        // Offset the indices to the index of the first vertex in the last ring.
        base_index = south_pole_index - ring_vertex_count;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            sub_mesh.indices.push_back(south_pole_index);
            sub_mesh.indices.push_back(base_index + i);
            sub_mesh.indices.push_back(base_index + i + 1);
        }

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Cone& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Cylinder& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Dome& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Ellipsoid& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Sphere& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const Tetrahedron& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }

    FaceSubMesh PrimitiveRenderer::GenFaceSubMesh(const TruncatedCone& primitive)
    {
        FaceSubMesh sub_mesh;

        return sub_mesh;
    }
}
