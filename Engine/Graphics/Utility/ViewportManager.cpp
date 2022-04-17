/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "ViewportManager.hpp"

#include "../../Math/Utility/MatrixUtility.hpp"

namespace Engine
{
    ViewportManager::ViewportManager()
    {
    }

    ViewportManager::~ViewportManager()
    {
    }

    Matrix44 ViewportManager::GetPerspectiveMatrix() const
    {
        return Math::Matrix44::PerspectiveMatrix(m_screen_aspect, m_field_of_view, m_far_plane, m_near_plane);
    }

    Matrix44 ViewportManager::GetOrthoGraphicMatrix() const
    {
        return Math::Matrix44::OrthoGraphicCenterMatrix(m_right, m_left, m_top, m_bottom, m_far_plane, m_near_plane);
    }

    Real ViewportManager::GetScreenRight() const
    {
        return m_right;
    }

    Real ViewportManager::GetScreenLeft() const
    {
        return m_left;
    }

    Real ViewportManager::GetScreenTop() const
    {
        return m_top;
    }

    Real ViewportManager::GetScreenBottom() const
    {
        return m_bottom;
    }

    Real ViewportManager::GetFarPlane() const
    {
        return m_far_plane;
    }

    Real ViewportManager::GetNearPlane() const
    {
        return m_near_plane;
    }

    Real ViewportManager::GetAspectRatio() const
    {
        return m_screen_aspect;
    }

    Real ViewportManager::GetScreenWidth() const
    {
        return m_right - m_left;
    }

    Real ViewportManager::GetScreenHeight() const
    {
        return m_top - m_bottom;
    }

    Vector2 ViewportManager::GetScreenResolution() const
    {
        return Vector2(m_right - m_left, m_top - m_bottom);
    }

    void ViewportManager::SetScreenAspect(Real screen_aspect)
    {
        m_screen_aspect      = screen_aspect;
        Real   client_height = m_top - m_bottom;
        Real   width         = screen_aspect * client_height;
        Sint32 client_width  = static_cast<Sint32>(width);
        Real   half_x        = static_cast<Real>(client_width >> 1);
        m_right              = half_x;
        m_left               = -half_x;
    }

    void ViewportManager::SetFieldOfView(Real field_of_view)
    {
        m_field_of_view = field_of_view;
    }

    void ViewportManager::SetFarPlane(Real far_plane)
    {
        m_far_plane = far_plane;
    }

    void ViewportManager::SetNearPlane(Real near_plane)
    {
        m_near_plane = near_plane;
    }

    void ViewportManager::SetClientWidth(Sint32 client_width)
    {
        Real half_x     = static_cast<Real>(client_width >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_screen_aspect = static_cast<Real>(client_width) / (m_top - m_bottom);
    }

    void ViewportManager::SetClientHeight(Sint32 client_height)
    {
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_top           = half_y;
        m_bottom        = -half_y;
        m_screen_aspect = (m_right - m_left) / static_cast<Real>(client_height);
    }

    void ViewportManager::SetClientRect(Sint32 client_width, Sint32 client_height)
    {
        Real half_x     = static_cast<Real>(client_width >> 1);
        Real half_y     = static_cast<Real>(client_height >> 1);
        m_right         = half_x;
        m_left          = -half_x;
        m_top           = half_y;
        m_bottom        = -half_y;
        m_screen_aspect = static_cast<Real>(client_width) / static_cast<Real>(client_height);
    }

    void ViewportManager::SetClientRect(Real right, Real left, Real top, Real bottom)
    {
        m_right         = right;
        m_left          = left;
        m_top           = top;
        m_bottom        = bottom;
        m_screen_aspect = (m_right - m_left) / (m_top - m_bottom);
    }
}
