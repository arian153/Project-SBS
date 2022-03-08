#ifndef SKYBOX_HLSLI_
#define SKYBOX_HLSLI_
#include "../Shader.includes/Params.hlsli"
#include "../Shader.includes/TextureProcessing.hlsli"

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

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    float4 view_pos = mul(float4(input.pos, 0), g_mat_view);
    float4 clip_space_pos = mul(view_pos, g_mat_proj);

    output.pos = clip_space_pos.xyww;
    output.tex = input.tex;

    return output;

}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = ProcessDiffuse(input.tex, 1, 0, float4(1.f, 1.f, 1.f, 1.f), 2.2f); 
    return color;
 }
#endif