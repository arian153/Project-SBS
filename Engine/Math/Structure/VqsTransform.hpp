#pragma once
#include "../Algebra/Vector3.hpp"
#include "../Algebra/Quaternion.hpp"
#include "../Algebra/Matrix44.hpp"

namespace Engine
{
    class VqsTransform
    {
    public:
        VqsTransform();
        VqsTransform(const VqsTransform& vqs);
        VqsTransform(const Vector3& pos, const Quaternion& rot, Real s = 1.0f);
        ~VqsTransform();

        void Set(const Vector3& pos, const Quaternion& rot, Real s);
        Matrix44 ToMatrix() const;

        static VqsTransform Identity();
        VqsTransform Inverse() const;

        Vector3 ApplyTransform(const Vector3& point) const;
        Vector3 ApplyInverse(const Vector3& point) const;

        Vector3 LocalToWorldPoint(const Vector3& local_point) const;
        Vector3 WorldToLocalPoint(const Vector3& world_point) const;
        Vector3 LocalToWorldVector(const Vector3& local_vector) const;
        Vector3 WorldToLocalVector(const Vector3& world_vector) const;

        static VqsTransform Concatenate(const VqsTransform& a, const VqsTransform& b);
        static VqsTransform Interpolation(const VqsTransform& start, const VqsTransform& end, Real t);
        static Matrix44 Concatenate(const Matrix44& a, const Matrix44& b);

    public:
        VqsTransform& operator=(const VqsTransform& vqs);
        VqsTransform  operator +(const VqsTransform& vqs) const;
        VqsTransform  operator *(Real real) const;
        VqsTransform  operator *(const VqsTransform& vqs) const;

        Vector3              operator *(const Vector3& rhs) const;
        friend std::ostream& operator<<(std::ostream& os, const VqsTransform& vqs);
        friend VqsTransform  operator*(Real real, const VqsTransform& vqs);

    public:
        Vector3    position;
        Real       scale = 1.0f;
        Quaternion rotation;
    };
}
