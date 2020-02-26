#include "pch.h"
#include "Terrain.h"
#include "Export_Function.h"


CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CTerrain::~CTerrain(void)
{
	Free();
}

HRESULT CTerrain::Ready_Object(void)
{
	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

	m_eState = STATE_SOLID;

	_int iVtxCntX = 0, iVtxCntZ = 0;
	iVtxCntX = dynamic_cast<Engine::CTerrainTex*>(m_pBufferCom)->Get_VtxCntX();
	iVtxCntZ = dynamic_cast<Engine::CTerrainTex*>(m_pBufferCom)->Get_VtxCntZ();

//	m_pGraphicDev->CreateTexture(iVtxCntX, iVtxCntZ, 1, 0, D3DFMT_A8B8G8R8, D3DPOOL_MANAGED, &m_pTerrainTex, NULL);
	
	D3DLOCKED_RECT		LockRect;
	ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

	//m_pTerrainTex->LockRect(0, &LockRect, NULL, 0);
	//DWORD* pDwColor = (DWORD*)LockRect.pBits;
	//int iWidth = iVtxCntX;
	//int iHeight = iVtxCntZ;
	//for (int y = 0.f; y < iHeight; ++y)
	//{
	//	for (int x = 0.f; x < iWidth; ++x)
	//	{
	//		int iIdx = y * iWidth + x;

	//		D3DXCOLOR tColor = pDwColor[iIdx];

	//		tColor.a = 0.0f;
	//		tColor.r = 1.0f;
	//		tColor.g = 1.0f;
	//		tColor.b = 1.0f;

	//		pDwColor[iIdx] = tColor;
	//	}
	//}
	//m_pTerrainTex->UnlockRect(0);

	return S_OK;
}

_int CTerrain::Update_Object(const _float& fTimeDelta)
{
	Engine::CGameObject::Update_Object(fTimeDelta);

	return 0;
}

void CTerrain::Render_Object(void)
{
	LPD3DXEFFECT tEffect = m_pShaderCom->Get_EffectHandle();
	NULL_CHECK(tEffect);
	FAILED_CHECK_RETURN(Set_ConstantTable(tEffect), );

	if (FAILED(Set_Material()))
		return;

	if (m_eState == STATE_WIRE)
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	else
		m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransCom->Get_WorldMatrix());
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	_uint uiPass = 0;
	tEffect->Begin(&uiPass, 0);
	tEffect->BeginPass(1);

	m_pBufferCom->Render();

	tEffect->EndPass();
	tEffect->End();
}

HRESULT CTerrain::Add_Component(void)
{
	Engine::CComponent*	pComponent = nullptr;

	// BufferCom
	pComponent = m_pBufferCom = dynamic_cast<Engine::CTerrainTex*>
		(Engine::Clone_Resources(RESOURCE_STATIC, L"Buffer_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Buffer", pComponent);

	// TransCom;
	pComponent = m_pTransCom = Engine::CTransform::Create();
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_DYNAMIC].emplace(L"Com_Transform", pComponent);

	// TextureCom
	pComponent = m_pTextureCom = dynamic_cast<Engine::CTexture*>
		(Engine::Clone_Resources(RESOURCE_STAGE, L"Texture_Terrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Texture", pComponent);

	// CalculatorCom
	pComponent = m_pCalculCom = Engine::CCalculator::Create(m_pGraphicDev);
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Calculator", pComponent);

	// ShaderCom
	pComponent = m_pShaderCom = dynamic_cast<Engine::CShader*>(Engine::Clone_Prototype(L"Proto_ShaderToolTerrain"));
	NULL_CHECK_RETURN(pComponent, E_FAIL);
	m_mapComponent[Engine::ID_STATIC].emplace(L"Com_Shader", pComponent);

	return S_OK;
}

HRESULT CTerrain::Set_Material(void)
{
	D3DMATERIAL9		tMtrlInfo;
	ZeroMemory(&tMtrlInfo, sizeof(D3DMATERIAL9));

	tMtrlInfo.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
	tMtrlInfo.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
	tMtrlInfo.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);
	tMtrlInfo.Power = 0.f;

	m_pGraphicDev->SetMaterial(&tMtrlInfo);
	return S_OK;
}

HRESULT CTerrain::Set_ConstantTable(LPD3DXEFFECT pEffect)
{
	_matrix matWorld, matView, matProj;

	m_pTransCom->Get_WorldMatrix2(&matWorld);
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matProj);

	pEffect->SetMatrix("g_matWorld", &matWorld);
	pEffect->SetMatrix("g_matView", &matView);
	pEffect->SetMatrix("g_matProj", &matProj);

	m_pTextureCom->Set_Texture(pEffect, "g_BaseTexture", 0);

	D3DXMatrixInverse(&matView, NULL, &matView);

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTerrain* pInstance = new CTerrain(pGraphicDev);

	if (FAILED(pInstance->Ready_Object()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrain::Free(void)
{
	//Engine::Safe_Release(m_pTerrainTex);
	Engine::CGameObject::Free();
}