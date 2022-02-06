#pragma once
#include <array>

#include "../../EngineDefine.hpp"

namespace Engine
{
    class Texture;
    class ShaderProgram;
    constexpr Uint32 MATERIAL_SINT_COUNT    = 5;
    constexpr Uint32 MATERIAL_REAL_COUNT    = 5;
    constexpr Uint32 MATERIAL_TEXTURE_COUNT = 5;

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

        std::array<Sint32, MATERIAL_SINT_COUNT> sint_params = {};
        std::array<Real, MATERIAL_REAL_COUNT>   real_params = {};
    };

    class Material
    {
    public:
        SPtr<ShaderProgram> GetShader();

        void SetShader(SPtr<ShaderProgram> shader);
        void SetSint(Uint32 index, Sint32 value);
        void SetReal(Uint32 index, Real value);
        void SetTexture(Uint32 index, SPtr<Texture> texture);

        void Bind();

    private:
        SPtr<ShaderProgram> m_shader;
        MaterialParams      m_material_params;

        std::array<SPtr<Texture>, MATERIAL_TEXTURE_COUNT> m_textures;
    };
}
