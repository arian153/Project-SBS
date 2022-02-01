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
