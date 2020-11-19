#pragma once
#include "ComponentMgr.h"
#include "Renderer.h"

BEGIN(Engine)

class CObjectMgr;
class CShader_Shadow;
class CShader_LimLight;
class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	CComponent* Get_Component(wstring wstrComponentTag, COMPONENTID eID);
	CTransform* Get_Transform() { return m_pTransCom; }
	CInfo* Get_Info() { return m_pInfoCom; }
	_float			Get_DepthOfView() { return m_fViewZ; }
	_long			Get_UIDepth() { return m_UIDepth; }
public:
	void			Dead_GameObject() { m_bIsDead = true; }
public:
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_Distortion(const _float& fTimeDelta);
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);
	virtual void    Render_LimLight(CShader_LimLight* pShader);
protected:
	virtual HRESULT Add_Component();
	void			Compute_ViewZ(const _vec3* pPosInWorld);
private:
	CComponent* Find_Component(wstring wstrComponentTag, const COMPONENTID& eID);

protected:
	/*____________________________________________________________________
	[ GraphicDevice / Mgr / Component ]
	______________________________________________________________________*/
	ID3D12Device* m_pGraphicDevice = nullptr;
	ID3D12GraphicsCommandList* m_pCommandList = nullptr;
	CRenderer* m_pRenderer = nullptr;
	CObjectMgr* m_pObjectMgr = nullptr;
	CComponentMgr* m_pComponentMgr = nullptr;

	CTransform* m_pTransCom = nullptr;
	CInfo* m_pInfoCom = nullptr;
	unordered_map< wstring, CComponent*>	m_mapComponent[COMPONENTID::ID_END];

	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
	_bool	m_bIsDead = false;
	_bool	m_bIsLateInit = false;
	_float	m_fViewZ = 0.f;
	_long	m_UIDepth = 0;

public:
	virtual CGameObject* Clone_GameObject(void* prg) PURE;
protected:
	virtual void			Free();
};

END