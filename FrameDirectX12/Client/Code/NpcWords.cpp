#include "stdafx.h"
#include "NpcWords.h"
#include "Font.h"
#include "HPBar.h"
#include "IconUI.h"
#include "GunUI.h"
#include "InvenUI.h"
#include "Player.h"
#include "PlayerStatus.h"
#include "EquipUI.h"
#include "Shepard.h"
#include "Weapon.h"
#include "MPBar.h"
#include "QuestUI.h"

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

HRESULT CNpcWords::Ready_GameObject(WORDS_TYPE eType)
{
	m_eWordsType = eType;
	if (FAILED(Add_Component()))
		return E_FAIL;

	Ready_NpcWords();
	m_bIsShow = true;

	m_pTransCom->m_vScale = _vec3(1.f, 1.f, 1.f);
	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX / 2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY / 2) + 1.f;

	Init_OthersUI();

	return S_OK;
}

void CNpcWords::Init_OthersUI()
{
	list<CGameObject*>* pHpBarUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"HPBarUI");
	if (pHpBarUIList != nullptr)
	{
		for (auto& pSrc : *pHpBarUIList)
			dynamic_cast<CHPBar*>(pSrc)->Set_ShowUI(false);
	}

	list<CGameObject*>* pIconUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"IconUI");
	if (pIconUIList != nullptr)
	{
		for (auto& pSrc : *pIconUIList)
			dynamic_cast<CIconUI*>(pSrc)->Set_ShowUI(false);
	}

	CGameObject* pMPBarUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"MPBarUI");
	if (pMPBarUI != nullptr)
		dynamic_cast<CMPBar*>(pMPBarUI)->Set_ShowUI(false);

	CGameObject* pGunUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"GunUI");
	if (pGunUI != nullptr)
		dynamic_cast<CGunUI*>(pGunUI)->Set_ShowUI(false);

	CGameObject* pInvenList = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"InvenUI");
	if (pInvenList != nullptr)
		dynamic_cast<CInvenUI*>(pInvenList)->Set_ShowUI(false);
}

void CNpcWords::Ready_NpcWords()
{
	switch (m_eWordsType)
	{
	case CNpcWords::NPC:
	{
		wstring wstrWords = L"이제 정신이 드는가?";
		m_vWords.push_back(wstrWords);
		m_iWordsCnt++;

		wstrWords = L"아...머리야 여기가 도대체 어디죠?";
		m_vWords.push_back(wstrWords);
		m_iWordsCnt++;

		wstrWords = L"여기는 화성행성이다. 지금 우리종족은 외계생물에게 공격을받고있다.";
		m_vWords.push_back(wstrWords);
		m_iWordsCnt++;

		wstrWords = L"저는 무엇을 해야하나요? 전투할 준비가 전혀 되있지않습니다.";
		m_vWords.push_back(wstrWords);
		m_iWordsCnt++;

		wstrWords = L"앞에있는 라이플을 가지고 문밖을나가서 동료를 돕도록해!";
		m_vWords.push_back(wstrWords);
		m_iWordsCnt++;
	break;
	}
	case CNpcWords::SHEPARD:
	{
		wstring wstrWords = L"왜 이제서야 왔어 지금좀비가 달려오고있어  빨리싸울태세를 갖춰!! ";
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

	Reset_OthersUI(false);
}

HRESULT CNpcWords::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	
	m_pWordsFont->LateInit_GameObject();
	
	Init_OthersUI();

	return S_OK;
}

_int CNpcWords::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_eWordsType == TYPE_END)
		return E_FAIL;

	m_fAccTime += fTimeDelta * 8.f;

	Reset_OthersUI(false);

	if (m_bIsDead)
	{	
		m_eWordsType = TYPE_END;
	//	Reset_OthersUI(true);
		return DEAD_OBJ;
	}
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

void CNpcWords::Check_Interaction()
{
	CGameObject* pPlayer = m_pObjectMgr->Get_GameObject(L"Layer_GameObject", L"Player");
	if (pPlayer == nullptr)
		return;
	switch (m_eWordsType)
	{
	case CNpcWords::NPC:
	{

		if (m_iWordsNum == 1)
		{

			static_cast<CPlayerStatus*>(static_cast<CPlayer*>(pPlayer)->Get_Status())->m_eCurState = CPlayer::HEADSTART;
		}
		else if (m_iWordsNum == 2)
		{

			static_cast<CPlayerStatus*>(static_cast<CPlayer*>(pPlayer)->Get_Status())->m_eCurState = CPlayer::HEADEXIT;
		}
		else if (m_iWordsNum == 3)
		{

			static_cast<CPlayerStatus*>(static_cast<CPlayer*>(pPlayer)->Get_Status())->m_eCurState = CPlayer::SPEAKING;         

		}
		else if (m_iWordsNum == 4)
		{

			static_cast<CPlayerStatus*>(static_cast<CPlayer*>(pPlayer)->Get_Status())->m_eCurState = CPlayer::NONEIDLE;
		}
		break;

	}
	case CNpcWords::SHEPARD:
		break;
	case CNpcWords::ETC:
		break;
	case CNpcWords::TYPE_END:
		break;
	default:
		break;
	}


}

void CNpcWords::Start_Interaction()
{
}

void CNpcWords::Finish_ConverSation()
{


	switch (m_eWordsType)
	{
	case CNpcWords::NPC:
	{
		if (nullptr != m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"StaticCamera"))
			m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"StaticCamera")->Dead_GameObject();
     
	   m_pObjectMgr->Add_GameObject(L"Layer_Camera", L"Prototype_DynamicCamera", L"DynamicCamera", nullptr);

	   CWeapon::WEAPONSTATE eState = CWeapon::DROP;
	   m_pObjectMgr->Add_GameObject(L"Layer_GameObject", L"Prototype_Rifle", L"Weapon", &eState);
		CGameObject* pPlayer = m_pObjectMgr->Get_GameObject(L"Layer_GameObject", L"Player");
		if (pPlayer == nullptr)
			return;
		static_cast<CPlayerStatus*>(static_cast<CPlayer*>(pPlayer)->Get_Status())->m_eCurScene = CPlayerStatus::NOSCENE;

		static_cast<CPlayer*>(pPlayer)->KeyLockPlayer(false);

		CObjectMgr::Get_Instance()->SetTimeStop(true);

		break;
	}
	case CNpcWords::SHEPARD:
	{
		if (nullptr != m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"StaticCamera"))
			m_pObjectMgr->Get_GameObject(L"Layer_Camera", L"StaticCamera")->Dead_GameObject();

		m_pObjectMgr->Add_GameObject(L"Layer_Camera", L"Prototype_DynamicCamera", L"DynamicCamera", nullptr);


		CGameObject* pPlayer = m_pObjectMgr->Get_GameObject(L"Layer_GameObject", L"Player");
		if (pPlayer == nullptr)
			return;
		static_cast<CPlayer*>(pPlayer)->KeyLockPlayer(false);
		CObjectMgr::Get_Instance()->SetTimeStop(true);


		CGameObject* pShapard = m_pObjectMgr->Get_GameObject(L"Layer_GameObject", L"Shepard");
		dynamic_cast<CShepard*>(pShapard)->SetChapter(CShepard::ATTACK);

		CGameObject* pQuestUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"QuestUI");
		if (pQuestUI != nullptr)
		{
			dynamic_cast<CQuestUI*>(pQuestUI)->Set_CurQUEST_TYPE(CQuestUI::QUEST_TYPE1);
			// 카드키 미션창은 여기서 말고 카드키 줍기 직전에 바꿔줘야돼 동료 미션으로 바꿔놓을게 
		}

		break;
	}
	case CNpcWords::ETC:
		break;
	case CNpcWords::TYPE_END:
		break;
	default:
		break;
	}

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
	if (m_bIsFinish == true && m_eWordsType==SHEPARD)
	{
		CObjectMgr::Get_Instance()->SetTimeStop(false);

	}



	if (m_bIsFinish == true && CDirectInput::Get_Instance()->KEY_DOWN(DIK_SPACE))
	{
		m_bIsFinish = false;
		m_iWordsNum++;
		m_fAccTime = 0.f;
		Check_Interaction();
		if (m_iWordsCnt <= m_iWordsNum)
		{ 
			Reset_OthersUI(true);
			Finish_ConverSation();

			m_eWordsType = TYPE_END;
			m_bIsDead = true;
		}
	}
}

void CNpcWords::Reset_OthersUI(_bool bIsReset)
{
	list<CGameObject*>* pHpBarUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"HPBarUI");
	if (pHpBarUIList != nullptr)
	{
		for (auto& pSrc : *pHpBarUIList)
			dynamic_cast<CHPBar*>(pSrc)->Set_ShowUI(bIsReset);
	}

	list<CGameObject*>* pIconUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"IconUI");
	if (pIconUIList != nullptr)
	{
		for (auto& pSrc : *pIconUIList)
			dynamic_cast<CIconUI*>(pSrc)->Set_ShowUI(bIsReset);
	}

	CGameObject* pMPBarUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"MPBarUI");
	if (pMPBarUI != nullptr)
		dynamic_cast<CMPBar*>(pMPBarUI)->Set_ShowUI(bIsReset);

	CGameObject* pGunUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"GunUI");
	if (pGunUI != nullptr)
		dynamic_cast<CGunUI*>(pGunUI)->Set_ShowUI(bIsReset);

	CGameObject* pInvenUI = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_UI", L"InvenUI");
	if (pInvenUI != nullptr)
		dynamic_cast<CInvenUI*>(pInvenUI)->Set_ShowUI(bIsReset);

	//list<CGameObject*>* pEquipUIList = CObjectMgr::Get_Instance()->Get_OBJLIST(L"Layer_UI", L"EquipUI");
	//if (pEquipUIList != nullptr)
	//{
	//	for (auto& pSrc : *pEquipUIList)
	//		dynamic_cast<CEquipUI*>(pSrc)->Set_ShowUI(!bIsReset);
	//}
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
	NULL_CHECK_RETURN(m_pTexture, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture); 

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	// Font
	m_pWordsFont = static_cast<CFont*>(CObjectMgr::Get_Instance()->Get_NewGameObject(L"Prototype_Font_NPC", L"fuck", nullptr));
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
	WORDS_TYPE eType = *reinterpret_cast<WORDS_TYPE*>(pArg);
	if (FAILED(static_cast<CNpcWords*>(pInstance)->Ready_GameObject(eType)))
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
