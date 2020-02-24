#include "Shader.h"

USING(Engine)

Engine::CShader::CShader(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pEffect(nullptr)
	, m_pErrMsg(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CShader::CShader(const CShader& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
	, m_pEffect(rhs.m_pEffect)
	, m_pErrMsg(rhs.m_pErrMsg)
{
	m_pGraphicDev->AddRef();
	m_pEffect->AddRef();
}

Engine::CShader::~CShader(void)
{

}

HRESULT Engine::CShader::Ready_Shader(const _tchar* pFilePath)
{

	// 에러인 경우 : pEffect는 null, pErrMsg는 값이 있다.
	// 경고인 경우 : pEffect는 값이 있다, pErrMsg는 값이 있다.

	// 정상인 경우 : pEffect는 값이 있다, pErrMsg는 null.







	if (FAILED(D3DXCreateEffectFromFile(m_pGraphicDev,
		pFilePath,
		NULL,			// 쉐이더에서 사용할 매크로
		NULL,			// 쉐이더에서 사용할 포함 파일
		D3DXSHADER_DEBUG,  // 쉐이더를 디버깅하겠다는 의미
		NULL,	// 각종 리소스 정보를 어디에 저장할 것인가(null인 경우 default로 gpu 메모리 장치에 저장)
		&m_pEffect,  // 쉐이더 객체
		&m_pErrMsg))) // 디버깅은 불가능, 에러 또는 경고가 발생했을 경우 마지막 인자값에 문자열 정보를 저장하게 됨
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "shader error", MB_OK);
		return E_FAIL;
	}

	else if (NULL != m_pErrMsg)
	{
		MessageBoxA(NULL, (char*)m_pErrMsg->GetBufferPointer(), "shader warning", MB_OK);
	}


	return S_OK;
}

CShader* Engine::CShader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath)
{
	CShader*		pInstance = new CShader(pGraphicDev);

	if (FAILED(pInstance->Ready_Shader(pFilePath)))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent* Engine::CShader::Clone(void)
{
	return new CShader(*this);
}

void Engine::CShader::Free(void)
{
	Safe_Release(m_pEffect);
	Safe_Release(m_pGraphicDev);
}

