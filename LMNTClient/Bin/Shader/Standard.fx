
#include "Share.fx"

VS_OUTPUT_COLOR StandardColorVS(VS_INPUT_COLOR input)
{
	VS_OUTPUT_COLOR output = (VS_OUTPUT_COLOR)0;
	//�������� �� ���ؽ� ��ġ�� ����� �����߽��� �ȴ�. �� ���� cbuffer�� �޾ƿ´�.
	//��� ����� ���� ���ؽ� ��ġ�� �����߽ɰ��� �Ÿ���ŭ �����̵� ��Ű�� �ȴ�.	

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
	
	// mul : ����� �����ִ� �Լ��̴�. ���͵� ��ķ� ����Ѵ�.
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

	// mul : ����� �����ִ� �Լ��̴�. ���͵� ��ķ� ����Ѵ�.
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


