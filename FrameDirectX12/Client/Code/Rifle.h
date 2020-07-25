#pragma once
#include "Include.h"
#include "Weapon.h"

class CPlayerArm;
class CPlayerLeg;
class CRifle :public CWeapon
{

public:
	enum ANISTATE { BASE, COLLAPSE, COLLAPSEBASE, EXPAND };
private:
	explicit CRifle(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CRifle(const CRifle& rhs);
	virtual ~CRifle() = default;


public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);

	virtual HRESULT AddComponent();
	void  FallowPlayer();
	void  FallowBag();
	void AniCheck();
	void DropCheck();
	void LightCheck(const _float& fTimeDelta);
	virtual void CreateShootEffect() override;
	void SpotLightCheck();
	void            ShowEquipUI(bool bIsRender);
public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CRifle*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	CPlayerArm*  m_pPlayerArm;

	_matrix*  m_pPlayerEquipMatrix;//착용용



	_matrix*  m_pPlayerBodyMatrixOffset;//등판용
	_matrix*  m_pPlayerBodyMatrix;//등판용

	_matrix*  m_pHandleMatrix;//총 손잡이
	_matrix*  m_pPlayerMatrix;// 플레이어


	_matrix* m_pFireMatrix;
	_matrix* m_pFireMatrixOffset;


private:
	_float m_fRight;
	_float m_fUp;
	_float m_fLook;

	ANISTATE m_eCurAniState = COLLAPSEBASE;
	ANISTATE m_ePreAniState = COLLAPSEBASE;
private:
	_float m_fLightAccTime=0.f;
	_uint m_uiLightIndex;
	_bool m_bIsLight = false;
	D3DLIGHT m_tagLight;
	_vec3 m_vLightPos;
private:
	D3DLIGHT m_tagSpotLight;
	_vec3 m_vSpotLightPos;
	_bool m_bIsSpotLight = false;
	_uint m_uiSpotLightIndex;
	_vec3 m_vLightDir;


	_bool m_bIsLimLight=false;

private:
	virtual void			Free();

};