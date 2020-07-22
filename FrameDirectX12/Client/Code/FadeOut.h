#pragma once
#include "Include.h"
#include "GameObject.h"


class CFadeOut : public Engine::CGameObject
{
public:
	enum FADETYPE{FADEOUT,FADEIN,FADEOUTIN,FIRST,SECOND};

private:
	explicit CFadeOut(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CFadeOut(const CFadeOut& rhs);
	virtual ~CFadeOut();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(FADETYPE tType);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);

	void CheckFadeIn();
	void CheckFadeOut();
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CRcTex* m_pBufferCom = nullptr;
	Engine::CShader_Effect* m_pShaderCom = nullptr;
	Engine::CTexture* m_pTexture = nullptr;
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/
	float m_fTime = 0.f;
	FADETYPE m_eType = FADEOUTIN;
	_bool m_bIsReturn = false;
public:
	virtual CGameObject* Clone_GameObject(void* pArg);
	static CFadeOut* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void			Free();
};

