#ifndef PARAMS_HLSLI_
#define PARAMS_HLSLI_

struct LightColor
{
    float4      diffuse;
    float4      ambient;
    float4      specular;
};

struct LightInfo
{
    LightColor  color;
    float4	    position;
    float4	    direction;
    int		    light_type;
    float	    range;
    float	    angle;
    int  	    padding;
};


cbuffer TRANSFORM_PARAMS : register(b0)
{
    row_major matrix g_mat_world;
    row_major matrix g_mat_view;
    row_major matrix g_mat_projection;
    row_major matrix g_mat_wv;
    row_major matrix g_mat_wvp;
};

cbuffer MATERIAL_PARAMS : register(b1)
{
    int     g_int_0;
    int     g_int_1;
    int     g_int_2;
    int     g_int_3;
    int     g_int_4;
    float   g_float_0;
    float   g_float_1;
    float   g_float_2;
    float   g_float_3;
    float   g_float_4;
};

Texture2D g_tex_0 : register(t0);
Texture2D g_tex_1 : register(t1);
Texture2D g_tex_2 : register(t2);
Texture2D g_tex_3 : register(t3);
Texture2D g_tex_4 : register(t4);

SamplerState g_sam_0 : register(s0);

#endif