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

   }
