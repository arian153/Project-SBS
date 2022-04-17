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

#include "../System.hpp"

namespace Engine
{
    class Texture;
    class SRVDescriptorHeap;

    class GUISystem final : public System
    {
    public:
        GUISystem();
        ~GUISystem() override;

        void AddImage(SPtr<Texture> texture);

    private:
        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        void SetUpImGUI();
        void CleanUpImGUI();
        void RenderDrawData() const;

        void BeginImGUI();
        void EndImGUI();

    private:
        friend class CoreSystem;
        friend class DirectX12Layer;
        friend class Win32Manager;
        friend class AppStateManager;

    private:
        UPtr<SRVDescriptorHeap> m_srv_descriptor_heap = nullptr;
    };
}
