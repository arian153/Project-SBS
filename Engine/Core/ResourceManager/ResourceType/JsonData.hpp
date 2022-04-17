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
#include <fstream>

#include "../../../EngineDefine.hpp"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../Graphics/Data/Color.hpp"
#include "../../../Math/MathIncludes.h"

namespace Engine
{
    class JsonData
    {
    public:
        static bool IsVector2(const Json::Value& value);
        static bool IsVector3(const Json::Value& value);
        static bool IsVector4(const Json::Value& value);
        static bool IsQuaternion(const Json::Value& value);
        static bool IsColor(const Json::Value& value);
        static bool HasMember(const Json::Value& value, const String& find);

        static Vector2    AsVector2(const Json::Value& value);
        static Vector3    AsVector3(const Json::Value& data);
        static Vector4    AsVector4(const Json::Value& data);
        static Quaternion AsQuaternion(const Json::Value& data);
        static Color      AsColor(const Json::Value& data);

        static void SetVector2(Json::Value& value, const Vector2& data);
        static void SetVector3(Json::Value& value, const Vector3& data);
        static void SetVector4(Json::Value& value, const Vector4& data);
        static void SetQuaternion(Json::Value& value, const Quaternion& data);
        static void SetColor(Json::Value& value, const Color& data);

        static bool WriteJsonFile(const Json::Value& value, const String& path);
    };

    inline bool JsonData::IsVector2(const Json::Value& value)
    {
        return value.isArray() &&
                value.size() == 2 &&
                value[0].isNumeric() &&
                value[1].isNumeric();
    }

    inline bool JsonData::IsVector3(const Json::Value& value)
    {
        return value.isArray() &&
                value.size() == 3 &&
                value[0].isNumeric() &&
                value[1].isNumeric() &&
                value[2].isNumeric();
    }

    inline bool JsonData::IsVector4(const Json::Value& value)
    {
        return value.isArray() &&
                value.size() == 4 &&
                value[0].isNumeric() &&
                value[1].isNumeric() &&
                value[2].isNumeric() &&
                value[3].isNumeric();
    }

    inline bool JsonData::IsQuaternion(const Json::Value& value)
    {
        return value.isArray() &&
                value.size() == 4 &&
                value[0].isNumeric() &&
                value[1].isNumeric() &&
                value[2].isNumeric() &&
                value[3].isNumeric();
    }

    inline bool JsonData::IsColor(const Json::Value& value)
    {
        return value.isArray() &&
                value.size() == 4 &&
                value[0].isNumeric() &&
                value[1].isNumeric() &&
                value[2].isNumeric() &&
                value[3].isNumeric();
    }

    inline bool JsonData::HasMember(const Json::Value& value, const String& find)
    {
        return !value[find].isNull();
    }

    inline Vector2 JsonData::AsVector2(const Json::Value& value)
    {
        return Vector2(value[0].asFloat(), value[1].asFloat());
    }

    inline Vector3 JsonData::AsVector3(const Json::Value& data)
    {
        return Vector3(data[0].asFloat(), data[1].asFloat(), data[2].asFloat());
    }

    inline Vector4 JsonData::AsVector4(const Json::Value& data)
    {
        return Vector4(data[0].asFloat(), data[1].asFloat(), data[2].asFloat(), data[3].asFloat());
    }

    inline Quaternion JsonData::AsQuaternion(const Json::Value& data)
    {
        return Quaternion(data[0].asFloat(), data[1].asFloat(), data[2].asFloat(), data[3].asFloat());
    }

    inline Color JsonData::AsColor(const Json::Value& data)
    {
        return Color(data[0].asFloat(), data[1].asFloat(), data[2].asFloat(), data[3].asFloat());
    }

    inline void JsonData::SetVector2(Json::Value& value, const Vector2& data)
    {
        value.append(data.x);
        value.append(data.y);
    }

    inline void JsonData::SetVector3(Json::Value& value, const Vector3& data)
    {
        value.append(data.x);
        value.append(data.y);
        value.append(data.z);
    }

    inline void JsonData::SetVector4(Json::Value& value, const Vector4& data)
    {
        value.append(data.x);
        value.append(data.y);
        value.append(data.z);
        value.append(data.w);
    }

    inline void JsonData::SetQuaternion(Json::Value& value, const Quaternion& data)
    {
        value.append(data.r);
        value.append(data.i);
        value.append(data.j);
        value.append(data.k);
    }

    inline void JsonData::SetColor(Json::Value& value, const Color& data)
    {
        value.append(data.r);
        value.append(data.g);
        value.append(data.b);
        value.append(data.a);
    }

    inline bool JsonData::WriteJsonFile(const Json::Value& value, const String& path)
    {
        Json::StreamWriterBuilder builder;
        const std::string         json_file = Json::writeString(builder, value);

        std::ofstream out(path);

        if (out.is_open())
        {
            out << json_file;
            out.close();
            return true;
        }

        out.close();
        return false;
    }
}
