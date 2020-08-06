#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CStaticMesh;
	class CShader_Mesh;
	class CBoxCollider;
}


typedef struct tageBullet
{
	int eType;
	_vec3  vPos;
	_vec3  vDir;

}REAPERBULLETINFO;

class CReapeorBullet : public Engine::CGameObject
{
	public:
	enum BUlletType {FIRSTSHOT,SECONDSHOT};

private:
	explicit CReapeorBullet(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CReapeorBullet(const CReapeorBullet& rhs);
	virtual ~CReapeorBullet();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(REAPERBULLETINFO eInfo);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);

	void SetBulletType(_vec3 vPos, _vec3 vDir, BUlletType eType);

	void  CreateSecondBullet();
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();

	void Moving(const float& fTimeDelta);
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Mesh* m_pShaderCom = nullptr;
	Engine::CBoxCollider* m_pBoxCom = nullptr;

	_vec3   m_vDir = _vec3(0.f, 0.f, 0.f);
	_float  m_fAccTime = 0.f;
	BUlletType m_eType = FIRSTSHOT;


public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CReapeorBullet* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

