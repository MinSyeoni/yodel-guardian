#pragma once
#include "Include.h"
#include "GameObject.h"



class CPlayerArm;
class CPlayerLeg;
class CPlayerStatus;
class CPlayer : public Engine::CGameObject
{
public:
	enum STATE { RIFLEIDLE, RIFLEWALKNORTH, RIFLEWALKSOUTH, RIFLEWALKEAST, RIFLEWALKWEST, NONEIDLE, NONEWALK, RIFLEHOLSTER, RIFLEDRAW, RIFLEATTACK, SNIPERDRAW, SNIPERHOSTER, SNIPERATTACK, SNIPERRELOAD
	};
	enum LEGSTATE { LEGIDLE, LEGNORTH, LEGSOUTH, LEGRIFLEWALKSOUTH, LEGRIFLEWALKEAST };


private:
	explicit CPlayer(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CPlayer(const CPlayer& rhs);
	virtual ~CPlayer();

public:
	void            KeyLockPlayer(_bool bIsLock);
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();

	virtual _int	Update_GameObject(const _float& fTimeDelta);
	void UpdateParts(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);

	float Get_SpineAngle() { return m_fSpineAngle; }//척추회전정도
public:
	CPlayerArm* Get_PlayerArm() { return m_pArm; };//팔
	CPlayerLeg* Get_PlayerLeg() { return m_pLeg; };//다리
	CPlayerStatus* Get_Status() { return m_pStatus; };


	/////////// HHHHHHHHPPPPPPPPP
	_float			Get_CurHP();

private:
	virtual HRESULT Add_Component();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	CPlayerArm* m_pArm;//갑옵
	CPlayerLeg* m_pLeg;//마스크
//상태관련
	CPlayerStatus* m_pStatus;


	STATE m_eCurState = RIFLEIDLE;
	STATE m_eCurLegState = RIFLEIDLE;
	float m_fTime = 0.f;
	float m_fSpineAngle = 0.f;

	//블랜딩수치
	_float m_fLegBlend = 0.8f;
	_float m_fArmBlend = 0.2f;

public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CPlayer* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void			Free();
};

