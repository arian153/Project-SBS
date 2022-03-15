#ifndef P_FACE_HLSLI_
#define P_FACE_HLSLI_
#include "../Shader.includes/Params.hlsli"

struct VS_IN
{
    float3 pos : POSITION;
    float3 n : NORMAL;

    row_major matrix mat_world : W;
    row_major matrix mat_wv : WV;
    row_major matrix mat_wvp : WVP;
    float4 diffuse : MAT_DI;
    uint instance_id : SV_InstanceID;
};

struct VS_OUT
{
    float4 pos : SV_Position;
    float3 view_p : POSITION;
    float3 view_n : NORMAL;
    float4 diffuse : MAT_DI;
};

VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    if (g_int_0 == 1)
    {
        output.pos = mul(float4(input.pos, 1.f), input.mat_wvp);
        output.view_p = mul(float4(input.pos, 1.f), input.mat_wv).xyz;
        output.view_n = normalize(mul(float4(input.n, 0.f), input.mat_wv).xyz);

        output.diffuse = input.diffuse;
    }
    else
    {
        output.pos = mul(float4(input.pos, 1.f), g_mat_wvp);
        output.view_p = mul(float4(input.pos, 1.f), g_mat_wv).xyz;
        output.view_n = normalize(mul(float4(input.n, 0.f), g_mat_wv).xyz);

        output.diffuse = float4(1.f, 1.f, 1.f, 1.f);
    }

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

    float4 color = input.diffuse;
    float3 normal = input.view_n;

    output.position = float4(input.view_p.xyz, 0.f);
    output.normal = float4(normal.xyz, 0.f);
    output.color = color;
   
    return output;
}
#endif




