#ifndef DEFERRED_PROCESS_HLSLI_
#define DEFERRED_PROCESS_HLSLI_

float3 CalculateGeoBufferPos(float2 tex)
{
	return g_diff_tex_0.Sample(g_sam_0, tex).xyz;
}

float3 CalculateGeoBufferNormal(float2 tex)
{
	return g_diff_tex_1.Sample(g_sam_0, tex).xyz;
}

float4 ProcessGeoBufferDiffuseMaterial(float2 tex)
{
	return g_diff_tex_0.Sample(g_sam_0, tex);
}

float4 ProcessGeoBufferSpecularMaterial(float2 tex)
{
	return g_diff_tex_1.Sample(g_sam_0, tex);
}

float4 ProcessLitBufferDiffuseLight(float2 tex)
{
	return g_diff_tex_2.Sample(g_sam_0, tex);
}

float4 ProcessLitBufferSpecularLight(float2 tex)
{
	return g_diff_tex_3.Sample(g_sam_0, tex);
}

#endif