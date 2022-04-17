/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "TransformCompo.hpp"

#include "../../../External/imgui/imgui.h"
#include "../../AppStateManager/AppState.hpp"
#include "../../ResourceManager/ResourceType/JsonData.hpp"

namespace Engine
{
    TransformCompo::TransformCompo()
    {
    }

    TransformCompo::~TransformCompo()
    {
    }

    void TransformCompo::Initialize()
    {
        Subscribe();
    }

    void TransformCompo::Update(Real dt)
    {
    }

    void TransformCompo::Shutdown()
    {
        Unsubscribe();
    }

    bool TransformCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void TransformCompo::Save(Json::Value& data) const
    {
    }

    void TransformCompo::Edit(CommandRegistry* command_registry)
    {
        if (ImGui::CollapsingHeader(MK_STRING(TransformCompo)))
        {
            ImGui::Text("Position");
            ImGui::InputFloat3("##Position", &m_transform.position.x);

            ImGui::Text("Scale");
            ImGui::InputFloat3("##Scale", &m_transform.scale.x);

            ImGui::Text("Orientation");

            ImGui::Text("Add Rotation");
            if (ImGui::Button("X + 90"))
            {
                AddRotationX(Math::HALF_PI);
            }
            ImGui::SameLine();
            if (ImGui::Button("Y + 90"))
            {
                AddRotationY(Math::HALF_PI);
            }
            ImGui::SameLine();
            if (ImGui::Button("Z + 90"))
            {
                AddRotationZ(Math::HALF_PI);
            }
            if (ImGui::Button("X - 90"))
            {
                AddRotationX(-Math::HALF_PI);
            }
            ImGui::SameLine();
            if (ImGui::Button("Y - 90"))
            {
                AddRotationY(-Math::HALF_PI);
            }
            ImGui::SameLine();
            if (ImGui::Button("Z - 90"))
            {
                AddRotationZ(-Math::HALF_PI);
            }

            ImGui::Text("Axis");
            if (ImGui::InputFloat3("##TransformEdit2", &m_axis_holder.axis.x))
            {
                m_axis_holder.axis.SetNormalize();
                SetOrientation(m_axis_holder);
            }

            ImGui::Text("Radian");
            if (ImGui::SliderAngle("##TransformEdit3", &m_axis_holder.radian))
            {
                SetOrientation(m_axis_holder);
            }

            ImGui::Text("Quaternion");
            if (ImGui::SliderFloat4("##TransformEdit4", &m_transform.orientation.r, -1.0f, 1.0f))
            {
                SetOrientation(m_transform.orientation);
            }

            Real degree = Math::RadiansToDegrees(m_axis_holder.radian);
            ImGui::Text("R : %.3f [cos(%.f)]", m_transform.orientation.r, degree);
            ImGui::Text("I : %.3f [sin(%.f) * %.3fi]", m_transform.orientation.i, degree, m_axis_holder.axis.x);
            ImGui::Text("J : %.3f [sin(%.f) * %.3fj]", m_transform.orientation.j, degree, m_axis_holder.axis.y);
            ImGui::Text("K : %.3f [sin(%.f) * %.3fk]", m_transform.orientation.k, degree, m_axis_holder.axis.z);
        }
    }

    void TransformCompo::CloneTo(RPtr<Component> destination)
    {
    }

    Matrix44 TransformCompo::GetLocalToWorldMatrix() const
    {
        return m_transform.LocalToWorldMatrix();
    }

    Matrix44 TransformCompo::GetWorldToLocalMatrix() const
    {
        return m_transform.LocalToWorldMatrix().Inverse();
    }

    Transform TransformCompo::GetTransform() const
    {
        return m_transform;
    }

    void TransformCompo::SetTransform(const Transform& transform)
    {
        m_transform = transform;
    }

    void TransformCompo::SetScale(const Vector3& scale)
    {
        m_transform.scale = scale;
    }

    void TransformCompo::SetPosition(const Vector3& pos)
    {
        m_transform.position = pos;
    }

    void TransformCompo::SetOrientation(const Quaternion& orientation)
    {
        m_transform.orientation = orientation;
        m_transform.orientation.SetNormalize();
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformCompo::SetOrientation(const AxisRadian& axis_radian)
    {
        m_axis_holder = axis_radian;
        m_transform.orientation.Set(axis_radian);
    }

    void TransformCompo::SetOrientation(const EulerAngle& rotation)
    {
        m_transform.orientation.Set(rotation);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformCompo::AddPosition(const Vector3& delta_position)
    {
        m_transform.position += delta_position;
    }

    void TransformCompo::AddRotation(const Quaternion& delta_rotation)
    {
        m_transform.orientation.AddRotation(delta_rotation);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformCompo::AddRotation(const AxisRadian& axis_radian)
    {
        m_transform.orientation.AddRotation(axis_radian.axis, axis_radian.radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformCompo::AddRotationX(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::X_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformCompo::AddRotationY(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::Y_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformCompo::AddRotationZ(Real radian)
    {
        m_transform.orientation.AddRotation(Math::Vector3::Z_AXIS, radian);
        m_axis_holder = m_transform.orientation.ToAxisRadian();
    }

    void TransformCompo::AddRotationA(Real radian)
    {
        m_axis_holder.radian += radian;
        m_transform.orientation.Set(m_axis_holder);
    }

    Vector3 TransformCompo::GetPosition() const
    {
        return m_transform.position;
    }

    Vector3 TransformCompo::GetScale() const
    {
        return m_transform.scale;
    }

    Quaternion TransformCompo::GetOrientation() const
    {
        return m_transform.orientation;
    }

    void TransformCompo::Subscribe()
    {
        m_space->GetBehaviorSubsystem()->AddTransform(this);
    }

    void TransformCompo::Unsubscribe()
    {
        m_space->GetBehaviorSubsystem()->RemoveTransform(this);
    }

    TransformFactory::TransformFactory()
    {
        m_type_name = typeid(TransformCompo).name();
        m_type_id   = typeid(TransformCompo).hash_code();
    }

    TransformFactory::~TransformFactory()
    {
    }

    RPtr<Component> TransformFactory::Create()
    {
        return new TransformCompo();
    }
}
