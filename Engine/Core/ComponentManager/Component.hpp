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
        virtual void CloneTo(Component* destination) = 0;

        String       Type() const;
        SPtr<Object> GetOwner() const;
        AppState*    GetSpace() const;

    protected:
        virtual void Subscribe() = 0;
        virtual void Unsubscribe() = 0;

    protected:
        SPtr<Object> m_owner = nullptr;
        AppState*    m_space = nullptr;
        String       m_type;

    private:
        friend class ComponentManager;
        friend class Object;

    private:
        void SetSpace(AppState* space);
        void SetOwner(SPtr<Object> owner);
        void ClearOwner();
    };
}
