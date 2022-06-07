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
#include "../../../Graphics/Data/Color.hpp"
#include "../../../Math/Algebra/Matrix44.hpp"

namespace Engine
{
    class Material;
    class Texture;
    class ShaderProgram;
    class MeshData;
    class ModelResource;
    class Model;

    enum eMaterialInfoInt
    {
        ProcessInstancedColor = 0,
        ProcessDiffuseMaterial = 1,
        ProcessNormalMapMaterial = 2,
        ProcessSpecularMaterial = 3
    };

    enum eProcessMaterialInfo
    {
        SkipProcessing = 0
    };

    enum eProcessDiffuseColor
    {
        HasInstancedColor = 1
    };

    enum eProcessDiffuseMaterial
    {
        SampleDiffuseTexture = 1,
        SampleAlphaMapTexture = 2,
        SampleLightMapTexture = 3,
        SampleMultipleTexture = 4
    };

    enum eProcessNormalMapMaterial
    {
        SampleNormalMapTexture = 1
    };

    enum eProcessSpecularMaterial
    {
        SampleSpecularTexture = 1
    };

    class MeshCompo final : public Component
    {
    public:
        MeshCompo();
        ~MeshCompo() override;

        void Initialize() override;
        void Update(Real dt) override;
        void Shutdown() override;
        bool Load(const Json::Value& data) override;
        void Save(Json::Value& data) const override;
        void Edit(CommandRegistry* command_registry) override;
        void CloneTo(RPtr<Component> destination) override;

        void CreateModel(const String& name);

        void SetModelResource(RPtr<ModelResource> resource);
        void SetMaterial(SPtr<Material> material) const;
        void SetMaterialInfoReal(Uint32 index, Real value) const;
        void SetMaterialInfo(Uint32 index, Sint32 value) const;
        void SetMaterialInfo(eMaterialInfoInt info_type, Sint32 value) const;
        void SetMaterialTexture(Uint32 index, SPtr<Texture> texture) const;
        void SetDiffuseTexture(Uint32 index, SPtr<Texture> texture) const;
        void SetSpecularMapTexture(SPtr<Texture> texture) const;
        void SetNormalMapTexture(SPtr<Texture> texture) const;
        void SetMappingTexture(SPtr<Texture> texture) const;
        void SetCubeMapTexture(SPtr<Texture> texture) const;
        void SetMeshData(const MeshData& mesh_data) const;
        void SetShader(SPtr<ShaderProgram> shader);
        void SetColor(const Color& color);

        void     Render() const;
        Matrix44 GetWorldMatrix() const;
        bool     IsDeferred() const;

        void UpdateInstanceData(const Matrix44& view, const Matrix44& proj) const;

    protected:
        void Subscribe() override;
        void Unsubscribe() override;

    private:
        friend class MeshFactory;

    private:
        bool  m_b_deferred = false;
        Color m_diffuse_color;

        SPtr<Model> m_model = nullptr;

        RPtr<ModelResource> m_model_resource = nullptr;
    };

    class MeshFactory final : public ComponentFactory
    {
    public:
        MeshFactory();
        ~MeshFactory() override;

        RPtr<Component> Create() override;
    };
}
