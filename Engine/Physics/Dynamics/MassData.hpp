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
