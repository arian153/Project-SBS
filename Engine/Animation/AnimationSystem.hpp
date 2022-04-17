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
    class AnimationSubsystem;

    class AnimationSystem final : public System
    {
    public:
        AnimationSystem();
        ~AnimationSystem() override;

        void Initialize() override;
        void Update() override;
        void Shutdown() override;

        SPtr<AnimationSubsystem> CreateSubsystem();

    private:
        std::vector<SPtr<AnimationSubsystem>> m_subsystems;
    };
}
