/*---------------------------------------------------------------------
//All content (C) 2022 DigiPen (USA) Corporation, all rights reserved
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
Author: Eric Han
E-Mail: jangseok.han@digipen.edu
Language: C++, std::c++17
Platform: Visual Studio 2019 (v142), Windows SDK 10.0 x64>
-----------------------------------------------------------------------*/

#include "ModelResource.hpp"

#include "../../../Utility/FileUtility.hpp"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Engine
{
    ModelResource::ModelResource(const String& path)
        : Resource(path)
    {
        m_type = eResourceType::Model;
    }

    ModelResource::~ModelResource()
    {
    }

    MeshData& ModelResource::GetMeshData(size_t i)
    {
        return m_meshes[i];
    }

    const MeshData& ModelResource::GetMeshData(size_t i) const
    {
        return m_meshes[i];
    }

    TexturePaths& ModelResource::GetPathData(size_t i)
    {
        return m_materials[i];
    }

    const TexturePaths& ModelResource::GetPathData(size_t i) const
    {
        return m_materials[i];
    }

    size_t ModelResource::Count() const
    {
        return m_meshes.size();
    }

    void ModelResource::Initialize()
    {
        Load();
    }

    void ModelResource::Shutdown()
    {
    }

    void ModelResource::Load()
    {
        m_directory = FileUtility::GetDirectory(m_file_path);

        Assimp::Importer importer;
        const aiScene*   ai_scene = importer.ReadFile(
                                                      m_file_path,
                                                      aiProcess_Triangulate | aiProcess_ConvertToLeftHanded |
                                                      aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

        if (ai_scene == nullptr)
        {
            m_b_loaded = false;
            return;
        }

        ProcessNode(ai_scene->mRootNode, ai_scene);

        m_b_loaded = true;
    }

    void ModelResource::ProcessNode(aiNode* node, const aiScene* scene)
    {
        for (Uint32 i = 0; i < node->mNumMeshes; i++)
        {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(ProcessMesh(mesh, scene));
            m_materials.push_back(ProcessMaterial(mesh, scene, m_directory));
        }

        for (Uint32 i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    MeshData ModelResource::ProcessMesh(aiMesh* mesh, [[maybe_unused]] const aiScene* scene)
    {
        // Data to fill
        MeshData mesh_data;
        mesh_data.vertices.resize(mesh->mNumVertices);

        //Get vertices
        for (Uint32 i = 0; i < mesh->mNumVertices; i++)
        {
            mesh_data.vertices[i].pos.x = mesh->mVertices[i].x;
            mesh_data.vertices[i].pos.y = mesh->mVertices[i].y;
            mesh_data.vertices[i].pos.z = mesh->mVertices[i].z;

            if (mesh->HasNormals())
            {
                mesh_data.vertices[i].n.x = mesh->mNormals[i].x;
                mesh_data.vertices[i].n.y = mesh->mNormals[i].y;
                mesh_data.vertices[i].n.z = mesh->mNormals[i].z;
            }

            if (mesh->mTextureCoords[0])
            {
                mesh_data.vertices[i].tex.x = mesh->mTextureCoords[0][i].x;
                mesh_data.vertices[i].tex.y = mesh->mTextureCoords[0][i].y;
                if (mesh->HasTangentsAndBitangents())
                {
                    mesh_data.vertices[i].t.x = mesh->mTangents[i].x;
                    mesh_data.vertices[i].t.y = mesh->mTangents[i].y;
                    mesh_data.vertices[i].t.z = mesh->mTangents[i].z;
                }
            }
            else
            {
                mesh_data.vertices[i].tex = Vector2(0, 0);
            }
        }

        //Get indices
        size_t count = mesh->mNumFaces;
        mesh_data.faces.resize(count);
        mesh_data.indices.resize(count * 3);

        for (size_t i = 0; i < count; i++)
        {
            aiFace& face = mesh->mFaces[i];

            mesh_data.indices[3 * i]     = mesh_data.faces[i].a = face.mIndices[0];
            mesh_data.indices[3 * i + 1] = mesh_data.faces[i].b = face.mIndices[1];
            mesh_data.indices[3 * i + 2] = mesh_data.faces[i].c = face.mIndices[2];
        }

        // Process Animation, Bone

        return mesh_data;
    }

    TexturePaths ModelResource::ProcessMaterial(aiMesh* mesh, const aiScene* scene, const String& dir)
    {
        TexturePaths material_path;
        aiMaterial*  material = scene->mMaterials[mesh->mMaterialIndex];

        for (Uint32 i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE); i++)
        {
            aiString ai_path;
            material->GetTexture(aiTextureType_DIFFUSE, i, &ai_path);
            String path = ai_path.C_Str();
            material_path.diff.push_back(dir + "/" + path);
        }

        for (Uint32 i = 0; i < material->GetTextureCount(aiTextureType_SPECULAR); i++)
        {
            aiString ai_path;
            material->GetTexture(aiTextureType_SPECULAR, i, &ai_path);
            String path = ai_path.C_Str();
            material_path.spec.push_back(dir + "/" + path);
        }

        //bump map, normal map...
        for (Uint32 i = 0; i < material->GetTextureCount(aiTextureType_HEIGHT); i++)
        {
            aiString ai_path;
            material->GetTexture(aiTextureType_HEIGHT, i, &ai_path);
            String path = ai_path.C_Str();
            material_path.norm.push_back(dir + "/" + path);
        }

        return material_path;
    }
}
