#include "stdafx.h"
#include "Pistol.h"

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
	m_pShaderCom->Set_Shader_Texture(m_pMeshCom->Get_Texture(), m_pMeshCom->Get_NormalTexture(), m_pMeshCom->Get_SpecularTexture());
	m_pTransCom->m_vPos = _vec3(300.f, 2.f, 350.f);
	m_pTransCom->m_vScale = _vec3(0.1f, 0.1f, 0.1f);
	return S_OK;
}

_int CPistol::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(Engine::CGameObject::LateInit_GameObject(), E_FAIL);


	if (m_bIsDead)
		return DEAD_OBJ;

	Engine::CGameObject::Update_GameObject(fTimeDelta);


	dynamic_cast<CMesh*>(m_pMeshCom)->Set_Animation((_int)BASE);
	m_vecBoneMatirx = dynamic_cast<CMesh*>(m_pMeshCom)->ExtractBoneTransforms(5000.f*fTimeDelta);


	return NO_EVENT;
}

_int CPistol::LateUpdate_GameObject(const _float & fTimeDelta)
{
	NULL_CHECK_RETURN(m_pRenderer, -1);

	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_NONALPHA, this), -1);
	FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_SHADOWDEPTH, this), -1);

	return S_OK;
}

HRESULT CPistol::AddComponent()
{

	m_pMeshCom = static_cast<Engine::CMesh*>(m_pComponentMgr->Clone_Component(L"Mesh_Pistol", COMPONENTID::ID_STATIC));
	NULL_CHECK_RETURN(m_pMeshCom, E_FAIL);
	m_mapComponent[ID_STATIC].emplace(L"Com_Mesh", m_pMeshCom);


	return S_OK;
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
