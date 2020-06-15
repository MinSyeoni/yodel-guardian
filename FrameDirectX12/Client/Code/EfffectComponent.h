#pragma once
#include "Include.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "Effect.h"





class CEffectComponent : public Engine::CGameObject
{
private:
	explicit CEffectComponent(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CEffectComponent(const CEffectComponent& rhs);
	virtual ~CEffectComponent();

public:
	HRESULT						Ready_GameObjectPrototype(EFFECTDATA tData, _vec3 vStartPos);
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);
	void UpdateAnimation();
	void BillBoard();
	void SpriteAnimation();
	void RotationAnimation();
	void ScaleAnimation();
	void FadeInAnimation();
	void FadeOutAnimation();
private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable();

private:
	EFFECTDATA m_tEffectData;
	float m_fAccTime = 0.f;
	float m_fSpriteAnimation = 0.f;
	int m_iSpriteRow = 0;

	
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CRcTex* m_pBufferCom = nullptr;
	Engine::CShader_Effect* m_pShaderCom = nullptr;
	Engine::CTexture* m_pTexture = nullptr;
	float m_fChapterX = 1.f;
	float m_fChapterY = 1.f;
	float m_fAlpha = 1.f;
	_float m_fRandZ = 0.f;
public:
	virtual CGameObject* Clone_GameObject(void* pArg);
	static CEffectComponent* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList,EFFECTDATA tData,_vec3 vStartPos);
private:
	virtual void				Free();
};
