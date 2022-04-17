/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "TextureResource.hpp"

#include "../../../Graphics/DirectX12/Texture.hpp"

namespace Engine
{
    TextureResource::TextureResource(const String& path)
        : Resource(path)
    {
        m_type = eResourceType::Texture;
    }

    TextureResource::~TextureResource()
    {
    }

    SPtr<Texture> TextureResource::GetTexture() const
    {
        return m_texture;
    }

    void TextureResource::Initialize()
    {
        m_texture = std::make_shared<Texture>();
        Load();

        
    }

    void TextureResource::Shutdown()
    {
    }

    void TextureResource::Load()
    {
        m_b_loaded = m_texture->Load(m_wide_path, m_extension);
    }
}
