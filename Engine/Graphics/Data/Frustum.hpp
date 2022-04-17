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
#include <array>

#include "../../Math/Primitive/Others/Plane.hpp"

namespace Engine
{
	enum class eFrustumPlaneType : Uint32
	{
		PlaneFront,
		PlaneBack,
		PlaneUp,
		PlaneDown,
		PlaneLeft,
		PlaneRight,
		PlaneEnd
	};

	class Frustum
	{
	public:
		void Generate(const Matrix44& view, const Matrix44& proj);
		bool ContainsSphere(const Vector3& pos, Real radius) const;

	private:
        std::array<Plane, static_cast<Uint32>(eFrustumPlaneType::PlaneEnd)> m_planes;
	};
}
