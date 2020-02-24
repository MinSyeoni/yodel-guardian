#include "FontMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFontMgr)

Engine::CFontMgr::CFontMgr(void)
{

}

Engine::CFontMgr::~CFontMgr(void)
{
	Free();
}

HRESULT Engine::CFontMgr::Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _tchar* pFontTag, 
									const _tchar* pFontType, 
									const _uint& iWidth, 
									const _uint& iHeight, 
									const _uint& iWeight)
{
	CFont*		pFont = NULL;

	pFont = Find_Font(pFontTag);

	if (nullptr != pFont)
		return E_FAIL;

	pFont = CFont::Create(pGraphicDev, pFontType, iWidth, iHeight, iWeight);
	
	if (nullptr == pFont)
		return E_FAIL;

	m_mapFont.emplace(pFontTag, pFont);

	return S_OK;
}

void Engine::CFontMgr::Render_Font(const _tchar* pFontTag, 
									const _tchar* pString, 
									const _vec2* pPos, 
									D3DXCOLOR Color)
{
	CFont*	pFont = Find_Font(pFontTag);

	if (nullptr == pFont)
		return;

	pFont->Render_Font(pString, pPos, Color);
}

Engine::CFont* Engine::CFontMgr::Find_Font(const _tchar* pFontTag)
{
	auto	iter = find_if(m_mapFont.begin(), m_mapFont.end(), CTag_Finder(pFontTag));

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}

void Engine::CFontMgr::Free(void)
{
	for_each(m_mapFont.begin(), m_mapFont.end(), CDeleteMap());
	m_mapFont.clear();
}

