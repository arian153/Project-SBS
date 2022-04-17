/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "MatrixUtility.hpp"
#include "../Algebra/Matrix44.hpp"
#include "../Algebra/Matrix33.hpp"
#include "../Algebra/Quaternion.hpp"

namespace Engine
{
    namespace Math
    {
        namespace Matrix44
        {
            Engine::Matrix44 Rotation(const Quaternion& quaternion)
            {
                Engine::Matrix44 result;
                if (quaternion.IsUnit())
                {
                    Real xs         = quaternion.i + quaternion.i;
                    Real ys         = quaternion.j + quaternion.j;
                    Real zs         = quaternion.k + quaternion.k;
                    Real wx         = quaternion.r * xs;
                    Real wy         = quaternion.r * ys;
                    Real wz         = quaternion.r * zs;
                    Real xx         = quaternion.i * xs;
                    Real xy         = quaternion.i * ys;
                    Real xz         = quaternion.i * zs;
                    Real yy         = quaternion.j * ys;
                    Real yz         = quaternion.j * zs;
                    Real zz         = quaternion.k * zs;
                    result.data[0]  = 1.0f - (yy + zz);
                    result.data[1]  = xy - wz;
                    result.data[2]  = xz + wy;
                    result.data[3]  = 0.0f;
                    result.data[4]  = xy + wz;
                    result.data[5]  = 1.0f - (xx + zz);
                    result.data[6]  = yz - wx;
                    result.data[7]  = 0.0f;
                    result.data[8]  = xz - wy;
                    result.data[9]  = yz + wx;
                    result.data[10] = 1.0f - (xx + yy);
                    result.data[11] = 0.0f;
                    result.data[12] = 0.0f;
                    result.data[13] = 0.0f;
                    result.data[14] = 0.0f;
                    result.data[15] = 1.0f;
                }
                return result;
            }

            Engine::Matrix44 Rotation(const Engine::Matrix33& rotation_matrix)
            {
                Engine::Matrix44 result;
                result.data[0]  = rotation_matrix.data[0];
                result.data[1]  = rotation_matrix.data[1];
                result.data[2]  = rotation_matrix.data[2];
                result.data[3]  = 0.0f;
                result.data[4]  = rotation_matrix.data[3];
                result.data[5]  = rotation_matrix.data[4];
                result.data[6]  = rotation_matrix.data[5];
                result.data[7]  = 0.0f;
                result.data[8]  = rotation_matrix.data[6];
                result.data[9]  = rotation_matrix.data[7];
                result.data[10] = rotation_matrix.data[8];
                result.data[11] = 0.0f;
                result.data[12] = 0.0f;
                result.data[13] = 0.0f;
                result.data[14] = 0.0f;
                result.data[15] = 1.0f;
                return result;
            }

            Engine::Matrix44 Rotation(const EulerAngle& euler_angle)
            {
                Engine::Matrix44 result;
                Real            sin_x = sinf(euler_angle.x_rotation);
                Real            cos_x = cosf(euler_angle.x_rotation);
                Real            sin_y = sinf(euler_angle.y_rotation);
                Real            cos_y = cosf(euler_angle.y_rotation);
                Real            sin_z = sinf(euler_angle.z_rotation);
                Real            cos_z = cosf(euler_angle.z_rotation);
                result.data[0]        = (cos_y * cos_z);
                result.data[1]        = -(cos_y * sin_z);
                result.data[2]        = sin_y;
                result.data[3]        = 0.0f;
                result.data[4]        = (sin_x * sin_y * cos_z) + (cos_x * sin_z);
                result.data[5]        = -(sin_x * sin_y * sin_z) + (cos_x * cos_z);
                result.data[6]        = -(sin_x * cos_y);
                result.data[7]        = 0.0f;
                result.data[8]        = -(cos_x * sin_y * cos_z) + (sin_x * sin_z);
                result.data[9]        = (cos_x * sin_y * sin_z) + (sin_x * cos_z);
                result.data[10]       = (cos_x * cos_y);
                result.data[11]       = 0.0f;
                result.data[12]       = 0.0f;
                result.data[13]       = 0.0f;
                result.data[14]       = 0.0f;
                result.data[15]       = 1.0f;
                return result;
            }

            Engine::Matrix44 Rotation(const AxisRadian& axis_radian)
            {
                Engine::Matrix44 result;
                Real            sin  = sinf(axis_radian.radian);
                Real            cos  = cosf(axis_radian.radian);
                Real            t    = 1.0f - cos;
                Engine::Vector3  Axis = axis_radian.axis;
                Axis.SetNormalize();
                Real tx         = t * Axis.x;
                Real ty         = t * Axis.y;
                Real tz         = t * Axis.z;
                Real sx         = sin * Axis.x;
                Real sy         = sin * Axis.y;
                Real sz         = sin * Axis.z;
                Real txy        = tx * Axis.y;
                Real tyz        = tx * Axis.z;
                Real txz        = tx * Axis.z;
                result.data[0]  = (tx * Axis.x + cos);
                result.data[1]  = (txy - sz);
                result.data[2]  = (txz + sy);
                result.data[3]  = 0.0f;
                result.data[4]  = (txy + sz);
                result.data[5]  = (ty * Axis.y + cos);
                result.data[6]  = (tyz - sx);
                result.data[7]  = 0.0f;
                result.data[8]  = (txz - sy);
                result.data[9]  = (tyz + sx);
                result.data[10] = (tz * Axis.z + cos);
                result.data[11] = 0.0f;
                result.data[12] = 0.0f;
                result.data[13] = 0.0f;
                result.data[14] = 0.0f;
                result.data[15] = 1.0f;
                return result;
            }

            Engine::Matrix44 RotationX(Real radian)
            {
                Engine::Matrix44 result;
                Real            sin = sinf(radian);
                Real            cos = cosf(radian);
                result.data[0]      = 1.0f;
                result.data[1]      = 0.0f;
                result.data[2]      = 0.0f;
                result.data[3]      = 0.0f;
                result.data[4]      = 0.0f;
                result.data[5]      = cos;
                result.data[6]      = -sin;
                result.data[7]      = 0.0f;
                result.data[8]      = 0.0f;
                result.data[9]      = sin;
                result.data[10]     = cos;
                result.data[11]     = 0.0f;
                result.data[12]     = 0.0f;
                result.data[13]     = 0.0f;
                result.data[14]     = 0.0f;
                result.data[15]     = 1.0f;
                return result;
            }

            Engine::Matrix44 RotationY(Real radian)
            {
                Engine::Matrix44 result;
                Real            sin = sinf(radian);
                Real            cos = cosf(radian);
                result.data[0]      = cos;
                result.data[1]      = 0.0f;
                result.data[2]      = sin;
                result.data[3]      = 0.0f;
                result.data[4]      = 0.0f;
                result.data[5]      = 1.0f;
                result.data[6]      = 0.0f;
                result.data[7]      = 0.0f;
                result.data[8]      = -sin;
                result.data[9]      = 0.0f;
                result.data[10]     = cos;
                result.data[11]     = 0.0f;
                result.data[12]     = 0.0f;
                result.data[13]     = 0.0f;
                result.data[14]     = 0.0f;
                result.data[15]     = 1.0f;
                return result;
            }

            Engine::Matrix44 RotationZ(Real radian)
            {
                Engine::Matrix44 result;
                Real            sin = sinf(radian);
                Real            cos = cosf(radian);
                result.data[0]      = cos;
                result.data[1]      = -sin;
                result.data[2]      = 0.0f;
                result.data[3]      = 0.0f;
                result.data[4]      = sin;
                result.data[5]      = cos;
                result.data[6]      = 0.0f;
                result.data[7]      = 0.0f;
                result.data[8]      = 0.0f;
                result.data[9]      = 0.0f;
                result.data[10]     = 1.0f;
                result.data[11]     = 0.0f;
                result.data[12]     = 0.0f;
                result.data[13]     = 0.0f;
                result.data[14]     = 0.0f;
                result.data[15]     = 1.0f;
                return result;
            }

            Engine::Matrix44 Scale(const Engine::Vector4& scale)
            {
                Engine::Matrix44 result;
                result.data[0]  = scale.x;
                result.data[1]  = 0.0f;
                result.data[2]  = 0.0f;
                result.data[3]  = 0.0f;
                result.data[4]  = 0.0f;
                result.data[5]  = scale.y;
                result.data[6]  = 0.0f;
                result.data[7]  = 0.0f;
                result.data[8]  = 0.0f;
                result.data[9]  = 0.0f;
                result.data[10] = scale.z;
                result.data[11] = 0.0f;
                result.data[12] = 0.0f;
                result.data[13] = 0.0f;
                result.data[14] = 0.0f;
                result.data[15] = scale.w;
                return result;
            }

            Engine::Matrix44 Scale(const Engine::Vector3& scale, Real w)
            {
                Engine::Matrix44 result;
                result.data[0]  = scale.x;
                result.data[1]  = 0.0f;
                result.data[2]  = 0.0f;
                result.data[3]  = 0.0f;
                result.data[4]  = 0.0f;
                result.data[5]  = scale.y;
                result.data[6]  = 0.0f;
                result.data[7]  = 0.0f;
                result.data[8]  = 0.0f;
                result.data[9]  = 0.0f;
                result.data[10] = scale.z;
                result.data[11] = 0.0f;
                result.data[12] = 0.0f;
                result.data[13] = 0.0f;
                result.data[14] = 0.0f;
                result.data[15] = w;
                return result;
            }

            Engine::Matrix44 Scale(Real x, Real y, Real z, Real w)
            {
                Engine::Matrix44 result;
                result.data[0]  = x;
                result.data[1]  = 0.0f;
                result.data[2]  = 0.0f;
                result.data[3]  = 0.0f;
                result.data[4]  = 0.0f;
                result.data[5]  = y;
                result.data[6]  = 0.0f;
                result.data[7]  = 0.0f;
                result.data[8]  = 0.0f;
                result.data[9]  = 0.0f;
                result.data[10] = z;
                result.data[11] = 0.0f;
                result.data[12] = 0.0f;
                result.data[13] = 0.0f;
                result.data[14] = 0.0f;
                result.data[15] = w;
                return result;
            }

            Engine::Matrix44 Translation(const Engine::Vector3& translation)
            {
                Engine::Matrix44 result;
                result.data[0]  = 1.0f;
                result.data[1]  = 0.0f;
                result.data[2]  = 0.0f;
                result.data[3]  = translation.x;
                result.data[4]  = 0.0f;
                result.data[5]  = 1.0f;
                result.data[6]  = 0.0f;
                result.data[7]  = translation.y;
                result.data[8]  = 0.0f;
                result.data[9]  = 0.0f;
                result.data[10] = 1.0f;
                result.data[11] = translation.z;
                result.data[12] = 0.0f;
                result.data[13] = 0.0f;
                result.data[14] = 0.0f;
                result.data[15] = 1.0f;
                return result;
            }

            Engine::Matrix44 AffineTransformation(const Engine::Vector3& scale, const Engine::Vector3& origin, const Quaternion& rotation, const Engine::Vector3& translation)
            {
                Engine::Matrix44 result = Scale(scale, 1.0f);
                result.AddVectorColumn(3, -origin);
                result = Rotation(rotation) * result;
                result.AddVectorColumn(3, origin);
                result.AddVectorColumn(3, translation);
                result.SetTranspose();
                return result;
            }

            Engine::Matrix44 PerspectiveMatrix(Real screen_aspect, Real field_of_view, Real far_plane, Real near_plane)
            {
                Real            alpha = field_of_view * 0.5f;
                Real            cot   = cosf(alpha) / sinf(alpha);
                Engine::Matrix44 result;
                result.data[0]  = cot / screen_aspect;
                result.data[5]  = cot;
                result.data[10] = far_plane / (far_plane - near_plane);
                result.data[11] = 1.0f;
                result.data[14] = -near_plane * far_plane / (far_plane - near_plane);
                result.data[15] = 0.0f;
                return result;
            }

            Engine::Matrix44 OrthoGraphicMatrix(size_t client_width, size_t client_height, Real far_plane, Real near_plane)
            {
                Real            plane = 1.0f / (far_plane - near_plane);
                Engine::Matrix44 result;
                result.data[0]  = 2.0f / client_width;
                result.data[5]  = 2.0f / client_height;
                result.data[10] = plane;
                result.data[14] = plane * -near_plane;
                result.data[15] = 1.0f;
                return result;
            }

            Engine::Matrix44 OrthoGraphicCenterMatrix(Real right, Real left, Real top, Real bottom, Real far_plane, Real near_plane)
            {
                Engine::Matrix44 result;
                result.data[0]  = 2.0f / (right - left);
                result.data[5]  = 2.0f / (top - bottom);
                result.data[10] = 1.0f / (far_plane - near_plane);
                result.data[12] = (left + right) / (left - right);
                result.data[13] = (top + bottom) / (bottom - top);
                result.data[14] = near_plane / (near_plane - far_plane);
                result.data[15] = 1.0f;
                return result;
            }

            Engine::Matrix44 LookAt(const Engine::Vector3& eye_position, const Engine::Vector3& focus_position, const Engine::Vector3& up_direction)
            {
                return LookTo(eye_position, focus_position - eye_position, up_direction);
            }

            Engine::Matrix44 LookTo(const Engine::Vector3& eye_position, const Engine::Vector3& eye_direction, const Engine::Vector3& up_direction)
            {
                if (eye_direction.IsZero())
                {
                    return Engine::Matrix44();
                }
                Engine::Vector3  r2               = eye_direction.Normalize();
                Engine::Vector3  r0               = CrossProduct(up_direction, r2).Normalize();
                Engine::Vector3  r1               = CrossProduct(r2, r0);
                Engine::Vector3  neg_eye_position = eye_position.Negate();
                Real            d0               = DotProduct(r0, neg_eye_position);
                Real            d1               = DotProduct(r1, neg_eye_position);
                Real            d2               = DotProduct(r2, neg_eye_position);
                Engine::Matrix44 result;
                result.SetColumns(Engine::Vector4(r0, d0), Engine::Vector4(r1, d1), Engine::Vector4(r2, d2), Math::Vector4::W_AXIS);
                return result;
            }
        }

        namespace Matrix33
        {
            Engine::Matrix33 Rotation(const Quaternion& quaternion)
            {
                Engine::Matrix33 result;
                if (quaternion.IsUnit())
                {
                    Real xs        = quaternion.i + quaternion.i;
                    Real ys        = quaternion.j + quaternion.j;
                    Real zs        = quaternion.k + quaternion.k;
                    Real wx        = quaternion.r * xs;
                    Real wy        = quaternion.r * ys;
                    Real wz        = quaternion.r * zs;
                    Real xx        = quaternion.i * xs;
                    Real xy        = quaternion.i * ys;
                    Real xz        = quaternion.i * zs;
                    Real yy        = quaternion.j * ys;
                    Real yz        = quaternion.j * zs;
                    Real zz        = quaternion.k * zs;
                    result.data[0] = 1.0f - (yy + zz);
                    result.data[1] = xy - wz;
                    result.data[2] = xz + wy;
                    result.data[3] = xy + wz;
                    result.data[4] = 1.0f - (xx + zz);
                    result.data[5] = yz - wx;
                    result.data[6] = xz - wy;
                    result.data[7] = yz + wx;
                    result.data[8] = 1.0f - (xx + yy);
                }
                return result;
            }

            Engine::Matrix33 Rotation(const Engine::Matrix44& rotation_matrix)
            {
                Engine::Matrix33 result;
                result.data[0] = rotation_matrix.data[0];
                result.data[1] = rotation_matrix.data[1];
                result.data[2] = rotation_matrix.data[2];
                result.data[3] = rotation_matrix.data[4];
                result.data[4] = rotation_matrix.data[5];
                result.data[5] = rotation_matrix.data[6];
                result.data[6] = rotation_matrix.data[8];
                result.data[7] = rotation_matrix.data[9];
                result.data[8] = rotation_matrix.data[10];
                return result;
            }

            Engine::Matrix33 Rotation(const EulerAngle& euler_angle)
            {
                Engine::Matrix33 result;
                Real            sin_x = sinf(euler_angle.x_rotation);
                Real            cos_x = cosf(euler_angle.x_rotation);
                Real            sin_y = sinf(euler_angle.y_rotation);
                Real            cos_y = cosf(euler_angle.y_rotation);
                Real            sin_z = sinf(euler_angle.z_rotation);
                Real            cos_z = cosf(euler_angle.z_rotation);
                result.data[0]        = (cos_y * cos_z);
                result.data[1]        = -(cos_y * sin_z);
                result.data[2]        = sin_y;
                result.data[3]        = (sin_x * sin_y * cos_z) + (cos_x * sin_z);
                result.data[4]        = -(sin_x * sin_y * sin_z) + (cos_x * cos_z);
                result.data[5]        = -(sin_x * cos_y);
                result.data[6]        = -(cos_x * sin_y * cos_z) + (sin_x * sin_z);
                result.data[7]        = (cos_x * sin_y * sin_z) + (sin_x * cos_z);
                result.data[8]        = (cos_x * cos_y);
                return result;
            }

            Engine::Matrix33 Rotation(const AxisRadian& axis_radian)
            {
                Engine::Matrix33 result;
                Real            sin  = sinf(axis_radian.radian);
                Real            cos  = cosf(axis_radian.radian);
                Real            t    = 1.0f - cos;
                Engine::Vector3  Axis = axis_radian.axis;
                Axis.SetNormalize();
                Real tx        = t * Axis.x;
                Real ty        = t * Axis.y;
                Real tz        = t * Axis.z;
                Real sx        = sin * Axis.x;
                Real sy        = sin * Axis.y;
                Real sz        = sin * Axis.z;
                Real txy       = tx * Axis.y;
                Real tyz       = tx * Axis.z;
                Real txz       = tx * Axis.z;
                result.data[0] = (tx * Axis.x + cos);
                result.data[1] = (txy - sz);
                result.data[2] = (txz + sy);
                result.data[3] = (txy + sz);
                result.data[4] = (ty * Axis.y + cos);
                result.data[5] = (tyz - sx);
                result.data[6] = (txz - sy);
                result.data[7] = (tyz + sx);
                result.data[8] = (tz * Axis.z + cos);
                return result;
            }

            Engine::Matrix33 RotationX(Real radian)
            {
                Engine::Matrix33 result;
                Real            sin = sinf(radian);
                Real            cos = cosf(radian);
                result.data[0]      = 1.0f;
                result.data[1]      = 0.0f;
                result.data[2]      = 0.0f;
                result.data[3]      = 0.0f;
                result.data[4]      = cos;
                result.data[5]      = -sin;
                result.data[6]      = 0.0f;
                result.data[7]      = sin;
                result.data[8]      = cos;
                return result;
            }

            Engine::Matrix33 RotationY(Real radian)
            {
                Engine::Matrix33 result;
                Real            sin = sinf(radian);
                Real            cos = cosf(radian);
                result.data[0]      = cos;
                result.data[1]      = 0.0f;
                result.data[2]      = sin;
                result.data[3]      = 0.0f;
                result.data[4]      = 1.0f;
                result.data[5]      = 0.0f;
                result.data[6]      = -sin;
                result.data[7]      = 0.0f;
                result.data[8]      = cos;
                return result;
            }

            Engine::Matrix33 RotationZ(Real radian)
            {
                Engine::Matrix33 result;
                Real            sin = sinf(radian);
                Real            cos = cosf(radian);
                result.data[0]      = cos;
                result.data[1]      = -sin;
                result.data[2]      = 0.0f;
                result.data[3]      = sin;
                result.data[4]      = cos;
                result.data[5]      = 0.0f;
                result.data[6]      = 0.0f;
                result.data[7]      = 0.0f;
                result.data[8]      = 1.0f;
                return result;
            }

            Engine::Matrix33 Scale(const Engine::Vector3& scale)
            {
                Engine::Matrix33 result;
                result.data[0] = scale.x;
                result.data[1] = 0.0f;
                result.data[2] = 0.0f;
                result.data[3] = 0.0f;
                result.data[4] = scale.y;
                result.data[5] = 0.0f;
                result.data[6] = 0.0f;
                result.data[7] = 0.0f;
                result.data[8] = scale.z;
                return result;
            }
        }
    }
}
