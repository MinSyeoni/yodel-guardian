#include "stdafx.h"
#include "NpcWords.h"
#include "Font.h"
#include "HPBar.h"
#include "IconUI.h"
#include "GunUI.h"
#include "InvenUI.h"

CNpcWords::CNpcWords(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CNpcWords::CNpcWords(const CNpcWords& rhs)
	: Engine::CGameObject(rhs)
	, m_eWordsType(rhs.m_eWordsType)
{
}

CNpcWords::~CNpcWords()
{
}

HRESULT CNpcWords::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CNpcWords::Ready_GameObject()
{
	if (FAILED(Add_Component()))
		return E_FAIL;

	m_pTransCom->m_vScale = _vec3(1.f, 1.f, 1.f);
	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + 1.f;

	return S_OK;
}

void CNpcWords::Ready_NpcWords()
{
	switch (m_eWordsType)
	{
	case CNpcWords::NPC:
	{
		wstring wstrWords = L"Å×½ºÆ® ÀÔ´Ï´Ù. ¹Î¼ö¿° ¼ö¿° ¹Î¼ö¿¬¾î ±èÈñÁÜÁÜ";
		m_vWords.push_back(wstrWords);
		m_iWordsCnt++;

		wstrWords = L"¸¶¶óÅÁ ¸Ô°í½Í´Ù ²ã¹Ù·Î¿ì¶û";
		m_vWords.push_back(wstrWords);
		m_iWordsCnt++;
	}
	break;
	case CNpcWords::ETC:
	{

	}
		break;
	default:
		break;
	}
}

HRESULT CNpcWords::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	
	m_pWordsFont->LateInit_GameObject();

	return S_OK;
}

_int CNpcWords::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_eWordsType == TYPE_END)
		return E_FAIL;

	m_fAccTime += fTimeDelta * 8.f;
	
	if (m_bIsDead)
		return DEAD_OBJ;

	Next_Conversation(fTimeDelta);
	Show_ConversationWords();	

	m_pWordsFont->Update_GameObject(fTimeDelta);
	
	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

void CNpcWords::Show_ConversationWords()
{
	wstring strText;

	for (_int i = 0; i < (_int)m_fAccTime; i++)
	{
		if ((_uint)i > m_vWords[m_iWordsNum].length())
		{
			m_bIsFinish = true;
			break;
		}
		strText += m_vWords[m_iWordsNum][i];
	}
	m_pWordsFont->Set_Pos(_vec2(WINCX * 0.2f, WINCY * 0.8f));
	m_pWordsFont->Set_Text(strText.c_str());
}

void CNpcWords::Next_Conversation(const _float& fTimeDelta)
{
	if (m_bIsNext == false)
		m_fNextTime += fTimeDelta;
	else
		m_fNextTime -= fTimeDelta;

	if (m_fNextTime > 1.f)
		m_bIsNext = true;
	else if (m_fNextTime < 0.f)
		m_bIsNext = false;
}

_int CNpcWords::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	if (m_eWordsType == TYPE_END)
		return E_FAIL;

	m_pWordsFont->LateUpdate_GameObject(fTimeDelta);

	Next_ConversationJudje();

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);
	return NO_EVENT;
}

void CNpcWords::Next_ConversationJudje()
{
	if (m_bIsFinish == true && CDirectInput::Get_Instance()->KEY_DOWN(DIK_SPACE))
	{
		m_bIsFinish = false;
		m_iWordsNum++;
		m_fAccTime = 0.f;

		if (m_iWordsCnt <= m_iWordsNum)
		{
			list<CGameObject*>* pHpBarUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"HPBarUI");
			for (auto& pSrc : *pHpBarUIList)
				dynamic_cast<CHPBar*>(pSrc)->Set_ShowUI(true);

			CGameObject* pIconUIList = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"IconUI");
			dynamic_cast<CIconUI*>(pIconUIList)->Set_ShowUI(true);

			CGameObject* pGunUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"GunUI");
			dynamic_cast<CGunUI*>(pGunUI)->Set_ShowUI(true);

			CGameObject* pInvenList = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"InvenUI");
			dynamic_cast<CInvenUI*>(pInvenList)->Set_ShowUI(true);

			m_eWordsType = TYPE_END;
			m_bIsShow = false;
		//	m_bIsDead = true;
		}
	}
}

void CNpcWords::Render_GameObject(const _float& fTimeDelta)
{
	if (m_eWordsType == TYPE_END)
		return;
	if (!m_bIsShow)
		return;

	Set_ConstantTable();

	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();
}

HRESULT CNpcWords::Add_Component()
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
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_NpcTalkBoard", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	// Font
	m_pWordsFont = static_cast<CFont*>(CObjectMgr::Get_Instance()->Get_NewGameObject(L"Prototype_Font_NetmarbleLight", L"fuck", nullptr));
	m_pWordsFont->Ready_GameObjectClone(L"", _vec2{ 0.f, 0.f }, D2D1::ColorF::White);

	return S_OK;
}

void CNpcWords::Set_ConstantTable()
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

CGameObject* CNpcWords::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CNpcWords(*this);

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CNpcWords* CNpcWords::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CNpcWords* pInstance = new CNpcWords(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNpcWords::Free()
{
	CGameObject::Free();
	Safe_Release(m_pWordsFont);
}
