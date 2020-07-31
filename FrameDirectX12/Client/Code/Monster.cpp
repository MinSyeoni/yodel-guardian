#include "stdafx.h"
#include "Monster.h"

#include "ObjectMgr.h"
#include "DirectInput.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "ColliderMgr.h"
#include "Frustom.h"


CMonster::CMonster(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CGameObject(pGraphicDevice,pCommandList)
{
} 

CMonster::CMonster(const CMonster& rhs)
	:CGameObject(rhs),
	m_tMeshInfo(rhs.m_tMeshInfo),
	m_pFlameThrower(rhs.m_pFlameThrower),
	m_pZombi(rhs.m_pZombi)
{
}

CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObjectPrototype()
{
#ifdef _DEBUG
	COUT_STR("Ready Prototype Monster");
#endif

	return S_OK;
}

HRESULT CMonster::Ready_GameObject()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);
	CGameObject::Ready_GameObject();

#ifdef _DEBUG
	COUT_STR("Success Monster - Clone Mesh");
#endif

	if(m_tMeshInfo.MeshTag == L"Flamethrower")
		m_eMonName = FLAMETHROWER;
	else if (m_tMeshInfo.MeshTag == L"Zombi")
		m_eMonName = ZOMBI;
	else if (m_tMeshInfo.MeshTag == L"Dron")
		m_eMonName = DRON;

	FAILED_CHECK_RETURN(Add_Component(),E_FAIL);

	m_pTransCom->m_vPos = m_tMeshInfo.Pos;
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vAngle = ToDegree(m_tMeshInfo.Rotation);

	m_iInitAni = m_tMeshInfo.iMeshID;
	m_iInitId = m_tMeshInfo.iDrawID;

	switch (m_eMonName)
	{
	case CMonster::NONAME:
		break;
	case CMonster::FLAMETHROWER:
		m_pFlameThrower = new CFlameThrower;
		m_pFlameThrower->Set_Transform(m_pTransCom);
		m_bIsActive = true;
		break;
	case CMonster::ZOMBI:
	{
		m_pZombi = new CZombi;
		m_pZombi->Set_InitAni(m_iInitAni);
		m_pZombi->Set_InitDrawID(m_iInitId);
		m_pZombi->Set_Transform(m_pTransCom);
		m_pZombi->Set_NaviMesh(m_pNaviCom);
	}
		break;
	case CMonster::DRON:
	{
		m_pDron = new CDron;
		m_pDron->Set_Transform(m_pTransCom);
		m_pDron->Set_NaviMesh(m_pNaviCom);
		m_bIsActive = true;
	}
	break;
	default:
		break;
	}

	return S_OK;
}

HRESULT CMonster::LateInit_GameObject()
{
#ifdef _DEBUG
	COUT_STR("LateInit Monster");
#endif
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture(), m_pDissolveTex->Get_Texture());

	m_pNaviCom->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &_vec3(0.f, 0.f, 0.f), 0, false);//이거안해도되~~
	m_pNaviCom->SetFirstNavi(m_pTransCom->m_vPos);//이걸로해줭
	m_pAstarCom->Init_AstarCell(m_pNaviCom->GetNaviCell());

	switch (m_eMonName)
	{
	case CMonster::NONAME:
		break;
	case CMonster::FLAMETHROWER:
		m_pFlameThrower->Late_Initialized();
		break;
	case CMonster::ZOMBI:	
	{
		m_pZombi->Set_Astar(m_pAstarCom);
		m_pZombi->Late_Initialized();
	}
		break;
	case CMonster::DRON:
		m_pDron->Late_Initialized();
		break;
	default:
		break;
	}

	return S_OK;
}

_int CMonster::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)	return DEAD_OBJ;
	if (!m_bIsActive)	return E_FAIL;

	Engine::CGameObject::Update_GameObject(fTimeDelta);
	m_pBoxCol->Update_Collider(&m_pTransCom->m_matWorld);
	CColliderMgr::Get_Instance()->Add_Collider(CColliderMgr::OBJECT, m_pBoxCol);

	_int iCurState = 0;
	switch (m_eMonName)
	{
	case CMonster::NONAME:
		break;
	case CMonster::FLAMETHROWER:
	{
		if (m_pFlameThrower == nullptr)
			return E_FAIL;
		
		m_pFlameThrower->Update_FlameThrower(fTimeDelta, m_pTransCom, m_pMeshCom);
		iCurState = m_pFlameThrower->Get_CurState();
	}
		break;
	case CMonster::ZOMBI:
	{
		if (m_pZombi == nullptr)
			return E_FAIL;
		
		// LeftWrist
		Update_BoneCollider(m_pShereCol[0], "LeftMiddleFinger", CColliderMgr::COMBAT);
		Update_BoneCollider(m_pShereCol[1], "RightMiddleFinger", CColliderMgr::COMBAT);
		Update_BoneCollider(m_pShereCol[2], "Chest", CColliderMgr::MONSTER);
		m_pZombi->Update_Zombi(fTimeDelta, m_pTransCom, m_pMeshCom);
		iCurState = m_pZombi->Get_CurState();

		if (CDirectInput::Get_Instance()->KEY_PRESSING(DIK_N))
		{
			m_fDissolve -= 0.2f * fTimeDelta;
			
			m_matDissolve._11 = m_fDissolve;
		}
	}
		break;
	case CMonster::DRON:
	{
		if (m_pDron == nullptr)
			return E_FAIL;

		// Gun, Cables
		Update_BoneCollider(m_pShereCol[0], "Gun", CColliderMgr::MONSTER);
		m_pDron->Update_Dron(fTimeDelta, m_pTransCom, m_pMeshCom);
		iCurState = m_pDron->Get_CurState();

		if (CDirectInput::Get_Instance()->KEY_PRESSING(DIK_N))
		{
			m_fDissolve -= 0.2f * fTimeDelta;

			m_matDissolve._11 = m_fDissolve;
		}
	}
	break;
	default:
		break;
	}

	dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation((_int)iCurState);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f * fTimeDelta);

	return NO_EVENT;
}

void CMonster::Update_BoneCollider(CSphereCollider* pSphereCol, string strBoneName, CColliderMgr::COLLIDER_TAG eTag)
{
	if (strBoneName == "")
		return;

	_matrix matBone = *m_pMeshCom->Find_BoneMatrix(strBoneName);
	_matrix matBoneOffset = *m_pMeshCom->Find_BoneOffset(strBoneName);

	_matrix Offset = XMMatrixInverse(nullptr, matBoneOffset);

	_matrix BoneMatrix = Offset * matBone * m_pTransCom->m_matWorld;

	pSphereCol->Update_Collider(&BoneMatrix);

	CColliderMgr::Get_Instance()->Add_Collider(eTag, pSphereCol);

	pSphereCol->Set_IsCol(true);
}

_int CMonster::LateUpdate_GameObject(const _float & fTimeDelta)
{
	if (!m_bIsActive)	
		return E_FAIL;

	if (!CFrustom::Get_Instance()->FrustomCulling(m_pMeshCom->Get_MeshComponent()->Get_MinPos(), m_pMeshCom->Get_MeshComponent()->Get_MaxPos(), m_pTransCom->m_matWorld))
		return NO_EVENT;

	NULL_CHECK_RETURN(m_pRenderer, -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pBoxCol), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pShereCol[0]), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pShereCol[1]), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_ColliderGroup(m_pShereCol[2]), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);

	switch (m_eMonName)
	{
	case CMonster::NONAME:
		break;
	case CMonster::FLAMETHROWER:
	{
		if (m_pFlameThrower == nullptr)
			return E_FAIL;
		m_pFlameThrower->LateUpdate_FlameThrower(fTimeDelta, m_pTransCom, m_pMeshCom);
	}
	break;
	case CMonster::ZOMBI:
	{
		if (m_pZombi == nullptr)
			return E_FAIL;
		m_pZombi->LateUpdate_Zombi(fTimeDelta, m_pTransCom, m_pMeshCom);

		if (m_pZombi->Get_IsDeadZombi())
		{
			m_fDissolve -= 1.f * fTimeDelta;
			m_matDissolve._11 = m_fDissolve;
			
			if (m_fDissolve <= -0.5f)
				m_bIsDead = true;	
		}
	}
	break;
	case CMonster::DRON:
	{		
		if (m_pDron == nullptr)
			return E_FAIL;
		m_pDron->LateUpdate_Dron(fTimeDelta, m_pTransCom, m_pMeshCom);

		if (m_pDron->Get_IsDeadDron())
		{
			m_fDissolve -= 1.f * fTimeDelta;
			m_matDissolve._11 = m_fDissolve;

			if (m_fDissolve <= -0.5f)
				m_bIsDead = true;
		}
	}
	break;
	default:
		break;
	}

	_vec3 vShaveDir;
	for (auto& pCol : CColliderMgr::Get_Instance()->Get_ColliderList(CColliderMgr::BOX, CColliderMgr::OBJECT))
	{
		if (CMathMgr::Get_Instance()->Collision_OBB(m_pBoxCol, pCol, &vShaveDir))
		{
			if (pCol != m_pBoxCol)
			{
				m_pTransCom->m_vPos += vShaveDir;

				m_pTransCom->m_matWorld._41 += vShaveDir.x;
				m_pTransCom->m_matWorld._42 += vShaveDir.y;
				m_pTransCom->m_matWorld._43 += vShaveDir.z;
			}
		}
	}

	return NO_EVENT;
}

void CMonster::Render_GameObject(const _float & fTimeDelta)
{
	if (!m_bIsActive)
		return;

	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();

	m_pMeshCom->Render_Mesh(m_pShaderCom, m_vecMatrix);
}

HRESULT CMonster::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Dissolve*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_Dissolve", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Buffer
	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(m_tMeshInfo.MeshTag.c_str(), COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);

	// 네비메쉬
	m_pNaviCom = static_cast<Engine::CNaviMesh*>(CComponentMgr::Get_Instance()->Clone_Component(L"Mesh_Navi", ID_STATIC));
	NULL_CHECK_RETURN(m_pNaviCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_NaviMesh", m_pNaviCom);

	// 에이스타
	m_pAstarCom = static_cast<Engine::CAstar*>(m_pComponentMgr->Clone_Component(L"Prototype_Astar", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pAstarCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Astar", m_pAstarCom);

	m_pBoxCol = static_cast<Engine::CBoxCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_BoxCol", COMPONENTID::ID_STATIC, CCollider::COL_BOX, true, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 0.f, _vec3(100.f, 100.f, 100.f), this));
	NULL_CHECK_RETURN(m_pBoxCol, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_BoxCol", m_pBoxCol);

	m_pShereCol[0] = static_cast<Engine::CSphereCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_SphereCol", COMPONENTID::ID_STATIC, CCollider::COL_SPHERE, false, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 20.f, _vec3(1.f, 1.f, 1.f), this));
	NULL_CHECK_RETURN(m_pShereCol[0], E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereCol1", m_pShereCol[0]);
	
	m_pShereCol[1] = static_cast<Engine::CSphereCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_SphereCol", COMPONENTID::ID_STATIC, CCollider::COL_SPHERE, false, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 20.f, _vec3(1.f, 1.f, 1.f), this));
	NULL_CHECK_RETURN(m_pShereCol[1], E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereCol2", m_pShereCol[1]);

	m_pShereCol[2] = static_cast<Engine::CSphereCollider*>(m_pComponentMgr->Clone_Collider(L"Prototype_SphereCol", COMPONENTID::ID_STATIC, CCollider::COL_SPHERE, false, m_pMeshCom, _vec3(0.f, 0.f, 0.f), _vec3(0.f, 0.f, 0.f), 60.f, _vec3(1.f, 1.f, 1.f), this));
	NULL_CHECK_RETURN(m_pShereCol[2], E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_SphereCol3", m_pShereCol[2]);

	// 디졸브
	m_pDissolveTex = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_Dissolve", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pDissolveTex, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pDissolveTex);

	return S_OK;
}

void CMonster::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;
	
	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

 	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;

	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_matDissolve));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

void CMonster::Render_ShadowDepth(CShader_Shadow * pShader)
{
	if (!m_bIsActive)	return;

	Set_ShadowTable(pShader);
	m_pMeshCom->Render_ShadowMesh(pShader, m_vecMatrix, true);
	pShader->Set_ShadowFinish();
}

void CMonster::Set_ShadowTable(CShader_Shadow* pShader)
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_SHADOW_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_SHADOW_INFO));

	matView = CFrustom::Get_Instance()->Get_LightView();
	matProj = CFrustom::Get_Instance()->Get_LightProj();

	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	tCB_MatrixInfo.blsMesh = true;


	_int offset = pShader->Get_CBMeshCount();
	pShader->Get_UploadBuffer_ShadowInfo()->CopyData(offset, tCB_MatrixInfo);
}

CGameObject * CMonster::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CMonster(*this);

	MeshInfo tMeshInfo = *reinterpret_cast<MeshInfo*>(prg);
	static_cast<CMonster*>(pInstance)->m_tMeshInfo = tMeshInfo;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CMonster* CMonster::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CMonster* pInstance = new CMonster(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CMonster::Free()
{
	Safe_Delete(m_pFlameThrower);
	Safe_Delete(m_pZombi);
	Safe_Delete(m_pDron);

	CGameObject::Free();
}
