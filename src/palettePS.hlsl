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

#include "palette.hlsli"
#include "color.hlsli"

float4 PSMain(PSInput input) : SV_TARGET
{
	// The triangle stores the data in CIE xyY color space. We convert the data to RGB format in Rec709 RGB color space.
	float4 color = g_hdrTexture.Sample(g_sampler, input.uv);
	color = color * pow(2.0, EVValue);
	
	return color;
}
