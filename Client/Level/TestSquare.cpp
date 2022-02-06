#include "TestSquare.h"

#include <iostream>
#include <Core/ObjectManager/Object.hpp>
using namespace Engine;

namespace Client
{
    TestSquare::TestSquare()
    {
        SetDefaultSubsystems();
    }

    TestSquare::~TestSquare()
    {
    }

    void TestSquare::Initialize()
    {
       /* std::vector<TexVertex> vertices(4);

        vertices[0].pos = Vector3(-0.5f, 0.5f, 0.5f);
        vertices[0].tex = Vector2(0.f, 0.f);
        vertices[1].pos = Vector3(0.5f, 0.5f, 0.5f);
        vertices[1].tex = Vector2(1.f, 0.f);
        vertices[2].pos = Vector3(0.5f, -0.5f, 0.5f);
        vertices[2].tex = Vector2(1.f, 1.f);
        vertices[3].pos = Vector3(-0.5f, -0.5f, 0.5f);
        vertices[3].tex = Vector2(0.f, 1.f);

        std::vector<Uint32> indices(6);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 0;
        indices[4] = 2;
        indices[5] = 3;

        m_mesh->Init(vertices, indices);

        auto material = std::make_shared<Material>();
        material->SetShader(GET_SHADER_BY_NAME("Default.shader"));
        material->SetReal(0, 0.3f);
        material->SetReal(1, 0.4f);
        material->SetReal(2, 0.3f);
        material->SetTexture(0, RESOURCE_MANAGER->GetTextureResourceName("test.png")->GetTexture());
        m_mesh->SetMaterial(material);*/

        {
            auto obj       = m_object_manager->AddObject("new_obj");
            auto transform = obj->AddComponent<TransformCompo>();
            auto mesh      = obj->AddComponent<MeshCompo>();

            MeshData mesh_data;
            mesh_data.vertices.resize(4);
            mesh_data.indices.resize(6);

            mesh_data.vertices[0].pos = Vector3(-0.5f, 0.5f, 0.5f);
            mesh_data.vertices[0].tex = Vector2(0.f, 0.f);
            mesh_data.vertices[1].pos = Vector3(0.5f, 0.5f, 0.5f);
            mesh_data.vertices[1].tex = Vector2(1.f, 0.f);
            mesh_data.vertices[2].pos = Vector3(0.5f, -0.5f, 0.5f);
            mesh_data.vertices[2].tex = Vector2(1.f, 1.f);
            mesh_data.vertices[3].pos = Vector3(-0.5f, -0.5f, 0.5f);
            mesh_data.vertices[3].tex = Vector2(0.f, 1.f);

            mesh_data.indices[0] = 0;
            mesh_data.indices[1] = 1;
            mesh_data.indices[2] = 2;
            mesh_data.indices[3] = 0;
            mesh_data.indices[4] = 2;
            mesh_data.indices[5] = 3;

            mesh_data.vertex_type = eVertexType::TexVertex;

            mesh->SetMeshData(mesh_data);

            mesh->SetShader(GET_SHADER_BY_NAME("Default.shader"));
            mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("test.png")->GetTexture());

            mesh->SetMaterialInfoReal(0, 0.3f);
            mesh->SetMaterialInfoReal(1, -0.4f);
            mesh->SetMaterialInfoReal(2, 0.3f);
        }
    }

    void TestSquare::Update(float dt)
    {
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Escape))
        {
            WIN32_MANAGER->SetQuit(true);
        }

        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::A))
        {
            m_offset.x -= dt;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::S))
        {
            m_offset.y -= dt;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::D))
        {
            m_offset.x += dt;
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::W))
        {
            m_offset.y += dt;
        }

        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Space))
        {
            RESOURCE_MANAGER->ReloadPath("Resource/Shader/Default.hlsli");
        }

        /* ImGui::Begin("Console");
 
         eButtonState curr_state = INPUT_MANAGER->GetButtonState(eKeyCodeKeyboard::A);
         eButtonState prev_state = INPUT_MANAGER->GetPrevButtonState(eKeyCodeKeyboard::A);
         String       info;
 
         switch (curr_state)
         {
         case eButtonState::None:
             ImGui::Text("Curr A : None");
             break;
         case eButtonState::Released:
             ImGui::Text("Curr A : Released");
             break;
         case eButtonState::Down:
             ImGui::Text("Curr A : Down");
             break;
         case eButtonState::Pressed:
             ImGui::Text("Curr A : Pressed");
             break;
 
         default:
             break;
         }
 
         switch (prev_state)
         {
         case eButtonState::None:
             ImGui::Text("Prev A : None");
             break;
         case eButtonState::Released:
             ImGui::Text("Prev A : Released");
             break;
         case eButtonState::Down:
             ImGui::Text("Prev A : Down");
             break;
         case eButtonState::Pressed:
             ImGui::Text("Prev A : Pressed");
             break;
 
         default:
             break;
         }
 
         ImGui::End();*/

    }

    void TestSquare::FixedUpdate(float dt)
    {
    }

    void TestSquare::Render()
    {
    }

    void TestSquare::Shutdown()
    {
    }

    TestSquareFactory::~TestSquareFactory()
    {
    }

    Engine::AppState* TestSquareFactory::Create()
    {
        return new TestSquare();
    }
}
