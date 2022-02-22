#include "MeshData.hpp"

namespace Engine
{
    Face::Face()
        : a(0), b(0), c(0)
    {
    }

    Face::Face(Uint32 a, Uint32 b, Uint32 c)
        : a(a), b(b), c(c)
    {
    }

    void MeshData::SetNormalize(Vector3& min, Vector3& max)
    {
        Uint32 size = static_cast<Uint32>(vertices.size());
        min         = vertices[0].pos;
        max         = vertices[0].pos;
        for (Uint32 i = 1; i < size; ++i)
        {
            Vector3 point = vertices[i].pos;
            min.x         = min.x > point.x ? point.x : min.x;
            min.y         = min.y > point.y ? point.y : min.y;
            min.z         = min.z > point.z ? point.z : min.z;
            max.x         = max.x < point.x ? point.x : max.x;
            max.y         = max.y < point.y ? point.y : max.y;
            max.z         = max.z < point.z ? point.z : max.z;
        }
        Vector3 scale        = max - min;
        Real    scale_factor = scale.x;
        scale_factor         = scale_factor < scale.y ? scale.y : scale_factor;
        scale_factor         = scale_factor < scale.z ? scale.z : scale_factor;
        Real    vertex_scale = scale_factor == 0.0f ? 0.0f : 2.0f / scale_factor;
        Vector3 center       = min + scale * 0.5f;
        for (auto& vertex : vertices)
        {
            vertex.pos = (vertex_scale * (vertex.pos - center));
        }
        Vector3 scale_center = vertex_scale * center;
    }

    Vector3 MeshData::GetFaceNormal(Uint32 a, Uint32 b, Uint32 c) const
    {
        Vector3 edge_ab = vertices[b].pos - vertices[a].pos;
        Vector3 edge_ac = vertices[c].pos - vertices[a].pos;
        return CrossProduct(edge_ab, edge_ac).Normalize();
    }

    void MeshData::Convert(std::vector<PosVertex>& new_vertices) const
    {
        size_t size = vertices.size();
        new_vertices.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            new_vertices[i].pos = vertices[i].pos;
        }
    }

    void MeshData::Convert(std::vector<TexVertex>& new_vertices) const
    {
        size_t size = vertices.size();
        new_vertices.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            new_vertices[i].pos = vertices[i].pos;
            new_vertices[i].tex = vertices[i].tex;
        }
    }

    void MeshData::Convert(std::vector<NormalVertex>& new_vertices) const
    {
        size_t size = vertices.size();
        new_vertices.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            new_vertices[i].pos = vertices[i].pos;
            new_vertices[i].n   = vertices[i].n;
        }
    }

    void MeshData::Convert(std::vector<LightVertex>& new_vertices) const
    {
        size_t size = vertices.size();
        new_vertices.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            new_vertices[i].pos = vertices[i].pos;
            new_vertices[i].tex = vertices[i].tex;
            new_vertices[i].n   = vertices[i].n;
        }
    }

    void MeshData::Convert(std::vector<GeneralVertex>& new_vertices) const
    {
        size_t size = vertices.size();
        new_vertices.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            new_vertices[i].pos = vertices[i].pos;
            new_vertices[i].tex = vertices[i].tex;
            new_vertices[i].n   = vertices[i].n;
            new_vertices[i].t   = vertices[i].t;
        }
    }
}
