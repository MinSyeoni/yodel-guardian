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

class CLobbyDoor : public Engine::CGameObject
{
public:
	enum DOORSTATE { DOOR_IDLE, DOOR_ALREADYOPEN, DOOR_OPEN, DOOR_CLOSE };

private:
	explicit CLobbyDoor(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CLobbyDoor(const CLobbyDoor& rhs);
	virtual ~CLobbyDoor();

public:
	_bool					Get_LobbyDoorIsOpen() { return m_bIsOpen; }

public:
	HRESULT					Ready_GameObjectPrototype();
	virtual HRESULT			Ready_GameObject();
	virtual HRESULT			LateInit_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	void OpenTheDoor();
	virtual _int			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject(const _float& fTimeDelta);

	virtual void			Render_ShadowDepth(CShader_Shadow* pShader);

private:
	void					Set_ConstantTable();
	void					Set_ShadowTable(CShader_Shadow* pShader);
	void					LobbyDoor_AniState();
	void					CollisionTheDoor();

private:
	virtual HRESULT			Add_Component();

private:
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*		m_pShaderCom = nullptr;
	Engine::CBoxCollider*		m_pBoxCol = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

private:
	DOORSTATE					m_eDoorState = DOOR_IDLE;
	float						m_fSpineAngle = 0.f;

	MeshInfo					m_tMeshInfo;
	vector<vector<_matrix>>		m_vecMatrix;
	_float						m_fAniTime = 0.f;
	_float						m_fAniDelay = 0.f;

	_bool						m_bIsOpen = false;
	_bool						m_bIsCollision = false;

	CGameObject*				m_pGameObject = nullptr;

public:
	virtual CGameObject*		Clone_GameObject(void* prg);
	static	CLobbyDoor*			Create(ID3D12Device* pGraphicDevice,ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void				Free();
};

