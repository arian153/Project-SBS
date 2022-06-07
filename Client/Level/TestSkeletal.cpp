#include "TestSkeletal.h"

#include <iostream>
#include <Core/ComponentManager/Components/LightCompo.hpp>
#include <Core/ComponentManager/Components/OrbitCameraCompo.hpp>
#include <Core/ComponentManager/Components/SoftBodyCompo.hpp>
#include <Core/ObjectManager/Object.hpp>
using namespace Engine;

namespace Client
{
    TestSkeletal::TestSkeletal()
    {
        SetDefaultSubsystems();
    }

    TestSkeletal::~TestSkeletal()
    {
    }

    void TestSkeletal::Initialize()
    {
        MeshData sphere = MeshDataGenerator::CreateGeodesicSphere(1.0f, 3);

        {
            auto obj       = m_object_manager->AddObject("Skeletal Object");
            auto transform = obj->AddComponent<TransformCompo>();
            transform->SetScale(Vector3(1, 1, 1));
            transform->SetPosition(Vector3(0, 0, 0));

            TruncatedCone t_cone;
            t_cone.SetTruncatedCone(Vector2(3, 4), 5, 0.5f);

            MeshData mesh_data = PrimitiveRenderer::ToMeshData(PrimitiveRenderer::GenFaceSubMesh(t_cone));

            auto mesh = obj->AddComponent<MeshCompo>();
            mesh->CreateModel("Test-Mesh");
            mesh->SetMeshData(sphere);
            mesh->SetShader(GET_SHADER("Deferred-Geometry.shader"));
            mesh->SetColor(X11ColorData::CHOCOLATE);
            mesh->SetMaterialInfo(ProcessInstancedColor, HasInstancedColor);
            mesh->SetMaterialInfo(ProcessDiffuseMaterial, SkipProcessing);
            mesh->SetMaterialInfo(ProcessNormalMapMaterial, SkipProcessing);
        }

        //Camera Object
        {
            auto obj = m_object_manager->AddObject("camera");
            obj->AddComponent<TransformCompo>();
            auto camera = obj->AddComponent<OrbitCameraCompo>();
            camera->SetAsMainCamera();
        }

      

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
            auto     mesh       = obj->AddComponent<MeshCompo>();
            MeshData sky_sphere = MeshDataGenerator::CreateSphere(1.0f, 30, 30);

            mesh->CreateModel("SkyBox");
            mesh->SetMeshData(sky_sphere);
            mesh->SetShader(GET_SHADER("SkyBox.shader"));
            mesh->SetCubeMapTexture(RESOURCE_MANAGER->GetTextureResourceName("SeaCube.dds")->GetTexture());
        }

        //Light 2 Red Point Light
        {
            auto obj       = m_object_manager->AddObject("Point");
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
            auto obj       = m_object_manager->AddObject("Spot");
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

    void TestSkeletal::Update(float dt)
    {
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Escape))
        {
            WIN32_MANAGER->SetQuit(true);
        }
    }

    void TestSkeletal::FixedUpdate(float dt)
    {
    }

    void TestSkeletal::Render()
    {
    }

    void TestSkeletal::Shutdown()
    {
    }

    TestSkeletalFactory::~TestSkeletalFactory()
    {
    }

    Engine::AppState* TestSkeletalFactory::Create()
    {
        return new TestSkeletal();
    }
}
