#pragma once
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Matrix44.hpp"

namespace Engine
{
    class VecQuatScale
    {
    public:
        VecQuatScale();
        VecQuatScale(const VecQuatScale& vqs);
        VecQuatScale(const Vector3& pos, const Quaternion& rot, Real s = 1.0f);
        ~VecQuatScale();

        void Set(const Vector3& pos, const Quaternion& rot, Real s);
        Matrix44 ToMatrix() const;

        static VecQuatScale Identity();
        VecQuatScale Inverse() const;

        Vector3 ApplyTransform(const Vector3& point) const;
        Vector3 ApplyInverse(const Vector3& point) const;

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;

        static VecQuatScale Concatenate(const VecQuatScale& a, const VecQuatScale& b);
        static VecQuatScale Interpolation(const VecQuatScale& start, const VecQuatScale& end, Real t);
        static Matrix44 Concatenate(const Matrix44& a, const Matrix44& b);

    public:
        VecQuatScale& operator=(const VecQuatScale& vqs);
        VecQuatScale  operator +(const VecQuatScale& vqs) const;
        VecQuatScale  operator *(Real real) const;
        VecQuatScale  operator *(const VecQuatScale& vqs) const;

        Vector3              operator *(const Vector3& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const VecQuatScale& vqs);
        friend VecQuatScale  operator*(Real real, const VecQuatScale& vqs);

    public:
        Vector3    position;
        Real       scale = 1.0f;
        Quaternion rotation;
    };
}
