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
