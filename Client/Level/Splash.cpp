/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "Splash.h"

#include <iostream>
#include <Core/ComponentManager/Components/LightCompo.hpp>
#include <Core/ComponentManager/Components/OrbitCameraCompo.hpp>
#include <Core/ComponentManager/Components/SoftBodyCompo.hpp>
#include <Core/ObjectManager/Object.hpp>
using namespace Engine;

namespace
{
    constexpr char JSON_CPP_LICENSE[] = R"(
========================================================================
Copyright (c) 2007-2010 Baptiste Lepilleur and The JsonCpp Authors

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use, copy,
modify, merge, publish, distribute, sublicense, and/or sell copies
of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
========================================================================
    )";

    constexpr char DX_TEX_LICENSE[] = R"(
==========================================================================================
                               The MIT License (MIT)

Copyright (c) 2011-2020 Microsoft Corp

Permission is hereby granted, free of charge, to any person obtaining a copy of this 
software and associated documentation files (the "Software"), to deal in the Software 
without restriction, including without limitation the rights to use, copy, modify, 
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to 
permit persons to whom the Software is furnished to do so, subject to the following 
conditions: 

The above copyright notice and this permission notice shall be included in all copies 
or substantial portions of the Software.  

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, 
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A 
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF 
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE 
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

==========================================================================================
    )";

    constexpr char IMGUI_LICENSE[] = R"(
--------------------------------------------------------------------------------------
 The MIT License (MIT)
 
 Copyright (c) 2014-2020 Omar Cornut
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 
--------------------------------------------------------------------------------------
    )";

    constexpr char ASSIMP_LICENSE[] = R"(
--------------------------------------------------------------------------------------
Open Asset Import Library (assimp)

Copyright (c) 2006-2021, assimp team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above
  copyright notice, this list of conditions and the
  following disclaimer.

* Redistributions in binary form must reproduce the above
  copyright notice, this list of conditions and the
  following disclaimer in the documentation and/or other
  materials provided with the distribution.

* Neither the name of the assimp team, nor the names of its
  contributors may be used to endorse or promote products
  derived from this software without specific prior
  written permission of the assimp team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--------------------------------------------------------------------------------------
    )";
}

namespace Client
{
    Splash::Splash()
    {
        SetDefaultSubsystems();
    }

    Splash::~Splash()
    {
    }

    void Splash::Initialize()
    {
        {
            auto obj       = m_object_manager->AddObject("Splash-Screen");
            auto transform = obj->AddComponent<TransformCompo>();
            transform->SetScale(Vector3(15.0f, 3.62f, 1.0f));

            auto mesh = obj->AddComponent<MeshCompo>();
            mesh->CreateModel("Splash-Screen");
            mesh->SetMeshData(MeshDataGenerator::CreateRectangle(3.f, 3.f));
            mesh->SetShader(GET_SHADER("Deferred-Geometry.shader"));
            mesh->SetMaterialTexture(0, RESOURCE_MANAGER->GetTextureResourceName("Credit.jpg")->GetTexture());
            mesh->SetMaterialInfo(ProcessInstancedColor, SkipProcessing);
            mesh->SetMaterialInfo(ProcessDiffuseMaterial, SampleDiffuseTexture);
            mesh->SetMaterialInfo(ProcessNormalMapMaterial, SkipProcessing);
        }

        //Camera Object
        {
            auto obj = m_object_manager->AddObject("camera");
            obj->AddComponent<TransformCompo>();
            auto camera = obj->AddComponent<OrbitCameraCompo>();
            camera->SetAsMainCamera();
        }

        //Light 1  Directional Light
        {
            auto obj = m_object_manager->AddObject("Directional");
            obj->AddComponent<TransformCompo>();
            auto light = obj->AddComponent<LightCompo>();
            light->SetLightDirection(Vector3(1.f, 0.f, 1.f).Normalize());
            light->SetLightType(eLightType::DirectionalLight);
            light->SetDiffuse(Color(1.f, 1.f, 1.f, 1.0f));
            light->SetAmbient(Color(0.1f, 0.1f, 0.1f, 1.0f));
            light->SetSpecular(Color(0.1f, 0.1f, 0.1f, 1.0f));
        }
    }

    void Splash::Update(float dt)
    {
        if (INPUT_MANAGER->IsDown(eKeyCodeKeyboard::Escape))
        {
            WIN32_MANAGER->SetQuit(true);
        }

        if (ImGui::Begin("Scene State Manager"))
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("Scene", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

            ImGui::EndChild();
            if (ImGui::Button("Close"))
            {
                APP_STATE_MANAGER->ResumeState();
            }
            ImGui::EndGroup();
            ImGui::End();
        }

        if (m_show_about)
        {
            if (ImGui::Begin("About", &m_show_about))
            {
                ImGui::BeginGroup();
                ImGui::BeginChild("About", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

                ImGui::Text("Soft Body Simulation");
                ImGui::Text("Version 1.0.04162022");
                ImGui::Text(R"(All content © 2022 DigiPen (USA) Corporation, all rights reserved.)");

                ImGui::EndChild();
                if (ImGui::Button("Close"))
                {
                    m_show_about = false;
                }
                ImGui::EndGroup();
                ImGui::End();
            }
        }

        if (m_show_credit)
        {
            if (ImGui::Begin("Credit", &m_show_credit))
            {
                ImGui::BeginGroup();
                ImGui::BeginChild("Credit", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

                ImGui::Text("Credit");
                ImGui::NewLine();
                ImGui::NewLine();
                ImGui::Text("Tech Lead");
                ImGui::Text(" Eric Han");
                ImGui::NewLine();
                ImGui::Text("Graphics Programming");
                ImGui::Text(" Eric Han");
                ImGui::NewLine();
                ImGui::Text("Tool Programming");
                ImGui::Text(" Eric Han");
                ImGui::NewLine();

                ImGui::Text("Faculty & Advisors");
                ImGui::Text(" Andrew Kaplan");
                ImGui::Text(" Jen Sward");
                ImGui::Text(" Kai Tagawa");
                ImGui::NewLine();

                ImGui::Text("Created at");
                ImGui::Text(" DigiPen Institute of Technology");
                ImGui::NewLine();

                ImGui::Text("PRESIDENT");
                ImGui::Text(" CLAUDE COMAIR");
                ImGui::NewLine();

                ImGui::Text("EXECUTIVES");
                ImGui::Text(" JASON CHU");
                ImGui::Text(" SAMIR ABOU SAMRA   ");
                ImGui::Text(" MICHELE COMAIR");
                ImGui::Text(" ANGELA KUGLER   ");
                ImGui::Text(" ERIK MOHRMANN");
                ImGui::Text(" BENJAMIN ELLINGER");
                ImGui::Text(" MELVIN GONSALVEZ");
                ImGui::NewLine();

                ImGui::Text(R"(All content © 2022 DigiPen (USA) Corporation, all rights reserved.)");

                ImGui::EndChild();
                if (ImGui::Button("Close"))
                {
                    m_show_credit = false;
                }
                ImGui::EndGroup();
                ImGui::End();
            }
        }

        if (m_show_license)
        {
            if (ImGui::Begin("License", &m_show_license))
            {
                ImGui::BeginGroup();
                ImGui::BeginChild("License", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));

                ImGui::Text("Open Source License");

                if (ImGui::CollapsingHeader("Assimp"))
                {
                    ImGui::Text(ASSIMP_LICENSE);
                }

                if (ImGui::CollapsingHeader("jsoncpp"))
                {
                    ImGui::Text(JSON_CPP_LICENSE);
                }
                if (ImGui::CollapsingHeader("DirectXTex texture processing library"))
                {
                    ImGui::Text(DX_TEX_LICENSE);
                }
                if (ImGui::CollapsingHeader("Dear ImGui"))
                {
                    ImGui::Text(IMGUI_LICENSE);
                }

                ImGui::EndChild();
                if (ImGui::Button("Close"))
                {
                    m_show_license = false;
                }
                ImGui::EndGroup();

                ImGui::End();
            }
        }
    }

    void Splash::FixedUpdate(float dt)
    {
    }

    void Splash::Render()
    {
    }

    void Splash::Shutdown()
    {
    }

    SplashFactory::~SplashFactory()
    {
    }

    Engine::AppState* SplashFactory::Create()
    {
        return new Splash();
    }
}
