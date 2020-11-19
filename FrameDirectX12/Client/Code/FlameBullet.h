#pragma once
#include "Include.h"
#include "GameObject.h"
#include "DirectInput.h"

namespace Engine
{
	class CBoxCollider;
	class CStaticMesh;
	class CShader_Mesh;
}
class CEffect;
class CDynamicCamera;

typedef struct FlameInfo
{
	float fSpeed=0.f;
	_vec3 vDir=_vec3(0.f,0.f,0.f);
	_vec3 vPos = _vec3(0.f, 0.f, 0.f );
	float flifeTime=5.f;


}FLAMEINFO;

class CFlameBullet : public Engine::CGameObject
{
private:
	explicit CFlameBullet(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CFlameBullet(const CFlameBullet& rhs);
	virtual ~CFlameBullet();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);
	virtual void				SetMeshInfo(MeshInfo tInfo) { m_tMeshInfo = tInfo; }
	void                        Move(const _float& fTimeDelta);
private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();
	void						Set_ShadowTable(CShader_Shadow* pShader);
	void						Render_ShadowDepth(CShader_Shadow* pShader);

private:
	Engine::CBoxCollider* m_pBoxCollider = nullptr;

	MeshInfo					m_tMeshInfo;
	CGameObject* m_pGameObject = nullptr;

	CEffect* m_pDronEffect = nullptr;


	_uint m_uiLightIndex;
	D3DLIGHT m_tagLight;
	FLAMEINFO m_tInfo;
	
	float m_fAccTime;
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CFlameBullet* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void				Free();
};

