/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "MassData.hpp"

#include "../../Math/Algebra/Quaternion.hpp"
#include "../../Math/Utility/Utility.hpp"

namespace Engine
{
    MassData::MassData()
    {
    }

    MassData::~MassData()
    {
    }

    MassData& MassData::operator=(const MassData& rhs)
    {
        if (this != &rhs)
        {
            mass                  = rhs.mass;
            inverse_mass          = rhs.inverse_mass;
            local_centroid        = rhs.local_centroid;
            local_inertia         = rhs.local_inertia;
            local_inverse_inertia = rhs.local_inverse_inertia;
        }
        return *this;
    }

    Matrix33 MassData::TranslateInertia(const Matrix33& input, Real mass, const Vector3& translation)
    {
        Real     r_dot_r = translation.DotProduct(translation);
        Matrix33 r_out_r = translation.OuterProduct(translation);
        return input + mass * (r_dot_r * Matrix33::Identity() - r_out_r);
    }

    Matrix33 MassData::RotateInertia(const Matrix33& input, const Quaternion& orientation)
    {
        return orientation * input * (orientation.Inverse());
    }

    void MassData::CalculateInverse()
    {
        bool is_inf  = Math::IsZero(mass);
        inverse_mass = is_inf ? 0.0f : 1.0f / mass;

        if (is_inf)
            local_inverse_inertia.SetZero();
        else
            local_inverse_inertia = local_inertia.Inverse();
    }
}
