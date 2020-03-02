#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CRenderer)

Engine::CRenderer::CRenderer(void)
{

}

Engine::CRenderer::~CRenderer(void)
{
	Free();
}

void Engine::CRenderer::AddGroup(RENDERID eGroup, CGameObject* pGameObject)
{
	if (RENDER_END <= eGroup)
		return;

	m_RenderGroup[eGroup].push_back(pGameObject);

	pGameObject->AddRef();
}

void Engine::CRenderer::Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev)
{
	/*Render_Prioristy(pGraphicDev);*/

	Render_Defferd(pGraphicDev);
	Render_UI(pGraphicDev);
	Clear_RenderGroup();
}

void Engine::CRenderer::Clear_RenderGroup(void)
{
	for (_uint i = 0; i < RENDER_END; ++i)
	{
		for_each(m_RenderGroup[i].begin(), m_RenderGroup[i].end(), CDeleteObj());
		m_RenderGroup[i].clear();
	}
}

HRESULT Engine::CRenderer::Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	FAILED_CHECK_RETURN(pGraphicDev->CreateVertexBuffer(sizeof(VTXSCREEN) * 4,
		0,
		FVF_SCREEN,
		D3DPOOL_MANAGED,
		&m_pVB,
		NULL), E_FAIL);

	FAILED_CHECK_RETURN(pGraphicDev->CreateIndexBuffer(sizeof(INDEX16) * 2,
		0,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIB,
		NULL), E_FAIL);

	D3DVIEWPORT9		ViewPort;
	pGraphicDev->GetViewport(&ViewPort);

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

	return S_OK;
}

void CRenderer::Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev)
{

	
	for (auto iter : m_RenderGroup[RENDER_PRIORITY])
		iter->Render_Object();



}

void CRenderer::Render_NonAlpha(LPDIRECT3DDEVICE9 pGraphicDev)
{


	for (auto iter : m_RenderGroup[RENDER_NONALPHA])
		iter->Render_Object();

}

_bool		Compare_Z(CGameObject* pDest, CGameObject* pSrc)
{
	return pDest->Get_ViewZ() > pSrc->Get_ViewZ();
}


void CRenderer::Render_Alpha(LPDIRECT3DDEVICE9 pGraphicDev)
{
	
	m_RenderGroup[RENDER_ALPHA].sort(Compare_Z);
	for (auto iter : m_RenderGroup[RENDER_ALPHA])
		iter->Render_Object();




}
void CRenderer::Render_UI(LPDIRECT3DDEVICE9 pGraphicDev)
{

	

	for (auto iter : m_RenderGroup[RENDER_UI])
		iter->Render_Object();

	
}

void CRenderer::Render_Depth(LPDIRECT3DDEVICE9 pGraphicDev)
{
	/*Begin_MRT(L"MRT_Defferd");
	CShader*		pShader = dynamic_cast<Engine::CShader*>
		(Engine::Clone_Prototype(L"Proto_ShaderDepth"));
	if (nullptr == pShader)
		return;

	LPD3DXEFFECT		pEffect = pShader->Get_EffectHandle();


	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pShader);



	End_MRT(L"MRT_Defferd");*/
}

void Engine::CRenderer::Render_Blend(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CShader*		pShader = dynamic_cast<Engine::CShader*>
		(Engine::Clone_Prototype(L"Proto_ShaderBlend"));
	if (nullptr == pShader)
		return;

	LPD3DXEFFECT		pEffect = pShader->Get_EffectHandle();

	Engine::Set_OnShader(pEffect, L"Target_Albedo","g_AlbedoTexture");
	Engine::Set_OnShader(pEffect, L"Target_Shade","g_ShaderTexture");
	Engine::Set_OnShader(pEffect, L"Target_GodRay","g_FlashTexture");
	Engine::Set_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	Engine::Set_OnShader(pEffect, L"Target_Lim", "g_LimTexture");

	pEffect->Begin(NULL, 0);
	pEffect->BeginPass(0);

	pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXSCREEN));
	pGraphicDev->SetFVF(D3DFVF_XYZRHW | D3DFVF_TEX1);
	pGraphicDev->SetIndices(m_pIB);
	pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);

	pEffect->EndPass();
	pEffect->End();

	Safe_Release(pShader);

}

void CRenderer::Render_Flash(LPDIRECT3DDEVICE9 pGraphicDev)
{

	Engine::Begin_MRT(L"MRT_GodRay");

	for (auto iter : m_RenderGroup[RENDER_FLASH])
		iter->Render_Object();

	Engine::End_MRT(L"MRT_GodRay");
}

void CRenderer::Render_Player(LPDIRECT3DDEVICE9 pGraphicDev)
{

	for (auto iter : m_RenderGroup[RENDER_PLAYER])
		iter->Render_Object();
}

void CRenderer::Render_Light(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Begin_MRT(L"MRT_ConeLight");
	for (auto iter : m_RenderGroup[RENDER_LIGHT])
		iter->Render_Object();
	Engine::End_MRT(L"MRT_ConeLight");
}

void CRenderer::Render_Terrain(LPDIRECT3DDEVICE9 pGraphicDev)
{
	for (auto iter : m_RenderGroup[RENDER_TERRAIN])
		iter->Render_Object();
}



void Engine::CRenderer::Render_LightAcc(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Engine::Begin_MRT(L"MRT_LightAcc");

	CShader*		pShader = dynamic_cast<Engine::CShader*>
		(Engine::Clone_Prototype(L"Proto_ShaderShade"));
	if (nullptr == pShader)
		return;

	LPD3DXEFFECT		pEffect = pShader->Get_EffectHandle();

	Engine::Set_OnShader(pEffect, L"Target_Normal", "g_NormalTexture");
	Engine::Set_OnShader(pEffect, L"Target_Depth", "g_DepthTexture");
	pEffect->Begin(NULL, 0);

	Engine::Render_Light(pEffect);




	pEffect->End();

	Engine::End_MRT(L"MRT_LightAcc");

	Safe_Release(pShader);
}

void Engine::CRenderer::Render_Defferd(LPDIRECT3DDEVICE9 pGraphicDev)
{

	Render_Priority(pGraphicDev);
	Render_NonAlpha(pGraphicDev);
	Render_Terrain(pGraphicDev);
	Render_Alpha(pGraphicDev);

	Render_Player(pGraphicDev);
}


void Engine::CRenderer::Free(void)
{
	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

	Clear_RenderGroup();
}

