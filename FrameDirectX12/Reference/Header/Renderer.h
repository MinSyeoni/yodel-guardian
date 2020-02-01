#pragma once
#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CGameObject;
class CComponentMgr;

class ENGINE_DLL CRenderer : public CBase
{
	DECLARE_SINGLETON(CRenderer)

public:
	enum RENDERGROUP 
	{
		RENDER_PRIORITY,
		RENDER_NONALPHA,
		RENDER_ALPHA,
		RENDER_UI,
		RENDER_END
	};

private:
	explicit CRenderer();
	virtual ~CRenderer();

public:
	HRESULT	Ready_Renderer(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	HRESULT	Add_Renderer(const RENDERGROUP& eRenderID, CGameObject* pGameObject);
	void	Render_Renderer(const _float& fTimeDelta);
private:
	void	Render_Priority(const _float& fTimeDelta);
	void	Render_NonAlpha(const _float& fTimeDelta);
	void	Render_Alpha(const _float& fTimeDelta);
	void	Render_UI(const _float& fTimeDelta);
public:
	void	Clear_RenderGroup();

private:
	HRESULT	Ready_ShaderPrototype();

private:
	ID3D12Device*				m_pGraphicDevice	= nullptr;
	ID3D12GraphicsCommandList*	m_pCommandList		= nullptr;
	CComponentMgr*				m_pComponentMgr		= nullptr;

	list<CGameObject*>			m_RenderList[RENDER_END];

private:
	virtual void		Free();
};

END