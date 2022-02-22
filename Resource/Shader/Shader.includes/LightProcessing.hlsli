#ifndef LIT_PROCESS_HLSLI_
#define LIT_PROCESS_HLSLI_

LightColor CalculateLightColor(int light_index, float3 view_normal, float3 view_pos)
{
    LightColor color = (LightColor)0.f;

    float3 view_light_dir = (float3)0.f;

    float diffuse_ratio  = 0.f;
    float specular_ratio = 0.f;
    float distance_ratio = 1.f;

    if (g_light[light_index].light_type == 0)
    {
        // Directional Light
        view_light_dir = normalize(mul(float4(g_light[light_index].direction.xyz, 0.f), g_mat_view).xyz);
        diffuse_ratio  = saturate(dot(-view_light_dir, view_normal));
    }
    else if (g_light[light_index].light_type == 1)
    {
        // Point Light
        float3 view_light_pos = mul(float4(g_light[light_index].position.xyz, 1.f), g_mat_view).xyz;
        view_light_dir        = normalize(view_pos - view_light_pos);
        diffuse_ratio         = saturate(dot(-view_light_dir, view_normal));

        float dist = distance(view_pos, view_light_pos);
        if (g_light[light_index].range == 0.f)
            distance_ratio = 0.f;
        else
            distance_ratio = saturate(1.f - pow(dist / g_light[light_index].range, 2));
    }
    else
    {
        // Spot Light
        float3 view_light_pos = mul(float4(g_light[light_index].position.xyz, 1.f), g_mat_view).xyz;
        view_light_dir        = normalize(view_pos - view_light_pos);
        diffuse_ratio         = saturate(dot(-view_light_dir, view_normal));

        if (g_light[light_index].range == 0.f)
            distance_ratio = 0.f;
        else
        {
            float half_angle = g_light[light_index].angle / 2;

            float3 view_light_vec        = view_pos - view_light_pos;
            float3 view_center_light_dir = normalize(mul(float4(g_light[light_index].direction.xyz, 0.f), g_mat_view).xyz);

            float center_dist = dot(view_light_vec, view_center_light_dir);
            distance_ratio    = saturate(1.f - center_dist / g_light[light_index].range);

            float light_angle = acos(dot(normalize(view_light_vec), view_center_light_dir));

            if (center_dist < 0.f || center_dist > g_light[light_index].range)
                distance_ratio = 0.f;
            else if (light_angle > half_angle)
                distance_ratio = 0.f;
            else
                distance_ratio = saturate(1.f - pow(center_dist / g_light[light_index].range, 2));
        }
    }

    float3 reflection_dir = normalize(view_light_dir + 2 * (saturate(dot(-view_light_dir, view_normal)) * view_normal));
    float3 eye_dir        = normalize(view_pos);
    specular_ratio       = saturate(dot(-eye_dir, reflection_dir));
    specular_ratio       = pow(specular_ratio, 2);

    color.diffuse  = g_light[light_index].color.diffuse * diffuse_ratio * distance_ratio;
    color.ambient  = g_light[light_index].color.ambient * distance_ratio;
    color.specular = g_light[light_index].color.specular * specular_ratio * distance_ratio;

    return color;
}

#endif
