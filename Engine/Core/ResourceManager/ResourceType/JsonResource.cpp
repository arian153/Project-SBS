#include "JsonResource.hpp"

namespace Engine
{
    JsonResource::JsonResource(const String& path)
        : Resource(path)
    {
        m_type = eResourceType::Json;
    }

    JsonResource::~JsonResource()
    {
    }

    void JsonResource::Initialize()
    {
    }

    void JsonResource::Shutdown()
    {
        m_b_unloaded = true;
    }

    void JsonResource::Load()
    {
        m_b_loaded = true;
    }
}
