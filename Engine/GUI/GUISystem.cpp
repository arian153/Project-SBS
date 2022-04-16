#include "GUISystem.hpp"

#include "../Core/CoreDefine.hpp"
#include "../Core/CoreSystem.hpp"
#include "../Core/Win32Manager/Win32Manager.hpp"
#include "../External/imgui/imgui.h"
#include "../External/imgui/imgui_impl_dx12.h"
#include "../External/imgui/imgui_impl_win32.h"
#include "../Graphics/GraphicsDefine.hpp"
#include "../Graphics/DirectX12/SRVDescriptorHeap.hpp"

namespace Engine
{
    GUISystem::GUISystem()
    {
    }

    GUISystem::~GUISystem()
    {
    }

    void GUISystem::AddImage()
    {
        m_srv_descriptor_heap->Increase();
    }

    void GUISystem::Initialize()
    {
        SetUpImGUI();
    }

    void GUISystem::Update()
    {
        ImGui::Begin("GAM450 Project Tool tip");
        ImGui::Text("Test UI");
        ImGui::Text("Press Mouse RB to Rotate Camera");
        ImGui::Text("Press WASD to move Camera");
        ImGui::Text("Press Arrow Key to apply force to mesh");
        ImGui::Text("Press Space Bar to Clear All velocity");
        ImGui::End();

        //ImGui::ShowDemoWindow();
    }

    void GUISystem::Shutdown()
    {
        CleanUpImGUI();
    }

    void GUISystem::SetUpImGUI()
    {
        m_srv_descriptor_heap = std::make_unique<SRVDescriptorHeap>();
        m_srv_descriptor_heap->Init();

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        //io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
        //io.ConfigViewportsNoAutoMerge = true;
        //io.ConfigViewportsNoTaskBarIcon = true;

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();

        style.WindowRounding              = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;

        ImGui_ImplWin32_Init(WIN32_MANAGER->AppHWnd());
        ImGui_ImplDX12_Init(
                            DEVICE.Get(), SWAP_CHAIN_BUFFER_COUNT,
                            DXGI_FORMAT_R8G8B8A8_UNORM, m_srv_descriptor_heap->GetHeap().Get(),
                            m_srv_descriptor_heap->GetHeap()->GetCPUDescriptorHandleForHeapStart(),
                            m_srv_descriptor_heap->GetHeap()->GetGPUDescriptorHandleForHeapStart());
    }

    void GUISystem::CleanUpImGUI()
    {
        ImGui_ImplDX12_Shutdown();
        ImGui_ImplWin32_Shutdown();
        ImGui::DestroyContext();
    }

    void GUISystem::RenderDrawData() const
    {
        auto srv_heap = m_srv_descriptor_heap->GetHeap().Get();
        CMD_LIST->SetDescriptorHeaps(1, &srv_heap);
        ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CMD_LIST.Get());
    }

    void GUISystem::BeginImGUI()
    {
        ImGui_ImplDX12_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        //ImGui::ShowDemoWindow();
    }

    void GUISystem::EndImGUI()
    {
        ImGui::Render();
    }
}
