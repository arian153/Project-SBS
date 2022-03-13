#ifndef G_BUFFER_HLSLI_
#define G_BUFFER_HLSLI_
#include "../Shader.includes/Params.hlsli"
#include "../Shader.includes/LightProcessing.hlsli"
#include "../Shader.includes/TextureProcessing.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
    float2 tex : TEXCOORD;
    float3 n : NORMAL;
    float3 t : TANGENT;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 tex : TEXCOORD;
    float3 view_p : POSITION;
    float3 view_n : NORMAL;
    float3 view_t : TANGENT;
    float3 view_b : BINORMAL;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    output.pos = mul(float4(input.pos, 1.f), g_mat_wvp);
    output.tex = input.tex;

    output.view_p = mul(float4(input.pos, 1.f), g_mat_wv).xyz;
    output.view_n = normalize(mul(float4(input.n, 0.f), g_mat_wv).xyz);
    output.view_t = normalize(mul(float4(input.t, 0.f), g_mat_wv).xyz);
    output.view_b = normalize(cross(output.view_t, output.view_n));

    return output;
}


struct PS_OUT
{
    float4 position : SV_Target0;
    float4 normal : SV_Target1;
    float4 color : SV_Target2;
};

PS_OUT PS_Main(VS_OUT input) 
{
    PS_OUT output;
    float4 color = ProcessDiffuse(input.tex, 1, 0, float4(1.f, 1.f, 1.f, 1.f), 2.2f); 
    float3 normal = ProcessNormal(input.tex, 1, input.view_t, input.view_b, input.view_n);
    
    output.position = float4(input.view_p.xyz, 0.f);
    output.normal = float4(normal.xyz, 0.f);
    output.color = color;
   
    return output;
}
#endif



