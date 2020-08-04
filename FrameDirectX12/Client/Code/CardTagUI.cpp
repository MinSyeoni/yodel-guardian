#include "stdafx.h"
#include "CardTagUI.h"
#include "Player.h"
#include "DirectInput.h"
#include "PassageDoor.h"
#include "TagBack.h"
#include "StaticCamera.h"

CCardTagUI::CCardTagUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CCardTagUI::CCardTagUI(const CCardTagUI& rhs)
	: Engine::CGameObject(rhs)
{
}

CCardTagUI::~CCardTagUI()
{
}

HRESULT CCardTagUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CCardTagUI::Ready_GameObject()
{
	Add_Component();

	return S_OK;
}

HRESULT CCardTagUI::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());

	m_pTransCom->m_vPos = _vec3(262, 18.f, 500.f);
	m_pTransCom->m_vAngle = _vec3(0.f, -90.f, 0.f);
	m_pTransCom->m_vScale = _vec3(8.f, 6.f, 8.f);

	m_fCurTag = 100.f;

	return S_OK;
}

_int CCardTagUI::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

//	BillBoard();

	DoorAndTag_Interaction();

	return NO_EVENT;
}

void CCardTagUI::ZoomCamera(bool bIsZoom)
{
	if (bIsZoom == true)
	{
		StaticCameraInfo tInfo;

		_vec3 vLook, vUp;
		memcpy(&vLook, &m_pTransCom->m_matWorld._31, sizeof(_vec3));
		memcpy(&vUp, &m_pTransCom->m_matWorld._21, sizeof(_vec3));

		_vec3 vPosTmp = m_pTransCom->m_vPos;
		vPosTmp.z = 500.f;
		vPosTmp.y = 15.f;
		_vec3 vEyePos = vPosTmp + vLook * - 2.f + vUp * -0.2f;
		tInfo.vAtPos = vPosTmp; 
		tInfo.vEyePos = vEyePos;
		m_pObjectMgr->Add_GameObject(L"Layer_Camera", L"Prototype_StaticCamera", L"StaticCamera", &tInfo);

		CGameObject* pPlayer = m_pObjectMgr->Get_GameObject(L"Layer_GameObject", L"Player");
		if (pPlayer == nullptr)
			return;
		dynamic_cast<CPlayer*>(pPlayer)->KeyLockPlayer(true);
	}
	else
	{
		if (nullptr != m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"StaticCamera"))
			m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"StaticCamera")->Dead_GameObject();

		m_pObjectMgr->Add_GameObject(L"Layer_Camera", L"Prototype_DynamicCamera", L"DynamicCamera", nullptr);

		CGameObject* pPlayer = m_pObjectMgr->Get_GameObject(L"Layer_GameObject", L"Player");
		if (pPlayer == nullptr)
			return;
		dynamic_cast<CPlayer*>(pPlayer)->KeyLockPlayer(false);
	}
}

void CCardTagUI::DoorAndTag_Interaction()
{
	CGameObject* pPassageDoor = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"PassageDoor");
	CGameObject* pTagBack = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"TagBack");

	if (m_bIsTagOn && nullptr != pTagBack && !m_bIsZoom && !m_bIsClear)
	{
		m_bIsShow = true;
		dynamic_cast<CTagBack*>(pTagBack)->Set_ShowUI(true);
		ZoomCamera(true);
		m_bIsZoom = true;
	}

	if (nullptr != pPassageDoor)
	{
		if (m_bIsClear && m_bIsZoom)
		{
			dynamic_cast<CTagBack*>(pTagBack)->Set_CurCardTag(CTagBack::TAG_CLEAR);
			dynamic_cast<CPassageDoor*>(pPassageDoor)->Set_IsCardToDoor(true);
			dynamic_cast<CPassageDoor*>(pPassageDoor)->Set_IsReaderOnDoor(true);
		}
	}
}

_int CCardTagUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	CardTagOn_Clear(fTimeDelta);

	m_pTransCom->m_matWorld._32 = m_fCurTag * 0.01;

	TagReaderAndZoom(fTimeDelta);

	return NO_EVENT;
}

void CCardTagUI::TagReaderAndZoom(const _float& fTimeDelta)
{
	CGameObject* pTagBack = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"TagBack");
	if (m_bIsZoom && m_bIsClear && m_bIsAlreadyZoom)
	{
		m_fTagDelay += fTimeDelta;

		if (m_fTagDelay >= 2.f)
		{
			m_fTagDelay = 0.f;
			m_bIsZoom = false;
			ZoomCamera(false);
			m_bIsShow = false;
			dynamic_cast<CTagBack*>(pTagBack)->Set_ShowUI(false);
		}
	}
}

void CCardTagUI::CardTagOn_Clear(const _float& fTimeDelta)
{
	if (m_bIsTagOn && !m_bIsClear &&!m_bIsAlreadyZoom)
	{
		if (m_fCurTag > 0)
			m_fCurTag -= 15.f * fTimeDelta;
		else
		{
			m_fCurTag = 0.f;
			m_bIsClear = true;
			m_bIsTagOn = false;
			m_bIsAlreadyZoom = true;
		}
	}
}

void CCardTagUI::Render_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsShow)
		return;

	Set_ConstantTable();
	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();
	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();
	m_pBufferCom->Render_Buffer();
}

void CCardTagUI::BillBoard()
{
	_matrix matBill, matView;

	matBill = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matView = matBill;
	ZeroMemory(&matBill.m[3][0], sizeof(_vec3));

	matBill._11 = matView._11;
	matBill._13 = matView._13;
	matBill._31 = matView._31;
	matBill._33 = matView._33;

	matBill = XMMatrixInverse(nullptr, matBill);
	m_pTransCom->m_matWorld = matBill * m_pTransCom->m_matWorld;
}

HRESULT CCardTagUI::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	// Shader
	m_pShaderCom = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_CARDTAG", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Texture 
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_CardTag", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CCardTagUI::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;

	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CCardTagUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CCardTagUI(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CCardTagUI* CCardTagUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CCardTagUI* pInstance = new CCardTagUI(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CCardTagUI::Free()
{
	CGameObject::Free();
}
