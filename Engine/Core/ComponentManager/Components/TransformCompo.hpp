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
#include "../../../Math/Structure/Transform.hpp"

namespace Engine
{
    class TransformCompo final : public Component
    {
    public:
        TransformCompo();
        ~TransformCompo() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void CloneTo(RPtr<Component> destination) override;

        Matrix44 GetLocalToWorldMatrix() const;
        Matrix44 GetWorldToLocalMatrix() const;

        Transform GetTransform() const;

        void SetTransform(const Transform& transform);

        void SetScale(const Vector3& scale);
        void SetPosition(const Vector3& pos);
        void SetOrientation(const Quaternion& orientation);
        void SetOrientation(const AxisRadian& axis_radian);
        void SetOrientation(const EulerAngle& rotation);


        void AddPosition(const Vector3& delta_position);
        void AddRotation(const Quaternion& delta_rotation);
        void AddRotation(const AxisRadian& axis_radian);
        void AddRotationX(Real radian);
        void AddRotationY(Real radian);
        void AddRotationZ(Real radian);
        void AddRotationA(Real radian);

        Vector3    GetPosition() const;
        Vector3 GetScale()const;
        Quaternion GetOrientation() const;

    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class TransformFactory;

    private:
        Transform  m_transform;
        AxisRadian m_axis_holder;
    };

    class TransformFactory final : public ComponentFactory
    {
    public:
        TransformFactory();
        ~TransformFactory() override;

        RPtr<Component> Create() override;
    };
}
