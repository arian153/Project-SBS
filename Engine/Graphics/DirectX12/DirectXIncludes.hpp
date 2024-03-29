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

#include "d3dx12.h"

#include <comdef.h>
#include <d3d12.h>
#include <d3dcompiler.h>
#include <dxgi1_4.h>
#include <wrl.h>

#include <DirectXMath.h>
#include <DirectXPackedVector.h>

#include <DirectXTex.h>

#include "../../Utility/Utility.hpp"

using namespace DirectX;
using namespace DirectX::PackedVector;
using namespace Microsoft::WRL;

#ifndef RELEASE_COM
#define RELEASE_COM(x) { if(x){ x->Release(); x = nullptr; } }
#endif

namespace Engine
{
    enum class eCBVRegister : Uint32
    {
        b0,
        b1,
        b2,
        b3,
        End
    };

    enum class eSRVRegister : Uint32
    {
        t0 = static_cast<Uint32>(eCBVRegister::End),
        t1,
        t2,
        t3,
        t4,
        t5,
        t6,
        t7,
        End
    };

    constexpr Uint32 SWAP_CHAIN_BUFFER_COUNT = 3;
    constexpr Uint32 CBV_REGISTER_COUNT      = static_cast<Uint32>(eCBVRegister::End);
    constexpr Uint32 SRV_REGISTER_COUNT      = static_cast<Uint32>(eSRVRegister::End) - CBV_REGISTER_COUNT;
    constexpr Uint32 REGISTER_COUNT          = CBV_REGISTER_COUNT + SRV_REGISTER_COUNT;
}
