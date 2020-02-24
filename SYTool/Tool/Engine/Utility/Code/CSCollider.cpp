#include "CSCollider.h"

USING(Engine)

Engine::CSCollider::CSCollider(LPDIRECT3DDEVICE9 pGraphicDev)
	:m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

Engine::CSCollider::~CSCollider(void)
{

}

HRESULT Engine::CSCollider::Ready_Collider(const _vec3* pPos, const _vec3* vSize)
{
	
	if (FAILED(D3DXCreateSphere(m_pGraphicDev,3, 3, 3, &m_pMesh, nullptr)))
		return E_FAIL;

	






#ifdef _DEBUG
	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT		LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);
		if(i==0)
		*((_ulong*)LockRect.pBits) = D3DXCOLOR(1.f, 0.f, 0.f, 1.f);
		else if(i==1)
			*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 1.f, 0.f, 1.f);
		else
			*((_ulong*)LockRect.pBits) = D3DXCOLOR(0.f, 0.f, 1.f, 1.f);
		m_pTexture[i]->UnlockRect(0);
		

	}
#endif

	return S_OK;
}

void Engine::CSCollider::Render_Collider(COLLTYPE eType, const _matrix* pColliderMatrix)
{
	m_matWorld = *pColliderMatrix;

#ifdef _DEBUG
	m_pGraphicDev->SetTransform(D3DTS_WORLD, pColliderMatrix);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pGraphicDev->SetTexture(0, m_pTexture[eType]);
	m_pMesh->DrawSubset(0);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);

#endif



}

Engine::CSCollider* Engine::CSCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _vec3* pPos, const _vec3* vSize)
{
	CSCollider*	pInstance = new CSCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider( pPos, vSize)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CSCollider::Free(void)
{
#ifdef _DEBUG
	Engine::Safe_Release(m_pGraphicDev);
	for (_uint i = 0; i < COL_END; ++i)
		Engine::Safe_Release(m_pTexture[i]);

#endif
}

