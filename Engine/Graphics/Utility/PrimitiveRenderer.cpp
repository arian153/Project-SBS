#include "PrimitiveRenderer.hpp"

#include "../GraphicsDefine.hpp"
#include "../../Math/Structure/Transform.hpp"
#include "../DirectX12/Buffer/ConstantBuffer.hpp"
#include "../DirectX12/Buffer/InstancingBuffer.hpp"
#include "../Element/Model.hpp"

namespace Engine
{
    PrimitiveRenderer::PrimitiveRenderer()
    {
        m_line_mesh = std::make_shared<Model>();
    }

    PrimitiveRenderer::~PrimitiveRenderer()
    {
    }

    void PrimitiveRenderer::Update(const Matrix44& view, const Matrix44& proj)
    {
        m_view = view;
        m_proj = proj;

        for (auto& [uuid, model] : m_inst_deferred_mesh)
        {
            model->ClearInstance();
        }

        for (auto& [uuid, model] : m_instanced_line_mesh)
        {
            model->ClearInstance();
        }

        for (auto& [uuid, model] : m_instanced_face_mesh)
        {
            model->ClearInstance();
        }

        m_line_data.indices.clear();
        m_line_data.vertices.clear();
    }

    void PrimitiveRenderer::DrawPrimitive(const Primitive& primitive, const Transform& tf, const Color& color, eRenderingMode mode)
    {
        SPtr<Model> model          = nullptr;
        size_t      uuid           = primitive.UUID();
        bool        is_created     = false;
        auto        primitive_type = primitive.Type();

        if (primitive_type == ePrimitiveType::Polygon ||
            primitive_type == ePrimitiveType::Polyhedron ||
            primitive_type == ePrimitiveType::Last)
            return;

        if (mode == eRenderingMode::Line)
        {
            model = AddLineModel(uuid, is_created);
            if (is_created)
            {
                switch (primitive_type)
                {
                case ePrimitiveType::Circle:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Circle&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Ellipse:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Ellipse&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Rectangle:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Rectangle&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Triangle:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Triangle&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Box:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Box&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Capsule:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Capsule&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Cone:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Cone&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Cylinder:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Cylinder&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Dome:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Dome&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Ellipsoid:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Ellipsoid&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Sphere:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Sphere&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::Tetrahedron:
                    model->SetMeshData(GenLineSubMesh(static_cast<const Tetrahedron&>(primitive)), eTopologyType::LineList);
                    break;
                case ePrimitiveType::TruncatedCone:
                    model->SetMeshData(GenLineSubMesh(static_cast<const TruncatedCone&>(primitive)), eTopologyType::LineList);
                    break;
                default: ;
                }
            }
        }
        else if (mode == eRenderingMode::Face)
        {
            model = AddFaceModel(uuid, is_created);
            if (is_created)
            {
                switch (primitive.Type())
                {
                case ePrimitiveType::Circle:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Circle&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Ellipse:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Ellipse&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Rectangle:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Rectangle&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Triangle:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Triangle&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Box:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Box&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Capsule:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Capsule&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Cone:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Cone&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Cylinder:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Cylinder&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Dome:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Dome&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Ellipsoid:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Ellipsoid&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Sphere:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Sphere&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::Tetrahedron:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const Tetrahedron&>(primitive))), eTopologyType::TriangleList);
                    break;
                case ePrimitiveType::TruncatedCone:
                    model->SetMeshData(SubMeshUtility::ToForward(GenFaceSubMesh(static_cast<const TruncatedCone&>(primitive))), eTopologyType::TriangleList);
                    break;
                default: ;
                }
            }
        }
        else if (mode == eRenderingMode::Lighting)
        {
            model = AddDeferredModel(uuid, is_created);
            if (is_created)
            {
                switch (primitive.Type())
                {
                case ePrimitiveType::Circle:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Circle&>(primitive)));
                    break;
                case ePrimitiveType::Ellipse:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Ellipse&>(primitive)));
                    break;
                case ePrimitiveType::Rectangle:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Rectangle&>(primitive)));
                    break;
                case ePrimitiveType::Triangle:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Triangle&>(primitive)));
                    break;
                case ePrimitiveType::Box:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Box&>(primitive)));
                    break;
                case ePrimitiveType::Capsule:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Capsule&>(primitive)));
                    break;
                case ePrimitiveType::Cone:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Cone&>(primitive)));
                    break;
                case ePrimitiveType::Cylinder:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Cylinder&>(primitive)));
                    break;
                case ePrimitiveType::Dome:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Dome&>(primitive)));
                    break;
                case ePrimitiveType::Ellipsoid:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Ellipsoid&>(primitive)));
                    break;
                case ePrimitiveType::Sphere:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Sphere&>(primitive)));
                    break;
                case ePrimitiveType::Tetrahedron:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const Tetrahedron&>(primitive)));
                    break;
                case ePrimitiveType::TruncatedCone:
                    model->SetMeshData(GenFaceSubMesh(static_cast<const TruncatedCone&>(primitive)));
                    break;
                default: ;
                }
            }
        }

        if (model == nullptr)
            return;

        InstancingParams params;
        params.mat_world = tf.LocalToWorldMatrix();
        params.mat_wv    = params.mat_world * m_view;
        params.mat_wvp   = params.mat_world * m_view * m_proj;
        params.diffuse   = color;
        model->AddInstance(params);
    }

    void PrimitiveRenderer::DrawLine(const Vector3& start, const Vector3& end)
    {
        Uint32 index = static_cast<Uint32>(m_line_data.vertices.size());

        PosVertex vertex;
        vertex.pos = start;
        m_line_data.vertices.push_back(vertex);
        vertex.pos = end;
        m_line_data.vertices.push_back(vertex);

        m_line_data.indices.push_back(index);
        m_line_data.indices.push_back(index + 1);
    }

    void PrimitiveRenderer::RenderDeferred(SPtr<ConstantBuffer> material_buffer)
    {
        for (auto& [fst, snd] : m_inst_deferred_mesh)
        {
            snd->Render(material_buffer);
        }
    }

    void PrimitiveRenderer::RenderForward(SPtr<ConstantBuffer> material_buffer)
    {
        for (auto& [fst, snd] : m_instanced_line_mesh)
        {
            snd->Render(material_buffer);
        }

        for (auto& [fst, snd] : m_instanced_face_mesh)
        {
            snd->Render(material_buffer);
        }
    }

    void PrimitiveRenderer::RenderLines(SPtr<ConstantBuffer> matrix_buffer, SPtr<ConstantBuffer> material_buffer) const
    {
        if (m_line_data.vertices.empty())
            return;

        m_line_mesh->SetMeshData(m_line_data, eTopologyType::LineList);
        m_line_mesh->SetShader(GET_SHADER("Forward-Line.shader"));

        MatrixParams matrix_params;
        matrix_params.view  = m_view;
        matrix_params.proj  = m_proj;
        matrix_params.world = Matrix44::Identity();
        matrix_params.wv    = matrix_params.view;
        matrix_params.wvp   = matrix_params.view * matrix_params.proj;

        matrix_buffer->PushData(&matrix_params, sizeof(matrix_params));
        m_line_mesh->Render(material_buffer);
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Circle& primitive)
    {
        DeferredSubMesh sub_mesh;

        sub_mesh.vertices.resize(202);

        Real radian_step = Math::TWO_PI / static_cast<Real>(100);
        for (Uint32 i = 0; i < 100; ++i)
        {
            Real angle               = static_cast<Real>(i) * radian_step;
            sub_mesh.vertices[i].pos = Vector3(cosf(angle) * primitive.radius, sinf(angle) * primitive.radius, 0.0f);
            sub_mesh.vertices[i].n   = Vector3(0, 1, 0);
        }

        NormalVertex vertex;
        sub_mesh.vertices[100].n = Vector3(0, 1, 0);

        for (Uint32 i = 0; i < 99; ++i)
        {
            sub_mesh.indices.push_back(100);
            sub_mesh.indices.push_back(i + 1);
            sub_mesh.indices.push_back(i);
        }

        sub_mesh.indices.push_back(100);
        sub_mesh.indices.push_back(0);
        sub_mesh.indices.push_back(99);

        for (Uint32 i = 101; i < 201; ++i)
        {
            Real angle               = static_cast<Real>(i) * radian_step;
            sub_mesh.vertices[i].pos = Vector3(cosf(angle) * primitive.radius, sinf(angle) * primitive.radius, 0.0f);
            sub_mesh.vertices[i].n   = Vector3(0, -1, 0);
        }

        sub_mesh.vertices[201].n = Vector3(0, -1, 0);

        for (Uint32 i = 101; i < 200; ++i)
        {
            sub_mesh.indices.push_back(201);
            sub_mesh.indices.push_back(i);
            sub_mesh.indices.push_back(i + 1);
        }

        sub_mesh.indices.push_back(201);
        sub_mesh.indices.push_back(200);
        sub_mesh.indices.push_back(101);
        return sub_mesh;
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Ellipse& primitive)
    {
        DeferredSubMesh sub_mesh;

        sub_mesh.vertices.resize(202);

        Real radian_step = Math::TWO_PI / static_cast<Real>(100);
        for (Uint32 i = 0; i < 100; ++i)
        {
            Real angle               = static_cast<Real>(i) * radian_step;
            sub_mesh.vertices[i].pos = Vector3(cosf(angle) * primitive.radius.x, sinf(angle) * primitive.radius.y, 0.0f);
            sub_mesh.vertices[i].n   = Vector3(0, 1, 0);
        }

        NormalVertex vertex;
        sub_mesh.vertices[100].n = Vector3(0, 1, 0);

        for (Uint32 i = 0; i < 99; ++i)
        {
            sub_mesh.indices.push_back(100);
            sub_mesh.indices.push_back(i + 1);
            sub_mesh.indices.push_back(i);
        }

        sub_mesh.indices.push_back(100);
        sub_mesh.indices.push_back(0);
        sub_mesh.indices.push_back(99);

        for (Uint32 i = 101; i < 201; ++i)
        {
            Real angle               = static_cast<Real>(i) * radian_step;
            sub_mesh.vertices[i].pos = Vector3(cosf(angle) * primitive.radius.x, sinf(angle) * primitive.radius.y, 0.0f);
            sub_mesh.vertices[i].n   = Vector3(0, -1, 0);
        }

        sub_mesh.vertices[201].n = Vector3(0, -1, 0);

        for (Uint32 i = 101; i < 200; ++i)
        {
            sub_mesh.indices.push_back(201);
            sub_mesh.indices.push_back(i);
            sub_mesh.indices.push_back(i + 1);
        }

        sub_mesh.indices.push_back(201);
        sub_mesh.indices.push_back(200);
        sub_mesh.indices.push_back(101);

        return sub_mesh;
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Rectangle& primitive)
    {
        DeferredSubMesh sub_mesh;

        sub_mesh.vertices.resize(8);

        for (Uint32 i = 0; i < 4; ++i)
        {
            sub_mesh.vertices[i].pos = primitive.vertices[i];
            sub_mesh.vertices[i].n   = Vector3(0, 1, 0);
        }

        for (Uint32 i = 4; i < 8; ++i)
        {
            sub_mesh.vertices[i].pos = primitive.vertices[i % 4];
            sub_mesh.vertices[i].n   = Vector3(0, -1, 0);
        }

        sub_mesh.indices.resize(12);

        sub_mesh.indices[0] = 0;
        sub_mesh.indices[1] = 1;
        sub_mesh.indices[2] = 2;

        sub_mesh.indices[3] = 1;
        sub_mesh.indices[4] = 3;
        sub_mesh.indices[5] = 2;

        sub_mesh.indices[6] = 4;
        sub_mesh.indices[7] = 6;
        sub_mesh.indices[8] = 5;

        sub_mesh.indices[9]  = 5;
        sub_mesh.indices[10] = 6;
        sub_mesh.indices[11] = 7;

        return sub_mesh;
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Triangle& primitive)
    {
        DeferredSubMesh sub_mesh;

        sub_mesh.vertices.resize(6);

        for (Uint32 i = 0; i < 3; ++i)
        {
            sub_mesh.vertices[i].pos = primitive.vertices[i];
            sub_mesh.vertices[i].n   = Vector3(0, 1, 0);
        }

        for (Uint32 i = 3; i < 6; ++i)
        {
            sub_mesh.vertices[i].pos = primitive.vertices[i % 3];
            sub_mesh.vertices[i].n   = Vector3(0, -1, 0);
        }

        sub_mesh.indices.resize(12);

        sub_mesh.indices[0] = 0;
        sub_mesh.indices[1] = 1;
        sub_mesh.indices[2] = 2;

        sub_mesh.indices[3] = 0;
        sub_mesh.indices[4] = 2;
        sub_mesh.indices[5] = 1;

        sub_mesh.indices[6] = 3;
        sub_mesh.indices[7] = 4;
        sub_mesh.indices[8] = 5;

        sub_mesh.indices[9]  = 3;
        sub_mesh.indices[10] = 5;
        sub_mesh.indices[11] = 4;

        return sub_mesh;
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Box& primitive)
    {
        DeferredSubMesh sub_mesh;

        sub_mesh.vertices.resize(8);

        sub_mesh.vertices[0].n = Vector3(+1.0f, +1.0f, +1.0f).Normalize();
        sub_mesh.vertices[1].n = Vector3(+1.0f, +1.0f, -1.0f).Normalize();
        sub_mesh.vertices[2].n = Vector3(+1.0f, -1.0f, +1.0f).Normalize();
        sub_mesh.vertices[3].n = Vector3(+1.0f, -1.0f, -1.0f).Normalize();
        sub_mesh.vertices[4].n = Vector3(-1.0f, +1.0f, +1.0f).Normalize();
        sub_mesh.vertices[5].n = Vector3(-1.0f, +1.0f, -1.0f).Normalize();
        sub_mesh.vertices[6].n = Vector3(-1.0f, -1.0f, +1.0f).Normalize();
        sub_mesh.vertices[7].n = Vector3(-1.0f, -1.0f, -1.0f).Normalize();

        for (Uint32 i = 0; i < 8; ++i)
        {
            sub_mesh.vertices[i].pos = primitive.vertices[i];
        }

        sub_mesh.indices.resize(36);

        //front
        sub_mesh.indices[0] = 6;
        sub_mesh.indices[1] = 2;
        sub_mesh.indices[2] = 4;
        sub_mesh.indices[3] = 2;
        sub_mesh.indices[4] = 0;
        sub_mesh.indices[5] = 4;

        //back
        sub_mesh.indices[6]  = 1;
        sub_mesh.indices[7]  = 3;
        sub_mesh.indices[8]  = 5;
        sub_mesh.indices[9]  = 3;
        sub_mesh.indices[10] = 7;
        sub_mesh.indices[11] = 5;

        //left
        sub_mesh.indices[12] = 6;
        sub_mesh.indices[13] = 5;
        sub_mesh.indices[14] = 7;
        sub_mesh.indices[15] = 5;
        sub_mesh.indices[16] = 6;
        sub_mesh.indices[17] = 4;

        //right
        sub_mesh.indices[18] = 1;
        sub_mesh.indices[19] = 0;
        sub_mesh.indices[20] = 3;
        sub_mesh.indices[21] = 0;
        sub_mesh.indices[22] = 2;
        sub_mesh.indices[23] = 3;

        //top
        sub_mesh.indices[24] = 1;
        sub_mesh.indices[25] = 4;
        sub_mesh.indices[26] = 0;
        sub_mesh.indices[27] = 1;
        sub_mesh.indices[28] = 5;
        sub_mesh.indices[29] = 4;

        //bottom
        sub_mesh.indices[30] = 3;
        sub_mesh.indices[31] = 2;
        sub_mesh.indices[32] = 6;
        sub_mesh.indices[33] = 3;
        sub_mesh.indices[34] = 6;
        sub_mesh.indices[35] = 7;

        return sub_mesh;
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Capsule& primitive)
    {
        DeferredSubMesh sub_mesh;

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

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Cone& primitive)
    {
        DeferredSubMesh sub_mesh;
        Uint32          stack_count = 1;
        Uint32          slice_count = 100;

        Real stack_height = primitive.height / stack_count;
        // Amount to increment radius as we move up each stack level from bottom to top.
        Real   radius_step = - 1.0f / stack_count;
        Uint32 ring_count  = stack_count + 1;
        // Compute vertices for each stack ring starting at the bottom and moving up.
        for (Uint32 i = 0; i < ring_count; ++i)
        {
            Real y = -0.5f * primitive.height + i * stack_height;
            Real r = 1.0f + i * radius_step;

            // vertices of ring
            Real d_theta = Math::TWO_PI / slice_count;
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real    c         = cosf(j * d_theta);
                Real    s         = sinf(j * d_theta);
                Real    dr        = - 1.0f;
                Vector3 bitangent = Vector3(primitive.radius.x * dr * c, -primitive.height, primitive.radius.y * dr * s).Normalize();
                Vector3 tangent   = Vector3(primitive.radius.x * -s, 0.0f, primitive.radius.y * c).Normalize();

                NormalVertex vertex;
                vertex.pos = Vector3(primitive.radius.x * r * c, y, primitive.radius.y * r * s);
                vertex.n   = CrossProduct(tangent, bitangent).Normalize();
                sub_mesh.vertices.push_back(vertex);
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
                sub_mesh.indices.push_back(i * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                sub_mesh.indices.push_back(i * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                sub_mesh.indices.push_back(i * ring_vertex_count + j + 1);
            }
        }
        Uint32 base_index = static_cast<Uint32>(sub_mesh.vertices.size());
        Real   y          = -0.5f * primitive.height;
        Real   dtheta     = Math::TWO_PI / slice_count;
        // vertices of ring
        for (Uint32 i = 0; i <= slice_count; ++i)
        {
            Real x = primitive.radius.x * cosf(i * dtheta);
            Real z = primitive.radius.y * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.

            NormalVertex vertex;
            vertex.pos = Vector3(x, y, z);
            vertex.n   = Vector3(0.0f, -1.0f, 0.0f);

            sub_mesh.vertices.push_back(vertex);
        }
        // Cap center vertex.
        NormalVertex cap_vertex;
        cap_vertex.pos = Vector3(0.0f, y, 0.0f);
        cap_vertex.n   = Vector3(0.0f, -1.0f, 0.0f);
        sub_mesh.vertices.push_back(cap_vertex);

        // Cache the index of center vertex.
        Uint32 center_index = static_cast<Uint32>(sub_mesh.vertices.size()) - 1;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            sub_mesh.indices.push_back(center_index);
            sub_mesh.indices.push_back(base_index + i);
            sub_mesh.indices.push_back(base_index + i + 1);
        }
        return sub_mesh;
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Cylinder& primitive)
    {
        DeferredSubMesh sub_mesh;
        Uint32          stack_count = 1;
        Uint32          slice_count = 100;

        Real stack_height = primitive.height / stack_count;
        // Amount to increment radius as we move up each stack level from bottom to top.
        Uint32 ring_count = stack_count + 1;
        // Compute vertices for each stack ring starting at the bottom and moving up.
        for (Uint32 i = 0; i < ring_count; ++i)
        {
            Real y = -0.5f * primitive.height + i * stack_height;

            // vertices of ring
            Real d_theta = Math::TWO_PI / slice_count;
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real    c         = cosf(j * d_theta);
                Real    s         = sinf(j * d_theta);
                Vector3 bitangent = Vector3(0.0f, -primitive.height, 0.0f).Normalize();
                Vector3 tangent   = Vector3(primitive.radius.x * -s, 0.0f, primitive.radius.y * c).Normalize();

                NormalVertex vertex;
                vertex.pos = Vector3(primitive.radius.x * c, y, primitive.radius.y * s);
                vertex.n   = CrossProduct(tangent, bitangent).Normalize();
                sub_mesh.vertices.push_back(vertex);
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
                sub_mesh.indices.push_back(i * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                sub_mesh.indices.push_back(i * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                sub_mesh.indices.push_back(i * ring_vertex_count + j + 1);
            }
        }
        Uint32 base_index = static_cast<Uint32>(sub_mesh.vertices.size());
        Real   y          = 0.5f * primitive.height;
        Real   dtheta     = Math::TWO_PI / slice_count;
        // Duplicate cap ring vertices because the texture coordinates and normals differ.
        for (Uint32 i = 0; i <= slice_count; ++i)
        {
            Real x = primitive.radius.x * cosf(i * dtheta);
            Real z = primitive.radius.y * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.

            NormalVertex vertex;
            vertex.pos = Vector3(x, y, z);
            vertex.n   = Vector3(0.0f, 1.0f, 0.0f);

            sub_mesh.vertices.push_back(vertex);
        }
        // Cap center vertex.

        NormalVertex cap_vertex;
        cap_vertex.pos = Vector3(0.0f, y, 0.0f);
        cap_vertex.n   = Vector3(0.0f, 1.0f, 0.0f);

        sub_mesh.vertices.push_back(cap_vertex);

        // Index of center vertex.
        Uint32 center_index = static_cast<Uint32>(sub_mesh.vertices.size()) - 1;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            sub_mesh.indices.push_back(center_index);
            sub_mesh.indices.push_back(base_index + i + 1);
            sub_mesh.indices.push_back(base_index + i);
        }
        base_index = static_cast<Uint32>(sub_mesh.vertices.size());
        y          = -0.5f * primitive.height;
        // vertices of ring
        for (Uint32 i = 0; i <= slice_count; ++i)
        {
            Real x = primitive.radius.x * cosf(i * dtheta);
            Real z = primitive.radius.y * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.

            NormalVertex vertex;
            vertex.pos = Vector3(x, y, z);
            vertex.n   = Vector3(0.0f, -1.0f, 0.0f);

            sub_mesh.vertices.push_back(vertex);
        }
        // Cap center vertex.
        cap_vertex.pos = Vector3(0.0f, y, 0.0f);
        cap_vertex.n   = Vector3(0.0f, -1.0f, 0.0f);
        sub_mesh.vertices.push_back(cap_vertex);

        // Cache the index of center vertex.
        center_index = static_cast<Uint32>(sub_mesh.vertices.size()) - 1;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            sub_mesh.indices.push_back(center_index);
            sub_mesh.indices.push_back(base_index + i);
            sub_mesh.indices.push_back(base_index + i + 1);
        }
        return sub_mesh;
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Dome& primitive)
    {
        DeferredSubMesh sub_mesh;

        Uint32 slice_count = 50;
        Uint32 stack_count = 24;

        Uint32 base_index = static_cast<Uint32>(sub_mesh.vertices.size());

        NormalVertex top_vertex;
        top_vertex.pos = Vector3(0.0f, primitive.radius.y, 0.0f);
        top_vertex.n   = Vector3(0, 1, 0);

        sub_mesh.vertices.push_back(top_vertex);
        Real phi_step   = Math::PI / stack_count;
        Real theta_step = Math::TWO_PI / slice_count;
        // Compute vertices for each stack ring (do not count the poles as rings).
        //Uint32 half_count = (stack_count - 1) / 2 + 1;
        for (Uint32 i = 1; i <= (stack_count - 1) / 2; ++i)
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
                sub_mesh.vertices.push_back(vertex);
            }
        }

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
        for (Uint32 i = 0; i < (stack_count / 2) - 1; ++i)
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

        for (Uint32 j = 0; j <= slice_count; ++j)
        {
            Real theta = j * theta_step;

            NormalVertex vertex;
            vertex.pos = Vector3(
                                 primitive.radius.x * cosf(theta),
                                 -0.0f,
                                 primitive.radius.z * sinf(theta));
            vertex.n = Vector3(0, -1, 0);
            sub_mesh.vertices.push_back(vertex);
        }

        NormalVertex bottom_vertex;
        bottom_vertex.pos = Vector3(0.0f, -0.0f, 0.0f);
        bottom_vertex.n   = Vector3(0, -1, 0);

        sub_mesh.vertices.push_back(bottom_vertex);

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

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Ellipsoid& primitive)
    {
        DeferredSubMesh sub_mesh;

        Uint32 slice_count = 50;
        Uint32 stack_count = 25;

        Uint32 base_index = static_cast<Uint32>(sub_mesh.vertices.size());

        NormalVertex top_vertex;
        top_vertex.pos = Vector3(0.0f, primitive.radius.y, 0.0f);
        top_vertex.n   = Vector3(0, 1, 0);
        NormalVertex bottom_vertex;
        bottom_vertex.pos = Vector3(0.0f, - primitive.radius.y, 0.0f);
        bottom_vertex.n   = Vector3(0, -1, 0);

        sub_mesh.vertices.push_back(top_vertex);
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

                NormalVertex vertex;
                vertex.pos = Vector3(
                                     primitive.radius.x * sinf(phi) * cosf(theta),
                                     primitive.radius.y * cosf(phi),
                                     primitive.radius.z * sinf(phi) * sinf(theta));
                vertex.n = vertex.pos.Normalize();
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

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Sphere& primitive)
    {
        DeferredSubMesh sub_mesh;

        Uint32 slice_count = 50;
        Uint32 stack_count = 25;

        Uint32 base_index = static_cast<Uint32>(sub_mesh.vertices.size());

        NormalVertex top_vertex;
        top_vertex.pos = Vector3(0.0f, primitive.radius, 0.0f);
        top_vertex.n   = Vector3(0, 1, 0);
        NormalVertex bottom_vertex;
        bottom_vertex.pos = Vector3(0.0f, -primitive.radius, 0.0f);
        bottom_vertex.n   = Vector3(0, -1, 0);

        sub_mesh.vertices.push_back(top_vertex);
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

                NormalVertex vertex;
                vertex.pos = Vector3(
                                     primitive.radius * sinf(phi) * cosf(theta),
                                     primitive.radius * cosf(phi),
                                     primitive.radius * sinf(phi) * sinf(theta));
                vertex.n = vertex.pos.Normalize();
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

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const Tetrahedron& primitive)
    {
        DeferredSubMesh sub_mesh;
        sub_mesh.vertices.resize(4);

        for (Uint32 i = 0; i < 4; ++i)
        {
            Uint32 t0 = (i + 1) % 4;
            Uint32 t1 = (i + 2) % 4;
            Uint32 t2 = (i + 3) % 4;

            Vector3 center = Triangle::ClosestPoint(
                                                    primitive.vertices[i],
                                                    primitive.vertices[t0],
                                                    primitive.vertices[t1],
                                                    primitive.vertices[t2]);

            sub_mesh.vertices[i].pos = primitive.vertices[i];
            sub_mesh.vertices[i].n   = (primitive.vertices[i] - center).Normalize();
        }

        sub_mesh.indices.resize(24);

        //1st
        sub_mesh.indices[0] = 0;
        sub_mesh.indices[1] = 1;
        sub_mesh.indices[2] = 2;
        sub_mesh.indices[3] = 0;
        sub_mesh.indices[4] = 2;
        sub_mesh.indices[5] = 1;

        //2nd
        sub_mesh.indices[6]  = 0;
        sub_mesh.indices[7]  = 3;
        sub_mesh.indices[8]  = 1;
        sub_mesh.indices[9]  = 0;
        sub_mesh.indices[10] = 1;
        sub_mesh.indices[11] = 3;

        //3rd
        sub_mesh.indices[12] = 0;
        sub_mesh.indices[13] = 2;
        sub_mesh.indices[14] = 3;
        sub_mesh.indices[15] = 0;
        sub_mesh.indices[16] = 3;
        sub_mesh.indices[17] = 2;

        //4th
        sub_mesh.indices[18] = 1;
        sub_mesh.indices[19] = 2;
        sub_mesh.indices[20] = 3;
        sub_mesh.indices[21] = 1;
        sub_mesh.indices[22] = 3;
        sub_mesh.indices[23] = 2;

        return sub_mesh;
    }

    DeferredSubMesh PrimitiveRenderer::GenFaceSubMesh(const TruncatedCone& primitive)
    {
        DeferredSubMesh sub_mesh;

        Uint32 stack_count = 1;
        Uint32 slice_count = 100;

        Real stack_height = primitive.height / stack_count;
        // Amount to increment radius as we move up each stack level from bottom to top.
        Real   radius_step = (primitive.ratio - 1.0f) / stack_count;
        Uint32 ring_count  = stack_count + 1;
        // Compute vertices for each stack ring starting at the bottom and moving up.
        for (Uint32 i = 0; i < ring_count; ++i)
        {
            Real y = -0.5f * primitive.height + i * stack_height;
            Real r = 1.0f + i * radius_step;

            // vertices of ring
            Real d_theta = Math::TWO_PI / slice_count;
            for (Uint32 j = 0; j <= slice_count; ++j)
            {
                Real    c         = cosf(j * d_theta);
                Real    s         = sinf(j * d_theta);
                Real    dr        = primitive.ratio - 1.0f;
                Vector3 bitangent = Vector3(primitive.radius.x * dr * c, -primitive.height, primitive.radius.y * dr * s).Normalize();
                Vector3 tangent   = Vector3(primitive.radius.x * -s, 0.0f, primitive.radius.y * c).Normalize();

                NormalVertex vertex;
                vertex.pos = Vector3(primitive.radius.x * r * c, y, primitive.radius.y * r * s);
                vertex.n   = CrossProduct(tangent, bitangent).Normalize();
                sub_mesh.vertices.push_back(vertex);
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
                sub_mesh.indices.push_back(i * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                sub_mesh.indices.push_back(i * ring_vertex_count + j);
                sub_mesh.indices.push_back((i + 1) * ring_vertex_count + j + 1);
                sub_mesh.indices.push_back(i * ring_vertex_count + j + 1);
            }
        }
        Uint32 base_index = static_cast<Uint32>(sub_mesh.vertices.size());
        Real   y          = 0.5f * primitive.height;
        Real   dtheta     = Math::TWO_PI / slice_count;
        // Duplicate cap ring vertices because the texture coordinates and normals differ.
        for (Uint32 i = 0; i <= slice_count; ++i)
        {
            Real x = primitive.ratio * primitive.radius.x * cosf(i * dtheta);
            Real z = primitive.ratio * primitive.radius.y * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.

            NormalVertex vertex;
            vertex.pos = Vector3(x, y, z);
            vertex.n   = Vector3(0.0f, 1.0f, 0.0f);

            sub_mesh.vertices.push_back(vertex);
        }
        // Cap center vertex.

        NormalVertex cap_vertex;
        cap_vertex.pos = Vector3(0.0f, y, 0.0f);
        cap_vertex.n   = Vector3(0.0f, 1.0f, 0.0f);

        sub_mesh.vertices.push_back(cap_vertex);

        // Index of center vertex.
        Uint32 center_index = static_cast<Uint32>(sub_mesh.vertices.size()) - 1;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            sub_mesh.indices.push_back(center_index);
            sub_mesh.indices.push_back(base_index + i + 1);
            sub_mesh.indices.push_back(base_index + i);
        }
        base_index = static_cast<Uint32>(sub_mesh.vertices.size());
        y          = -0.5f * primitive.height;
        // vertices of ring
        for (Uint32 i = 0; i <= slice_count; ++i)
        {
            Real x = primitive.radius.x * cosf(i * dtheta);
            Real z = primitive.radius.y * sinf(i * dtheta);
            // Scale down by the height to try and make top cap texture coordinate area
            // proportional to base.

            NormalVertex vertex;
            vertex.pos = Vector3(x, y, z);
            vertex.n   = Vector3(0.0f, -1.0f, 0.0f);

            sub_mesh.vertices.push_back(vertex);
        }
        // Cap center vertex.
        cap_vertex.pos = Vector3(0.0f, y, 0.0f);
        cap_vertex.n   = Vector3(0.0f, -1.0f, 0.0f);
        sub_mesh.vertices.push_back(cap_vertex);

        // Cache the index of center vertex.
        center_index = static_cast<Uint32>(sub_mesh.vertices.size()) - 1;
        for (Uint32 i = 0; i < slice_count; ++i)
        {
            sub_mesh.indices.push_back(center_index);
            sub_mesh.indices.push_back(base_index + i);
            sub_mesh.indices.push_back(base_index + i + 1);
        }

        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Circle& primitive)
    {
        ForwardSubMesh sub_mesh;

        size_t count = 100;
        sub_mesh.vertices.resize(count);

        Real radian_step = Math::TWO_PI / static_cast<Real>(count);
        for (size_t i = 0; i < count; ++i)
        {
            Real angle               = static_cast<Real>(i) * radian_step;
            sub_mesh.vertices[i].pos = Vector3(cosf(angle) * primitive.radius, sinf(angle) * primitive.radius, 0.0f);
        }

        sub_mesh.indices.reserve(count * 2);

        Uint32 size = static_cast<Uint32>(count - 1);
        for (Uint32 i = 0; i < size; ++i)
        {
            sub_mesh.indices.push_back(i);
            sub_mesh.indices.push_back(i + 1);
        }

        sub_mesh.indices.push_back(size);
        sub_mesh.indices.push_back(0);
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Ellipse& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Rectangle& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Triangle& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Box& primitive)
    {
        ForwardSubMesh sub_mesh;
        sub_mesh.vertices.resize(8);

        for (size_t i = 0; i < 8; ++i)
        {
            sub_mesh.vertices[i].pos = primitive.vertices[i];
        }

        //front
        sub_mesh.indices.push_back(0);
        sub_mesh.indices.push_back(2);

        sub_mesh.indices.push_back(2);
        sub_mesh.indices.push_back(6);

        sub_mesh.indices.push_back(6);
        sub_mesh.indices.push_back(4);

        sub_mesh.indices.push_back(4);
        sub_mesh.indices.push_back(0);

        //back
        sub_mesh.indices.push_back(1);
        sub_mesh.indices.push_back(3);

        sub_mesh.indices.push_back(3);
        sub_mesh.indices.push_back(7);

        sub_mesh.indices.push_back(7);
        sub_mesh.indices.push_back(5);

        sub_mesh.indices.push_back(5);
        sub_mesh.indices.push_back(1);
        //side
        sub_mesh.indices.push_back(0);
        sub_mesh.indices.push_back(1);

        sub_mesh.indices.push_back(2);
        sub_mesh.indices.push_back(3);

        sub_mesh.indices.push_back(6);
        sub_mesh.indices.push_back(7);

        sub_mesh.indices.push_back(4);
        sub_mesh.indices.push_back(5);

        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Capsule& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Cone& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Cylinder& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Dome& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Ellipsoid& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Sphere& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const Tetrahedron& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    ForwardSubMesh PrimitiveRenderer::GenLineSubMesh(const TruncatedCone& primitive)
    {
        ForwardSubMesh sub_mesh;
        return sub_mesh;
    }

    SPtr<Model> PrimitiveRenderer::AddLineModel(size_t uuid, bool& is_created)
    {
        auto found = m_instanced_line_mesh.find(uuid);
        if (found == m_instanced_line_mesh.end())
        {
            is_created = true;
            auto model = std::make_shared<Model>();
            model->SetSize(1);
            model->SetMaterial(MATERIAL_MANAGER->GetMaterial("PrimitiveRenderer-Line"));
            m_instanced_line_mesh.emplace(uuid, model);
            return model;
        }
        is_created = false;
        return found->second;
    }

    SPtr<Model> PrimitiveRenderer::AddFaceModel(size_t uuid, bool& is_created)
    {
        auto found = m_instanced_face_mesh.find(uuid);
        if (found == m_instanced_face_mesh.end())
        {
            auto model = std::make_shared<Model>();
            model->SetSize(1);
            model->SetMaterial(MATERIAL_MANAGER->GetMaterial("PrimitiveRenderer-Face"));
            m_instanced_face_mesh.emplace(uuid, model);
            is_created = true;
            return model;
        }
        is_created = false;
        return found->second;
    }

    SPtr<Model> PrimitiveRenderer::AddDeferredModel(size_t uuid, bool& is_created)
    {
        auto found = m_inst_deferred_mesh.find(uuid);
        if (found == m_inst_deferred_mesh.end())
        {
            auto model = std::make_shared<Model>();
            model->SetSize(1);
            model->SetMaterial(MATERIAL_MANAGER->GetMaterial("PrimitiveRenderer-Deferred"));
            m_inst_deferred_mesh.emplace(uuid, model);
            is_created = true;
            return model;
        }
        is_created = false;
        return found->second;
    }
}
