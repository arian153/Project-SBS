#include "Simplex.hpp"

namespace Engine
{
    Simplex::Simplex()
    {
    }

    Simplex::~Simplex()
    {
    }

    SupportPoint& Simplex::operator[](size_t index)
    {
    }

    SupportPoint Simplex::operator[](size_t index) const
    {
    }

    bool Simplex::DoSimplexPoint(Vector3& dir)
    {
    }

    bool Simplex::DoSimplexLine(Vector3& dir)
    {
    }

    bool Simplex::DoSimplexTriangle(Vector3& dir)
    {
    }

    bool Simplex::DoSimplexTetrahedron(Vector3& dir)
    {
    }

    bool Simplex::DoSimplex(Vector3& dir)
    {
    }

    bool Simplex::IsValid()
    {
    }

    bool Simplex::DoSimplex2DPoint(Vector3& dir)
    {
    }

    bool Simplex::DoSimplex2DLine(Vector3& dir)
    {
    }

    bool Simplex::DoSimplex2DTriangle(Vector3& dir)
    {
    }

    bool Simplex::DoSimplex2D(Vector3& dir)
    {
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c, const SupportPoint& d)
    {
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b, const SupportPoint& c)
    {
    }

    void Simplex::Set(const SupportPoint& a, const SupportPoint& b)
    {
    }

    void Simplex::Set(const SupportPoint& a)
    {
    }

    Vector3 Simplex::ClosestPointGlobal(const Vector3& point) const
    {
    }

    Vector3 Simplex::ClosestPointLocalA(const Vector3& point) const
    {
    }

    Vector3 Simplex::ClosestPointLocalB(const Vector3& point) const
    {
    }

    int Simplex::Count() const
    {
    }

    Vector3 Simplex::ClosestPointLine(const Vector3& point, const Vector3& a, const Vector3& b)
    {
    }

    Vector3 Simplex::ClosestPointTriangle(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c)
    {
    }

    Vector3 Simplex::ClosestPointTetrahedron(const Vector3& point, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
    {
    }

    bool Simplex::PointOutsidePlane(const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c)
    {
    }

    bool Simplex::PointOutsidePlane(const Vector3& p, const Vector3& a, const Vector3& b, const Vector3& c, const Vector3& d)
    {
    }
}
