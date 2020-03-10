
/*____________________________________________________________________
- register는 상수버퍼와 리소들을 등록하는시멘틱입니다. 숫자는 옵션입니다.
b - 상수버퍼
t - 텍스처
s - 샘플러

- packoffset은 한 번에 부동소수점 4개를 처리할 수 있는 GPU를 고려해서 상수버퍼의 맴버들을 묶는 옵션입니다.
 float3 과 float 을 이 옵션으로 묶을 수 있습니다.
______________________________________________________________________*/
cbuffer cbMatrixInfo	: register(b0)
{
    float4x4 matWorld : packoffset(c0);
    float4x4 matView : packoffset(c4);
    float4x4 matProj : packoffset(c8);
    float4x4 matWVP : packoffset(c12);
};

struct VS_IN
{
	float3 vPos		: POSITION;
	float4 vColor	: COLOR;
};

struct VS_OUT
{
	float4 vPos		: SV_POSITION;
	float4 vColor	: COLOR;
};

VS_OUT VS_MAIN(VS_IN Input)
{
	VS_OUT Output= (VS_OUT)0 ;
	
	Output.vPos		= mul(float4(Input.vPos, 1.0f), matWVP);
	Output.vColor	= Input.vColor;
	
	return (Output);
}

float4 PS_MAIN(VS_OUT Input) : SV_TARGET
{
	return (Input.vColor);
}

