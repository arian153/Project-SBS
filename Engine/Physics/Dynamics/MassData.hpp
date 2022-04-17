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
#include "../../Math/Algebra/Matrix33.hpp"

namespace Engine
{
    class MassData
    {
    public:
        MassData();
        ~MassData();

        MassData& operator=(const MassData& rhs);

        static Matrix33 TranslateInertia(const Matrix33& input, Real mass, const Vector3& translation);
        static Matrix33 RotateInertia(const Matrix33& input, const Quaternion& orientation);

        void CalculateInverse();

    public:
        //mass
        Real mass         = 1.0f;
        Real inverse_mass = 1.0f;

        //center of mass
        Vector3 local_centroid;

        //inertia
        Matrix33 local_inertia;
        Matrix33 local_inverse_inertia;
    };
}
