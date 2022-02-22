#include "InAppEditor.hpp"

#include "../Core/AppStateManager/AppState.hpp"
#include "../Core/ObjectManager/Object.hpp"
#include "../External/imgui/imgui.h"

namespace Engine
{
    static bool ItemsObjectGetter(void* data, int idx, const char** out_text)
    {
        auto& objects = *static_cast<std::vector<SPtr<Object>>*>(data);
        if (idx < 0 || idx >= static_cast<int>(objects.size()))
        {
            return false;
        }

        *out_text = objects[idx]->GetCStrName();
        return true;
    }

    void InAppEditor::Update(Real dt)
    {
        if (m_app_state == nullptr)
        {
            return;
        }

        ImGuiWindowFlags flag = ImGuiWindowFlags_MenuBar;

        ImGui::Begin("AppState", nullptr, flag);
        ImGui::Text("Delta Time: %f", dt);
        ImGui::End();

        {
            auto& objects = m_app_state->m_object_manager->m_objects;

            ImGui::Begin("Objects");
            ImGui::Text("Objects");
            if (ImGui::ListBox(
                               "##ObjectList", &m_object_index, ItemsObjectGetter,
                               static_cast<void*>(&objects), static_cast<int>(objects.size()), ImVec2(-1, -1)))
            {
                m_object = objects[m_object_index].get();
            }

            ImGui::End();
        }

        {
            ImGui::Begin("Inspector");
            if (m_object != nullptr)
            {
                //edit name
                std::string name = m_object->GetName();
                ImGui::Text("Name      : ");
                ImGui::SameLine();
                ImVec2 min = ImGui::GetWindowContentRegionMin();
                ImVec2 max = ImGui::GetWindowContentRegionMax();
                ImGui::PushItemWidth(max.x - min.x);
                ImGui::InputText("##ObjectName", &name);
                if (ImGui::IsItemEdited() == true)
                {
                    m_object->SetName(name);
                }
                //edit component
                for (auto& component : m_object->m_components)
                {
                    component->Edit(nullptr);
                }
            }

            ImGui::End();
        }
    }

    void InAppEditor::SetAppState(AppState* app_state)
    {
        m_app_state = app_state;
    }
}
