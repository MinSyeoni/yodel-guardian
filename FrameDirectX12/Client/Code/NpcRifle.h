#pragma once
#include "Include.h"
#include "Weapon.h"
class CShepard;

class CNpcRifle :public CWeapon
{

public:
	enum ANISTATE { BASE, COLLAPSE, COLLAPSEBASE, EXPAND };
	enum OWNER {SHEPARD,KEN};
private:
	explicit CNpcRifle(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CNpcRifle(const CNpcRifle& rhs);
	virtual ~CNpcRifle() = default;


public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(OWNER eOwner);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);

	virtual HRESULT AddComponent();
	void  FallowNPC();
	void AniCheck();
	void LightCheck(const _float& fTimeDelta);
	virtual void CreateShootEffect() override;
	void SetFinish() { m_bIsFinish = true; }
	OWNER Get_Owner() { return m_eOwner; };
public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CNpcRifle* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	CShepard* m_pShepard;

	_matrix* m_pNpcEquipMatrix;//착용용

	_matrix* m_pHandleMatrix;//총 손잡이
	_matrix* m_pNpcMatrix;// 플레이어

	_matrix* m_pFireMatrix;
	_matrix* m_pFireMatrixOffset;

	_bool m_bIsFinish = false;

private:
	_float m_fRight;
	_float m_fUp;
	_float m_fLook;

	OWNER m_eOwner = SHEPARD;
	ANISTATE m_eCurAniState = BASE;
	ANISTATE m_ePreAniState = BASE;
private:
	_float m_fLightAccTime = 0.f;
	_uint m_uiLightIndex;
	_bool m_bIsLight = false;
	D3DLIGHT m_tagLight;
	_vec3 m_vLightPos;


private:
	virtual void			Free();

};