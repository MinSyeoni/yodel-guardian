#include "SphereCollider.h"

USING(Engine)

Engine::CSphereCollider::CSphereCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CToolCollider(pGraphic_Device)
{
	ZeroMemory(&m_vCenterPos, sizeof(_vec3));
}

Engine::CSphereCollider::~CSphereCollider(void)
{
}

HRESULT Engine::CSphereCollider::Ready_Collider(const _vec3* pPos, const _ulong & dwNumVtx, const _ulong & dwStride,COLLID eColID)
{
	m_pVtxPos = *pPos;
	m_dwNumVtx = dwNumVtx;
	m_dwStride = dwStride;

	if (FAILED(D3DXComputeBoundingSphere(pPos, dwNumVtx, sizeof(_vec3), &m_vCenterPos, &m_fRadius)))
		return E_FAIL;

	if (FAILED(D3DXCreateSphere(m_pGraphicDev, m_fRadius, 10, 10, &m_pMesh, NULL)))
		return E_FAIL;

	m_fOriRadius = m_fRadius;

	m_bIsFit = true;

	m_eShape = COL_SPHERE;

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT		LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		D3DXCOLOR dColor = {};

		if (eColID == COLID_COLLIDER)
			dColor = { (1.f - i), 1.f, 0.f, 1.f };
		else if (eColID == COLID_TERRAIN)
			dColor = { (0.2f + (i * 0.8f)),  (1.f + (i * -1.f)),  (1.f + (i * -1.f)), 1.f };
		else if (eColID == COLID_COMBAT)
			dColor = { (1.f - i), 0.5f, 1.0f, 1.f };
		else if (eColID == COLID_TRIGGER)
			dColor = { (1.f - i),(1.f - i), (1.f - i), 1.f };
		else if (eColID == COLID_NAVI)
			dColor = { 1.f,(1.f - i), 0.f, 1.f };

		*((_ulong*)LockRect.pBits) = dColor;

		m_pTexture[i]->UnlockRect(0);

	}

	m_eCollID = eColID;
	return NOERROR;
}


void Engine::CSphereCollider::Render_ShaderCollider()
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pGraphicDev->SetTexture(0, m_pTexture[m_eCollType]);
	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT Engine::CSphereCollider::Ready_Collider(const _float& fRadius, COLLID eColID)
{
	if (FAILED(D3DXCreateSphere(m_pGraphicDev,fRadius,10,10,
		&m_pMesh, NULL)))
		return E_FAIL;

	m_bIsFit = false;
	
	m_fOriRadius = m_fRadius = fRadius;
	m_eShape = COL_SPHERE;

	for (_uint i = 0; i < COL_END; ++i)
	{
		m_pGraphicDev->CreateTexture(1, 1, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture[i], NULL);

		D3DLOCKED_RECT		LockRect;
		ZeroMemory(&LockRect, sizeof(D3DLOCKED_RECT));

		m_pTexture[i]->LockRect(0, &LockRect, NULL, 0);

		D3DXCOLOR dColor = {};

		if (eColID == COLID_COLLIDER)
			dColor = { (1.f - i), 1.f, 0.f, 1.f };
		else if (eColID == COLID_TERRAIN)
			dColor = { (0.2f + (i * 0.8f)),  (1.f + (i * -1.f)),  (1.f + (i * -1.f)), 1.f };
		else if (eColID == COLID_COMBAT)
			dColor = { (1.f - i), 0.5f, 1.0f, 1.f };
		else if (eColID == COLID_TRIGGER)
			dColor = { (1.f - i),(1.f - i), (1.f - i), 1.f };
		else if (eColID == COLID_NAVI)
			dColor = { 1.f,(1.f - i), 0.f, 1.f };

		*((_ulong*)LockRect.pBits) = dColor;

		m_pTexture[i]->UnlockRect(0);
	}

	m_eCollID = eColID;
	return NOERROR;
}

void Engine::CSphereCollider::Update_Collider(const _matrix * pPareMatrix)
{
	_matrix matAddWorld;
	matAddWorld = m_matPariOriWorld;

	matAddWorld._41 += m_vAddPos.x;
	matAddWorld._42 += m_vAddPos.y;
	matAddWorld._43 += m_vAddPos.z;

	m_matWorld = matAddWorld;

	_vec3	vScaleMat[3]; 
	memcpy(&vScaleMat[0], &matAddWorld._11, sizeof(_vec3));
	memcpy(&vScaleMat[1], &matAddWorld._21, sizeof(_vec3));
	memcpy(&vScaleMat[2], &matAddWorld._31, sizeof(_vec3));

	_vec3 vScale = { D3DXVec3Length(&vScaleMat[0]) * 10.f,D3DXVec3Length(&vScaleMat[1]) * 10.f ,D3DXVec3Length(&vScaleMat[2]) * 10.f};
	_float fRatio = 0.f;
	fRatio = max(vScale.x, vScale.y);
	fRatio = max(fRatio, vScale.z);

	m_fRadius = m_fOriRadius * fRatio;
}

void Engine::CSphereCollider::Render_Collider(_int iIsCol, const _matrix * pColliderMatrix)
{
	_matrix matAddWorld;
	matAddWorld = *pColliderMatrix;

	matAddWorld._41 += m_vAddPos.x;
	matAddWorld._42 += m_vAddPos.y;
	matAddWorld._43 += m_vAddPos.z;

	m_matWorld = matAddWorld;

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pGraphicDev->SetTexture(0, m_pTexture[iIsCol]);
	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void Engine::CSphereCollider::Render_Collider(const _matrix * pColliderMatrix)
{
	_matrix matAddWorld;
	matAddWorld = *pColliderMatrix;

	matAddWorld._41 += m_vAddPos.x;
	matAddWorld._42 += m_vAddPos.y;
	matAddWorld._43 += m_vAddPos.z;

	_vec3	vScaleMat[3];
	memcpy(&vScaleMat[0], &matAddWorld._11, sizeof(_vec3));
	memcpy(&vScaleMat[1], &matAddWorld._21, sizeof(_vec3));
	memcpy(&vScaleMat[1], &matAddWorld._31, sizeof(_vec3));

	_vec3 vScale = { D3DXVec3Length(&vScaleMat[0]),D3DXVec3Length(&vScaleMat[1]) ,D3DXVec3Length(&vScaleMat[2]) };
	_float fRatio = max(vScale.x, vScale.y);
	fRatio = max(fRatio, vScale.z);

	m_fRadius = m_fOriRadius * fRatio;
	m_matWorld = matAddWorld;

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pGraphicDev->SetTexture(0, m_pTexture[m_eCollType]);
	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void Engine::CSphereCollider::Render_Collider(void)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pGraphicDev->SetTexture(0, m_pTexture[m_eCollType]);

	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

CSphereCollider * Engine::CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _vec3* pPos,
	const _ulong& dwNumVtx,
	const _ulong& dwStride, COLLID eColID)
{
	CSphereCollider* pInstance = new CSphereCollider(pGraphicDev);

	if (FAILED(pInstance->Ready_Collider(pPos, dwNumVtx, dwStride, eColID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CSphereCollider * Engine::CSphereCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _float& fRadius, COLLID eColID)
{
	CSphereCollider* pInstance = new CSphereCollider(pGraphic_Device);

	if (FAILED(pInstance->Ready_Collider(fRadius, eColID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CSphereCollider::Free(void)
{
	CToolCollider::Free();
}
