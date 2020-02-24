#include "LightMgr.h"
#include "Export_Function.h"

USING(Engine)
IMPLEMENT_SINGLETON(CLightMgr)

Engine::CLightMgr::CLightMgr(void)
{

}

Engine::CLightMgr::~CLightMgr(void)
{
	Free();
}

void Engine::CLightMgr::Free(void)
{
	for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
	m_LightList.clear();
}

HRESULT Engine::CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLight,
	const _uint& iIndex,CLight::STYLE _Style,_vec3 Pos)
{
	CLight*		pInstance = CLight::Create(pGraphicDev, pLight, iIndex,_Style,Pos);

	NULL_CHECK_RETURN(pInstance, E_FAIL);

	m_LightList.push_back(pInstance);

	return S_OK;
}

const D3DLIGHT9* Engine::CLightMgr::Get_LightInfo(const _uint& iIndex /*= 0*/)
{
	auto	iter = m_LightList.begin();

	for (_uint i = 0; i < iIndex; ++i)
		++iter;

	return (*iter)->Get_LightInfo();
}

void Engine::CLightMgr::Render_Light(LPD3DXEFFECT pEffect)
{
	for (auto& iter : m_LightList)
	{
		iter->Render(pEffect);
	}
}

