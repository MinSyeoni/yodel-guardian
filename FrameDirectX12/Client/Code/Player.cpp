#include "stdafx.h"
#include "Player.h"
#include "PlayerArm.h"
#include "PlayerLeg.h"
#include "PlayerHed.h"
#include "PlayerStatus.h"
#include "ObjectMgr.h"
#include "DirectInput.h"


CPlayer::CPlayer(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	:CGameObject(pGraphicDevice, pCommandList)
{
}

CPlayer::CPlayer(const CPlayer& rhs)
	: CGameObject(rhs)
{
}

CPlayer::~CPlayer()
{
}


void CPlayer::KeyLockPlayer(_bool bIsLock)
{
	m_pStatus->m_bIsKeyLock = bIsLock;
}

HRESULT CPlayer::Ready_GameObjectPrototype()
{
#ifdef _DEBUG

	COUT_STR("Ready Prototype Player");
#endif

	CGameObject* pGameObject = nullptr;
	pGameObject = CPlayerArm::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_PlayerArm", pGameObject), E_FAIL);

	pGameObject = nullptr;
	pGameObject = CPlayerLeg::Create(m_pGraphicDevice, m_pCommandList);
	NULL_CHECK_RETURN(pGameObject, E_FAIL);
	FAILED_CHECK_RETURN(m_pObjectMgr->Add_GameObjectPrototype(L"Prototype_PlayerLeg", pGameObject), E_FAIL);



	return S_OK;
}

HRESULT CPlayer::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);

#ifdef _DEBUG
	COUT_STR("Success Player - Clone Mesh");
#endif

	m_pLeg = static_cast<CPlayerLeg*>(m_pObjectMgr->Get_NewGameObject(L"Prototype_PlayerLeg", L"TPlayerLeg", nullptr));
	m_pLeg->SetPlayerTransform(m_pTransCom);

	m_pArm = static_cast<CPlayerArm*>(m_pObjectMgr->Get_NewGameObject(L"Prototype_PlayerArm", L"TPlayerArm", nullptr));
	m_pArm->SetPlayerTransform(m_pTransCom);

	m_pStatus = new CPlayerStatus();
	m_pStatus->SetMesh(static_cast<CMesh*>(m_pArm->Get_Component(L"Com_Mesh", ID_STATIC)));


	m_pTransCom->m_vPos = _vec3(300.f, 0.f, 480.f);
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vDir = _vec3(0.f, 0.0f, 1.f);


	return S_OK;
}

HRESULT CPlayer::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit Player");
#endif

	return S_OK;
}

_int CPlayer::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);



	m_pStatus->UpdateState(fTimeDelta, m_pTransCom);

	m_fSpineAngle = m_pStatus->Get_Spine();
	UpdateParts(fTimeDelta);
	m_pArm->Set_Animation(m_eCurState);
	m_pArm->Set_LegAnimation(m_eCurLegState);

	_vec3 Pos = m_pTransCom->m_vPos;

	
	CGameObject::Update_GameObject(fTimeDelta);
	return NO_EVENT;
}

void CPlayer::UpdateParts(const _float& fTimeDelta)
{
	m_pArm->Set_Spine(m_fSpineAngle);

	m_pArm->Update_GameObject(fTimeDelta);
	m_eCurState = m_pStatus->m_eCurState;
	m_eCurLegState = m_pStatus->m_eLegState;
}

_int CPlayer::LateUpdate_GameObject(const _float& fTimeDelta)
{

	m_pStatus->LateUpdate(fTimeDelta);
	m_pArm->SetLegMatrix(m_pLeg->GetLegMatrix());
	m_pArm->LateUpdate_GameObject(fTimeDelta);

	return  NO_EVENT;
}

void CPlayer::Render_GameObject(const _float& fTimeDelta)
{
}

_float CPlayer::Get_CurHP()
{
	return m_pStatus->m_uiHp;
}

HRESULT CPlayer::Add_Component()
{
	return S_OK;
}


CGameObject* CPlayer::Clone_GameObject(void* prg)
{

	CGameObject* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CPlayer* CPlayer::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CPlayer* pInstance = new CPlayer(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPlayer::Free()
{
	CGameObject::Free();
	Safe_Release(m_pArm);
	Safe_Release(m_pLeg);
	Safe_Delete(m_pStatus);

}
