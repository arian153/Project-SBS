#pragma once
#include "../Resource.hpp"

namespace Engine
{
    class JsonResource final : public Resource
    {
    public:
        explicit JsonResource(const String& path);
        ~JsonResource() override;

    private:
        void Initialize() override;
        void Shutdown() override;

        void Load();

    private:
        friend class ResourceManager;
    };
}
