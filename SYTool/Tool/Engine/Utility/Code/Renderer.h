#ifndef Renderer_h__
#define Renderer_h__

#include "Engine_Defines.h"
#include "Component.h"

#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CRenderer : public CComponent
{
	DECLARE_SINGLETON(CRenderer)

private:
	explicit CRenderer(void);
	virtual ~CRenderer(void);

public:
	HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev);
	void	AddGroup(RENDERID eGroup, CGameObject* pGameObject);
	void	Render_GameObject(LPDIRECT3DDEVICE9 pGraphicDev);

	void	Clear_RenderGroup(void);

private:
	void	Render_Priority(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_NonAlpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_Alpha(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_UI(LPDIRECT3DDEVICE9 pGraphicDev);
	void    Render_Depth(LPDIRECT3DDEVICE9  pGrphicDev);
	void	Render_Defferd(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_LightAcc(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Render_Blend(LPDIRECT3DDEVICE9 pGraphicDev);
	void    Render_Flash(LPDIRECT3DDEVICE9 pGraphicDev);
	void    Render_Player(LPDIRECT3DDEVICE9 pGraphicDev);
	void    Render_Light(LPDIRECT3DDEVICE9 pGraphicDev);
	void    Render_Terrain(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	list<CGameObject*>		m_RenderGroup[RENDER_END];

	LPDIRECT3DVERTEXBUFFER9	m_pVB = nullptr;
	LPDIRECT3DINDEXBUFFER9	m_pIB = nullptr;

private:
	virtual void Free(void);

};


END
#endif // Renderer_h__
