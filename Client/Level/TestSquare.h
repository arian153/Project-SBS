/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#pragma once
#include <Engine.h>

namespace Client
{
    class TestSquare final : public Engine::AppState
    {
    public:
        TestSquare();
        ~TestSquare() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:
        Engine::TruncatedCone m_tr_cone;
        Engine::Sphere m_sphere;

    };

    class TestSquareFactory final : public Engine::AppStateFactory
    {
    public:
        ~TestSquareFactory() override;
        Engine::AppState* Create() override;
    private:
    };
}
