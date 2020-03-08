#include "ToolCollider.h"

USING(Engine)

Engine::CToolCollider::CToolCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
	,m_eCollType(COL_FALSE),
	m_eShape(SHAPE_END)
{
	ZeroMemory(&m_vFSize, sizeof(_vec3));
	ZeroMemory(&m_vAddPos, sizeof(_vec3));
	m_pGraphicDev->AddRef();
}

Engine::CToolCollider::~CToolCollider(void)
{

}

void Engine::CToolCollider::Update_Collider(const _matrix * pPareMatrix)
{
}

void Engine::CToolCollider::Free(void)
{
//	Engine::CComponent::Free();

	Engine::Safe_Release(m_pMesh);

	for (_uint i = 0; i < COL_END; ++i)
		Engine::Safe_Release(m_pTexture[i]);
	Engine::Safe_Release(m_pGraphicDev);
}

const _vec3 * Engine::CToolCollider::Get_Min(void)
{
	return &m_vMin;
}

const _vec3 * Engine::CToolCollider::Get_Max(void)
{
	return &m_vMax;
}