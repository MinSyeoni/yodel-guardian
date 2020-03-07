#pragma once
#include "Include.h"
#include "Weapon.h"

class CPlayerArm;
class CPlayerLeg;
class CPistol :public CWeapon
{

public:
	enum ANISTATE { BASE, COLLAPSE, COLLAPSEBASE, EXPAND };
private:
	explicit CPistol(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CPistol(const CPistol& rhs);
	virtual ~CPistol()=default;


public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	
	virtual HRESULT AddComponent();
	void  FallowPlayer();

public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CPistol*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	CPlayerArm*  m_pPlayerArm;
	CPlayerLeg*  m_pPlayerLeg;

	_matrix*  m_pPlayerArmMatrix;//상하체해서 둘다 가져와야함.
	_matrix*  m_pPlayerLegMatrix; //상하체해서 둘다 가져와야함.
	_matrix*  m_pHandleMatrix;
	_matrix*  m_pPlayerMatrix;
private:
	virtual void			Free();

};