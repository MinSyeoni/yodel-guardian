#pragma once
#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CMesh;
	class CShader_Dissolve;
	class CBoxCollider;
}

class CDynamicCamera;

class CPassageDoor : public Engine::CGameObject
{
public:
	enum PASSAGE_STATE { PASSAGE_IDLE, PASSAGE_ALREADYOPEN, PASSAGE_OPEN, PASSAGE_CLOSE};

private:
	explicit CPassageDoor(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CPassageDoor(const CPassageDoor& rhs);
	virtual ~CPassageDoor();

public:
	_bool					Get_LobbyDoorIsOpen() { return m_bIsOpen; }
	Engine::CBoxCollider*	Get_DoorCollider() { return m_pBoxCol; }
	void					Set_IsCardToDoor(_bool bIsCard) { m_bIsCardKey = bIsCard; }

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
	void					PassageDoor_AniState();
	void					ColiisionTheDoor();
	void					OpenTheDoor();

private:
	virtual HRESULT			Add_Component();

private:
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*		m_pShaderCom = nullptr;
	Engine::CBoxCollider*		m_pBoxCol = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

private:
	PASSAGE_STATE				m_eDoorState = PASSAGE_IDLE;
	float						m_fSpineAngle = 0.f;

	MeshInfo					m_tMeshInfo;
	vector<vector<_matrix>>		m_vecMatrix;
	_float						m_fAniTime = 0.f;
	_float						m_fAniDelay = 0.f;

	_bool						m_bIsOpen = false;
	_bool						m_bIsCollision = false;
	_bool						m_bIsCardKey = false;

public:
	virtual CGameObject*		Clone_GameObject(void* prg);
	static	CPassageDoor*		Create(ID3D12Device* pGraphicDevice,ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void				Free();
};

