#pragma once
#include "Include.h"
#include "GameObject.h"


class CWeapon : public CGameObject
{

public:
	enum WEAPONSTATE { DROP, EQUIP, BAG };

	enum WEAPONTYPE { PISTOL, RIFLE, SNIPER,NpcRifle };
protected:
	explicit CWeapon(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CWeapon(const CWeapon& rhs);
	virtual ~CWeapon();

public:
	virtual HRESULT AddComponent();

	 virtual void CreateShootEffect();

	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);

	void SetWeaponState(WEAPONSTATE eState) { m_eWeaponState = eState; };
	WEAPONTYPE Get_WeaponType() { return m_eWeaponType; };
	WEAPONSTATE Get_WeaponState(){return m_eWeaponState;};
private:
	void			Set_ConstantTable();
	void            Set_ShadowTable(CShader_Shadow* pShader);

protected:
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*       m_pShaderCom = nullptr;

protected:
	vector<vector<_matrix>> m_vecBoneMatirx;   

	WEAPONSTATE m_eWeaponState = BAG;

	WEAPONTYPE m_eWeaponType = RIFLE;

	_float m_iCurBullet;
	_float m_iMaxBullet;


protected:
	virtual void			Free();


};