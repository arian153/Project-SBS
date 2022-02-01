#pragma once
#include "../EngineDefine.hpp"

namespace Engine
{
    StringWide ToWString(const String& str);
    String     ToString(const StringWide& wstr);

    #define MK_STRING(s) #s
}
