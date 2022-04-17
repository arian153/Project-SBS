/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Frustum.hpp"

#include "../../Math/Algebra/Matrix44.hpp"

namespace Engine
{
    void Frustum::Generate(const Matrix44& view, const Matrix44& proj)
    {
        Matrix44 mat_view_inv       = view.Inverse();
        Matrix44 mat_projection_inv = proj.Inverse();
        Matrix44 mat_inv            = mat_projection_inv * mat_view_inv;

        std::vector<Vector3> world_pos =
        {
            mat_inv.TransformPoint(Vector3(-1.f, 1.f, 0.f)),
            mat_inv.TransformPoint(Vector3(1.f, 1.f, 0.f)),
            mat_inv.TransformPoint(Vector3(1.f, -1.f, 0.f)),
            mat_inv.TransformPoint(Vector3(-1.f, -1.f, 0.f)),
            mat_inv.TransformPoint(Vector3(-1.f, 1.f, 1.f)),
            mat_inv.TransformPoint(Vector3(1.f, 1.f, 1.f)),
            mat_inv.TransformPoint(Vector3(1.f, -1.f, 1.f)),
            mat_inv.TransformPoint(Vector3(-1.f, -1.f, 1.f))
        };

        m_planes[static_cast<Uint32>(eFrustumPlaneType::PlaneFront)] = Plane(world_pos[0], world_pos[1], world_pos[2]); // CW
        m_planes[static_cast<Uint32>(eFrustumPlaneType::PlaneBack)]  = Plane(world_pos[4], world_pos[7], world_pos[5]); // CCW
        m_planes[static_cast<Uint32>(eFrustumPlaneType::PlaneUp)]    = Plane(world_pos[4], world_pos[5], world_pos[1]); // CW
        m_planes[static_cast<Uint32>(eFrustumPlaneType::PlaneDown)]  = Plane(world_pos[7], world_pos[3], world_pos[6]); // CCW
        m_planes[static_cast<Uint32>(eFrustumPlaneType::PlaneLeft)]  = Plane(world_pos[4], world_pos[0], world_pos[7]); // CW
        m_planes[static_cast<Uint32>(eFrustumPlaneType::PlaneRight)] = Plane(world_pos[5], world_pos[6], world_pos[1]); // CCW
    }

    bool Frustum::ContainsSphere(const Vector3& pos, Real radius) const
    {
        for (auto& plane : m_planes)
        {
            // ax + by + cz + d > radius
            if (DotProduct(plane.Normal(), pos) + plane.d > radius)
                return false;
        }

        return true;
    }
}
