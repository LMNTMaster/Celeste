
#include "Share.fx"

VS_OUTPUT_COLOR StandardColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;
	//폴리곤의 각 버텍스 위치의 평균이 무게중심이 된다. 이 값은 cbuffer로 받아온다.
	//행렬 계산이 끝난 버텍스 위치에 무게중심과의 거리만큼 평행이동 시키면 된다.	

	float3	vPos = input.vPos - g_vPivot * g_vMeshLength;

	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	
	output.vColor = input.vColor;
	
	output.vLocalPosition = input.vPos;

	return output;
}

PS_OUTPUT StandardColorPS(VS_OUTPUT_COLOR input)
{
	PS_OUTPUT output = (PS_OUTPUT)0;
	output.vTarget0 = (input.vColor * input.vColor.a) + (float4(1.0f, 0.78125f, 0, 1) * (1 - input.vColor.a));			

	return output;
}




VS_OUTPUT_TEX StandardTextureVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;
	
	float3	vPos = input.vPos - g_vPivot * g_vMeshLength;
	
	// mul : 행렬을 곱해주는 함수이다. 벡터도 행렬로 취급한다.
	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	float2	vUV = ComputeAnimation2DUV(input.vUV);
	output.vUV = vUV;
	return output;
}

PS_OUTPUT StandardTexturePS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;
	float4 vColor;

	if (g_iAnim2DType == AT_FRAME)
	{
		float3	vUVW = float3(input.vUV, g_iAnim2DFrameX);
		vColor = g_FrameAnimationTexture.Sample(g_DiffuseSampler, vUVW) * g_vMaterialDiffuse;
	}

	else
		vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV) * g_vMaterialDiffuse;
	if (vColor.a == 0.0f)
	{
		clip(-1);
	}

	output.vTarget0 = vColor;

	return output;
}


VS_OUTPUT_TEX ColoredTextureVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vPivot * g_vMeshLength;

	// mul : 행렬을 곱해주는 함수이다. 벡터도 행렬로 취급한다.
	output.vPos = mul(float4(vPos, 1.f), g_matWVP);
	float2	vUV = ComputeAnimation2DUV(input.vUV);
	output.vUV = vUV;
	return output;
}

PS_OUTPUT ColoredTexturePS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;
	float4 vColor;

	if (g_iAnim2DType == AT_FRAME)
	{
		float3	vUVW = float3(input.vUV, g_iAnim2DFrameX);
		vColor = g_FrameAnimationTexture.Sample(g_DiffuseSampler, vUVW) * g_vMaterialDiffuse;
	}

	else
		vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV) * g_vMaterialDiffuse;

	if (vColor.a == 0.0f)
	{
		clip(-1);
	}
	else
	{
		vColor = g_vMaterialDiffuse;
	}

	output.vTarget0 = vColor;

	return output;
}




VS_OUTPUT_TEX UITextureVS(VS_INPUT_TEX input)
{
	VS_OUTPUT_TEX	output = (VS_OUTPUT_TEX)0;

	float3	vPos = input.vPos - g_vPivot * g_vMeshLength;

	output.vPos = mul(float4(vPos, 1.f), g_matWP);

	float2	vUV = ComputeAnimation2DUV(input.vUV);
	output.vUV = vUV;

	return output;
}

PS_OUTPUT UITexturePS(VS_OUTPUT_TEX input)
{
	PS_OUTPUT	output = (PS_OUTPUT)0;

	float4	vColor;

	if (g_iAnim2DType == AT_FRAME)
	{
		float3	vUVW = float3(input.vUV, g_iAnim2DFrameX);
		vColor = g_FrameAnimationTexture.Sample(g_DiffuseSampler, vUVW) * g_vMaterialDiffuse;
	}

	else
		vColor = g_DiffuseTexture.Sample(g_DiffuseSampler, input.vUV) * g_vMaterialDiffuse;

	if (vColor.a == 0.f)
		clip(-1);

	output.vTarget0 = vColor;

	return output;
}


