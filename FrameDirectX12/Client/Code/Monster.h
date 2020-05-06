#pragma once
#include "Include.h"
#include "GameObject.h"
#include "FlameThrower.h"
#include "Zombi.h"

namespace Engine
{
	class CMesh;
	class CShader_Mesh;
	class CBoxCollider;
	class CSphereCollider;
	class CNaviMesh;
}

class CDynamicCamera;

class CMonster : public Engine::CGameObject
{
public:
	enum MONKIND { NONAME, FLAMETHROWER, ZOMBI };

private:
	explicit CMonster(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CMonster(const CMonster& rhs);
	virtual ~CMonster();

public:
	HRESULT					Ready_GameObjectPrototype();
	virtual HRESULT			Ready_GameObject();
	virtual HRESULT			LateInit_GameObject();
	virtual _int			Update_GameObject(const _float& fTimeDelta);
	virtual _int			LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void			Render_GameObject(const _float& fTimeDelta);
	CZombi* Get_Zombi() { return m_pZombi; };//이거넣엇어
	MONKIND    Get_MONKIND() { return m_eMonName; }//이것두넣엇어

private:
	void					Set_ConstantTable();
	void					Render_ShadowDepth(CShader_Shadow* pShader);
	void					Set_ShadowTable(CShader_Shadow* pShader);
	void					Update_BoneCollider(CSphereCollider* pSphereCol, string strBoneName);

private:
	virtual HRESULT			Add_Component();

private:
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*		m_pShaderCom = nullptr;
	Engine::CBoxCollider*		m_pBoxCol = nullptr;
	Engine::CSphereCollider*	m_pShereCol[2] = {};
	Engine::CNaviMesh*			m_pNaviMesh = nullptr;
	CDynamicCamera*				m_pDynamicCamera = nullptr;

	CFlameThrower*				m_pFlameThrower = nullptr;
	
	list<CZombi*>				m_pZombiLst;
	CZombi*						m_pZombi = nullptr;

private:
	MONKIND						m_eMonName = NONAME;

	float						m_fSpineAngle = 0.f;

	MeshInfo					m_tMeshInfo;

	vector<vector<_matrix>>		m_vecMatrix;

	_vec3						m_pPlayerPos = _vec3(0.f, 0.f, 0.f);
	_vec3						m_pMonsterPos = _vec3(0.f, 0.f, 0.f);

public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static	CMonster*		Create(ID3D12Device* pGraphicDevice,ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

