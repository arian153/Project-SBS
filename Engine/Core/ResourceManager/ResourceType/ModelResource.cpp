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

    MeshData& ModelResource::ReloadAndGetMeshData(size_t i)
    {
        Load();
        return m_meshes[i];
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
        }

        for (Uint32 i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(node->mChildren[i], scene);
        }
    }

    MeshData ModelResource::ProcessMesh(aiMesh* mesh, const aiScene* scene)
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
                    mesh_data.vertices[i].b.x = mesh->mBitangents[i].x;
                    mesh_data.vertices[i].b.y = mesh->mBitangents[i].y;
                    mesh_data.vertices[i].b.z = mesh->mBitangents[i].z;

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

        // Process Material

        // Process Animation, Bone

        return mesh_data;
    }
}
