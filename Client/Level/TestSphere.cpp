#include "TestSphere.h"

#include <Core/ComponentManager/Components/SoftBodyCompo.hpp>

using namespace Engine;

namespace Client
{
    TestSphere::TestSphere()
    {
        SetDefaultSubsystems();
    }

    TestSphere::~TestSphere()
    {
    }

    void TestSphere::Initialize()
    {
        {
            auto obj = m_object_manager->AddObject("Hemisphere Slime");
            obj->AddComponent<TransformCompo>();
            auto soft_body = obj->AddComponent<SoftBodyCompo>();
            soft_body->CreateSampleSphere(true);
            

            auto mesh = obj->AddComponent<MeshCompo>();
            mesh->CreateModel("SoftBody-Custom");
            mesh->SetMeshData(soft_body->GetSoftBodyMeshData());
            mesh->SetShader(GET_SHADER("Deferred-Geometry.shader"));
            mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("Fabric05 diffuse 1k.jpg")->GetTexture());
            mesh->SetNormalMapTexture(RESOURCE_MANAGER->GetTextureResourceName("Fabric05 normal 1k.jpg")->GetTexture());
            mesh->SetMaterialInfo(0, 1);
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
        }

        //Sky Box
        {
            auto obj = m_object_manager->AddObject("SkyBox");
            obj->AddComponent<TransformCompo>();
            auto     mesh = obj->AddComponent<MeshCompo>();
            MeshData sky_sphere = MeshDataGenerator::CreateSphere(1.0f, 30, 30);

            mesh->CreateModel("SkyBox");
            mesh->SetMeshData(sky_sphere);
            mesh->SetShader(GET_SHADER("SkyBox.shader"));
            mesh->SetCubeMapTexture(RESOURCE_MANAGER->GetTextureResourceName("SeaCube.dds")->GetTexture());
        }

        //Light 2 Red Point Light
        {
            auto obj = m_object_manager->AddObject("Point");
            auto transform = obj->AddComponent<TransformCompo>();
            transform->SetPosition(Vector3(0.f, 100.f, 150.f));
            auto light = obj->AddComponent<LightCompo>();
            light->SetLightType(eLightType::PointLight);
            light->SetDiffuse(Color(1.f, 0.1f, 0.1f, 1.0f));
            light->SetAmbient(Color(0.1f, 0.f, 0.f, 1.0f));
            light->SetSpecular(Color(0.1f, 0.1f, 0.1f, 1.0f));
            light->SetLightRange(200.f);
        }

        //Light 3 Blue Spot Light
        {
            auto obj = m_object_manager->AddObject("Spot");
            auto transform = obj->AddComponent<TransformCompo>();
            transform->SetPosition(Vector3(75.f, 0.f, 150.f));
            auto light = obj->AddComponent<LightCompo>();
            light->SetLightDirection(Vector3(1.f, 0.f, 0.f));
            light->SetLightType(eLightType::SpotLight);
            light->SetAmbient(Color(0.0f, 0.f, 0.f, 1.0f));
            light->SetDiffuse(Color(0.f, 0.1f, 1.f, 1.0f));
            light->SetSpecular(Color(0.1f, 0.1f, 0.1f, 1.0f));
            light->SetLightRange(200.f);
            light->SetLightAngle(Math::PI_DIV_4);
        }
    }

    void TestSphere::Update(float dt)
    {
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Escape))
        {
            WIN32_MANAGER->SetQuit(true);
        }

         }

    void TestSphere::FixedUpdate(float dt)
    {
    }

    void TestSphere::Render()
    {
        /*  m_shader->Bind();
  
          m_mesh->Render();*/
    }

    void TestSphere::Shutdown()
    {
    }

    TestSphereFactory::~TestSphereFactory()
    {
    }

    Engine::AppState* TestSphereFactory::Create()
    {
        return new TestSphere();
    }
}
