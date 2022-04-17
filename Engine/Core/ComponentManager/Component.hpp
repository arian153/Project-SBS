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
#include "../../EngineDefine.hpp"

namespace Json
{
    class Value;
}

namespace Engine
{
    class CommandRegistry;
    class AppState;
    class Object;

    class Component
    {
    public:
        virtual ~Component();

        virtual void Initialize() = 0;
        virtual void Update(Real dt) = 0;
        virtual void Shutdown() = 0;
        virtual bool Load(const Json::Value& data) = 0;
        virtual void Save(Json::Value& data) const = 0;
        virtual void Edit(CommandRegistry* command_registry) = 0;
        virtual void CloneTo(RPtr<Component> destination) = 0;

        String       TypeName() const;
        size_t       TypeID() const;
        SPtr<Object> GetOwner() const;
        AppState*    GetSpace() const;

    protected:
        virtual void Subscribe() = 0;
        virtual void Unsubscribe() = 0;

    protected:
        SPtr<Object> m_owner = nullptr;
        AppState*    m_space = nullptr;
        String       m_type_name;
        size_t       m_type_id = 0;

    private:
        friend class ComponentManager;
        friend class Object;

    private:
        void SetSpace(AppState* space);
        void SetOwner(SPtr<Object> owner);
        void ClearOwner();
    };
}
