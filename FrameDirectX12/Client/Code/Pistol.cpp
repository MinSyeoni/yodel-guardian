#include "stdafx.h"
#include "Pistol.h"
#include "Player.h"
#include "PlayerArm.h"
#include "PlayerLeg.h"
#include "ObjectMgr.h"

CPistol::CPistol(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CWeapon(pGraphicDevice,pCommandList)
{

}

CPistol::CPistol(const CPistol & rhs)
	:CWeapon(rhs)
{
}

HRESULT CPistol::Ready_GameObjectPrototype()
{

	return S_OK;
}

HRESULT CPistol::Ready_GameObject()
{
	CWeapon::AddComponent();
	AddComponent();
	
	return S_OK;
}

HRESULT CPistol::LateInit_GameObject()
{
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture(), m_pMeshCom->Get_EmissiveTexture());
	m_pTransCom->m_vScale = _vec3(1.0f, 1.0f, 1.0f);
	m_pTransCom->m_vPos = _vec3(0.f, 0.f, 0.f);
	CGameObject* pPlayer = CObjectMgr::Get_Instance()->Get_GameObject(L"Layer_GameObject", L"Player");

	if (pPlayer != nullptr)
	{
		m_pPlayerArm = static_cast<CPlayer*>(pPlayer)->Get_PlayerArm();
		m_pPlayerLeg = static_cast<CPlayer*>(pPlayer)->Get_PlayerLeg();
	}


	m_pPlayerArmMatrix = static_cast<CMesh*>(m_pPlayerArm->Get_Component(L"Com_Mesh", COMPONENTID::ID_STATIC))->Get_AnimationComponent()->Get_WeaponMatrix();
	m_pPlayerMatrix = &(m_pPlayerArm->Get_Transform()->m_matWorld);
	m_pHandleMatrix = m_pMeshCom->Find_BoneMatrix("handle");

	m_eWeaponType = PISTOL;
	return S_OK;
}

_int CPistol::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);


	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);

	

	//dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation((_int)BASE);
	//m_vecBoneMatirx = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f*fTimeDelta);



	return NO_EVENT;
}

_int CPistol::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	//FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	//FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);

	FallowPlayer();
	return S_OK;
}

HRESULT CPistol::AddComponent()
{

	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Pistol", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);


	return S_OK;
}

void CPistol::FallowPlayer()
{

	_matrix Rotation = XMMatrixRotationY(XMConvertToRadians(-90.f));

	_matrix matBlend;
	if (m_pPlayerArmMatrix != nullptr)
	{

		matBlend = (*m_pPlayerArmMatrix* 1.0f);

		m_pTransCom->m_matWorld = *m_pHandleMatrix* matBlend* (Rotation* *m_pPlayerMatrix);

	};
}

CGameObject * CPistol::Clone_GameObject(void * prg)
{
	CGameObject* pInstance = new CPistol(*this);

	if (FAILED(dynamic_cast<CPistol*>(pInstance)->Ready_GameObject()))
		return nullptr;

	return pInstance;
}

CPistol * CPistol::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
{
	CPistol* pInstance = new CPistol(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CPistol::Free()
{
	CWeapon::Free();
}
