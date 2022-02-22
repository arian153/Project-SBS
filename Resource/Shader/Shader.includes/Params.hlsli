#ifndef PARAMS_HLSLI_
#define PARAMS_HLSLI_

struct LightColor
{
    float4 diffuse;
    float4 ambient;
    float4 specular;
};

struct LightInfo
{
    LightColor color;
    float3     position;
    float      padding0;
    float3     direction;
    float      padding1;
    int        light_type;
    float      range;
    float      angle;
    int        padding2;
};

cbuffer GLOBAL_PARAMS : register(b0)
{
int       g_light_count;
float3    g_light_padding;
LightInfo g_light[50];
}

cbuffer TRANSFORM_PARAMS : register(b1)
{
row_major matrix g_mat_world;
row_major matrix g_mat_view;
row_major matrix g_mat_proj;
row_major matrix g_mat_wv;
row_major matrix g_mat_wvp;
};

cbuffer MATERIAL_PARAMS : register(b2)
{
int   g_int_0;
int   g_int_1;
int   g_int_2;
int   g_int_3;
int   g_int_4;
float g_float_0;
float g_float_1;
float g_float_2;
float g_float_3;
float g_float_4;
};

Texture2D g_diff_tex_0 : register(t0);
Texture2D g_diff_tex_1 : register(t1);
Texture2D g_spec_tex_0 : register(t2);
Texture2D g_norm_tex_0 : register(t3);
Texture2D g_map_tex_0 : register(t4);

SamplerState g_sam_0 : register(s0);

#endif
