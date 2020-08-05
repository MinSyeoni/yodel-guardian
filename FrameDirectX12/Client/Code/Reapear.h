#pragma once
#include "Include.h"
#include "GameObject.h"


class CReapear : public Engine::CGameObject
{
public:
	enum ANISTATE{ ADOVEPLATFORM,FOREATTACK,LEFTATTACK,RIGHTATTACK,BELOWPLAT,ENTRANCE,EXIT,IDLEINTIMIDATE,IDLERESLE,STATICPOS,STATICATTACK,STTATICDAMEGE};


private:
	explicit CReapear(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CReapear(const CReapear& rhs);
	virtual ~CReapear();

public:

	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_LimLight(CShader_LimLight* pShader);
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
	void            Set_LimTable(CShader_LimLight* pShader);
private:
	void            UpdateLight(const _float& fTimeDelta);



	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Mesh* m_pShaderCom = nullptr;

	MeshInfo         m_tMeshInfo;

	float m_fTime = 0.f;
	vector<vector<_matrix>> m_vecMatrix;
	_int  m_iHp = 30.f;
	
	ANISTATE m_eCurAniState;
	ANISTATE m_ePreAniState;

private:

	_matrix* m_pLeftEyeBone = nullptr;
	_matrix* m_pRightEyeBone = nullptr;

	_matrix* m_pLeftEyeOffset = nullptr;
	_matrix* m_pRightEyeOffset = nullptr;

	_matrix* m_pBodyBone = nullptr;
	_matrix* m_pBodyOffset = nullptr;


	D3DLIGHT m_tagSpotLightEye;
	D3DLIGHT m_tagSpotLightBody;

	_uint m_uiSpotLightLeftEyeIndex = 0;
	_uint m_uiSpotLightRightEyeIndex = 0;

	_uint m_uiSpotLightBodyIndex = 0;


public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CReapear* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

