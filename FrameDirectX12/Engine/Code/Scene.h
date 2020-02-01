#pragma once
#include "Engine_Include.h"
#include "Base.h"
#include "ObjectMgr.h"
#include "Renderer.h"

BEGIN(Engine)

class ENGINE_DLL CScene : public CBase
{
protected:
	explicit CScene(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CScene();

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene(const _float& fTimeDelta);

protected:
	/*____________________________________________________________________
	[ GraphicDevice / Mgr ]
	______________________________________________________________________*/
	ID3D12Device*				m_pGraphicDevice	= nullptr;
	ID3D12GraphicsCommandList*	m_pCommandList		= nullptr;
	CObjectMgr*					m_pObjectMgr		= nullptr;
	CRenderer*					m_pRenderer			= nullptr;


protected:
	virtual void Free();
};

END