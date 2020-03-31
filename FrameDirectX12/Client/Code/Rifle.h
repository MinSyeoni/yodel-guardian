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

private:
	_float m_fRight;
	_float m_fUp;
	_float m_fLook;

	ANISTATE m_eCurAniState = COLLAPSEBASE;
	ANISTATE m_ePreAniState = COLLAPSEBASE;
private:
	virtual void			Free();

};