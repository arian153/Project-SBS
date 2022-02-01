#pragma once
#include <Engine.h>

namespace Client
{
    class TestTriangle final : public Engine::AppState
    {
    public:
        ~TestTriangle() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:
        std::shared_ptr<Engine::SimpleMesh>    m_mesh   = std::make_shared<Engine::SimpleMesh>();
        std::shared_ptr<Engine::ShaderProgram> m_shader = nullptr;

        Engine::Vector4 m_offset;
    };

    class TestTriangleFactory final : public Engine::AppStateFactory
    {
    public:
        ~TestTriangleFactory() override;
        Engine::AppState* Create() override;
    private:
    };
}
