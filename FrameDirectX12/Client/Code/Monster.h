#pragma once
#include "Include.h"
#include "GameObject.h"

namespace Engine
{
	class CMesh;
	class CShader_Mesh;
}

class CDynamicCamera;

class CMonster : public Engine::CGameObject
{
public:
	enum MONSTATE { MON_IDLE, MON_ATTACK, MON_HIT, MON_DEAD, MON_END };

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

private:
	void					Set_ConstantTable();
	void					Set_ShadowTable(CShader_Shadow* pShader);


private:
	virtual HRESULT			Add_Component();

private:
	Engine::CMesh*			m_pMeshCom = nullptr;
	Engine::CShader_Mesh*	m_pShaderCom = nullptr;
	CDynamicCamera*			m_pDynamicCamera = nullptr;

private:
	MONSTATE				m_eCurState = MON_IDLE;
	MONSTATE				m_ePreState = MON_IDLE;
	float					m_fTime = 0.f;
	float					m_fSpineAngle = 0.f;

	MeshInfo				m_tMeshInfo;

	vector<vector<_matrix>> m_vecMatrix;

public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static	CMonster*		Create(ID3D12Device* pGraphicDevice,ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

