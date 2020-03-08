#include "BoxCollider.h"

USING(Engine)

Engine::CBoxCollider::CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device)
	:CToolCollider(pGraphic_Device)
{
	ZeroMemory(m_vAddMin, sizeof(_vec3));
	ZeroMemory(m_vAddMax, sizeof(_vec3));
}

Engine::CBoxCollider::~CBoxCollider(void)
{
}



HRESULT Engine::CBoxCollider::Ready_Collider(const _vec3* pPos, const _ulong & dwNumVtx, const _ulong & dwStride, COLLID eColID)
{
// 	ZeroMemory(&m_vMin, sizeof(_vec3));
// 	ZeroMemory(&m_vMax, sizeof(_vec3));

	m_pVtxPos = *pPos;
	m_dwNumVtx = dwNumVtx;
	m_dwStride = dwStride;

 	if (FAILED(D3DXComputeBoundingBox(pPos, dwNumVtx,sizeof(_vec3), &m_vMin, &m_vMax)))
 		return E_FAIL;
	
	if (FAILED(D3DXCreateBox(m_pGraphicDev, m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMax.z - m_vMin.z,
		&m_pMesh, NULL)))
		return E_FAIL;
	m_bIsFit = true;

	m_vFSize = m_vMax - m_vMin;
	m_eShape = COL_BOX;

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
		//	dColor = { 0.95f, 0.38f, 0.65f, 1.f };
			dColor = { 1.f, 0.f, 0.5f + (1.f * i * (-0.5f)), 1.f };
		else if (eColID == COLID_COMBAT)
			dColor = { (1.f - i), 0.5f, 1.0f, 1.f };
		else if (eColID == COLID_TRIGGER)
			dColor = { (1.f - i),(1.f - i), (1.f - i), 1.f };

		*((_ulong*)LockRect.pBits) = dColor;

		m_pTexture[i]->UnlockRect(0);
	}

	m_eCollID = eColID;
	return NOERROR;
}

HRESULT Engine::CBoxCollider::Ready_Collider(const _float & fX, const _float & fY, const _float & fZ, COLLID eColID)
{
	if (FAILED(D3DXCreateBox(m_pGraphicDev, fX, fY, fZ,
		&m_pMesh, NULL)))
		return E_FAIL;

	m_vScale = { fX,fY,fZ };

	m_bIsFit = false;
	m_vFSize = { fX,fY,fZ };
	m_eShape = COL_BOX;

	m_vMin = { fX*-0.5f ,fY*-0.5f  ,fZ * -0.5f };
	m_vMax = { fX*0.5f ,fY*0.5f  ,fZ * 0.5f };

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
			dColor = { 1.f, 0.f, 0.5f + (1.f * i * -0.5f), 1.f };
		else if (eColID == COLID_COMBAT)
			dColor = { (1.f - i), 0.5f, 1.0f, 1.f };
		else if (eColID == COLID_TRIGGER)
			dColor = { (1.f - i),(1.f - i), (1.f - i), 1.f };

		*((_ulong*)LockRect.pBits) = dColor;

		m_pTexture[i]->UnlockRect(0);
	}

	m_eCollID = eColID;
	return NOERROR;
}

void Engine::CBoxCollider::Render_Collider(_int iIsCol, const _matrix * pColliderMatrix)
{
	_matrix matAddWorld;
	matAddWorld = *pColliderMatrix;

	_matrix matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], D3DXToRadian(m_vAngle.x));
	D3DXMatrixRotationX(&matRot[ROT_Y], D3DXToRadian(m_vAngle.y));
	D3DXMatrixRotationX(&matRot[ROT_Z], D3DXToRadian(m_vAngle.z));

	matAddWorld._41 += m_vAddPos.x;
	matAddWorld._42 += m_vAddPos.y;
	matAddWorld._43 += m_vAddPos.z;

	for (int i = 0; i < ROT_END; i++)
		matAddWorld = matRot[i] * matAddWorld;

	m_matWorld = matAddWorld;

	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);
	
	m_pGraphicDev->SetTexture(0, m_pTexture[iIsCol]);
	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void Engine::CBoxCollider::Render_Collider(const _matrix * pColliderMatrix)
{
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &m_matWorld);

	m_pGraphicDev->SetTexture(0, m_pTexture[m_eCollType]);
	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

void Engine::CBoxCollider::Render_Collider(void)
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

void Engine::CBoxCollider::Render_Collider(_int iIsCol)
{
	_matrix matWorld;
	matWorld = m_matWorld;
	matWorld._41 = m_matWorld._41 + m_vAddPos.x;
	matWorld._42 = m_matWorld._42 + m_vAddPos.y;
	matWorld._43 = m_matWorld._43 + m_vAddPos.z;

	
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	m_pGraphicDev->SetTransform(D3DTS_WORLD, &matWorld);

	m_pGraphicDev->SetTexture(0, m_pTexture[iIsCol]);
	m_pMesh->DrawSubset(0);

	m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphicDev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
}

HRESULT Engine::CBoxCollider::Ready_Collider(const _vec3 & vPos, const _float & fX, const _float & fY, const _float & fZ, COLLID eColID)
{
	if (FAILED(D3DXCreateBox(m_pGraphicDev, fX, fY, fZ,
		&m_pMesh, NULL)))
		return E_FAIL;

	m_vScale = { fX,fY,fZ };

	D3DXMatrixTranslation(&m_matWorld, vPos.x, vPos.y, vPos.z);

	m_vAddPos += vPos;

	m_bIsFit = false;
	m_vFSize = { fX,fY,fZ };
	m_eShape = COL_BOX;

	m_vMin = { fX*-0.5f ,fY*-0.5f  ,fZ * -0.5f };
	m_vMax = { fX*0.5f ,fY*0.5f  ,fZ * 0.5f };

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
			dColor = { 1.f, 0.f, 0.5f + (1.f * i * 0.5f), 1.f };
		else if (eColID == COLID_COMBAT)
			dColor = { (1.f - i), 0.5f, 1.0f, 1.f };
		else if (eColID == COLID_TRIGGER)
			dColor = { (1.f - i),(1.f - i), (1.f - i), 1.f };

		*((_ulong*)LockRect.pBits) = dColor;

		m_pTexture[i]->UnlockRect(0);
	}

	m_eCollID = eColID;
	return NOERROR;
}

HRESULT Engine::CBoxCollider::Ready_OffsetCollider(const _vec3 * pVtxPos, const _ulong & dwNumVtx, const _ulong & dwStride, const _float & fX, const _float & fY, const _float & fZ, COLLID eColID)
{
	if (FAILED(D3DXComputeBoundingBox(pVtxPos, dwNumVtx, sizeof(_vec3), &m_vMin, &m_vMax)))
		return E_FAIL;

	m_vMax.x += fX,m_vMin.x -= fX;
	m_vMax.y += fY, m_vMin.y -= fY;
	m_vMax.z += fZ, m_vMin.z -= fZ;

	if (FAILED(D3DXCreateBox(m_pGraphicDev, m_vMax.x - m_vMin.x, m_vMax.y - m_vMin.y, m_vMax.z - m_vMin.z,
		&m_pMesh, NULL)))
		return E_FAIL;
	m_bIsFit = true;

	m_vFSize = m_vMax - m_vMin;
	m_eShape = COL_BOX;

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
			dColor = { 1.f, 0.f, 0.5f, 1.f };
		else if (eColID == COLID_COMBAT)
			dColor = { (1.f - i), 0.5f, 1.0f, 1.f };
		else if (eColID == COLID_TRIGGER)
			dColor = { (1.f - i),(1.f - i), (1.f - i), 1.f };

		*((_ulong*)LockRect.pBits) = dColor;

		m_pTexture[i]->UnlockRect(0);

	}

	m_eCollID = eColID;
	return NOERROR;
}

void Engine::CBoxCollider::Update_Collider(const _matrix * pPareMatrix)
{

	_matrix matRot[ROT_END];

	D3DXMatrixRotationX(&matRot[ROT_X], D3DXToRadian(m_vAngle.x));
	D3DXMatrixRotationY(&matRot[ROT_Y], D3DXToRadian(m_vAngle.y));
	D3DXMatrixRotationZ(&matRot[ROT_Z], D3DXToRadian(m_vAngle.z));

 	_matrix matAddWorld;
 	_matrix matNonScaleWorld;
 	matNonScaleWorld = matAddWorld = *pPareMatrix;
 
 	_vec3 vPos = m_vAddPos;

	_vec3 vScale;

	
 	D3DXVec3TransformNormal(&vPos, &vPos, &matNonScaleWorld);

	vPos.x *= 10.f;
	vPos.y *= 10.f;
	vPos.z *= 10.f;

	_matrix matTrans;
	D3DXMatrixTranslation(&matTrans, vPos.x, vPos.y, vPos.z);
 	 	
//  	matAddWorld._41 += vPos.x;
//  	matAddWorld._42 += vPos.y;
//  	matAddWorld._43 += vPos.z;
 
  	for (int i = 0; i < ROT_END; i++)
  		matAddWorld = matRot[i] * matAddWorld;

	matAddWorld = matAddWorld * matTrans;

 	m_matWorld = matAddWorld;
}


CBoxCollider * Engine::CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3* pPos, const _ulong & dwNumVtx, const _ulong & dwStride, COLLID eColID)
{
 	CBoxCollider* pInstance = new CBoxCollider(pGraphic_Device);
	
	if (FAILED(pInstance->Ready_Collider(pPos, dwNumVtx, dwStride,eColID)))
		Engine::Safe_Release(pInstance);


	return pInstance;
}

CBoxCollider * Engine::CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device,const _float & fX, const _float & fY, const _float & fZ, COLLID eColID)
{
	CBoxCollider* pInstance = new CBoxCollider(pGraphic_Device);

	if (FAILED(pInstance->Ready_Collider( fX, fY, fZ,eColID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CBoxCollider * Engine::CBoxCollider::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3 & vPos, const _float & fX, const _float & fY, const _float & fZ, COLLID eColID)
{
	CBoxCollider* pInstance = new CBoxCollider(pGraphic_Device);

	if (FAILED(pInstance->Ready_Collider(vPos,fX, fY, fZ, eColID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CBoxCollider * Engine::CBoxCollider::Create_Offset(LPDIRECT3DDEVICE9 pGraphic_Device, const _vec3 * pVtxPos, const _ulong & dwNumVtx, const _ulong & dwStride, const _float & fX, const _float & fY, const _float & fZ, COLLID eColID)
{
	CBoxCollider* pInstance = new CBoxCollider(pGraphic_Device);

	if (FAILED(pInstance->Ready_OffsetCollider(pVtxPos, dwNumVtx, dwNumVtx,fX, fY, fZ, eColID)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}


void Engine::CBoxCollider::Free(void)
{
	CToolCollider::Free();
}