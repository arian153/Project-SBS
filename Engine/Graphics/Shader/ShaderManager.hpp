#pragma once
#include "../../System.hpp"

namespace Engine
{
    class ShaderResource;
    class ShaderProgram;
    class InputLayout;

    class ShaderManager final : public System
    {
    public:
        ShaderManager();
        ~ShaderManager() override;

        void AddInputLayout(const String& name, const InputLayout& input_layout);
        bool AddShaderProgram(ShaderResource* shader_resource);

        InputLayout&       GetInputLayout(const String& name);
        const InputLayout& GetInputLayout(const String& name) const;

        SPtr<ShaderProgram> GetShaderProgram(const String& name);

        bool RecompileShader(const String& name);

        template <typename T>
        void AddInputLayout(const InputLayout& input_layout, bool b_instanced = false);

        template <typename T>
        InputLayout& GetInputLayout(bool b_instanced = false);

        template <typename T>
        const InputLayout& GetInputLayout(bool b_instanced = false) const;

    private:
        friend class RenderSystem;

    private:
        void Initialize() override;
        void Update() override;
        void Shutdown() override;

    private:
        HashMap<String, InputLayout>         m_input_layouts;
        HashMap<String, SPtr<ShaderProgram>> m_shader_map_name;
        std::vector<ShaderResource*>         m_shader_resources;
        std::vector<SPtr<ShaderProgram>>     m_shader_programs;
    };

    template <typename T>
    void ShaderManager::AddInputLayout(const InputLayout& input_layout, bool b_instanced)
    {
        String name = typeid(T).name();
        name        = b_instanced ? "I" + name : name;
        m_input_layouts.emplace(name, input_layout);
    }

    template <typename T>
    InputLayout& ShaderManager::GetInputLayout(bool b_instanced)
    {
        String name = typeid(T).name();
        name        = b_instanced ? "I" + name : name;
        auto found  = m_input_layouts.find(name);
        if (found != m_input_layouts.end())
            return found->second;

        return m_input_layouts.find("DefaultLayout")->second;
    }

    template <typename T>
    const InputLayout& ShaderManager::GetInputLayout(bool b_instanced) const
    {
        String name = typeid(T).name();
        name        = b_instanced ? "I" + name : name;
        auto found  = m_input_layouts.find(name);

        if (found != m_input_layouts.end())
            return found->second;

        return m_input_layouts.find("DefaultLayout")->second;
    }
}
