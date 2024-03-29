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

#include "CoreSystem.hpp"
#include "Win32Manager/InputManager.hpp"
#include "Win32Manager/KeyCode.hpp"
#include "Win32Manager/Win32Manager.hpp"

#include "AppStateManager/AppState.hpp"
#include "AppStateManager/AppStateFactory.hpp"
#include "AppStateManager/AppStateManager.hpp"
#include "AppStateManager/DefaultAppState.hpp"
#include "AppStateManager/SubsystemFlag.hpp"

#include "ResourceManager/ResourceManager.hpp"
#include "ResourceManager/ResourceType/JsonResource.hpp"
#include "ResourceManager/ResourceType/ModelResource.hpp"
#include "ResourceManager/ResourceType/ShaderResource.hpp"
#include "ResourceManager/ResourceType/TextureResource.hpp"

#include "ComponentManager/ComponentFactory.hpp"
#include "ComponentManager/ComponentManager.hpp"
#include "ComponentManager/ComponentRegistry.hpp"

#include "ComponentManager/Components/MeshCompo.hpp"
#include "ComponentManager/Components/TransformCompo.hpp"
#include "ComponentManager/Components/LightCompo.hpp"
#include "ComponentManager/Components/OrbitCameraCompo.hpp"
#include "ComponentManager/Components/RigidBodyCompo.hpp"

#include "ObjectManager/Object.hpp"
#include "ObjectManager/ObjectManager.hpp"

#include "CoreDefine.hpp"
