#pragma once
#include <array>

#include "../../EngineDefine.hpp"
#include "../../Math/Algebra/Vector2.hpp"

namespace Engine
{
    class ConstantBuffer;
    class Texture;
    class ShaderProgram;
    constexpr Uint32 MATERIAL_SINT_COUNT    = 4;
    constexpr Uint32 MATERIAL_REAL_COUNT    = 4;
    constexpr Uint32 MATERIAL_VEC2_COUNT    = 4;
    constexpr Uint32 MATERIAL_TEXTURE_COUNT = 8;

    struct TexturePaths
    {
        std::vector<String> diff;
        std::vector<String> spec;
        std::vector<String> norm;
    };

    struct MaterialParams
    {
        void SetSint(Uint32 index, Sint32 value);
        void SetReal(Uint32 index, Real value);
        void SetVec2(Uint32 index, const Vector2& value);

        std::array<Sint32, MATERIAL_SINT_COUNT>  sint_params = {};
        std::array<Real, MATERIAL_REAL_COUNT>    real_params = {};
        std::array<Vector2, MATERIAL_VEC2_COUNT> vec2_params = {};
    };

    class Material
    {
    public:
        SPtr<ShaderProgram> GetShader();

        void SetShader(SPtr<ShaderProgram> shader);
        void SetSint(Uint32 index, Sint32 value);
        void SetReal(Uint32 index, Real value);
        void SetVector2(Uint32 index, const Vector2& value);
        void SetTexture(Uint32 index, SPtr<Texture> texture);
        void SetSpecularMap(SPtr<Texture> texture);
        void SetNormalMap(SPtr<Texture> texture);
        void SetOtherMap(SPtr<Texture> texture);
        void SetCubeMap(SPtr<Texture> texture);

        void Bind(SPtr<ConstantBuffer> material_buffer) const;

    private:
        SPtr<ShaderProgram> m_shader;
        MaterialParams      m_material_params;

        std::array<SPtr<Texture>, MATERIAL_TEXTURE_COUNT> m_textures;
    };
}
