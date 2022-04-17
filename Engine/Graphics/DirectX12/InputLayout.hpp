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
#include <d3d12.h>

#include "../../EngineDefine.hpp"

namespace Engine
{
    enum class eAttributeType : int
    {
        T32, //Typeless
        R32, //Real
        U32, //Unsigned
        I32  //Integer
    };

    enum class eInputSlotType : int
    {
        VertexData,
        InstanceData
    };

    class VertexAttribute
    {
    public:
        VertexAttribute()  = default;
        ~VertexAttribute() = default;

        static Uint32 SizeOf(eAttributeType type);

    public:
        String sem_name     = "";
        Uint32 sem_idx      = 0;
        Uint32 input_slot   = 0;
        Uint32 step_rate    = 0;
        Uint32 format_count = 0;

        eInputSlotType slot_type   = eInputSlotType::VertexData;
        eAttributeType format_type = eAttributeType::R32;
    };

    class InputLayout
    {
    public:
        InputLayout()  = default;
        ~InputLayout() = default;

        void PushAttribute(const VertexAttribute& attribute);
        void PushAttribute(eAttributeType type, Uint32 count, const String& semantic_name, Uint32 semantic_idx);
        void PushAttribute(eAttributeType type, Uint32 count, const String& semantic_name, Uint32 semantic_idx, eInputSlotType slot_type, Uint32 input_slot, Uint32 step_rate);

        void PushLayout(const InputLayout& layout);

        void   Clear();
        Uint32 GetStride() const;

        std::vector<VertexAttribute>&       GetAttributes();
        const std::vector<VertexAttribute>& GetAttributes() const;

        bool GetLayoutDesc(std::vector<D3D12_INPUT_ELEMENT_DESC>& layout) const;

    protected:
        std::vector<VertexAttribute> m_attributes;
        Uint32                       m_stride = 0;
    };
}
