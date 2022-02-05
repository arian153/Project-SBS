#pragma once
#include "../Resource.hpp"

namespace Engine
{
    class Texture;

    class TextureResource final : public Resource
    {
    public:
        explicit TextureResource(const String& path);
        ~TextureResource() override;

        SPtr<Texture> GetTexture() const;

    private:
        void Initialize() override;
        void Shutdown() override;
        void Load();

    private:
        friend class ResourceManager;

    private:
       SPtr<Texture> m_texture = nullptr;
    };
}
