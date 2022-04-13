#include "RigidContactPoint.hpp"

#include "../../../Math/Utility/Utility.hpp"

namespace Engine
{
    RigidContactPoint::RigidContactPoint()
    {
    }

    RigidContactPoint::~RigidContactPoint()
    {
    }

    void RigidContactPoint::Clear()
    {
        global_position_a.SetZero();
        global_position_b.SetZero();
        local_position_a.SetZero();
        local_position_b.SetZero();
        normal.SetZero();
        depth            = 0.0f;
        normal_lambda    = 0.0f;
        tangent_lambda   = 0.0f;
        bitangent_lambda = 0.0f;
        b_valid          = true;
        b_persistent     = false;
    }

    RigidContactPoint& RigidContactPoint::operator=(const RigidContactPoint& rhs)
    {
        if (this != &rhs)
        {
            this->primitive_a        = rhs.primitive_a;
            this->primitive_b        = rhs.primitive_b;
            this->global_position_a = rhs.global_position_a;
            this->global_position_b = rhs.global_position_b;
            this->local_position_a  = rhs.local_position_a;
            this->local_position_b  = rhs.local_position_b;
            this->normal            = rhs.normal;
            this->depth             = rhs.depth;
            this->normal_lambda     = rhs.normal_lambda;
            this->tangent_lambda    = rhs.tangent_lambda;
            this->bitangent_lambda  = rhs.bitangent_lambda;
            this->b_valid           = rhs.b_valid;
            this->b_persistent      = rhs.b_persistent;
        }
        return *this;
    }

    bool RigidContactPoint::operator==(const RigidContactPoint& rhs) const
    {
        return (normal == rhs.normal && depth == rhs.depth && local_position_a == rhs.local_position_a && local_position_b == rhs.local_position_b);
    }

    void RigidContactPoint::Swap()
    {
        Math::Swap(primitive_a, primitive_b);
        Math::Swap(global_position_a, global_position_b);
        Math::Swap(local_position_a, local_position_b);
        normal = -normal;
    }

    void RigidContactPoint::UpdateContactPoint(const RigidContactPoint& rhs)
    {
        normal            = rhs.normal;
        depth             = rhs.depth;
        global_position_a = rhs.global_position_a;
        global_position_b = rhs.global_position_b;
        local_position_a  = rhs.local_position_a;
        local_position_b  = rhs.local_position_b;
        b_valid           = rhs.b_valid;
        b_persistent      = true;
    }

    RigidContactPoint RigidContactPoint::SwappedContactPoint() const
    {
        RigidContactPoint result;
        result.normal            = -normal;
        result.depth             = this->depth;
        result.primitive_a        = this->primitive_b;
        result.primitive_b        = this->primitive_a;
        result.global_position_a = this->global_position_b;
        result.global_position_b = this->global_position_a;
        result.local_position_a  = this->local_position_b;
        result.local_position_b  = this->local_position_a;
        result.b_valid           = this->b_valid;
        return result;
    }
}
