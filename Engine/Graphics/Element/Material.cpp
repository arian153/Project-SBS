#include "Material.hpp"

#include "../GraphicsDefine.hpp"
#include "../DirectX12/Texture.hpp"
#include "../DirectX12/Buffer/ConstantBuffer.hpp"
#include "../Shader/ShaderProgram.hpp"

namespace Engine
{
    void MaterialParams::SetSint(Uint32 index, Sint32 value)
    {
        sint_params[index] = value;
    }

    void MaterialParams::SetReal(Uint32 index, Real value)
    {
        real_params[index] = value;
    }

    SPtr<ShaderProgram> Material::GetShader()
    {
        return m_shader;
    }

    void Material::SetShader(SPtr<ShaderProgram> shader)
    {
        m_shader = shader;
    }

    void Material::SetSint(Uint32 index, Sint32 value)
    {
        m_material_params.SetSint(index, value);
    }

    void Material::SetReal(Uint32 index, Real value)
    {
        m_material_params.SetReal(index, value);
    }

    void Material::SetTexture(Uint32 index, SPtr<Texture> texture)
    {
        m_textures[index] = texture;
    }

    void Material::Bind(SPtr<ConstantBuffer> material_buffer) const
    {
        material_buffer->PushData(&m_material_params, sizeof(m_material_params));

        for (size_t i = 0; i < m_textures.size(); i++)
        {
            if (m_textures[i] == nullptr)
                continue;

            auto reg = static_cast<eSRVRegister>(static_cast<Uint32>(eSRVRegister::t0) + i);
            DESCRIPTOR_HEAP->SetSRV(m_textures[i]->GetSRVHandle(), reg);
        }

        //Bind Pipeline State in Shader Program
        m_shader->Bind();
    }
}
