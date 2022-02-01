#include "InputLayout.hpp"

namespace Engine
{
    Uint32 VertexAttribute::SizeOf(eAttributeType type)
    {
        switch (type)
        {
        case eAttributeType::T32:
            return sizeof(int);
        case eAttributeType::R32:
            return sizeof(Real32);
        case eAttributeType::I32:
            return sizeof(Sint32);
        case eAttributeType::U32:
            return sizeof(Uint32);
        default:
            return 0;
        }
    }

    void InputLayout::PushAttribute(const VertexAttribute& attribute)
    {
        m_attributes.push_back(attribute);
        m_stride += VertexAttribute::SizeOf(attribute.format_type) * attribute.format_count;
    }

    void InputLayout::PushAttribute(eAttributeType type, Uint32 count, const String& semantic_name, Uint32 semantic_idx)
    {
        VertexAttribute attribute;
        attribute.format_type  = type;
        attribute.format_count = count;
        attribute.sem_name     = semantic_name;
        attribute.sem_idx      = semantic_idx;

        m_attributes.push_back(attribute);
        m_stride += VertexAttribute::SizeOf(type) * count;
    }

    void InputLayout::PushAttribute(eAttributeType type, Uint32 count, const String& semantic_name, Uint32 semantic_idx, eInputSlotType slot_type, Uint32 input_slot, Uint32 step_rate)
    {
        VertexAttribute attribute;
        attribute.format_type  = type;
        attribute.format_count = count;
        attribute.sem_name     = semantic_name;
        attribute.sem_idx      = semantic_idx;
        attribute.slot_type    = slot_type;
        attribute.input_slot   = input_slot;
        attribute.step_rate    = step_rate;

        m_attributes.push_back(attribute);
        m_stride += VertexAttribute::SizeOf(type) * count;
    }

    void InputLayout::PushLayout(const InputLayout& layout)
    {
        if (this != &layout)
        {
            size_t size = layout.m_attributes.size();
            for (size_t i = 0; i < size; ++i)
            {
                PushAttribute(layout.m_attributes[i]);
            }
        }
    }

    void InputLayout::Clear()
    {
        m_attributes.clear();
        m_stride = 0;
    }

    Uint32 InputLayout::GetStride() const
    {
        return m_stride;
    }

    std::vector<VertexAttribute>& InputLayout::GetAttributes()
    {
        return m_attributes;
    }

    const std::vector<VertexAttribute>& InputLayout::GetAttributes() const
    {
        return m_attributes;
    }

    bool InputLayout::GetLayoutDesc(std::vector<D3D12_INPUT_ELEMENT_DESC>& layout) const
    {
        size_t size = m_attributes.size();

        if (size == 0)
            return false;

        layout.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            layout[i].SemanticName         = m_attributes[i].sem_name.c_str();
            layout[i].SemanticIndex        = m_attributes[i].sem_idx;
            layout[i].InputSlot            = m_attributes[i].input_slot;
            layout[i].AlignedByteOffset    = D3D12_APPEND_ALIGNED_ELEMENT;
            layout[i].InputSlotClass       = static_cast<D3D12_INPUT_CLASSIFICATION>(m_attributes[i].slot_type);
            layout[i].InstanceDataStepRate = m_attributes[i].step_rate;

            size_t count = m_attributes[i].format_count;
            auto   type  = m_attributes[i].format_type;

            Uint32 format;
            switch (count)
            {
            case 1:
                format = static_cast<Uint32>(DXGI_FORMAT_D32_FLOAT);
                break;
            case 2:
                format = static_cast<Uint32>(DXGI_FORMAT_R32G32_TYPELESS);
                break;
            case 3:
                format = static_cast<Uint32>(DXGI_FORMAT_R32G32B32_TYPELESS);
                break;
            case 4:
                format = static_cast<Uint32>(DXGI_FORMAT_R32G32B32A32_TYPELESS);
                break;
            default:
                format = static_cast<Uint32>(DXGI_FORMAT_UNKNOWN);
                break;
            }

            switch (type)
            {
            case eAttributeType::T32: //typeless
                if (count == 1)
                    format -= 1;
                break;
            case eAttributeType::R32:
                format += 1;
                break;
            case eAttributeType::U32:
                format += 2;
                break;
            case eAttributeType::I32:
                format += 3;
                break;
            default:
                break;
            }

            layout[i].Format = static_cast<DXGI_FORMAT>(format);
        }

        return true;
    }
}
