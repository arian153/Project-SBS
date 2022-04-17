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
#include "../System.hpp"

namespace Engine
{
    class PhysicsSubsystem;

    class PhysicsSystem final : public System
    {
    public:
        PhysicsSystem();
        ~PhysicsSystem() override;

        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        SPtr<PhysicsSubsystem> CreateSubsystem();

    private:
        std::vector<SPtr<PhysicsSubsystem>> m_subsystems;
    };
}
