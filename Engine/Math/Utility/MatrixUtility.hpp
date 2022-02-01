#pragma once
#include "MathDef.hpp"

namespace Engine
{
    class AxisRadian;
    class Quaternion;
    class Matrix44;
    class Matrix33;
    class EulerAngle;
    class Vector3;
    class Vector4;

    namespace Math
    {
        namespace Matrix44
        {
            //rotation
            Engine::Matrix44 Rotation(const Quaternion& quaternion);
            Engine::Matrix44 Rotation(const Engine::Matrix33& rotation_matrix);
            Engine::Matrix44 Rotation(const EulerAngle& euler_angle);
            Engine::Matrix44 Rotation(const AxisRadian& axis_radian);

            //principle rotation
            Engine::Matrix44 RotationX(Real radian);
            Engine::Matrix44 RotationY(Real radian);
            Engine::Matrix44 RotationZ(Real radian);

            //scale
            Engine::Matrix44 Scale(const Engine::Vector4& scale);
            Engine::Matrix44 Scale(const Engine::Vector3& scale, Real w = 1.0f);
            Engine::Matrix44 Scale(Real x, Real y, Real z, Real w = 1.0f);


            //translation
            Engine::Matrix44 Translation(const Engine::Vector3& translation);

            //Matrix Generation
            Engine::Matrix44 AffineTransformation
            (
                const Engine::Vector3& scale,
                const Engine::Vector3& origin,
                const Quaternion&     rotation,
                const Engine::Vector3& translation
            );

            Engine::Matrix44 PerspectiveMatrix
            (
                Real screen_aspect,
                Real field_of_view,
                Real far_plane,
                Real near_plane
            );

            Engine::Matrix44 OrthoGraphicMatrix
            (
                size_t client_width,
                size_t client_height,
                Real   far_plane,
                Real   near_plane
            );

            Engine::Matrix44 OrthoGraphicCenterMatrix
            (
                Real right,
                Real left,
                Real top,
                Real bottom,
                Real far_plane,
                Real near_plane
            );

            Engine::Matrix44 LookAt
            (
                const Engine::Vector3& eye_position,
                const Engine::Vector3& focus_position,
                const Engine::Vector3& up_direction
            );

            Engine::Matrix44 LookTo
            (
                const Engine::Vector3& eye_position,
                const Engine::Vector3& eye_direction,
                const Engine::Vector3& up_direction
            );
        }

        namespace Matrix33
        {
            //rotation
            Engine::Matrix33 Rotation(const Quaternion& quaternion);
            Engine::Matrix33 Rotation(const Engine::Matrix44& rotation_matrix);
            Engine::Matrix33 Rotation(const EulerAngle& euler_angle);
            Engine::Matrix33 Rotation(const AxisRadian& axis_radian);

            //principle rotation
            Engine::Matrix33 RotationX(Real radian);
            Engine::Matrix33 RotationY(Real radian);
            Engine::Matrix33 RotationZ(Real radian);

            //scale
            Engine::Matrix33 Scale(const Engine::Vector3& scale);
        }
    }
}
