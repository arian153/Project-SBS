/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "JsonResource.hpp"

namespace Engine
{
    JsonResource::JsonResource(const String& path)
        : Resource(path)
    {
        m_type = eResourceType::Json;
    }

    JsonResource::~JsonResource()
    {
    }

    void JsonResource::Initialize()
    {
    }

    void JsonResource::Shutdown()
    {
        m_b_unloaded = true;
    }

    void JsonResource::Load()
    {
        m_b_loaded = true;
    }
}
