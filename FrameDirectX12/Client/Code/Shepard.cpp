#include "stdafx.h"
#include "Shepard.h"
#include "ObjectMgr.h"
#include "GraphicDevice.h"
#include "DynamicCamera.h"
#include "DirectInput.h"
#include "PlayerStatus.h"
#include "Frustom.h"

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
	//여기야시영

#ifdef _DEBUG
	COUT_STR("Success Static - Clone Mesh");
#endif
	m_pTransCom->m_vPos = _vec3(285.f, 0.f, 398.f);
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	m_pTransCom->m_vAngle = _vec3(0.f, 180.f, 0.f);

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



	//m_pAstarCom->Init_AstarCell(m_pNaviCom->GetNaviCell());

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
	if (KEY_DOWN(DIK_C))
	{
		m_eCurChapter = TURNPLAYER;
	}


	ChapterCheck(fTimeDelta);
	/*if (KEY_PRESSING(DIK_0))
		MoveByAstar(fTimeDelta);*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return NO_EVENT;
}

_int CShepard::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);
	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/
	m_pMeshCom->Set_AnimationBlend((int)m_eCurState, (int)m_eCurState);
	m_vecMatrix = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransformsBlend(fTimeDelta * 3000.f, fTimeDelta * 3000.f);

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

	if (m_pTransCom->Chase_Target(vPos, fTimeDelta * 0.03) && m_pMeshCom->Set_IsAniFinsh(400.f))
	{
		m_eCurChapter = GOTOPLAYER;
	}


}

void CShepard::MoveByAstar(const _float& fTimeDelta)
{

	//CPlayer* pPlayer = static_cast<CPlayer*>(CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player"));
	//if (pPlayer == nullptr)
	//	return;

	//CTransform* pPlayerTranForm = pPlayer->Get_Transform();

	//CNaviMesh* pPlayerNavi = pPlayer->Get_Status()->m_pNaviMesh;
	//m_pAstarCom->Start_Aster(m_pTransCom->m_vPos, pPlayerTranForm->m_vPos, m_pNaviCom->GetIndex(), pPlayerNavi->GetIndex());

	//list<Engine::CCell*>& BestLst = m_pAstarCom->GetBestLst();


	//if (!BestLst.empty())
	//{
	//	_vec3 vecDir = BestLst.front()->m_vPos - m_pTransCom->m_vPos;

	//	BestLst.pop_front();
	//	if (!BestLst.empty())
	//	{
	//		_vec3 vecDir2 = BestLst.front()->m_vPos - m_pTransCom->m_vPos;

	//		vecDir = (vecDir + vecDir2) * 0.5;
	//		BestLst.pop_front();
	//		if (!BestLst.empty())
	//		{
	//			vecDir2 = BestLst.front()->m_vPos - m_pTransCom->m_vPos;
	//			vecDir = (vecDir + vecDir2) * 0.5;
	//		}

	//	}

	//	vecDir.Normalize();
	//	_vec3 vMovePos;

	//	vMovePos = m_pNaviCom->MoveOn_NaviMesh(&m_pTransCom->m_vPos, &vecDir, fTimeDelta * 12.f);

	//	m_pTransCom->m_vPos = vMovePos;



	//}





}

void CShepard::ChapterCheck(const _float& fTimeDelta)
{

	switch (m_eCurChapter)
	{
	case START:
		m_eCurState = SEARCH;
		break;
	case TURNPLAYER:
	{
		m_eCurState = TURNRIGHT;
		TurnToPlayer(fTimeDelta);
		break;
	}
	case GOTOPLAYER:
	{
		m_eCurState = RUNNORTH;

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
