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
#include "AppState.hpp"
#include "AppStateFactory.hpp"

namespace Engine
{
    class DefaultAppState final : public AppState
    {
    public:
        DefaultAppState();
        ~DefaultAppState() override;

        void Initialize() override;
        void Update(float dt) override;
        void FixedUpdate(float dt) override;
        void Render() override;
        void Shutdown() override;

    private:
    };

    class DefaultAppStateFactory final : public AppStateFactory
    {
    public:
        ~DefaultAppStateFactory() override;
        AppState* Create() override;
    private:
    };
}
