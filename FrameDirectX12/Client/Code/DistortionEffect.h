#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CStaticMesh;
	class CShader_Mesh;
	class CBoxCollider;
	class CShader_Destortion;
}
class CDynamicCamera;

class CDistortionEffect : public Engine::CGameObject
{
private:
	explicit CDistortionEffect(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CDistortionEffect(const CDistortionEffect& rhs);
	virtual ~CDistortionEffect();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(_vec3 vPos);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_Distortion(const _float& fTimeDelta);
	virtual void    SetMeshInfo(MeshInfo tInfo) { m_tMeshInfo = tInfo; }

	void BillBoard();

private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTableDistort();
	void            Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Destortion* m_pDistortShaderCom = nullptr;
	MeshInfo         m_tMeshInfo;
	_float m_fTime = 0.f;
	_float m_fSize = 0.01f;

	_vec3 vPos;
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CDistortionEffect* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

