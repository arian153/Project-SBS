#include "TestBox.h"

#include <iostream>
#include <Core/ComponentManager/Components/LightCompo.hpp>
#include <Core/ComponentManager/Components/OrbitCameraCompo.hpp>
#include <Core/ComponentManager/Components/SoftBodyCompo.hpp>
#include <Core/ObjectManager/Object.hpp>
using namespace Engine;

namespace Client
{
    TestBox::TestBox()
    {
        SetDefaultSubsystems();
    }

    TestBox::~TestBox()
    {
    }

    void TestBox::Initialize()
    {
        ////Mesh Object
        //{
        //    auto obj       = m_object_manager->AddObject("Bunny");
        //    auto transform = obj->AddComponent<TransformCompo>();
        //    auto mesh      = obj->AddComponent<MeshCompo>();

        //    Transform tf;
        //    tf.position = Vector3(-40, 0, 0);
        //    tf.scale    = Vector3(100, 100, 100);

        //    transform->SetTransform(tf);

        //    MeshData cube = MeshDataGenerator::CreateBox(0.1f, 0.1f, 0.1f, 3);

        //    mesh->CreateModel("Cube-Custom");
        //    mesh->SetMeshData(cube);
        //    //mesh->SetMeshData(RESOURCE_MANAGER->GetModelResourceName("bunny_high_poly.obj")->GetMeshData(0));
        //    mesh->SetShader(GET_SHADER("Deferred-Geometry.shader"));
        //    mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("Leather.jpg")->GetTexture());
        //    mesh->SetNormalMapTexture(RESOURCE_MANAGER->GetTextureResourceName("Leather_Normal.jpg")->GetTexture());
        //    mesh->SetMaterialInfo(0, 1);
        //}

        //{
        //    auto obj       = m_object_manager->AddObject("Capsule");
        //    auto transform = obj->AddComponent<TransformCompo>();
        //    auto mesh      = obj->AddComponent<MeshCompo>();
        //    mesh->CreateModel("Test-Primitive");
        //    obj->AddComponent<RigidBodyCompo>();

        //    Transform tf;
        //    tf.position = Vector3(0, 0, 0);
        //    tf.scale    = Vector3(50, 50, 50);

        //    transform->SetTransform(tf);

        //    Capsule capsule;
        //    capsule.height = 0.3f;
        //    capsule.radius = Vector3(0.1f, 0.1f, 0.1f);

        //    TruncatedCone tr_cone;
        //    tr_cone.height = 0.3f;
        //    tr_cone.radius = Vector2(0.13f, 0.1f);
        //    tr_cone.ratio  = 0.3f;

        //    m_tr_cone.height = 3.0f;
        //    m_tr_cone.radius = Vector2(3.0f, 4.0f);
        //    m_tr_cone.ratio  = 0.3f;

        //    Cone cone;
        //    cone.height = 0.3f;
        //    cone.radius = Vector2(0.13f, 0.1f);

        //    Cylinder cylinder;
        //    cylinder.height = 0.3f;
        //    cylinder.radius = Vector2(0.13f, 0.1f);

        //    Dome dome;
        //    dome.radius = Vector3(0.1f, 0.1f, 0.1f);

        //    Ellipsoid ellipsoid;
        //    ellipsoid.radius = Vector3(0.1f, 0.12f, 0.11f);

        //    Tetrahedron tetrahedron;
        //    tetrahedron.SetUnit();

        //    Box box;
        //    box.SetBox(0.3f, 0.2f, 0.2f);

        //    Circle circle;
        //    circle.radius = 0.2f;

        //    Engine::Rectangle rect;
        //    rect.SetUnit();

        //    Triangle triangle;
        //    triangle.SetUnit();

        //    m_sphere.radius = 1.0f;

        //    auto [vertices, indices] = PrimitiveRenderer::GenFaceSubMesh(capsule);

        //    MeshData mesh_data;
        //    mesh_data.Set(vertices);
        //    mesh_data.indices = indices;

        //    //mesh->SetMeshData(RESOURCE_MANAGER->GetModelResourceName("bunny_high_poly.obj")->GetMeshData(0));
        //    mesh->SetMeshData(mesh_data);
        //    mesh->SetShader(GET_SHADER("Deferred-Primitive.shader"));
        //    mesh->SetMaterialInfo(0, 1);

        //    /* mesh->SetMaterialInfoReal(0, 0.3f);
        //     mesh->SetMaterialInfoReal(1, -0.4f);
        //     mesh->SetMaterialInfoReal(2, 0.3f);*/
        //}

        //{
        //    auto obj       = m_object_manager->AddObject("Capsule2");
        //    auto transform = obj->AddComponent<TransformCompo>();
        //    auto mesh      = obj->AddComponent<MeshCompo>();
        //    mesh->CreateModel("Test-Primitive");
        //    Transform tf;
        //    tf.position = Vector3(20, 0, 0);
        //    tf.scale    = Vector3(50, 50, 50);

        //    transform->SetTransform(tf);
        //}

        {
            auto obj       = m_object_manager->AddObject("Trampoline Object");
            auto transform = obj->AddComponent<TransformCompo>();
            transform->SetScale(Vector3(3, 3, 3));
            transform->SetPosition(Vector3(-20, -10, -10));
            auto soft_body = obj->AddComponent<SoftBodyCompo>();
            soft_body->CreateSampleBox();

            auto mesh = obj->AddComponent<MeshCompo>();
            mesh->CreateModel("SoftBody-Custom");
            mesh->SetMeshData(soft_body->GetSoftBodyMeshData());
            mesh->SetShader(GET_SHADER("Deferred-Geometry.shader"));
            mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("Fabric05 diffuse 1k.jpg")->GetTexture());
            mesh->SetNormalMapTexture(RESOURCE_MANAGER->GetTextureResourceName("Fabric05 normal 1k.jpg")->GetTexture());
            mesh->SetMaterialInfo(0, 1);
            mesh->SetMaterialInfo(1, 1);
            mesh->SetMaterialInfo(2, 1);
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

    void TestBox::Update(float dt)
    {
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Escape))
        {
            WIN32_MANAGER->SetQuit(true);
        }

        ImGui::Begin("Scene State Manager");

        if (ImGui::Button("Hemisphere"))
        {
            APP_STATE_MANAGER->ChangeState("TestSphere");
        }

        if (ImGui::Button("Cloth"))
        {
            APP_STATE_MANAGER->ChangeState("TestSquare");
        }

        if (ImGui::Button("Splash-Screen"))
        {
            APP_STATE_MANAGER->PauseAndChangeState("Splash");
        }

        ImGui::End();
    }

    void TestBox::FixedUpdate(float dt)
    {
    }

    void TestBox::Render()
    {
        /* auto primitive_renderer = m_render_subsystem->GetPrimitiveRenderer();

         Transform tf;
         tf.position = Vector3(-14, 0, 0);
         primitive_renderer->DrawPrimitive(m_sphere, tf, Color(1.0f, 0.0f, 0.0f, 1.0f), eRenderingMode::Lighting);
         tf.position = Vector3(-24, 0, 0);
         primitive_renderer->DrawPrimitive(m_sphere, tf, Color(1.0f, 1.0f, 0.0f, 1.0f), eRenderingMode::Lighting);
         tf.position = Vector3(-34, 0, 0);
         primitive_renderer->DrawPrimitive(m_sphere, tf, Color(1.0f, 0.0f, 1.0f, 1.0f), eRenderingMode::Lighting);
         tf.position = Vector3(-44, 0, 0);
         primitive_renderer->DrawPrimitive(m_sphere, tf, Color(0.0f, 1.0f, 1.0f, 1.0f), eRenderingMode::Lighting);

         primitive_renderer->DrawLine(Vector3(1, 0, 0), Vector3(0, 4, 0));
         primitive_renderer->DrawLine(Vector3(0, 4, 0), Vector3(-9, 4, 0));*/
    }

    void TestBox::Shutdown()
    {
    }

    TestBoxFactory::~TestBoxFactory()
    {
    }

    Engine::AppState* TestBoxFactory::Create()
    {
        return new TestBox();
    }
}
