#include "stdafx.h"
#include "Aim.h"
#include "OptionUI.h"
#include "HPBar.h"
#include "IconUI.h"
#include "GunUI.h"
#include "InvenUI.h"
#include "DirectInput.h"
#include "QuestUI.h"

CAim::CAim(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CAim::CAim(const CAim& rhs)
	: Engine::CGameObject(rhs)
{
}

CAim::~CAim()
{
}

HRESULT CAim::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CAim::Ready_GameObject()	
{
	Add_Component();

	return S_OK;
}

HRESULT CAim::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	

	return S_OK;
}

_int CAim::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX/2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY/2) + 1.f;
//	m_pTransCom->m_vPos.x = 0.f;
//	m_pTransCom->m_vPos.y = 0.f;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CAim::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	CGameObject* pOptionUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"OptionUI");
	if (pOptionUI != nullptr)
	{
		if(!dynamic_cast<COptionUI*>(pOptionUI)->Get_ShowUI())
			Show_OtherUI(true);
	}

	if (m_bIsRender == true)
	{
		GetCursorPos(&m_pt);
		ScreenToClient(g_hWnd, &m_pt);
		if(m_iDrawId == 1)
			Show_OtherUI(false);

		FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);
	}

	return NO_EVENT;
}

void CAim::Show_OtherUI(_bool bIsShow)
{
	list<CGameObject*>* pHpBarUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"HPBarUI");
	if (pHpBarUIList != nullptr)
	{
		for (auto& pSrc : *pHpBarUIList)
			dynamic_cast<CHPBar*>(pSrc)->Set_ShowUI(bIsShow);
	}

	list<CGameObject*>* pIconUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"IconUI");
	if (pIconUIList != nullptr)
	{
		for (auto& pSrc : *pIconUIList)
			dynamic_cast<CIconUI*>(pSrc)->Set_ShowUI(bIsShow);
	}

	list<CGameObject*>* pQuestUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"QuestUI");
	if (pQuestUIList != nullptr)
	{
		for (auto& pSrc : *pQuestUIList)
			dynamic_cast<CQuestUI*>(pSrc)->Set_ShowUI(bIsShow);
	}

	CGameObject* pGunUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"GunUI");
	if (pGunUI != nullptr)
		dynamic_cast<CGunUI*>(pGunUI)->Set_ShowUI(bIsShow);

	CGameObject* pInvenUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"InvenUI");
	if (pInvenUI != nullptr)
		dynamic_cast<CInvenUI*>(pInvenUI)->Set_ShowUI(bIsShow);

}

void CAim::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader(m_iDrawId);
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();

}

HRESULT CAim::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UI", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Texture
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_Aim", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom) 
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CAim::Set_ConstantTable()
{
	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matView = INIT_MATRIX;
	_matrix matProj = CGraphicDevice::Get_Instance()->GetOrthoMatrix();
	_matrix matWorld = INIT_MATRIX;



	matWorld._11 = WINSIZEX/2;
	matWorld._22 = WINSIZEY/2;
	matWorld._33 = 1.0f;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;
	matWorld._43 = 0.11f;
	_matrix matWVP = matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CAim::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CAim(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CAim* CAim::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CAim* pInstance = new CAim(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CAim::Free()
{
	CGameObject::Free();
}
