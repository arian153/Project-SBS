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
#include "../../Math/Algebra/Vector3.hpp"

namespace Engine
{
    inline const char* e_collider_type[] = {
        "Circle",
        "Ellipse",
        "Polygon",
        "Rectangle",
        "Triangle",
        "Box",
        "Capsule",
        "Cone",
        "Cylinder",
        "Dome",
        "Ellipsoid",
        "Polyhedron",
        "Sphere",
        "Tetrahedron",
        "Point",
        "Invalid"
    };

    enum class eMotionMode : int
    {
        Dynamic,
        Static,
        Kinematic,
        Invalid
    };

    inline const char* e_motion_mode[] = {
        "Dynamic",
        "Static",
        "Kinematic"
        "Invalid"
    };


    namespace Physics::Dynamics
    {
        constexpr Real ELASTIC_THRESHOLD = 1.0f;
        constexpr Real BAUMGRATE         = 0.2f;
    }

    namespace Physics::Collision
    {
        constexpr Real   BROAD_PHASE_MARGIN           = 0.2f;
        constexpr Real   PERSISTENT_THRESHOLD         = 0.2f;
        constexpr Real   PERSISTENT_THRESHOLD_SQUARED = PERSISTENT_THRESHOLD * PERSISTENT_THRESHOLD;
        constexpr size_t MAX_MANIFOLD_POINT_COUNT     = 4;

        constexpr Real CONTACT_BETA  = 0.55f;
        constexpr Real VELOCITY_SLOP = 0.1f;

        constexpr Real POSITION_LINEAR_SLOP     = 0.05f;
        constexpr Real POSITION_ANGULAR_SLOP    = Math::RADIAN * 2.0f;
        constexpr Real MAX_LINEAR_CORRECTION    = 0.2f;
        constexpr Real POSITION_SEPARATION_SLOP = 0.02f;

        constexpr Real SLEEP_THRESHOLD         = 0.0007f;
        constexpr Real SLEEP_THRESHOLD_EXTREME = SLEEP_THRESHOLD * 100.0f;
        constexpr Real SLEEP_AWAKE             = SLEEP_THRESHOLD * 33.0f;
        constexpr Real SLEEP_BIAS              = 0.75f;
    }}
