/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#pragma once
#include "../../Math/Primitive/ConvexHull.hpp"
#include "../Data/SubMeshData.hpp"
#include "../Element/Mesh.hpp"

namespace Engine
{
    class ConstantBuffer;
    class Transform;
    class Model;

    enum class eRenderingMode
    {
        Line,
        Face,
        Lighting
    };

    class PrimitiveRenderer
    {
    public:
        PrimitiveRenderer();
        ~PrimitiveRenderer();

        void Update(const Matrix44& view, const Matrix44& proj);

        void DrawPrimitive(const Primitive& primitive, const Transform& tf, const Color& color, eRenderingMode mode, bool is_updated);
        void DrawLine(const Vector3& start, const Vector3& end);

        void RenderDeferred(SPtr<ConstantBuffer> material_buffer);
        void RenderForward(SPtr<ConstantBuffer> material_buffer);
        void RenderLines(SPtr<ConstantBuffer> matrix_buffer, SPtr<ConstantBuffer> material_buffer) const;

    public:
        static DeferredSubMesh GenFaceSubMesh(const Circle& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Ellipse& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Rectangle& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Triangle& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Box& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Capsule& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Cone& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Cylinder& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Dome& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Ellipsoid& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Sphere& primitive);
        static DeferredSubMesh GenFaceSubMesh(const Tetrahedron& primitive);
        static DeferredSubMesh GenFaceSubMesh(const TruncatedCone& primitive);

    public:
        static ForwardSubMesh GenLineSubMesh(const Circle& primitive);
        static ForwardSubMesh GenLineSubMesh(const Ellipse& primitive);
        static ForwardSubMesh GenLineSubMesh(const Rectangle& primitive);
        static ForwardSubMesh GenLineSubMesh(const Triangle& primitive);
        static ForwardSubMesh GenLineSubMesh(const Box& primitive);
        static ForwardSubMesh GenLineSubMesh(const Capsule& primitive);
        static ForwardSubMesh GenLineSubMesh(const Cone& primitive);
        static ForwardSubMesh GenLineSubMesh(const Cylinder& primitive);
        static ForwardSubMesh GenLineSubMesh(const Dome& primitive);
        static ForwardSubMesh GenLineSubMesh(const Ellipsoid& primitive);
        static ForwardSubMesh GenLineSubMesh(const Sphere& primitive);
        static ForwardSubMesh GenLineSubMesh(const Tetrahedron& primitive);
        static ForwardSubMesh GenLineSubMesh(const TruncatedCone& primitive);

        static MeshData ToMeshData(const DeferredSubMesh& sub_mesh);
        static MeshData ToMeshData(const ForwardSubMesh& sub_mesh);

    private:
        SPtr<Model> AddLineModel(size_t uuid, bool& is_created, bool is_updated);
        SPtr<Model> AddFaceModel(size_t uuid, bool& is_created, bool is_updated);
        SPtr<Model> AddDeferredModel(size_t uuid, bool& is_created, bool is_updated);

    private:
        HashMap<size_t, SPtr<Model>> m_instanced_line_mesh;
        HashMap<size_t, SPtr<Model>> m_instanced_face_mesh;
        HashMap<size_t, SPtr<Model>> m_inst_deferred_mesh;

        Matrix44 m_view;
        Matrix44 m_proj;

        ForwardSubMesh m_line_data;
        SPtr<Model>    m_line_mesh;
    };
}
