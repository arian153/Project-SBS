#include "Collider.hpp"

#include "RigidBody.hpp"
#include "../../Math/Primitive/ConvexHull.hpp"
#include "../../Math/Primitive/Primitive.hpp"
#include "../../Math/Utility/Utility.hpp"

namespace Engine
{
    Collider::Collider()
    {
    }

    Collider::~Collider()
    {
    }

    void Collider::Shutdown()
    {
        for (auto& primitive : m_primitives)
        {
            primitive->Shutdown();
            delete primitive;
            primitive = nullptr;
        }

        m_primitives.clear();
    }

    Primitive* Collider::AddPrimitive(ePrimitiveType type)
    {
        Primitive* primitive = nullptr;

        switch (type)
        {
        case ePrimitiveType::Circle:
            primitive = new Circle();
            break;
        case ePrimitiveType::Ellipse:
            primitive = new Ellipse();
            break;
        case ePrimitiveType::Polygon:
            break;
        case ePrimitiveType::Rectangle:
            primitive = new Rectangle();
            break;
        case ePrimitiveType::Triangle:
            primitive = new Triangle();
            break;
        case ePrimitiveType::Box:
            primitive = new Box();
            break;
        case ePrimitiveType::Capsule:
            primitive = new Capsule();
            break;
        case ePrimitiveType::Cone:
            primitive = new Cone();
            break;
        case ePrimitiveType::Cylinder:
            primitive = new Cylinder();
            break;
        case ePrimitiveType::Dome:
            primitive = new Dome();
            break;
        case ePrimitiveType::Ellipsoid:
            primitive = new Ellipsoid();
            break;
        case ePrimitiveType::Polyhedron:
            break;
        case ePrimitiveType::Sphere:
            primitive = new Sphere();
            break;
        case ePrimitiveType::Tetrahedron:
            primitive = new Tetrahedron();
            break;
        case ePrimitiveType::TruncatedCone:
            primitive = new TruncatedCone();
            break;
        case ePrimitiveType::Last:
            break;
        default: ;
        }

        if (primitive != nullptr)
        {
            m_primitives.push_back(primitive);
        }

        return primitive;
    }

    MassData Collider::GetMassData(Real density) const
    {
        MassData result;

        result.mass = 0.0f;
        result.local_centroid.SetZero();

        std::vector<MassData> primitive_mass_data;

        size_t count = m_primitives.size();

        primitive_mass_data.resize(count);

        for (size_t i = 0; i < count; ++i)
        {
            primitive_mass_data[i] = m_primitives[i]->CalculateMassData(density);

            // accumulate mass
            result.mass += primitive_mass_data[i].mass;

            Vector3 world_centroid = m_primitives[i]->LocalToWorldPoint(primitive_mass_data[i].local_centroid);

            // accumulate weighted contribution
            result.local_centroid += primitive_mass_data[i].mass * world_centroid;
        }

        // compute inverse mass
        result.inverse_mass = Math::IsZero(result.mass) ? 0.0f : 1.0f / result.mass;

        // compute final local centroid
        result.local_centroid *= result.inverse_mass;
        // compute local inertia tensor
        result.local_inertia.SetZero();

        for (size_t i = 0; i < count; ++i)
        {
            Vector3  world_centroid = m_primitives[i]->LocalToWorldPoint(primitive_mass_data[i].local_centroid);
            Vector3  r              = result.local_centroid - world_centroid;
            Real     r_dot_r        = r.DotProduct(r);
            Matrix33 r_out_r        = r.OuterProduct(r);
            // accumulate local inertia tensor contribution, using Parallel Axis Theorem

            Matrix33 world_inertia = MassData::TranslateInertia(
                                                                MassData::RotateInertia(
                                                                                        primitive_mass_data[i].local_inertia,
                                                                                        m_primitives[i]->transform.rotation),
                                                                primitive_mass_data[i].mass,
                                                                m_primitives[i]->transform.position);

            result.local_inertia += world_inertia +
                    primitive_mass_data[i].mass *
                    (r_dot_r * Matrix33::Identity() - r_out_r);
        }

        // compute inverse inertia tensor
        Math::IsZero(result.mass)
            ? result.local_inverse_inertia.SetZero()
            : result.local_inverse_inertia = result.local_inertia.Inverse();

        return result;
    }

    RigidBody* Collider::GetRigidBody() const
    {
        return m_rigid_body;
    }

    Vector3 Collider::LocalToWorldPoint(const Vector3& point) const
    {
        return m_rigid_body->GetVqs().LocalToWorldPoint(point);
    }
}
