#pragma once
#include "Vertex.hpp"
#include "../../EngineDefine.hpp"

namespace Engine
{
    struct ForwardSubMesh
    {
        std::vector<PosVertex> vertices;
        std::vector<Uint32>    indices;
    };

    struct DeferredSubMesh
    {
        std::vector<NormalVertex> vertices;
        std::vector<Uint32>       indices;
    };
}

namespace Engine::SubMeshUtility
{
    ForwardSubMesh ToForward(const DeferredSubMesh& sub_mesh);
}
