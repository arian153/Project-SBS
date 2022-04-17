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
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using Sint8 = __int8;
using Sint16 = __int16;
using Sint32 = __int32;
using Sint64 = __int64;
using Uint8 = unsigned __int8;
using Uint16 = unsigned __int16;
using Uint32 = unsigned __int32;
using Uint64 = unsigned __int64;

using Real = float;
using Real32 = float;
using Real64 = double;

using String = std::string;
using StringWide = std::wstring;

template <typename T>
using UPtr = std::unique_ptr<T>;

template <typename T>
using SPtr = std::shared_ptr<T>;

template <typename T>
using WPtr = std::weak_ptr<T>;

template <typename T>
using RPtr = T*;

template <typename K, typename T>
using HashMap = std::unordered_map<K, T>;

template <typename K, typename T>
using HashMulMap = std::unordered_multimap<K, T>;


namespace Engine
{
    //global core
    extern std::unique_ptr<class CoreSystem> g_core;

    extern CoreSystem* GetCore();
}
