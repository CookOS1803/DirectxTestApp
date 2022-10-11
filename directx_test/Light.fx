// Per-vertex data input to the vertex shader
struct VertexShaderInput
{
	float3 position : POSITION;
	float3 color : COLOR;
	float3 normal : NORMAL;
	//float3 backColor : COLOR1;
};

// Per-vertex data output from the vertex shader
struct VertexShaderOutput
{
	float4 clipSpaceOutput : SV_POSITION;
	float4 sceneSpaceOutput : SCENE_POSITION;
	float3 normalModel : NORMAL0;
	float3 normalView : NORMAL1;
	float3 color : COLOR;
	//float3 backColor : COLOR1;
};

// Constant buffer provided by effect
cbuffer VertexShaderConstantBuffer : register(b1)
{
	float4x4 modelMatrix;
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
};

// Called for each vertex
VertexShaderOutput VS(VertexShaderInput input)
{
	// Output structure
	VertexShaderOutput output;
	// Get the input vertex, and include a W coordinate
	float4 pos = float4(input.position.xyz, 1.0f);
	// Pass through the resultant scene space output value
	// (not necessary — can be removed from both shaders)
	output.sceneSpaceOutput = pos;
	// Apply transforms to that vertex
	pos = mul(pos, modelMatrix);
	pos = mul(pos, viewMatrix);
	pos = mul(pos, projectionMatrix);
	// The result is clip space output
	output.clipSpaceOutput = pos;
	// Apply model transform to normal
	float4 normal = float4(input.normal, 0);
	normal = mul(normal, modelMatrix);
	output.normalModel = normal.xyz;
	// Apply view transform to normal
	normal = mul(normal, viewMatrix);
	output.normalView = normal.xyz;
	// Transfer colors
	output.color = input.color;
	output.backColor = input.backColor;
	return output;
}