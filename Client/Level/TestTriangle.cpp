#include "TestTriangle.h"

using namespace Engine;

namespace Client
{
    TestTriangle::~TestTriangle()
    {
    }

    void TestTriangle::Initialize()
    {
        std::vector<ColorVertex> vertices(4);
        vertices[0].pos   = Vector3(-1, 0, 0);
        vertices[0].color = Color(1.f, 0.f, 0.f, 1.f);
        vertices[1].pos   = Vector3(0, 1, 0);
        vertices[1].color = Color(0.f, 1.0f, 0.f, 1.f);
        vertices[2].pos   = Vector3(1, 0, 0);
        vertices[2].color = Color(0.f, 0.f, 1.f, 1.f);
        vertices[3].pos   = Vector3(0, -1, 0);
        vertices[3].color = Color(0.f, 0.f, 0.f, 1.f);

        std::vector<Uint32> indices(6);
        indices[0] = 0;
        indices[1] = 1;
        indices[2] = 2;
        indices[3] = 0;
        indices[4] = 2;
        indices[5] = 3;

        //m_mesh->Init(vertices, indices);

        //m_shader = GET_SHADER("Default.shader");
    }

    void TestTriangle::Update(float dt)
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

       
       // m_mesh->SetOffset(m_offset);
    }

    void TestTriangle::FixedUpdate(float dt)
    {
    }

    void TestTriangle::Render()
    {
      /*  m_shader->Bind();

        m_mesh->Render();*/
    }

    void TestTriangle::Shutdown()
    {
    }

    TestTriangleFactory::~TestTriangleFactory()
    {
    }

    Engine::AppState* TestTriangleFactory::Create()
    {
        return new TestTriangle();
    }
}
