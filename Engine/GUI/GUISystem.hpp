#pragma once

#include "../System.hpp"




namespace Engine
{
    class SRVDescriptorHeap;

    class GUISystem final : public System
    {
    public:
        GUISystem();
        ~GUISystem() override;

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
