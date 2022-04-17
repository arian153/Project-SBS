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
    class TestSphere final : public Engine::AppState
    {
    public:
        TestSphere();
        ~TestSphere() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:
    };

    class TestSphereFactory final : public Engine::AppStateFactory
    {
    public:
        ~TestSphereFactory() override;
        Engine::AppState* Create() override;
    private:
    };
}
