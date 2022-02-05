#pragma once
#include "../Resource.hpp"
#include "../../../Graphics/Data/MeshData.hpp"

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

        MeshData& GetMeshData(size_t i);
        const MeshData& GetMeshData(size_t i) const;

        MeshData& ReloadAndGetMeshData(size_t i);

        size_t Count() const;

    private:
        void Initialize() override;
        void Shutdown() override;

        void Load();

        void            ProcessNode(aiNode* node, const aiScene* scene);
        static MeshData ProcessMesh(aiMesh* mesh, const aiScene* scene);


    private:
        friend class ResourceManager;

        std::vector<MeshData> m_meshes;

    };
}
