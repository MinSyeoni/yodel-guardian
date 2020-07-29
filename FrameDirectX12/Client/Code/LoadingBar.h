#pragma once
#include "Include.h"
#include "GameObject.h"




class CLoadingBar : public Engine::CGameObject
{
private:
	explicit CLoadingBar(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CLoadingBar(const CLoadingBar& rhs);
	virtual ~CLoadingBar();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);

	void SetGauge(float fGaugePercent) { m_fNewGaugeBar = (fGaugePercent); }

private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CRcTex* m_pBufferCom = nullptr;
	Engine::CShader_UI* m_pShaderCom = nullptr;
	Engine::CTexture* m_pTexture = nullptr;
	/*____________________________________________________________________
	[ Value ]
	______________________________________________________________________*/

	float TestX = 0.2f;
	float TestY = 0.2f;


	float m_fNewGaugeBar = 0.3f;

	float m_fGaugeBar = 0.3f;

public:
	virtual CGameObject* Clone_GameObject(void* pArg);
	static CLoadingBar* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void			Free();
};

