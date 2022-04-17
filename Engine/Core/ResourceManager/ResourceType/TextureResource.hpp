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
