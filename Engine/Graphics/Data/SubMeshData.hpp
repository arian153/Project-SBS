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
