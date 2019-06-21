
struct VS_INPUT_COLOR
{
	float3	vPos	: POSITION;
	float4	vColor	: COLOR;
};

struct VS_OUTPUT_COLOR
{
	// Semantic ���� �տ� SV_ �� �ٰԵǸ� �� ������ System Value��
	// �Ǿ ���� ������ �Ұ����ϴ�. ������ ��¿����θ� �����
	// ���������� ������ �ȴ�.
	float4	vPos	: SV_POSITION;
	float4	vColor	: COLOR;
	float3	vLocalPosition : POSITION;
};


struct VS_INPUT_TEX
{
	float3	vPos	: POSITION;
	float2	vUV		: TEXCOORD0;
};

struct VS_OUTPUT_TEX
{
	float4	vPos	: SV_POSITION;
	float2	vUV		: TEXCOORD0;
};



struct PS_OUTPUT
{
	// SV_TARGET �� SV_TARGET0 �̴�. 0�� Ÿ�ٿ� ������ ���
	// �ϰڴٴ� ���̴�.
	float4	vTarget0	: SV_TARGET;
};

// Transform Constant Buffer
cbuffer Transform	: register(b0)
{
	matrix	g_matWorld;
	matrix	g_matView;
	matrix	g_matProj;
	matrix	g_matWV;
	matrix	g_matWVP;
	matrix	g_matVP;
	matrix	g_matWP;
	float3	g_vPivot;
	float		g_vPivotEmpty;
	float3	g_vMeshLength;
	float		g_vMeshLengthEmpty;
}

// Animation 2D Constant Buffer
cbuffer	Animation2D	: register(b9)
{
	int		g_iAnim2DType;
	int		g_iAnim2DFrameX;
	int		g_iAnim2DFrameY;
	int		g_iAnim2DStartX;
	int		g_iAnim2DStartY;
	int		g_iAnim2DLengthX;
	int		g_iAnim2DLengthY;
	int		g_iAnim2DMaxX;
	int		g_iAnim2DMaxY;
	float3	g_vAnim2DEmpty;
}

// Material Constant Buffer
cbuffer Material	: register(b1)
{
	float4	g_vMaterialDiffuse;
}

Texture2D		g_DiffuseTexture : register(t0);
SamplerState	g_DiffuseSampler : register(s0);
Texture2DArray	g_FrameAnimationTexture: register(t9);

#define	AT_NONE	0
#define	AT_ATLAS	1
#define	AT_FRAME	2
#define	AT_CUSTOM	3

float2 ComputeAnimation2DUV(float2 _vUV)
{
	float2	vUV = (float2)0;


	if (g_iAnim2DType == AT_ATLAS)
	{
		vUV.x = (float)(g_iAnim2DFrameX + _vUV.x + g_iAnim2DStartX) / (float)g_iAnim2DMaxX;
		vUV.y = (float)(g_iAnim2DFrameY + _vUV.y + g_iAnim2DStartY) / (float)g_iAnim2DMaxY;
	}
	else if (g_iAnim2DType == AT_CUSTOM)
	{
		//�ȼ� ������ �����Ѵ�.
		vUV.x = floor(_vUV.x * g_iAnim2DMaxX) ;
		vUV.y = floor(_vUV.y * g_iAnim2DMaxY) ;

		if (vUV.x < 0.0f)
		{
			vUV.x = 0.0f;
		}
		if (vUV.y < 0.0f)
		{
			vUV.y = 0.0f;
		}
		vUV.x = vUV.x + 0.5f;
		vUV.y = vUV.y + 0.5f;

		vUV.x = vUV.x / g_iAnim2DMaxX;
		vUV.y = vUV.y / g_iAnim2DMaxY;

		vUV.x = (float)(g_iAnim2DFrameX + vUV.x * g_iAnim2DLengthX) / (float)g_iAnim2DMaxX;
		vUV.y = (float)(g_iAnim2DFrameY + vUV.y * g_iAnim2DLengthY) / (float)g_iAnim2DMaxY;
	}
	else
		vUV = _vUV;

	return vUV;
}
