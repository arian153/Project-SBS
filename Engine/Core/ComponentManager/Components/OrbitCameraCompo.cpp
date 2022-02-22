#include "OrbitCameraCompo.hpp"

#include "TransformCompo.hpp"
#include "../../CoreDefine.hpp"
#include "../../../External/imgui/imgui.h"
#include "../../../External/JSONCPP/json/json.h"
#include "../../../Graphics/Element/Camera.hpp"
#include "../../../Math/Utility/Utility.hpp"
#include "../../AppStateManager/AppState.hpp"
#include "../../ObjectManager/Object.hpp"
#include "../../Win32Manager/KeyCode.hpp"

namespace Engine
{
    OrbitCameraCompo::OrbitCameraCompo()
    {
    }

    OrbitCameraCompo::~OrbitCameraCompo()
    {
    }

    void OrbitCameraCompo::Initialize()
    {
        Subscribe();
        // Restrict the angle phi.
        m_phi = Math::Clamp(m_phi, 0.1f, Math::PI - 0.1f);
        // Convert Spherical to Cartesian coordinates.
        m_eye_pos.x = m_radius * sinf(m_phi) * cosf(m_theta);
        m_eye_pos.z = m_radius * sinf(m_phi) * sinf(m_theta);
        m_eye_pos.y = m_radius * cosf(m_phi);
        m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
    }

    void OrbitCameraCompo::Update(Real dt)
    {
        if (m_owner->HasComponent<TransformCompo>())
        {
            m_owner->GetComponent<TransformCompo>()->SetTransform(m_camera->GetTransform());
        }

        int   curr_x = INPUT_MANAGER->CurrentPosition().x;
        int   curr_y = INPUT_MANAGER->CurrentPosition().y;
        int   prev_x = INPUT_MANAGER->PreviousPosition().x;
        int   prev_y = INPUT_MANAGER->PreviousPosition().y;
        Real  dx     = static_cast<Real>(curr_x - prev_x) * dt;
        Real  dy     = static_cast<Real>(curr_y - prev_y) * dt;
        Basis basis  = m_camera->GetBasis();
        if (INPUT_MANAGER->IsDown(eKeyCodeMouse::Right))
        {
            //camera->AddDistanceInUpDirection(-dt * dy);
            //camera->AddDistanceInRightDirection(dt * dx);
            m_theta += dx;
            m_phi += dy;
            // Restrict the angle phi.
            m_phi = Math::Clamp(m_phi, 0.1f, Math::PI - 0.1f);
            // Convert Spherical to Cartesian coordinates.
            m_eye_pos.x = m_radius * sinf(m_phi) * cosf(m_theta);
            m_eye_pos.z = m_radius * sinf(m_phi) * sinf(m_theta);
            m_eye_pos.y = m_radius * cosf(m_phi);
            m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
        }
        if (INPUT_MANAGER->IsWheelRolling())
        {
            m_camera->AddPosition(basis.k * dt * 20.0f * INPUT_MANAGER->MouseWheelRollingDirection());

            // Update angles based on input to orbit camera around box.
            m_radius += 2.0f * INPUT_MANAGER->MouseWheelRollingDirection();
            // Restrict the radius.
            m_radius = Math::Clamp(m_radius, 5.0f, 150.0f);
            // Convert Spherical to Cartesian coordinates.
            m_eye_pos.x = m_radius * sinf(m_phi) * cosf(m_theta);
            m_eye_pos.z = m_radius * sinf(m_phi) * sinf(m_theta);
            m_eye_pos.y = m_radius * cosf(m_phi);
            m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::R))
        {
            m_target_pos += basis.j * dt * 10.0f;
            m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::F))
        {
            m_target_pos += basis.j * -dt * 10.0f;
            m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::A))
        {
            m_target_pos += basis.i * -dt * 10.0f;
            m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::D))
        {
            m_target_pos += basis.i * dt * 10.0f;
            m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::W))
        {
            m_target_pos += basis.k * dt * 10.0f;
            m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
        }
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::S))
        {
            m_target_pos += basis.k * -dt * 10.0f;
            m_camera->LookAt(m_target_pos + m_eye_pos, m_target_pos);
        }

          }

    void OrbitCameraCompo::Shutdown()
    {
        Unsubscribe();
    }

    bool OrbitCameraCompo::Load(const Json::Value& data)
    {
        if (data.isNull())
            return false;

        return true;
    }

    void OrbitCameraCompo::Save(Json::Value& data) const
    {
    }

    void OrbitCameraCompo::Edit(CommandRegistry* command_registry)
    {
        ImGui::CollapsingHeader(MK_STRING(OrbitCameraCompo));
    }

    void OrbitCameraCompo::CloneTo(RPtr<Component> destination)
    {
    }

    void OrbitCameraCompo::SetAsMainCamera() const
    {
        m_space->GetRenderSubsystem()->SetCurrentCamera(m_camera);
    }

    void OrbitCameraCompo::Subscribe()
    {
        m_camera = m_space->GetRenderSubsystem()->CreateCamera();
        m_space->GetRenderSubsystem()->AddOrbitCompo(this);
    }

    void OrbitCameraCompo::Unsubscribe()
    {
        m_space->GetRenderSubsystem()->RemoveOrbitCompo(this);
    }

    OrbitCameraFactory::OrbitCameraFactory()
    {
        m_type_name = typeid(OrbitCameraCompo).name();
        m_type_id   = typeid(OrbitCameraCompo).hash_code();
    }

    OrbitCameraFactory::~OrbitCameraFactory()
    {
    }

    RPtr<Component> OrbitCameraFactory::Create()
    {
        return new OrbitCameraCompo();
    }
}
