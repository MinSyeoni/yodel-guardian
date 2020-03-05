#pragma once
#include "Include.h"
#include "Weapon.h"


class CPistol :public CWeapon
{

public:
	enum STATE { BASE, COLLAPSE, COLLAPSEBASE, EXPAND };
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


public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CPistol*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);


private:
	virtual void			Free();
};