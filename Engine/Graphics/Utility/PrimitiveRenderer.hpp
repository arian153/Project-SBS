#pragma once
#include "../DirectX12/Buffer/IndexBuffer.hpp"
#include "../DirectX12/Buffer/VertexBuffer.hpp"

namespace Engine
{
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
    private:
    };
}
