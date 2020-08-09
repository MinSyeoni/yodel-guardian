#pragma once
#include "Include.h"
#include "Weapon.h"

class CPlayerArm;
class CPlayerLeg;
class CSniper :public CWeapon
{

public:
	enum ANISTATE { BASE, COLLAPSE, COLLAPSEBASE, EXPAND };
private:
	explicit CSniper(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CSniper(const CSniper& rhs);
	virtual ~CSniper() = default;


public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(WEAPONSTATE eState);
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
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CSniper* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	CPlayerArm* m_pPlayerArm;

	_matrix* m_pPlayerEquipMatrix;//착용용

	_matrix* m_pPlayerBodyMatrixOffset;//등판용
	_matrix* m_pPlayerBodyMatrix;//등판용

	_matrix* m_pHandleMatrix;//총 손잡이
	_matrix* m_pPlayerMatrix;// 플레이어


	_matrix* m_pFireMatrix;
	_matrix* m_pFireMatrixOffset;

	_bool		m_bIsNextSound3 = false;	// 사운드용 축가가ㅏ
private:
	_float m_fRight;
	_float m_fUp;
	_float m_fLook;

	ANISTATE m_eCurAniState = COLLAPSEBASE;
	ANISTATE m_ePreAniState = COLLAPSEBASE;
private:
	_float m_fLightAccTime = 0.f;
	_uint m_uiLightIndex;
	_bool m_bIsLight = false;
	D3DLIGHT m_tagLight;
	_vec3 m_vLightPos;

	bool m_bIsLimLight = false;
private:
	virtual void			Free();

};