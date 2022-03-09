#ifndef P_FACE_HLSLI_
#define P_FACE_HLSLI_
#include "../Shader.includes/Params.hlsli"
#include "../Shader.includes/LightProcessing.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
    float3 n : NORMAL;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 view_p : POSITION;
    float3 view_n : NORMAL;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    output.pos = mul(float4(input.pos, 1.f), g_mat_wvp);
    output.view_p = mul(float4(input.pos, 1.f), g_mat_wv).xyz;
    output.view_n = normalize(mul(float4(input.n, 0.f), g_mat_wv).xyz);

    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    float3 normal = input.view_n;

    LightColor total_color = (LightColor)0.f;

    for (int i = 0; i < g_light_count; ++i)
    {
        LightColor l_color = CalculateLightColor(i, normal, input.view_p);
        total_color.diffuse += l_color.diffuse;
        total_color.ambient += l_color.ambient;
        total_color.specular += l_color.specular;
    }

    color.xyz = (total_color.diffuse.xyz * color.xyz)
        + total_color.ambient.xyz * color.xyz
        + total_color.specular.xyz;

    return color;
}
#endif




