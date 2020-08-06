#pragma once
#include "Include.h"
#include "GameObject.h"


class CReapearTube : public Engine::CGameObject
{
public:
	enum TUBEPOS{LEFT,RIGHT};


private:
	explicit CReapearTube(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CReapearTube(const CReapearTube& rhs);
	virtual ~CReapearTube();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(int iType);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	   

	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);



	void SetDamage();

private:
	TUBEPOS m_eTubePos;
	CTransform* m_pReapear;
	_matrix* m_pBoneMatrix;
	_matrix* m_pBoneMatrixOffset;
	float m_fAccTime = 0.f;
	_bool   m_bIsDrop=false;
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
	void            Set_ShadowTable(CShader_Shadow* pShader);
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Mesh* m_pShaderCom = nullptr;
	Engine::CBoxCollider* m_pBoxCom = nullptr;

public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CReapearTube* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

