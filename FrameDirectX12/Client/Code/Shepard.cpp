#include "stdafx.h"
#include "Shepard.h"
#include "ObjectMgr.h"
#include "GraphicDevice.h"
#include "DynamicCamera.h"
#include "DirectInput.h"
#include "PlayerStatus.h"
#include "Frustom.h"
#include "NpcWords.h"
#include "StaticCamera.h"
#include "Zombi.h"
#include "Monster.h"
#include "NpcRifle.h"
CShepard::CShepard(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CShepard::CShepard(const CShepard& rhs)
	: Engine::CGameObject(rhs)
{

}

CShepard::~CShepard()
{
}

HRESULT CShepard::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype RectObject");
#endif


	return S_OK;
}

HRESULT CShepard::Ready_GameObject()
{

	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

	FAILED_CHECK_RETURN(Add_Component(), E_FAIL);
	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Shepard", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);


	m_pAstarCom = static_cast<Engine::CAstar*>(m_pComponentMgr->Clone_Component(L"Prototype_Astar", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pAstarCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Astar", m_pAstarCom);

	m_pNaviCom = static_cast<Engine::CNaviMesh*>(CComponentMgr::Get_Instance()->Clone_Component(L"Mesh_Navi", ID_STATIC));
	NULL_CHECK_RETURN(m_pNaviCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Navi", m_pNaviCom);


	m_pBoxCollider = static_cast<Engine::CBoxCollider*>(CComponentMgr::Get_Instance()->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, false, nullptr, _vec3(0.f, 6.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(100.f, 150.f, 100.f), nullptr));
	NULL_CHECK_RETURN(m_pBoxCollider, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BoxCollider", m_pBoxCollider);

	m_matChest = m_pMeshCom->Find_BoneMatrix("Chest");
	m_matChestOffset = m_pMeshCom->Find_BoneOffset("Chest");
	//여기야시영

#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif
	m_pTransCom->m_vPos = _vec3(285.f, 0.f, 398.f);
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vAngle = _vec3(0.f, 180.f, 0.f);

	m_vMove1Pos = _vec3(314.f, 0.f, 220.f);
	m_vMove2Pos = _vec3(505.f, 0.f, 237.f);
	m_vMove3Pos = _vec3(490.f, 0.f, 475.f);
	m_vMove4Pos = _vec3(330.f, 0.f, 480.f);


	m_uiMove1NaviIndex = m_pNaviCom->FoundIndex(m_vMove1Pos);
	m_uiMove2NaviIndex = m_pNaviCom->FoundIndex(m_vMove2Pos);
	m_uiMove3NaviIndex = m_pNaviCom->FoundIndex(m_vMove3Pos);
	m_uiMove4NaviIndex = m_pNaviCom->FoundIndex(m_vMove4Pos);


	return S_OK;
}

HRESULT CShepard::LateInit_GameObject()
{


	/*____________________________________________________________________
	Get GameObject - DynamicCamera
	______________________________________________________________________*/

	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());


	m_pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");

	if (m_pPlayer == nullptr)
		return E_FAIL;

	//m_pNaviCom->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(0.f, 0.f, 0.f), 0, false);

	m_pNaviCom->SetFirstNavi(m_pTransCom->m_vPos);

	m_pAstarCom->Init_AstarCell(m_pNaviCom->GetNaviCell());

	m_eCurState = IDLE;
	m_eLegState = IDLE;
	return S_OK;

}

_int CShepard::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;
	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	if (m_bIsStartConver && !m_bIsTalk)
	{
		m_eCurChapter = TURNPLAYER;
		m_bIsTalk = true;
	}
	if (!m_bIsStartConver)
	{
		CheckPlayer();
	}


	ChapterCheck(fTimeDelta);

	if (m_bIsReload)
		m_eCurState = RELOAD;

	ReloadCheck();

	m_pBoxCollider->Update_Collider(&m_pTransCom->m_matWorld);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::NPC, m_pBoxCollider);
	return NO_EVENT;
}

_int CShepard::LateUpdate_GameObject(const _float& fTimeDelta)
{

	if (m_bIsFinish)
		return NO_EVENT;


	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	m_pMeshCom->Set_AnimationBlend((int)m_eCurState, (int)m_eLegState);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransformsBlend(fTimeDelta * 3000.f, fTimeDelta * 3000.f);
	

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);
	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/

	return NO_EVENT;
}

void CShepard::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

void CShepard::Render_ShadowDepth(CShader_Shadow* pShader)
{
	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader, m_vecMatrix, true);
	pShader->Set_ShadowFinish();
}

void CShepard::TurnToPlayer(const _float& fTimeDelta)
{
	_vec3 vPos = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC))->m_vPos;

	if (m_pTransCom->Chase_Target(vPos, fTimeDelta * 0.03) )
	{
		m_eCurChapter = GOTOPLAYER;
	}


}

void CShepard::GotoPlayer(const _float& fTimeDelta)
{
	CPlayer* pPlayer = nullptr;

	CGameObject* pGameObject = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject",L"Player");
	if (pGameObject == nullptr)
		return;

	pPlayer = static_cast<CPlayer*>(pGameObject);

	_vec3 vPlayerPos = m_pPlayer->Get_Transform()->m_vPos;
	_vec3 vPos = m_pTransCom->m_vPos;

	_vec3 vDir  = _vec3(vPlayerPos - vPos);

	_float fDist = vDir.Get_Length();

	vDir.Normalize();

	if (fDist < 20.f)
	{
		m_eCurChapter = FIRSTTALK;
		_vec3 vLook;
		memcpy(&vLook, &m_pTransCom->m_matWorld._31, sizeof(_vec3));

		_vec3 vEye = m_pTransCom->m_vPos + vLook *60.f;
		vEye.y += 16.f;
		_vec3 vAt = m_pTransCom->m_vPos;
		vAt.y += 16.f;

		StaticCameraInfo tInfo;
		tInfo.vAtPos = vAt;
		tInfo.vEyePos = vEye;
		m_pObjectMgr->Add_GameObject(L"Layer_Camera", L"Prototype_StaticCamera", L"StaticCamera", &tInfo);

		CNpcWords::WORDS_TYPE eType = CNpcWords::SHEPARD;
		m_pObjectMgr->Add_GameObject(L"Layer_UI", L"Prototype_NpcBoard", L"NpcBoard", &eType);


	}

	else
	{
		_vec3 vMovePos = m_pNaviCom->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vDir, fTimeDelta * 12.f);
		m_pTransCom->m_vPos = vMovePos;
		CGameObject* pPlayer = m_pObjectMgr->Get_GameObject(L"Layer_GameObject", L"Player");
		if (pPlayer == nullptr)
			return;
		static_cast<CPlayer*>(pPlayer)->KeyLockPlayer(true);
	}
}

void CShepard::TalkCheck(const _float& fTimeDelta)
{
}

void CShepard::AttackCheck(const _float& fTimeDelta)
{
	CGameObject* pTarget = nullptr;

	list<CGameObject*> pList = *CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"Zombi");

	_bool bIsFoundTarget=false;
	CMonster* PMonster=nullptr;
	_float fMinDist = 999.f;


	for (auto& pSrc : pList)
	{
	 _bool Active=dynamic_cast<CMonster*>(pSrc)->Get_BisActive();
	 if (Active)
		 bIsFoundTarget = true;
	 
	 if (!Active)
		 continue;

	 _vec3 vTargetPos = pSrc->Get_Transform()->m_vPos;

	 _float fDist = _vec3(m_pTransCom->m_vPos - vTargetPos).Get_Length();

	 if (fDist < fMinDist)
	 {
		 fMinDist = fDist;
		 PMonster = static_cast<CMonster*>(pSrc);
	 }

	}

	if (PMonster != nullptr)
	{
		if(m_pTransCom->Chase_Target(PMonster->Get_Transform()->m_vPos, fTimeDelta * 0.03f))
		CShepard::ShootingCheck(fTimeDelta, PMonster);

		MoveDirectionCheck(fTimeDelta, PMonster->Get_Transform()->m_vPos);
	}
	else
	{
		m_eCurChapter = MOVINGPOINT;
		m_iFightCount++;
	}


}

void CShepard::MoveByAstar(const _float& fTimeDelta)
{

	_vec3 MovePos=_vec3(0.f,0.f,0.f);
	_uint uiNavi=0;
	if (m_iFightCount == 1)
	{
		MovePos = m_vMove1Pos;
		uiNavi = m_uiMove1NaviIndex;
	}
	else if (m_iFightCount == 2)
	{

		MovePos = m_vMove2Pos;
		uiNavi = m_uiMove2NaviIndex;

	}
	else if (m_iFightCount == 3)
	{
		MovePos = m_vMove3Pos;
		uiNavi = m_uiMove3NaviIndex;
	}
	else if (m_iFightCount == 4)
	{

		MovePos = m_vMove4Pos;
		uiNavi = m_uiMove4NaviIndex;

	}
	else
		return;


	if (_vec3(m_pTransCom->m_vPos - MovePos).Get_Length() < 30.f)
	{
		m_eCurChapter = PATROLCUT;
		if (m_iFightCount == 4)
			m_bIsFinish = true;

		list<CGameObject*> pList = *CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"NpcWeapon");
		for (auto& pSrc : pList)
		{

			if (CNpcRifle::SHEPARD == static_cast<CNpcRifle*>(pSrc)->Get_Owner())
			{

				static_cast<CNpcRifle*>(pSrc)->CreateShootEffect();

			}


		}



		return;
	}

	m_pAstarCom->Start_Aster(_vec3(0.f,0.f,0.f), _vec3(0.f, 0.f, 0.f), m_pNaviCom->GetIndex(), uiNavi);

	list<Engine::CCell*>& BestLst = m_pAstarCom->GetBestLst();




	if (!BestLst.empty())
	{
		_vec3 vecDir = BestLst.front()->m_vPos - m_pTransCom->m_vPos;

		BestLst.pop_front();
		if (!BestLst.empty())
		{
			_vec3 vecDir2 = BestLst.front()->m_vPos - m_pTransCom->m_vPos;

			vecDir = (vecDir + vecDir2) * 0.5;
			BestLst.pop_front();
			if (!BestLst.empty())
			{
				vecDir2 = BestLst.front()->m_vPos - m_pTransCom->m_vPos;
				vecDir = (vecDir + vecDir2) * 0.5;
			}

		}

		vecDir.Normalize();
		_vec3 vMovePos;
		vMovePos = m_pNaviCom->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vecDir, fTimeDelta * 20.f);

		m_pTransCom->Chase_Target(vMovePos, fTimeDelta * 0.03f);
		m_pTransCom->m_vPos = vMovePos;



	}





}

void CShepard::PatrolCheck()
{
	CGameObject* pTarget = nullptr;

	list<CGameObject*> pList = *CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"Zombi");

	_bool bIsFoundTarget=false;
	CMonster* PMonster = nullptr;
	_float fMinDist = 999.f;


	for (auto& pSrc : pList)
	{
		_bool Active = dynamic_cast<CMonster*>(pSrc)->Get_BisActive();
		if (Active)
			bIsFoundTarget = true;

	}


	if(bIsFoundTarget)
		m_eCurChapter = ATTACK;
	
	


}

void CShepard::MoveDirectionCheck(const _float& fTimeDelta, _vec3 vTargetPos)
{
	_float fDist = _vec3(m_pTransCom->m_vPos - vTargetPos).Get_Length();
	_vec3 vMovePos = m_pTransCom->m_vPos;
	
	_vec3 vDir;
	memcpy(&vDir, &m_pTransCom->m_matWorld._31, sizeof(_vec3));
	vDir.Normalize();

	if (fDist < 40.f)
	{
		vMovePos = m_pNaviCom->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(vDir*-1.f), fTimeDelta * 10.f);

		m_pTransCom->m_vPos = vMovePos;
		m_eLegState = WALKSOUTH;
	}
	else if (fDist > 70.f)
	{
		vMovePos = m_pNaviCom->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vDir, fTimeDelta * 10.f);
		m_pTransCom->m_vPos = vMovePos;
		m_eLegState = WALKNORTH;

	}
	else
		m_eLegState = IDLE;

}

void CShepard::ReloadCheck()
{

	if (m_bIsReload&&m_eCurState ==RELOAD)
	{
		if (m_pMeshCom->Set_FindAnimation(2000.f, (int)RELOAD))
			m_bIsReload = false;

	}


}

void CShepard::ShootingCheck(const _float& fTimeDelta,CMonster* pTarget)
{
	if (m_eCurChapter == ATTACK &&!m_bIsReload)
	{
		m_eCurState = FIRE;

		if (pTarget == nullptr)
			m_eLegState = IDLE;

		m_fDelayTime += fTimeDelta;
		if (m_fDelayTime > 0.3)
		{
			m_fRifleBulletCount--;

			list<CGameObject*> pList = *CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_GameObject", L"NpcWeapon");
			for (auto& pSrc : pList)
			{

				if (CNpcRifle::SHEPARD == static_cast<CNpcRifle*>(pSrc)->Get_Owner())
				{

					static_cast<CNpcRifle*>(pSrc)->CreateShootEffect();

				}


			}

			if (m_fRifleBulletCount == 3)
			{
				if (pTarget != nullptr)
				{

					CMonster::MONKIND  eMonKind = CMonster::NONAME;
					eMonKind = dynamic_cast<CMonster*>(pTarget)->Get_MONKIND();

					if (eMonKind == CMonster::ZOMBI)
					{
						CZombi* pZombi = dynamic_cast<CMonster*>(pTarget)->Get_Zombi();
						pZombi->Set_HitDamage(15);
						pZombi->Set_IsHit(true);

					}



				}

			}
			if (m_fRifleBulletCount == 0)
			{
				m_bIsReload = true;
				m_fRifleBulletCount = 7;

			}
			m_fDelayTime = 0.f;
		}


	}



}

void CShepard::CollisionWithObject()
{

	_vec3 vShaveDir;
	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::OBJECT))
	{
		if (pCol == m_pBoxCollider)
			continue;

		if (CMathMgr::Get_Instance()->Collision_OBB(m_pBoxCollider, pCol, &vShaveDir))
		{
			m_pTransCom->m_vPos += vShaveDir;

			m_pTransCom->m_matWorld._41 += vShaveDir.x;
			m_pTransCom->m_matWorld._42 += vShaveDir.y;
			m_pTransCom->m_matWorld._43 += vShaveDir.z;
		}

	}


}

void CShepard::CheckPlayer()
{

	_vec3 vPos = static_cast<CTransform*>(m_pPlayer->Get_Component(L"Com_Transform", ID_DYNAMIC))->m_vPos;

	_float fDist = _vec3(m_pTransCom->m_vPos - vPos).Get_Length();

	if (fDist < 45.f)
		m_bIsStartConver = true;


}

void CShepard::MonsterCheck(const _float& fTimeDelta)
{





}

void CShepard::ChapterCheck(const _float& fTimeDelta)
{

	switch (m_eCurChapter)
	{
	case START:
	{m_eCurState = IDLE;
	m_eLegState = IDLE;
	break;

	}
	case TURNPLAYER:
	{
		TurnToPlayer(fTimeDelta);
		break;
	}
	case GOTOPLAYER:
	{
		m_eCurState = RUNNORTH;
		m_eLegState = RUNNORTH;
		GotoPlayer(fTimeDelta);
		break;
	}
	case  PATROLCUT:
	{
		m_eCurState = SEARCH;
		m_eLegState = SEARCH;
		PatrolCheck();
		break;
	}
	case MOVINGPOINT:
	{

		MoveByAstar(fTimeDelta);
		m_eCurState = RUNNORTH;
		m_eLegState = RUNNORTH;
		break;
	}
	case FIRSTTALK:
	{
		m_eCurState = CHECK;
		m_eLegState = CHECK;
		break;
	}
	case ATTACK:
	{
		AttackCheck(fTimeDelta);
		break;
	}
	default:
		break;
	}

}

HRESULT CShepard::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);


	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Mesh*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Mesh", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);
#ifdef _DEBUG
	COUT_STR("Success RectObject - Clone ShaderCom");
#endif

	return S_OK;
}

void CShepard::Set_ConstantTable()
{
	_matrix matRotY = XMMatrixRotationY(XMConvertToRadians(-90));
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();


	_matrix matWVP = matRotY * m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matRotY * m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);

}

void CShepard::Set_ShepardCurHP(_uint iHP)
{
	if (m_iCurHP < 314)
		m_iCurHP += iHP;
}

void CShepard::Set_ShadowTable(CShader_Shadow* pShader)
{
	_matrix matRotY = XMMatrixRotationY(XMConvertToRadians(-90));
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_SHADOW_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_SHADOW_INFO));

	matView = CFrustom::Get_Instance()->Get_LightView();
	matProj = CFrustom::Get_Instance()->Get_LightProj();

	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matRotY * m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	tCB_MatrixInfo.blsMesh = true;


	_int offset = pShader->Get_CBMeshCount();
	pShader->Get_UploadBuffer_ShadowInfo()->CopyData(offset, tCB_MatrixInfo);

}
CGameObject* CShepard::Clone_GameObject(void* prg)
{
	CGameObject* pInstance = new CShepard(*this);


	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CShepard* CShepard::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CShepard* pInstance = new CShepard(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CShepard::Free()
{
	CGameObject::Free();
}
