#include "TestSquare.h"

#include <iostream>
#include <Core/ComponentManager/Components/LightCompo.hpp>
#include <Core/ComponentManager/Components/OrbitCameraCompo.hpp>
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
        
        //Mesh Object
        {
            auto obj       = m_object_manager->AddObject("Bunny");
            auto transform = obj->AddComponent<TransformCompo>();
            auto mesh      = obj->AddComponent<MeshCompo>();
            obj->AddComponent<RigidBodyCompo>();

            Transform tf;
            tf.position = Vector3(0, 0, 0);
            tf.scale    = Vector3(100, 100, 100);

            transform->SetTransform(tf);

            MeshData cube = MeshDataGenerator::CreateBox(0.1f, 0.1f, 0.1f, 3);

            //mesh->SetMeshData(RESOURCE_MANAGER->GetModelResourceName("bunny_high_poly.obj")->GetMeshData(0));
            mesh->SetMeshData(cube);
            mesh->SetShader(GET_SHADER_BY_NAME("Default.shader"));
            mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("Leather.jpg")->GetTexture());
            mesh->SetMaterialTexture(3, RESOURCE_MANAGER->GetTextureResourceName("Leather_Normal.jpg")->GetTexture());

           /* mesh->SetMaterialInfoReal(0, 0.3f);
            mesh->SetMaterialInfoReal(1, -0.4f);
            mesh->SetMaterialInfoReal(2, 0.3f);*/
        }

        //Camera Object
        {
            auto obj = m_object_manager->AddObject("camera");
            obj->AddComponent<TransformCompo>();
            auto camera = obj->AddComponent<OrbitCameraCompo>();
            camera->SetAsMainCamera();
        }

        MeshData sphere = MeshDataGenerator::CreateGeodesicSphere(1.0f, 3);

        //Light 1 Green Directional Light
        {
            auto obj = m_object_manager->AddObject("Directional");
            obj->AddComponent<TransformCompo>();
            auto light = obj->AddComponent<LightCompo>();
            light->SetLightDirection(Vector3(1.f, 0.f, 1.f).Normalize());
            light->SetLightType(eLightType::DirectionalLight);
            light->SetDiffuse(Color(1.f, 1.f, 1.f, 1.0f));
            light->SetAmbient(Color(0.1f, 0.1f, 0.1f, 1.0f));
            light->SetSpecular(Color(0.1f, 0.1f, 0.1f, 1.0f));
           /* auto mesh = obj->AddComponent<MeshCompo>();
            mesh->SetMeshData(sphere);
            mesh->SetShader(GET_SHADER_BY_NAME("Default.shader"));
            mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("test.png")->GetTexture());*/

        }

        //Light 2 Red Point Light
       /* {
            auto obj       = m_object_manager->AddObject("Point");
            auto transform = obj->AddComponent<TransformCompo>();
            transform->SetPosition(Vector3(150.f, 150.f, 150.f));
            auto light = obj->AddComponent<LightCompo>();
            light->SetLightType(eLightType::PointLight);
            light->SetDiffuse(Color(1.f, 0.1f, 0.1f, 1.0f));
            light->SetAmbient(Color(0.1f, 0.f, 0.f, 1.0f));
            light->SetSpecular(Color(0.1f, 0.1f, 0.1f, 1.0f));
            light->SetLightRange(10000.f);

            auto mesh = obj->AddComponent<MeshCompo>();
            mesh->SetMeshData(sphere);
            mesh->SetShader(GET_SHADER_BY_NAME("Default.shader"));
            mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("test.png")->GetTexture());

        }*/

        //Light 3 Blue Spot Light
       /* {
            auto obj       = m_object_manager->AddObject("Spot");
            auto transform = obj->AddComponent<TransformCompo>();
            transform->SetPosition(Vector3(-150.f, 0.f, 150.f));
            auto light = obj->AddComponent<LightCompo>();
            light->SetLightDirection(Vector3(1.f, 0.f, 0.f));
            light->SetLightType(eLightType::SpotLight);
            light->SetAmbient(Color(0.0f, 0.f, 0.f, 1.0f));
            light->SetDiffuse(Color(0.f, 0.1f, 1.f, 1.0f));
            light->SetSpecular(Color(0.1f, 0.1f, 0.1f, 1.0f));
            light->SetLightRange(10000.f);
            light->SetLightAngle(XM_PI / 4);

            auto mesh = obj->AddComponent<MeshCompo>();
            mesh->SetMeshData(sphere);
            mesh->SetShader(GET_SHADER_BY_NAME("Default.shader"));
            mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("test.png")->GetTexture());

        }*/
    }

    void TestSquare::Update(float dt)
    {
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Escape))
        {
            WIN32_MANAGER->SetQuit(true);
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
