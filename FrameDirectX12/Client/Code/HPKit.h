#pragma once
#include "Include.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "DirectInput.h"

namespace Engine
{
	class CBoxCollider;
	class CStaticMesh;
	class CShader_Mesh;
}

class CDynamicCamera;

class CHPKit : public Engine::CGameObject
{
public:
	enum HPKIT_STATE { KIT_IDLE, KIT_OPEN, KIT_CLOSE, KIT_ALREADYOPEN};

private:
	explicit CHPKit(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CHPKit(const CHPKit& rhs);
	virtual ~CHPKit();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);
	virtual void				SetMeshInfo(MeshInfo tInfo) { m_tMeshInfo = tInfo; }

private:
	void						Get_EquipUI();
	virtual HRESULT				Add_Component();
	void						Set_ConstantTable();
    void						Set_ShadowTable(CShader_Shadow* pShader);
	void						Render_ShadowDepth(CShader_Shadow* pShader);

	void						HPKit_Ani();
	void						Open_TheKit();

private:
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*       m_pShaderCom = nullptr;
	Engine::CBoxCollider*		m_pBoxCollider = nullptr;
	CDynamicCamera*				m_pCamera = nullptr;

	MeshInfo					m_tMeshInfo;
	vector<vector<_matrix>>		m_vecMatrix;
	_float						m_fAniTime = 0.f;
	_float						m_fAniDelay = 0.f;
	CGameObject*				m_pGameObject = nullptr;

	HPKIT_STATE					m_eState = KIT_IDLE;
	_bool						m_bIsOpen = false;
	_uint						m_iMeshID = 0;

public:
	virtual CGameObject*		Clone_GameObject(void* prg);
	static CHPKit*				Create(ID3D12Device* pGraphicDevice,
								ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void				Free();
};

