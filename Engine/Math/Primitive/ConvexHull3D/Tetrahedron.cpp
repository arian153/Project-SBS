/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Tetrahedron.hpp"

#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Tetrahedron::Tetrahedron()
    {
        m_type = ePrimitiveType::Tetrahedron;
    }

    Tetrahedron::~Tetrahedron()
    {
    }

    void Tetrahedron::Initialize()
    {
        SetUnit();
    }

    void Tetrahedron::Shutdown()
    {
    }

    void Tetrahedron::SetUnit()
    {
        vertices[0] = Math::Vector3::ORIGIN;
        vertices[1] = Math::Vector3::X_AXIS;
        vertices[2] = Math::Vector3::Y_AXIS;
        vertices[3] = Math::Vector3::Z_AXIS;
    }

    Vector3 Tetrahedron::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (size_t i = 0; i < 4; ++i)
        {
            Real projection = vertices[i].DotProduct(direction);
            if (projection > p)
            {
                result = vertices[i];
                p      = projection;
            }
        }
        return result;
    }

    bool Tetrahedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 Tetrahedron::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive;
    }

    MassData Tetrahedron::CalculateMassData(Real density) const
    {
        MassData data;

        data.local_centroid.SetZero();
        data.local_inertia.SetZero();
        Vector3  ref  = vertices[0];
        Vector3  v1   = vertices[1];
        Vector3  v2   = vertices[2];
        Vector3  v3   = vertices[3];
        Real     axis = 1.0f / 60.0f;
        Real     prod = 1.0f / 120.0f;
        Matrix33 canonical_matrix(axis, prod, prod, prod, axis, prod, prod, prod, axis);
        Matrix33 transformation_matrix;
        transformation_matrix.SetColumns(v1 - ref, v2 - ref, v3 - ref);
        //calculate sub inertia
        data.local_inertia = transformation_matrix.Determinant() * transformation_matrix * canonical_matrix * transformation_matrix.Transpose();
        //volume is 1 / 6 of triple product, that is 1/6 det of transformation matrix.
        data.mass = density * transformation_matrix.Determinant() / 6.0f;
        //The center-of-mass is just the mean of the four vertex coordinates. 
        data.local_centroid = (ref + v1 + v2 + v3) * 0.25f;
        if (!ref.IsZero())
        {
            data.local_inertia += data.mass * (ref.OuterProduct(data.local_centroid) + data.local_centroid.OuterProduct(ref) + ref.OuterProduct(ref));
        }

        data.CalculateInverse();
        return data;
    }

    Real Tetrahedron::CalculateVolume() const
    {
        Vector3  ref = vertices[0];
        Vector3  v1  = vertices[1];
        Vector3  v2  = vertices[2];
        Vector3  v3  = vertices[3];
        Matrix33 transformation_matrix;
        transformation_matrix.SetColumns(v1 - ref, v2 - ref, v3 - ref);
        return transformation_matrix.Determinant() / 6.0f;
    }

    Vector3Pair Tetrahedron::CalculateBoundPair(const VecQuatScale& world) const
    {
        Vector3 min = world.LocalToWorldPoint(transform.LocalToWorldPoint(vertices[0]));
        Vector3 max = min;

        for (int i = 1; i < 4; ++i)
        {
            Vector3 vertex = world.LocalToWorldPoint(transform.LocalToWorldPoint(vertices[i]));
            min.x          = Math::Min(min.x, vertex.x);
            min.y          = Math::Min(min.y, vertex.y);
            min.z          = Math::Min(min.z, vertex.z);
            max.x          = Math::Max(max.x, vertex.x);
            max.y          = Math::Max(max.y, vertex.y);
            max.z          = Math::Max(max.z, vertex.z);
        }

        return Vector3Pair(min, max);
    }
}
