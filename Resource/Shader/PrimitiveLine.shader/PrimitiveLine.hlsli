#ifndef P_LINE_HLSLI_
#define P_LINE_HLSLI_
#include "../Shader.includes/Params.hlsli"
#include "../Shader.includes/LightProcessing.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
};

struct VS_OUT
{
    float4 pos : SV_Position;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    output.pos = mul(float4(input.pos, 1.f), g_mat_wvp);
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    float4 color = float4(1.f, 1.f, 1.f, 1.f);
    return color;
}
#endif




