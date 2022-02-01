#include "ModelResource.hpp"

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

    void ModelResource::Initialize()
    {
    }

    void ModelResource::Shutdown()
    {
    }

    void ModelResource::Load()
    {
        m_b_loaded = true;
    }
}
