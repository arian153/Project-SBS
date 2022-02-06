#ifndef DEFAULT_HLSLI_
#define DEFAULT_HLSLI_
#include "../Shader.includes/Params.hlsli"


struct VS_IN
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float2 uv : TEXCOORD;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    output.pos = mul(float4(input.pos, 1.f), g_mat_world);
    output.pos = mul(output.pos, g_mat_view);
    output.pos = mul(output.pos, g_mat_projection);
    output.uv = input.uv;
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = g_tex_0.Sample(g_sam_0, input.uv);
    return color;
}
#endif




