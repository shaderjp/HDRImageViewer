//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#include "present.hlsli"
#include "color.hlsli"

float4 PSMain(PSInput input) : SV_TARGET
{
	// The scene, including brightness bars and color palettes, is rendered with linear gamma and Rec.709 primaries. (DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709) 
	float3 scene = g_scene.Sample(g_sampler, input.uv).rgb;
	float3 result = scene;

	if (displayCurve == DISPLAY_CURVE_SRGB)
	{
		result = LinearToSRGB(result);
	}
	else if (displayCurve == DISPLAY_CURVE_ST2084)
	{
		const float st2084max = 10000.0;
        const float hdrScalar = standardNits / st2084max;

		// The HDR scene is in Rec.709, but the display is Rec.2020
		result = Rec709ToRec2020(result);

		// Apply the ST.2084 curve to the scene.
		result = LinearToST2084(result * hdrScalar);
	}
	else // displayCurve == DISPLAY_CURVE_LINEAR
	{
		// Just pass through
	}

	if (HeatmapFlag)
	{
//		float luminance = 0.299f * result.x + 0.587f * result.y + 0.114f * result.z;	// NTSC
		float luminance = Rec2020ToXYZ(result).y;
		float2 uv = float2(luminance / 1.0f, 0.05f);
		result = g_heatMapTexture.Sample(g_sampler, uv).rgb;
	}

	return float4(result, 1.0f);
}