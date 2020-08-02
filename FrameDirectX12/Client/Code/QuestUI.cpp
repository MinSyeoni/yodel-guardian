#include "stdafx.h"
#include "QuestUI.h"
#include "DirectInput.h"

CQuestUI::CQuestUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CQuestUI::CQuestUI(const CQuestUI& rhs)
	: Engine::CGameObject(rhs)
{
}

CQuestUI::~CQuestUI()
{
}

HRESULT CQuestUI::Ready_GameObjectPrototype()
{
	return S_OK;
}

HRESULT CQuestUI::Ready_GameObject()	
{
	Add_Component();

	return S_OK;
}

HRESULT CQuestUI::LateInit_GameObject()
{
	for(int i = 0; i < 10; ++i)
		m_pShaderCom[i]->Set_Shader_Texture(m_pTexture[i]->Get_Texture());	

	return S_OK;
}

_int CQuestUI::Update_GameObject(const _float& fTimeDelta)
{
	if (KEY_DOWN(DIK_8))
		m_bIsShow *= -1;
           
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	m_pTransCom->m_vPos.x = _float(2.f / WINCX * WINCX/2) - 1.f;
	m_pTransCom->m_vPos.y = _float(-2.f / WINCY * WINCY/2) + 1.f;


	Engine::CGameObject::Update_GameObject(fTimeDelta);

	return NO_EVENT;
}

_int CQuestUI::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	/////// 퀘스트 창 넘어가기 테스트 ///////
	if (CDirectInput::Get_Instance()->KEY_DOWN(DIK_N))
	{
		if (m_iTest < 9)
			m_iTest++;
		else
			m_iTest--;

		m_eQuestType = (QUEST_TYPE)m_iTest;
	}

	return NO_EVENT;
}

void CQuestUI::Render_GameObject(const _float& fTimeDelta)
{
	if (!m_bIsShow)
		return;

	Set_ConstantTable((_uint)m_eQuestType);

	m_pShaderCom[(_uint)m_eQuestType]->Begin_Shader();
	m_pBufferCom->Begin_Buffer();
	m_pShaderCom[(_uint)m_eQuestType]->End_Shader();
	m_pBufferCom->End_Buffer();
	m_pBufferCom->Render_Buffer();
}

HRESULT CQuestUI::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);

	// Shader
	for (int i = 0; i < 10; ++i)
	{
		wstring wstrText = L"";
		string strTemp = "";
		strTemp = "Prototype_Texture_QuestUI_" + to_string(i);
		wstrText.assign(strTemp.begin(), strTemp.end());

		wstring wstrText2 = L"";
		string strTemp2 = "";
		strTemp2 = "Com_Shader" + to_string(i);
		wstrText2.assign(strTemp2.begin(), strTemp2.end());

		m_pShaderCom[i] = static_cast<Engine::CShader_UI*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_UI", COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pShaderCom[i], E_FAIL);
		m_mapComponent[ID_STATIC].emplace(wstrText2.c_str(), m_pShaderCom[i]);
		
		wstrText2 = L"";
		strTemp2 = "Com_Texture" + to_string(i);
		wstrText2.assign(strTemp2.begin(), strTemp2.end());

		// Texture
		m_pTexture[i] = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(wstrText.c_str(), COMPONENTID::ID_STATIC));
		NULL_CHECK_RETURN(m_pTexture[i], E_FAIL);
		m_mapComponent[ID_STATIC].emplace(wstrText2.c_str(), m_pTexture[i]);
	}

	// TransCom 
	m_pTransCom = static_cast<CTransform*>(m_pComponentMgr->Clone_Component(L"Prototype_Transform", COMPONENTID::ID_DYNAMIC));
	if (nullptr != m_pTransCom)
		m_mapComponent[ID_DYNAMIC].emplace(L"Com_Transform", m_pTransCom);

	return S_OK;
}

void CQuestUI::Set_ConstantTable(_uint iIdx)
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

	m_pShaderCom[iIdx]->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CQuestUI::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CQuestUI(*this);

	_uint iQuestIdx = *reinterpret_cast<_uint*>(pArg);
	static_cast<CQuestUI*>(pInstance)->m_eQuestType = (QUEST_TYPE)iQuestIdx;

	if (FAILED(pInstance->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CQuestUI* CQuestUI::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CQuestUI* pInstance = new CQuestUI(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CQuestUI::Free()
{
	CGameObject::Free();
}
