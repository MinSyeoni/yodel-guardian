#include "stdafx.h"
#include "FadeOut.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "DirectInput.h"
#include "StaticCamera.h"
#include "NpcWords.h"
#include "PlayerStatus.h"
#include "Management.h"
#include "Scene_Rail.h"
#include "Scene_Stage.h"
CFadeOut::CFadeOut(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: Engine::CGameObject(pGraphicDevice, pCommandList)
{
}

CFadeOut::CFadeOut(const CFadeOut& rhs)
	: Engine::CGameObject(rhs)
{

}


CFadeOut::~CFadeOut()
{
}

HRESULT CFadeOut::Ready_GameObjectPrototype()
{

	m_fTime = 0.f;
	return S_OK;
}


HRESULT CFadeOut::Ready_GameObject(FADETYPE tType)	// 복사본을 레디할 때 
{
#ifdef _DEBUG
	COUT_STR("Ready Clone FadeOut");
#endif
	//CGameObject::Ready_GameObject();
	Add_Component();

	m_eType = tType;
	if (m_eType == FADEIN)
		m_fTime = 1.f;
	if (m_eType == FADEOUTTOSCENERAIL)
		m_fTime = 0.f;

	return S_OK;
}

HRESULT CFadeOut::LateInit_GameObject()
{

	m_pShaderCom->Set_Shader_Texture(m_pTexture->Get_Texture());	// 셰이더에 텍스쳐 알려줘여 함.

	return S_OK;
}

_int CFadeOut::Update_GameObject(const _float& fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);

	if (m_bIsDead)
		return DEAD_OBJ;

	//m_pTransCom->m_vAngle.y += 90.f * fTimeDelta;

	if (m_eType == FIRST)
	{
		if (!m_bIsReturn)
			m_fTime += fTimeDelta;
		else
			m_fTime -= fTimeDelta;

		if (!m_bIsReturn)
		{
			if (m_fTime > 1.f)
			{

				CheckFadeIn();
				m_bIsReturn = true;

			}
		}

		if (m_bIsReturn && m_fTime < 0)
		{
			CheckFadeOut();
			return DEAD_OBJ;

		}

	}
	else if (m_eType == FADEIN)
	{

		m_fTime -= fTimeDelta*0.3f;


		if (m_fTime < 0.f)
			return DEAD_OBJ;


	}
	else if (m_eType == FADEOUTTOSCENERAIL)
	{

		m_fTime += fTimeDelta;
		if (m_fTime > 1.0f)
		{
			m_fTime = 1.f;

			static_cast<CScene_Stage*>(CManagement::Get_Instance()->Get_CurScene())->SceneChange();
		}




	}
	else if (m_eType == FADEOUTSCENEBOSS)
	{

		m_fTime += fTimeDelta;
		if (m_fTime > 1.0f)
		{
			m_fTime = 1.f;
			static_cast<CScene_Rail*>(CManagement::Get_Instance()->Get_CurScene())->SceneChange();
		}



	}


	/*____________________________________________________________________
	TransCom - Update WorldMatrix.
	______________________________________________________________________*/
	Engine::CGameObject::Update_GameObject(fTimeDelta);


	return NO_EVENT;
}

_int CFadeOut::LateUpdate_GameObject(const _float& fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	/*____________________________________________________________________
	[ Renderer - Add Render Group ]
	______________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_UI, this), -1);

	/*____________________________________________________________________
	[ Set PipelineState ]
	______________________________________________________________________*/


	return NO_EVENT;
}

void CFadeOut::Render_GameObject(const _float& fTimeDelta)
{
	Set_ConstantTable();
	// 세이더 - 버퍼 - 세이더 - 버퍼 순서 중요 

	m_pShaderCom->Begin_Shader();
	m_pBufferCom->Begin_Buffer();

	m_pShaderCom->End_Shader();
	m_pBufferCom->End_Buffer();

	m_pBufferCom->Render_Buffer();
}

void CFadeOut::CheckFadeIn()
{
	switch (m_eType)
	{
	case CFadeOut::FADEOUT:
		break;
	case CFadeOut::FADEIN:
		break;
	case CFadeOut::FADEOUTIN:
		break;
	case CFadeOut::FIRST:
	{
			StaticCameraInfo tInfo;
			tInfo.vAtPos = _vec3(330.f, 15.f, 495.f);
			tInfo.vEyePos = _vec3(280.f, 15.f, 495.f);
	        m_pObjectMgr->Add_GameObject(L"Layer_Camera", L"Prototype_StaticCamera", L"StaticCamera", &tInfo);
         

			CNpcWords::WORDS_TYPE eType = CNpcWords::NPC;
			m_pObjectMgr->Add_GameObject(L"Layer_UI", L"Prototype_NpcBoard", L"NpcBoard", &eType);



			CGameObject* pPlayer = m_pObjectMgr->Get_GameObject(L"Layer_GameObject", L"Player");
			if (pPlayer == nullptr)
				return;

			static_cast<CPlayerStatus*>(static_cast<CPlayer*>(pPlayer)->Get_Status())->m_eCurScene = CPlayerStatus::FIRSTSCENE;
			static_cast<CPlayerStatus*>(static_cast<CPlayer*>(pPlayer)->Get_Status())->m_eCurState = CPlayer::NONEIDLE;
		break;
	}
	case CFadeOut::SECOND:
		break;
	default:
		break;
	}



}

void CFadeOut::CheckFadeOut()
{
}

HRESULT CFadeOut::Add_Component()
{
	NULL_CHECK_RETURN(m_pComponentMgr, E_FAIL);

	// Buffer
	m_pBufferCom = static_cast<Engine::CRcTex*>(m_pComponentMgr->Clone_Component(L"Prototype_RcTex", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pBufferCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Buffer", m_pBufferCom);


	// Shader
	m_pShaderCom = static_cast<Engine::CShader_Effect*>(m_pComponentMgr->Clone_Component(L"Prototype_Shader_FadeOut", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Shader", m_pShaderCom);

	// Texture
	m_pTexture = static_cast<Engine::CTexture*>(m_pComponentMgr->Clone_Component(L"Prototype_Texture_Logo", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Texture", m_pTexture);

	return S_OK;
}

void CFadeOut::Set_ConstantTable()
{
	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	_matrix matView = INIT_MATRIX;
	_matrix matProj = CGraphicDevice::Get_Instance()->GetOrthoMatrix();
	_matrix matWorld = INIT_MATRIX;

	matWorld._11 = WINSIZEX / 2;
	matWorld._22 = WINSIZEY / 2;
	matWorld._33 = 1.00;
	matWorld._41 = 0.f;
	matWorld._42 = 0.f;
	matWorld._43 = 0.12f;
	_matrix matWVP = matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));

	matWorld._11 = m_fTime;

	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));
	m_pShaderCom->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);
}

CGameObject* CFadeOut::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CFadeOut(*this);

	  FADETYPE  eType = *reinterpret_cast<FADETYPE*>(pArg);


	if (FAILED(static_cast<CFadeOut*>(pInstance)->Ready_GameObject(eType)))
		return nullptr;

	return pInstance;
}

CFadeOut* CFadeOut::Create(ID3D12Device* pGraphicDevice,
	ID3D12GraphicsCommandList* pCommandList)
{
	CFadeOut* pInstance = new CFadeOut(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFadeOut::Free()
{
	CGameObject::Free();
}
