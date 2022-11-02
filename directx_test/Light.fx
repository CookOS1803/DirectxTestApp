Texture2D txDiffuse : register(t0);
TextureCube skymap : register(t1);
SamplerState samLinear : register(s0);

// Per-vertex data input to the vertex shader
struct VertexShaderInput
{
	float3 position : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

// Per-vertex data output from the vertex shader
struct VertexShaderOutput
{
	float4 position : SV_POSITION;
	float3 localpos : LOCALPOS;
	float3 color : COLOR;
	float3 normalModel : NORMALM;
	float2 texCoord : TEXCOORD;
};

// Constant buffer provided by effect
cbuffer VertexShaderConstantBuffer : register(b1)
{
	matrix modelMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

// Constant buffer provided by effect
cbuffer PixelShaderConstantBuffer : register(b0)
{
	float4 ambientLight;
	float4 directionalLight;
	float4 lightDirection;
	float sine;
};

// Called for each vertex
VertexShaderOutput VS(VertexShaderInput input)
{
	// Output structure
	VertexShaderOutput output;
	output.localpos = input.position.xyz;
	// Get the input vertex, and include a W coordinate
	float4 pos = float4(input.position.xyz, 1.0f);
	// Apply transforms to that vertex
	pos = mul(pos, modelMatrix);
	pos = mul(pos, viewMatrix);
	pos = mul(pos, projectionMatrix);
	// The result is clip space output
	output.position = pos;

	// Apply model transform to normal
	float4 normal = float4(input.normal, 0);
	normal = normalize(mul(normal, modelMatrix));
	output.normalModel = normal.xyz;

	// Transfer colors
	output.color = input.color;

	output.texCoord = input.texCoord;

	return output;
}

// Called for each pixel
float4 PS(VertexShaderOutput input) : SV_TARGET
{
	// Find angle between light and normal
	float3 lightDir = normalize(lightDirection.xyz);
	float cosine = -dot(input.normalModel, lightDir);
	cosine = max(cosine, 0);

	float3 color = (ambientLight.xyz + cosine * directionalLight.xyz) * input.color;

	// Return color with opacity of 1
	return float4(color, 1);
}

float4 PSSolid(VertexShaderOutput input) : SV_TARGET
{
	return float4(input.color, 1);
}

float4 PSTexture(VertexShaderOutput input) : SV_TARGET
{
	return txDiffuse.Sample(samLinear, input.texCoord) * PS(input); //* float4(input.color, 1);
}

float4 PSCustom(VertexShaderOutput input) : SV_TARGET
{
	return float4(float3(1, 1, 1) * sin(input.localpos.y * 50), 1) * PS(input);
}
//float4 PSCustom2(VertexShaderOutput input) : SV_TARGET
//{
//	if (input.localpos.y > sine - 0.)
//	return float4(float3(1, 1, 1) * sin(input.localpos.y * 50), 1) * PS(input);
//}


struct SKYMAP_VS_OUTPUT    //output structure for skymap vertex shader
{
    float4 Pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

SKYMAP_VS_OUTPUT SKYMAP_VS(VertexShaderOutput input)
{
    SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT)0;
	float4 pos = float4(input.position.xyz, 1.0f);
	// Apply transforms to that vertex
	pos = mul(pos, modelMatrix);
	pos = mul(pos, viewMatrix);
	pos = mul(pos, projectionMatrix);
    //Set Pos to xyww instead of xyzw, so that z will always be 1 (furthest from camera)
    output.Pos = pos.xyww;

    output.texCoord = input.position; //pos;

    return output;
}

float4 SkymapPShader(SKYMAP_VS_OUTPUT input) : SV_Target
{
    return skymap.Sample(samLinear, input.texCoord);
}