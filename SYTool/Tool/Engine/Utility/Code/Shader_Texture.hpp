// 상수 테이블(전역) : 클라이언트에 존재하는 각종 데이터를 대표하는 이름

matrix     g_matWorld;
matrix     g_matView;
matrix     g_matProj;
float g_fChapterX = 0.f;
float g_fChapterY = 0.f;
float g_fX = 1.0f;
float g_fY = 1.0f;

float g_fTime;
//sampler 란 : 색상 정보만 담고 있는 팔레트 형태의 텍스쳐 정의 구조체

texture		g_BaseTexture;
sampler  BaseSampler = sampler_state
{
	texture = g_BaseTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_RoundTexture;
sampler  RoundSampler = sampler_state
{
	texture = g_RoundTexture;

minfilter = linear;
magfilter = linear;
};

texture		g_HpTexture;
sampler  HpSampler = sampler_state
{
	texture = g_HpTexture;

minfilter = linear;
magfilter = linear;
};








struct  VS_IN
{
	vector   vPostion : POSITION;			// VECTOR == float4, 변수명 : 시멘틱(고유 속성 값, fvf, 반드시 대문자여야 함)
	float2	 vTexUV : TEXCOORD0;
};

struct  VS_OUT
{
	vector   vPostion : POSITION;
	float2	 vTexUV : TEXCOORD0;
};

// 버텍스 쉐이더
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT  Out = (VS_OUT)0;

	matrix		matWV, matWVP;

	matWV = mul(g_matWorld, g_matView);
	matWVP = mul(matWV, g_matProj);

	Out.vPostion = mul(vector(In.vPostion.xyz, 1.f), matWVP);
	Out.vTexUV = In.vTexUV;

	return Out;
}


// 픽셀 구조체에서 멤버 변수를 선언하는 도중 시멘틱의 값이 POSITION인 경우는 있을 수가 없다!

struct PS_IN
{
	// 텍스쳐 유브이
	float2			vTexUV : TEXCOORD0;
};

struct PS_OUT
{
	vector			vColor : COLOR0;
};

PS_OUT		PS_MAIN(PS_IN In)
{
	PS_OUT			Out = (PS_OUT)0;

	Out.vColor = tex2D(BaseSampler, In.vTexUV);	// 2차원 텍스쳐로부터 uv좌표에 해당하는 색을 얻어와 vector 타입으로 반환해주는 함수

	return Out;
}



technique Default_Device
{
	pass    // 기능의 캡슐화//0
	{
		cullmode = none;
		zwriteEnable = true;
		alphablendenable = false;
		srcblend = srcalpha;
		destblend = invsrcalpha;
		vertexshader = compile vs_3_0 VS_MAIN();
		pixelshader = compile ps_3_0 PS_MAIN();
	}
};