#ifndef D_LIGHTING_HLSLI_
#define D_LIGHTING_HLSLI_
#include "../Shader.includes/Params.hlsli"
#include "../Shader.includes/LightProcessing.hlsli"
#include "../Shader.includes/DeferredProcessing.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 tex : TEXCOORD;
};

struct PS_OUT
{
    float4 diffuse : SV_Target0;
    float4 specular : SV_Target1;
};

// [Directional Light]
// g_int_0 : Light index
// g_diff_tex_0 : Position RT
// g_diff_tex_1 : Normal RT
// Mesh : Rectangle

VS_OUT VS_DirLight(VS_IN input)
{
    VS_OUT output;
    output.pos = float4(input.pos * 2.f, 1.f);
    output.tex = input.tex;

    return output;
}

PS_OUT PS_DirLight(VS_OUT input)
{
    PS_OUT output;
    float3 view_pos = CalculateGeoBufferPos(input.tex);
    if (view_pos.z <= 0.f)
        clip(-1);

    float3 view_normal = CalculateGeoBufferNormal(input.tex);

    LightColor color = CalculateLightColor(g_int_0, view_normal, view_pos);
    output.diffuse = color.diffuse + color.ambient;
    output.specular = color.specular;

    return output;
}

// [Point Light]
// g_int_0 : Light index
// g_diff_tex_0 : Position RT
// g_diff_tex_1 : Normal RT
// g_vec2_0 : RenderTarget Resolution
// Mesh : Sphere

VS_OUT VS_PointLight(VS_IN input)
{
    VS_OUT output;

    output.pos = mul(float4(input.pos, 1.f), g_mat_wvp);
    output.tex = input.tex;

    return output;
}

PS_OUT PS_PointLight(VS_OUT input)
{
    PS_OUT output;

    // input.pos = SV_Position = Screen
    float2 tex = float2(input.pos.x / g_vec2_0.x, input.pos.y / g_vec2_0.y);
    float3 view_pos = CalculateGeoBufferPos(tex);;
    if (view_pos.z <= 0.f)
        clip(-1);

    int light_index = g_int_0;
    float3 view_light_pos = mul(float4(g_light[light_index].position.xyz, 1.f), g_mat_view).xyz;
    float distance = length(view_pos - view_light_pos);
    if (distance > g_light[light_index].range)
        clip(-1);

    float3 view_normal = CalculateGeoBufferNormal(tex);

    LightColor color = CalculateLightColor(g_int_0, view_normal, view_pos);

    output.diffuse = color.diffuse + color.ambient;
    output.specular = color.specular;

    return output;
}



// [Final]
// g_diff_tex_0 : Diffuse Material Target
// g_diff_tex_1 : Specular Material Target
// g_diff_tex_2 : Diffuse Light Target
// g_diff_tex_3 : Specular Light Target
// Mesh : Rectangle

VS_OUT VS_Final(VS_IN input)
{
    VS_OUT output = (VS_OUT)0;

    output.pos = float4(input.pos * 2.f, 1.f);
    output.tex = input.tex;

    return output;
}

float4 PS_Final(VS_OUT input) : SV_Target
{
    float4 diffuse_light = ProcessLitBufferDiffuseLight(input.tex);
    if (diffuse_light.x == 0.f && diffuse_light.y == 0.f && diffuse_light.z == 0.f)
        clip(-1);
    
    float4 specular_light = ProcessLitBufferSpecularLight(input.tex);

    float4 diffuse_material = ProcessGeoBufferDiffuseMaterial(input.tex);

    float4 output = (diffuse_material * diffuse_light) + specular_light;
    return output;
}


#endif

