#include "stdafx.h"
#include "FlameThrower.h"
#include "DirectInput.h"
#include "GraphicDevice.h"
#include "ObjectMgr.h"

CFlameThrower::CFlameThrower()
{
	Initialized();
}

CFlameThrower::~CFlameThrower()
{
	Release();
}

void CFlameThrower::Initialized()
{
	m_eCurState = CB_Idle;
	m_ePreState = m_eCurState;
}

HRESULT CFlameThrower::Late_Initialized()
{
	return S_OK;
}

_int CFlameThrower::Update_FlameThrower(const _float& fTimeDelta, CTransform* pTransform)
{
	m_fTime += fTimeDelta;

	if (m_ePreState != m_eCurState)
	{
		m_fTime = 0.f;
		m_ePreState = m_eCurState;
	}

	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");
	//m_pTransCom->Chase_Target(pPlayer->Get_Transform()->Get_PositionVector(), fTimeDelta);


	return S_OK();
}

_int CFlameThrower::LateUpdate_FlameThrower(const _float& fTimeDelta, CTransform* pTransform)
{
	srand((unsigned int)time(0));

	
	m_fAniTime += fTimeDelta;
	if (m_fAniTime >= 3.f)
	{
		m_eCurState = CB_Twitch;
		m_fAniTime = 0.f;
	}
	

	return S_OK();
}

void CFlameThrower::Release()
{
	Safe_Release(m_pTransCom);
}
