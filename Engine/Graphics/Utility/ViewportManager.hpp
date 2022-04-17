/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#pragma once
#include "../../Math/Algebra/Matrix44.hpp"
#include "../../Math/Algebra/Vector2.hpp"

namespace Engine
{
    struct MatrixParams
    {
        Matrix44 world;
        Matrix44 view;
        Matrix44 proj;
        Matrix44 wv;
        Matrix44 wvp;
    };

    class ViewportManager
    {
    public:
        ViewportManager();
        ~ViewportManager();

        Matrix44 GetPerspectiveMatrix() const;
        Matrix44 GetOrthoGraphicMatrix() const;

        Real GetScreenRight() const;
        Real GetScreenLeft() const;
        Real GetScreenTop() const;
        Real GetScreenBottom() const;

        Real GetFarPlane() const;
        Real GetNearPlane() const;
        Real GetAspectRatio() const;

        Real GetScreenWidth() const;
        Real GetScreenHeight() const;

        Vector2 GetScreenResolution() const;

    private:
        void SetScreenAspect(Real screen_aspect);
        void SetFieldOfView(Real field_of_view);
        void SetFarPlane(Real far_plane);
        void SetNearPlane(Real near_plane);
        void SetClientWidth(Sint32 client_width);
        void SetClientHeight(Sint32 client_height);
        void SetClientRect(Sint32 client_width, Sint32 client_height);
        void SetClientRect(Real right, Real left, Real top, Real bottom);

    private:
        friend class RenderSystem;

    private:
        Real m_right  = 0.0f;
        Real m_left   = 0.0f;
        Real m_top    = 0.0f;
        Real m_bottom = 0.0f;

        Real m_screen_aspect = 1.0f;
        Real m_field_of_view = Math::PI_DIV_4;

        Real m_far_plane  = 1000.f;
        Real m_near_plane = 0.001f;
    };
}
