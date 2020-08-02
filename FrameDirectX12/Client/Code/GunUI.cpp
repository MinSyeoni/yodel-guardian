#include "stdafx.h"
#include "GunUI.h"
#include "Font.h"
#include "DirectInput.h"

CGunUI::CGunUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CGunUI::CGunUI(const CGunUI& rhs)
	: Engine::CGameObject(rhs)
	, m_pGunTag(rhs.m_pGunTag)
	, m_iCurBullet(rhs.m_iCurBullet)
	, m_iMaxBullet(rhs.m_iMaxBullet)
{
}

CGunUI::~CGunUI()
{
}

HRESULT CGunUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CGunUI::Ready_GameObject()
{
	Add_Component();

	if (m_pGunTag == L"Prototype_Texture_Rifle")
		m_iCurBullet = m_iMaxBullet = 30;

	return S_OK;
}

HRESULT CGunUI::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	
	m_pBulletFont->LateInit_GameObject();

	return S_OK;
}

_int CGunUI::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vScale = _vec3(0.25f, 0.2f, 0.21f);

	m_pTransCom->m_vPos.x = _float(WINCX / 2.f) / _float(WINCX / 1.4f) + 0.01f;
	m_pTransCom->m_vPos.y = _float(WINCY / 1.5f) / _float(WINCY / 0.1f) - 0.85f;

	///// [임시]총알 폰트 설정 ///// 
	wstring strText = L"";
	string strTemp = "";

	strTemp = to_string(m_iCurBullet);	// 플레이어 정보 받아와야 함. 
	strTemp += "	";
	strTemp += to_string(m_iMaxBullet);

	strText.assign(strTemp.begin(), strTemp.end());

	m_pBulletFont->Set_Pos(_vec2(WINCX * 0.9f, WINCY * 0.89f));
	m_pBulletFont->Set_Text(strText.c_str());
	////////////////////////
	
	if (m_bIsShow)
		m_pBulletFont->Update_GameObject(fTimeDelta);

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CGunUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	if (m_bIsShow)
		m_pBulletFont->LateUpdate_GameObject(fTimeDelta);

	////// <임시> //////
	if (Engine::CDirectInput::Get_Instance()->Mouse_KeyDown(DIM_LB))
	{
		if(m_iCurBullet > 0)
			m_iCurBullet -= 1;
		else
			m_iCurBullet = 0;
	}

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);


	return NO_EVENT;
}

void CGunUI::Render_GameObject(const _float& fTimeDelta)
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

HRESULT CGunUI::Add_Component()
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
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(m_pGunTag, COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	// Font
	m_pBulletFont = static_cast<CFont*>(CObjectMgr::Get_Instance()->Get_NewGameObject(L"Prototype_Font_NetmarbleLight", L"fuck", nullptr));
	m_pBulletFont->Ready_GameObjectClone(L"A", _vec2{ 0.f, 0.f }, D2D1::ColorF::White);

	return S_OK;
}

void CGunUI::Set_ConstantTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matWVP = m_pTransCom->m_matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(m_pTransCom->m_matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CGunUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CGunUI(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CGunUI* CGunUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const _tchar* pTag)
{
	CGunUI* pInstance = new CGunUI(pGraphicDevice, pCommandList);
	pInstance->m_pGunTag = pTag;

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CGunUI::Free()
{
	CGameObject::Free();
	Safe_Release(m_pBulletFont);
}
