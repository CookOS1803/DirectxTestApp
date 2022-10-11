//--------------------------------------------------------------------------------------
// File: Tutorial04.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
cbuffer ConstantBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
    float4 vLightDir[2];
    float4 vLightColor[2];
    float4 vOutputColor;
}

//--------------------------------------------------------------------------------------
struct VS_INPUT
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float3 Norm : NORMAL;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float4 Color : COLOR0;
    float3 Norm : TEXCOORD0;
    float4 pLightDir[2] : DD;
    float4 pLightColor[2] : VV;
};

//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    output.Pos = mul(input.Pos, World);
    output.Pos = mul(output.Pos, View);
    output.Pos = mul(output.Pos, Projection);
    output.Color = vOutputColor;
    output.pLightDir[0] = vLightDir[0];
    output.pLightDir[1] = vLightDir[1];
    output.pLightColor[0] = vLightColor[0];
    output.pLightColor[1] = vLightColor[1];
    output.Norm = mul(input.Norm, World);

    return output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(PS_INPUT input) : SV_Target
{
    float4 finalColor = input.Color;

    //do NdotL lighting for 2 lights
    for (int i = 0; i < 2; i++)
    {
        finalColor += saturate(dot((float3)input.pLightDir[i],input.Norm) * input.pLightColor[i]);
    }
    finalColor.a = 1;

    return finalColor;
}


//--------------------------------------------------------------------------------------
// PSSolid - render a solid color
//--------------------------------------------------------------------------------------
float4 PSSolid(PS_INPUT input) : SV_Target
{
    return vOutputColor;
}
