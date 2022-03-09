#pragma once
#include "../../Math/Primitive/ConvexHull.hpp"
#include "../DirectX12/Buffer/IndexBuffer.hpp"
#include "../DirectX12/Buffer/VertexBuffer.hpp"

namespace Engine
{
    enum class eRenderingMode
    {
        Line,
        Face
    };

    struct LineSubMesh
    {
        std::vector<PosVertex> vertices;
        std::vector<Uint32>    indices;
    };

    struct FaceSubMesh
    {
        std::vector<NormalVertex> vertices;
        std::vector<Uint32>       indices;
    };

    struct PrimitiveSubMesh
    {
        VertexBuffer vertex_buffer;
        IndexBuffer  index_buffer;
    };

    //struct InstancingSubMesh
    //{
    //    VertexBuffer   vertex_buffer;
    //    IndexBuffer    index_buffer;
    //    InstanceBuffer instance_buffer;
    //    Uint32         max_count = 0;

    //    std::vector<InstanceBufferData> instances;
    //};

    class PrimitiveRenderer
    {
    public:
        PrimitiveRenderer();
        ~PrimitiveRenderer();

        PrimitiveSubMesh GenSubMesh(const Circle& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Ellipse& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Rectangle& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Triangle& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Box& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Capsule& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Cone& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Cylinder& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Dome& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Ellipsoid& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Sphere& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const Tetrahedron& primitive, eRenderingMode mode);
        PrimitiveSubMesh GenSubMesh(const TruncatedCone& primitive, eRenderingMode mode);

    private:
        static FaceSubMesh GenFaceSubMesh(const Circle& primitive);
        static FaceSubMesh GenFaceSubMesh(const Ellipse& primitive);
        static FaceSubMesh GenFaceSubMesh(const Rectangle& primitive);
        static FaceSubMesh GenFaceSubMesh(const Triangle& primitive);
        static FaceSubMesh GenFaceSubMesh(const Box& primitive);
        static FaceSubMesh GenFaceSubMesh(const Capsule& primitive);
        static FaceSubMesh GenFaceSubMesh(const Cone& primitive);
        static FaceSubMesh GenFaceSubMesh(const Cylinder& primitive);
        static FaceSubMesh GenFaceSubMesh(const Dome& primitive);
        static FaceSubMesh GenFaceSubMesh(const Ellipsoid& primitive);
        static FaceSubMesh GenFaceSubMesh(const Sphere& primitive);
        static FaceSubMesh GenFaceSubMesh(const Tetrahedron& primitive);
        static FaceSubMesh GenFaceSubMesh(const TruncatedCone& primitive);

        /* private:
             LineSubMesh GenLineSubMesh(const Circle& primitive);
             LineSubMesh GenLineSubMesh(const Ellipse& primitive);
             LineSubMesh GenLineSubMesh(const Rectangle& primitive);
             LineSubMesh GenLineSubMesh(const Triangle& primitive);
             LineSubMesh GenLineSubMesh(const Box& primitive);
             LineSubMesh GenLineSubMesh(const Capsule& primitive);
             LineSubMesh GenLineSubMesh(const Cone& primitive);
             LineSubMesh GenLineSubMesh(const Cylinder& primitive);
             LineSubMesh GenLineSubMesh(const Dome& primitive);
             LineSubMesh GenLineSubMesh(const Ellipsoid& primitive);
             LineSubMesh GenLineSubMesh(const Sphere& primitive);
             LineSubMesh GenLineSubMesh(const Tetrahedron& primitive);
             LineSubMesh GenLineSubMesh(const TruncatedCone& primitive);*/
    };
}
