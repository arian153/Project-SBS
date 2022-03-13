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
