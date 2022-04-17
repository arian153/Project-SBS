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

#include "EngineDefine.hpp"

namespace Engine
{
    class System
    {
    public:
        System() = default;
        virtual ~System() = default;

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void Shutdown() = 0;

    protected:
    };

    class Subsystem
    {
    public:
        Subsystem() = default;
        virtual ~Subsystem() = default;

        virtual void Initialize() = 0;
        virtual void Update(Real dt) = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;

    protected:
    };
}
