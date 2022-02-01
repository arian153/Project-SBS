#include "Polyhedron.hpp"
#include "../../Utility/Utility.hpp"
#include "../Others/Ray.hpp"

namespace Engine
{
    Polyhedron::Polyhedron()
    {
        m_type = ePrimitiveType::Polyhedron;
    }

    Polyhedron::~Polyhedron()
    {
        if (vertices != nullptr)
        {
            Shutdown();
        }
    }

    void Polyhedron::Initialize()
    {
        vertices = new std::vector<Vector3>();
    }

    void Polyhedron::Shutdown()
    {
        if (vertices != nullptr)
        {
            vertices->clear();
            delete vertices;
            vertices = nullptr;
        }
    }

    void Polyhedron::SetUnit()
    {
        Vector3 scale = m_max_point - m_min_point;
        scale.SetInverse();

        if(vertices != nullptr)
        {
            for(auto& vertex : *vertices)
            {
               vertex = vertex.HadamardProduct(scale);
            }
        }
    }

    Vector3 Polyhedron::Support(const Vector3& direction)
    {
        Real    p = Math::REAL_NEGATIVE_MAX;
        Vector3 result;
        for (auto& vertex : *vertices)
        {
            Real projection = vertex.DotProduct(direction);
            if (projection > p)
            {
                result = vertex;
                p      = projection;
            }
        }
        return result;
    }

    bool Polyhedron::TestRayIntersection(const Ray& local_ray, Real& minimum_t, Real& maximum_t) const
    {
        minimum_t = local_ray.direction.DotProduct(local_ray.position);
        maximum_t = -1.0f;
        return false;
    }

    Vector3 Polyhedron::GetNormal(const Vector3& local_point_on_primitive)
    {
        return local_point_on_primitive;
    }

   
    void Polyhedron::UpdateMinMaxPoint()
    {
        m_min_point = Vector3(Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX, Math::REAL_POSITIVE_MAX);
        m_max_point = Vector3(Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX, Math::REAL_NEGATIVE_MAX);

        for (auto& vertex : *vertices)
        {
            if (vertex.x < m_min_point.x)
            {
                m_min_point.x = vertex.x;
            }

            if (vertex.y < m_min_point.y)
            {
                m_min_point.y = vertex.y;
            }

            if (vertex.z < m_min_point.z)
            {
                m_min_point.z = vertex.z;
            }

            if (vertex.x > m_max_point.x)
            {
                m_max_point.x = vertex.x;
            }

            if (vertex.y > m_max_point.y)
            {
                m_max_point.y = vertex.y;
            }

            if (vertex.z > m_max_point.z)
            {
                m_max_point.z = vertex.z;
            }
        }
    }
}
