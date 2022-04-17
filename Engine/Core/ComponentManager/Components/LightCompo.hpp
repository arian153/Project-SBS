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
#include "../Component.hpp"
#include "../ComponentFactory.hpp"
#include "../../../Graphics/Element/Light.hpp"
#include "../../../Math/Structure/Transform.hpp"

namespace Engine
{
    class Model;

    class LightCompo final : public Component
    {
    public:
        LightCompo();
        ~LightCompo() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void CloneTo(RPtr<Component> destination) override;

        const LightInfo& GetLightInfo() const;
        LightInfo&       GetLightInfo();

        void SetLightDirection(const Vector3& direction);
        void SetDiffuse(const Color& diffuse);
        void SetAmbient(const Color& ambient);
        void SetSpecular(const Color& specular);
        void SetLightType(eLightType type);
        void SetLightRange(float range);
        void SetLightAngle(float angle);

        void DeferredBind(Sint32 light_index) const;

        Matrix44 GetWorldMatrix() const;

    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class LightFactory;

    private:
        LightInfo   m_light_info;
        SPtr<Model> m_model = nullptr;
        Transform   m_transform;
    };

    class LightFactory final : public ComponentFactory
    {
    public:
        LightFactory();
        ~LightFactory() override;

        RPtr<Component> Create() override;
    };
}
