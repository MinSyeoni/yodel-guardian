#include "Light.h"

USING(Engine)

Engine::CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
	m_pGraphicDev->AddRef();
}

Engine::CLight::~CLight(void)
{

}

HRESULT Engine::CLight::Ready_Light(const D3DLIGHT9* pLightInfo,
	const _uint& iIndex)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	m_iIndex = iIndex;

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0,
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL), E_FAIL);

	FAILED_CHECK_RETURN(m_pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL), E_FAIL);

	D3DVIEWPORT9		ViewPort;
	m_pGraphicDev->GetViewport(&ViewPort);

	VTXSCREEN*		pVertex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertex, 0);

	pVertex[0].vPos = _vec4(0.f, 0.f, 0.f, 1.f);
	pVertex[0].vTexUV = _vec2(0.f, 0.f);

	pVertex[1].vPos = _vec4(_float(ViewPort.Width), 0.f, 0.f, 1.f);
	pVertex[1].vTexUV = _vec2(1.f, 0.f);

	pVertex[2].vPos = _vec4(_float(ViewPort.Width), _float(ViewPort.Height), 0.f, 1.f);
	pVertex[2].vTexUV = _vec2(1.f, 1.f);

	pVertex[3].vPos = _vec4(0.f, _float(ViewPort.Height), 0.f, 1.f);
	pVertex[3].vTexUV = _vec2(0.f, 1.f);

	m_pVB->Unlock();

	INDEX16*		pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	pIndex[0]._0 = 0;
	pIndex[0]._1 = 1;
	pIndex[0]._2 = 2;

	pIndex[1]._0 = 0;
	pIndex[1]._1 = 2;
	pIndex[1]._2 = 3;

	m_pIB->Unlock();

	m_pGraphicDev->SetLight(iIndex, &m_tLightInfo);
	m_pGraphicDev->LightEnable(iIndex, TRUE);

	return S_OK;
}

Engine::CLight* Engine::CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLightInfo,
	const _uint& iIndex,STYLE style,_vec3 Pos)
{
	CLight*	pInstance = new CLight(pGraphicDev);
	pInstance->m_eStyle = style;
	pInstance->m_vecLightPos = Pos;
	if (FAILED(pInstance->Ready_Light(pLightInfo, iIndex)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CLight::Free(void)
{
	m_pGraphicDev->LightEnable(m_iIndex, FALSE);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Safe_Release(m_pGraphicDev);
}

void Engine::CLight::Render(LPD3DXEFFECT pEffect)
{

	if (m_eStyle == SPOT&&m_bisRender == false)
		return;
	_uint iPassIdx = 0;
	if (m_eStyle == DIRECTION)
		pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));
	else if (m_eStyle == SPOT)
	{
		iPassIdx = 1;
		pEffect->SetVector("g_vLightPos", &_vec4(m_vecLightPos, 1.f));
		pEffect->SetFloat("g_fRange",5.f);
	}
	else if (m_eStyle == POINT)
	{

		iPassIdx = 2;
		pEffect->SetVector("g_vLightPos", &_vec4(m_vecLightPos, 1.f));
		pEffect->SetVector("g_vLightDir", &_vec4(m_tLightInfo.Direction, 0.f));
		pEffect->SetFloat("g_vCosPhi", m_tLightInfo.Phi);
	}

	_matrix			matView, matProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	D3DXMatrixInverse(&matView, nullptr, &matView);
	pEffect->SetMatrix("g_matViewInv", &matView);
	D3DXMatrixInverse(&matProj, nullptr, &matProj);
	pEffect->SetMatrix("g_matProjInv", &matProj);
		

		pEffect->SetVector("g_vMtrilDiffuse", &_vec4{ 1.0f,1.0f,1.0f,1.0f });
		pEffect->SetVector("g_vMtrilAmbient", &_vec4{ 1.0f,1.0f,1.0f,1.0f });

		pEffect->SetVector("g_vLightDiffuse", (_vec4*)&m_tLightInfo.Diffuse);
		pEffect->SetVector("g_vLightAmbient", (_vec4*)&m_tLightInfo.Ambient);

		pEffect->CommitChanges();

		pEffect->BeginPass(iPassIdx);

		m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
		m_pGraphicDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
		m_pGraphicDev->SetIndices(m_pIB);
		m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

		pEffect->EndPass();
	}


