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
#include "../Resource.hpp"
#include "../../../Graphics/Data/MeshData.hpp"
#include "../../../Graphics/Element/Material.hpp"

// ReSharper disable CppInconsistentNaming
struct aiMesh;
struct aiScene;
struct aiNode;
// ReSharper restore CppInconsistentNaming

namespace Engine
{
    class ModelResource final : public Resource
    {
    public:
        explicit ModelResource(const String& path);
        ~ModelResource() override;

        MeshData&       GetMeshData(size_t i);
        const MeshData& GetMeshData(size_t i) const;

        TexturePaths&       GetPathData(size_t i);
        const TexturePaths& GetPathData(size_t i) const;

        size_t Count() const;

    private:
        void Initialize() override;
        void Shutdown() override;

        void Load();

        void                ProcessNode(aiNode* node, const aiScene* scene);
        static MeshData     ProcessMesh(aiMesh* mesh, const aiScene* scene);
        static TexturePaths ProcessMaterial(aiMesh* mesh, const aiScene* scene, const String& dir);

    private:
        friend class ResourceManager;
        String m_directory;

        std::vector<MeshData>     m_meshes;
        std::vector<TexturePaths> m_materials;
    };
}
