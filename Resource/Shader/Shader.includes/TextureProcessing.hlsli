#ifndef TEX_PROCESS_HLSLI_
#define TEX_PROCESS_HLSLI_

float4 ProcessDiffuse(float2 tex, int type, int blending, float4 diffuse, float gamma)
{
    float4 color = diffuse;
    if (type == 1)
    {
        // sample simple diffuse texture.
        color = g_diff_tex_0.Sample(g_sam_0, tex);
    }
    else if (type == 2)
    {
        // alpha mapping
        float4 diffuse1 = g_diff_tex_0.Sample(g_sam_0, tex);
        float4 diffuse2 = g_diff_tex_1.Sample(g_sam_0, tex);
        float4 alpha    = g_map_tex_0.Sample(g_sam_0, tex);

        color = saturate((alpha * diffuse1) + ((1.0f - alpha) * diffuse2));
    }
    else if (type == 3)
    {
        // light mapping
        float4 diffuse_map = g_diff_tex_0.Sample(g_sam_0, tex);
        float4 light_map   = g_map_tex_0.Sample(g_sam_0, tex);
        color              = diffuse_map * light_map;
    }
    else if (type == 4)
    {
        // multiple texture blending
        float4 diffuse1 = g_diff_tex_0.Sample(g_sam_0, tex);
        float4 diffuse2 = g_diff_tex_1.Sample(g_sam_0, tex);
        color           = diffuse1 * diffuse2 * gamma;
    }

    if (blending)
    {
        color = color * diffuse;
    }

    return color;
}

float4 ProcessSpecular(float2 tex, int mapping, int blending, float4 specular)
{
    float4 color = specular;
    if (mapping)
    {
        color = g_spec_tex_0.Sample(g_sam_0, tex);
    }
    if (blending)
    {
        color = color * specular;
    }

    return color;
}

float3 ProcessNormal(float2 tex, int type, float3 t, float3 b, float3 n)
{
    float3 normal = n;
    if (type)
    {
        float3 tangent_space_normal = g_norm_tex_0.Sample(g_sam_0, tex).xyz;
        tangent_space_normal        = (tangent_space_normal - 0.5f) * 2.f;
        float3x3 tbn_mat            = { t, b, n };
        normal                      = normalize(mul(tangent_space_normal, tbn_mat));
    }

    return normal;
}

#endif
