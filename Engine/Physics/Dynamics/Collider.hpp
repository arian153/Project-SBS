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
#include <vector>

#include "MassData.hpp"

namespace Engine
{
    class RigidBody;
    enum class ePrimitiveType;
    class Primitive;

    class Collider
    {
    public:
        Collider();
        ~Collider();

        void Shutdown();

        Primitive* AddPrimitive(ePrimitiveType type);
        MassData   GetMassData(Real density) const;

        RigidBody* GetRigidBody() const;
        Vector3   LocalToWorldPoint(const Vector3& point) const;

    private:
        friend class RigidContactManifold;

    private:
        RigidBody*              m_rigid_body = nullptr;
        std::vector<Primitive*> m_primitives;
    };
}
