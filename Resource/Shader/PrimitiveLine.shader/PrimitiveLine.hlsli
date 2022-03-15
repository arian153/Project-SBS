#ifndef P_LINE_HLSLI_
#define P_LINE_HLSLI_
#include "../Shader.includes/Params.hlsli"
#include "../Shader.includes/LightProcessing.hlsli"

struct VS_IN
{
    float3 pos : POSITION;

    row_major matrix mat_world : W;
    row_major matrix mat_wv : WV;
    row_major matrix mat_wvp : WVP;
    float4 diffuse : MAT_DI;
    uint instance_id : SV_InstanceID;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float4 diffuse : MAT_DI;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    if (g_int_0 == 1)
    {
        output.pos = mul(float4(input.pos, 1.f), input.mat_wvp);
        output.diffuse = input.diffuse;
    }
    else
    {
        output.pos = mul(float4(input.pos, 1.f), g_mat_wvp);
        output.diffuse = float4(1.f, 1.f, 1.f, 1.f);
    }
 
    return output;
}

float4 PS_Main(VS_OUT input) : SV_Target
{
    return input.diffuse;
}
#endif




