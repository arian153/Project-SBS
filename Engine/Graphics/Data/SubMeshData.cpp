/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "SubMeshData.hpp"

namespace Engine::SubMeshUtility
{
    ForwardSubMesh ToForward(const DeferredSubMesh& sub_mesh)
    {
        ForwardSubMesh result;

        result.indices.assign(sub_mesh.indices.begin(), sub_mesh.indices.end());

        size_t size = sub_mesh.vertices.size();
        result.vertices.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            result.vertices[i].pos = sub_mesh.vertices[i].pos;
        }

        return result;
    }
}
