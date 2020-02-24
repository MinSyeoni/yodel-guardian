#include "Font.h"

USING(Engine)
Engine::CFont::CFont(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
	, m_pFont(nullptr)
	, m_pSprite(nullptr)
{
	m_pGraphicDev->AddRef();
}

Engine::CFont::~CFont(void)
{

}

HRESULT Engine::CFont::Ready_Font(const _tchar* pFontType, 
								const _uint& iWidth, 
								const _uint& iHeight, 
								const _uint& iWeight)
{
	D3DXFONT_DESC			Font_Desc;
	ZeroMemory(&Font_Desc, sizeof(D3DXFONT_DESC));

	Font_Desc.CharSet = HANGEUL_CHARSET;
	lstrcpy(Font_Desc.FaceName, pFontType);
	Font_Desc.Width = iWidth;
	Font_Desc.Height = iHeight;
	Font_Desc.Weight = iWeight;

	FAILED_CHECK_RETURN(D3DXCreateFontIndirect(m_pGraphicDev, &Font_Desc, &m_pFont), E_FAIL);
	FAILED_CHECK_RETURN(D3DXCreateSprite(m_pGraphicDev, &m_pSprite), E_FAIL);

	return S_OK;
}

void Engine::CFont::Render_Font(const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	RECT		rcFont{ long(pPos->x), long(pPos->y) };

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pFont->DrawTextW(m_pSprite, pString, lstrlen(pString), &rcFont, DT_NOCLIP, Color);

	m_pSprite->End();
}

Engine::CFont* Engine::CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _tchar* pFontType, 
										const _uint& iWidth, 
										const _uint& iHeight, 
										const _uint& iWeight)
{
	CFont*	pInstance = new CFont(pGraphicDev);

	if (FAILED(pInstance->Ready_Font(pFontType, iWidth, iHeight, iWeight)))
	{
		MSG_BOX("Font Created Failed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CFont::Free(void)
{
	Safe_Release(m_pFont);
	Safe_Release(m_pSprite);
	Safe_Release(m_pGraphicDev);
}

