#pragma once
#include "Include.h"
#include "GameObject.h"
#include "BoxCollider.h"

namespace Engine
{
	class CMesh;
	class CShader_Dissolve;
	class CBoxCollider;
}

class CDynamicCamera;

class CCardReader : public Engine::CGameObject
{
private:
	explicit CCardReader(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CCardReader(const CCardReader& rhs);
	virtual ~CCardReader();

public:
	void					Set_CardReaderOn(_bool bIsOn) { m_bIsReaderOn = bIsOn; }
	_bool					Get_CardReaderOn() { return m_bIsReaderOn; }
	void					Set_CardKeyEquip(_bool bIsEquip) { m_bIsEquipCard = bIsEquip; }

public:
	HRESULT					Ready_GameObjectPrototype();
	virtual HRESULT			Ready_GameObject();
	virtual HRESULT			LateInit_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject(const _float& fTimeDelta);
	virtual void			Render_ShadowDepth(CShader_Shadow* pShader);

private:
	void					Set_ConstantTable();
	void					Set_ShadowTable(CShader_Shadow* pShader);
	virtual HRESULT			Add_Component();
	void					Collision_CardReader();

private:
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*		m_pShaderCom = nullptr;
	Engine::CBoxCollider*		m_pBoxCol = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

private:
	MeshInfo					m_tMeshInfo;

	CGameObject*				m_pGameObject = nullptr;
	_bool						m_bIsReaderOn = false;
	_bool						m_bIsCollision = false;
	_bool						m_bIsEquipCard = false;

public:
	virtual CGameObject*		Clone_GameObject(void* prg);
	static	CCardReader*		Create(ID3D12Device* pGraphicDevice,ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void				Free();
};

