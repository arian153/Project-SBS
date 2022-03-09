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
    };
}
