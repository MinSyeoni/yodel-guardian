#include "Buffer_Terrain.h"

USING(Engine)

CBuffer_Terrain::CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer(pGraphic_Device)
	, m_pPos(nullptr)
	, m_bClone(false)
{

}

CBuffer_Terrain::CBuffer_Terrain(const CBuffer_Terrain & rhs)
	: CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_bClone(true)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
	, m_pVtxTex(nullptr)
	, m_dwVtxCntX(rhs.m_dwVtxCntX)
	, m_dwVtxCntZ(rhs.m_dwVtxCntZ)
{

}

HRESULT CBuffer_Terrain::Ready_VIBuffer(const _uint& iNumVerticesX, const _uint& iNumVerticesZ, const _float& fInterval)
{
	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_fInterval = fInterval;

	m_dwVtxCnt = iNumVerticesX * iNumVerticesZ;

	m_pVtxTexOrigin = new VTXTEX[m_dwVtxCnt];

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = FVF_TEX;
	m_dwTriCnt = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;

			pVertices[iIndex].vPos = _vec3(j * m_fInterval, 0.0f, i * m_fInterval);
			pVertices[iIndex].vTexUV = _vec2(j / (iNumVerticesX - 1.f), i / (iNumVerticesZ - 1.f));

			m_pVtxTexOrigin[iIndex].vPos = pVertices[iIndex].vPos;
		}
	}
	



	INDEX32*		pIndices = nullptr;
	
	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint			iPolygonIndex = 0;

	for (size_t i = 0; i < iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < iNumVerticesX - 1; j++)
		{
			size_t iIndex = i * iNumVerticesX + j;

			// 우.상
			pIndices[iPolygonIndex]._0 = iIndex + iNumVerticesX;
			pIndices[iPolygonIndex]._1 = iIndex + iNumVerticesX + 1;
			pIndices[iPolygonIndex]._2 = iIndex + 1;

			_vec3		vSour, vDest;
			_vec3		vNormal;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPos - pVertices[pIndices[iPolygonIndex]._0].vPos;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPos - pVertices[pIndices[iPolygonIndex]._1].vPos;

			++iPolygonIndex;

			// 좌.하
			pIndices[iPolygonIndex]._0 = iIndex + iNumVerticesX;
			pIndices[iPolygonIndex]._1 = iIndex + 1;
			pIndices[iPolygonIndex]._2 = iIndex;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPos - pVertices[pIndices[iPolygonIndex]._0].vPos;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPos - pVertices[pIndices[iPolygonIndex]._1].vPos;

			++iPolygonIndex;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return NOERROR;
}

HRESULT CBuffer_Terrain::Ready_VIBuffer(const _tchar * pHeightMap, const _float & fInterval)
{
	_uint		iNumVerticesX, iNumVerticesZ;

	HANDLE		hFile = 0;
	_ulong		dwByte = 0;

	hFile = CreateFile(pHeightMap, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	// 파일정보
	ReadFile(hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, nullptr);

	// 이미지정보
	ReadFile(hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, nullptr);

	iNumVerticesX = m_iH.biWidth;
	iNumVerticesZ = m_iH.biHeight;

	m_pPos = new _vec3[iNumVerticesX * iNumVerticesZ];

	m_pPixel = new _ulong[iNumVerticesX * iNumVerticesZ];

	// 픽셀정보
	ReadFile(hFile, m_pPixel, sizeof(_ulong) * (iNumVerticesX * iNumVerticesZ), &dwByte, nullptr);

	CloseHandle(hFile);

	m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_fInterval = fInterval;

	m_dwVtxCnt = iNumVerticesX * iNumVerticesZ;
	m_pVtxTexOrigin = new VTXTEX[m_dwVtxCnt];

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = FVF_TEX/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_dwTriCnt = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX*		pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < iNumVerticesZ; i++)
	{
		for (size_t j = 0; j < iNumVerticesX; j++)
		{
			_uint		iIndex = i * iNumVerticesX + j;

			pVertices[iIndex].vPos = _vec3(j * m_fInterval, (m_pPixel[iIndex] & 0x000000ff) / 10.f, i * m_fInterval);
			m_pPos[iIndex] = pVertices[iIndex].vPos;
			pVertices[iIndex].vTexUV = _vec2(j / (iNumVerticesX - 1.f), i / (iNumVerticesZ - 1.f));

		//	m_pVtxTexOrigin[iIndex].vPosition = pVertices[iIndex].vPosition;
		}
	}

	INDEX32*		pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint			iPolygonIndex = 0;


	for (size_t i = 0; i < iNumVerticesZ - 1; i++)
	{
		for (size_t j = 0; j < iNumVerticesX - 1; j++)
		{
			size_t iIndex = i * iNumVerticesX + j;

			// 우.상
			pIndices[iPolygonIndex]._0 = iIndex + iNumVerticesX;
			pIndices[iPolygonIndex]._1 = iIndex + iNumVerticesX + 1;
			pIndices[iPolygonIndex]._2 = iIndex + 1;

			_vec3		vSour, vDest;
			_vec3		vNormal;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPos - pVertices[pIndices[iPolygonIndex]._0].vPos;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPos - pVertices[pIndices[iPolygonIndex]._1].vPos;

			++iPolygonIndex;

			// 좌.하
			pIndices[iPolygonIndex]._0 = iIndex + iNumVerticesX;
			pIndices[iPolygonIndex]._1 = iIndex + 1;
			pIndices[iPolygonIndex]._2 = iIndex;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPos - pVertices[pIndices[iPolygonIndex]._0].vPos;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPos - pVertices[pIndices[iPolygonIndex]._1].vPos;

			++iPolygonIndex;
		}
	}

	m_pVB->Unlock();

	/*
m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

m_pPos = new _vec3[m_dwVtxCnt];

m_dwVtxFVF = FVF_TEX;
m_dwVtxSize = sizeof(VTXTEX);

m_dwIdxSize = sizeof(INDEX32);
m_IdxFmt = D3DFMT_INDEX32;

if (FAILED(CVIBuffer::Ready_Buffer()))
	return E_FAIL;

		m_iNumVerticesX = iNumVerticesX;
	m_iNumVerticesZ = iNumVerticesZ;
	m_fInterval = fInterval;

	m_iNumVertices = iNumVerticesX * iNumVerticesZ;

	m_pVtxTexOrigin = new VTXNORTEX[m_iNumVertices];

	m_iStride = sizeof(VTXNORTEX);
	m_iNumPolygons = (iNumVerticesX - 1) * (iNumVerticesZ - 1) * 2;

	m_iPolygonSize = sizeof(POLYGON32);
	m_eFormat = D3DFMT_INDEX32;

*/
	m_pPolygonVertexIndex = new INDEX32[m_dwTriCnt];
	memcpy(m_pPolygonVertexIndex, pIndices, sizeof(INDEX32) * m_dwTriCnt);

	m_pIB->Unlock();
	
	return NOERROR;
}

void CBuffer_Terrain::Render_VIBuffer()
{
	m_pGraphicDev->SetStreamSource(0, m_pVB, 0, m_dwVtxSize);
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->SetIndices(m_pIB);
	m_pGraphicDev->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, m_dwVtxCnt, 0, m_dwTriCnt);
}

_float CBuffer_Terrain::Compute_HeightOnTerrain(const _vec3 * pPosition)
{
	_ulong dwIndex = _ulong(pPosition->z / m_fInterval) * m_iNumVerticesX + _ulong(pPosition->x / m_fInterval);

	_float	fRatioX = (pPosition->x - m_pPos[dwIndex + m_iNumVerticesX].x) / m_fInterval;
	_float	fRatioZ = (m_pPos[dwIndex + m_iNumVerticesX].z - pPosition->z) / m_fInterval;

	D3DXPLANE			Plane;

	if (fRatioX > fRatioZ)
	{
		D3DXPlaneFromPoints(&Plane, &m_pPos[dwIndex + m_iNumVerticesX], &m_pPos[dwIndex + m_iNumVerticesX + 1], &m_pPos[dwIndex + 1]);

		// ax + by + cz + d == 0
		// y = (-ax + -cz + -d) / b
		return ((Plane.a * pPosition->x * -1.f) + (Plane.c * pPosition->z * -1.f) + Plane.d * -1.f) / Plane.b;


	}
	else
	{
		D3DXPlaneFromPoints(&Plane, &m_pPos[dwIndex + m_iNumVerticesX], &m_pPos[dwIndex + 1], &m_pPos[dwIndex]);

		return ((Plane.a * pPosition->x * -1.f) + (Plane.c * pPosition->z * -1.f) + Plane.d * -1.f) / Plane.b;
	}	
}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint& iNumVerticesX, const _uint& iNumVerticesZ, const _float& fInterval)
{
	CBuffer_Terrain*	pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(iNumVerticesX, iNumVerticesZ, fInterval)))
	{
		MessageBox(0, L"CBuffer_Terrain Created Failed", L"System Error", MB_OK);
		Engine::Safe_Release(pInstance);
	}
	return pInstance;

}

CBuffer_Terrain * CBuffer_Terrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar * pHeightMap, const _float & fInterval)
{
	CBuffer_Terrain*	pInstance = new CBuffer_Terrain(pGraphic_Device);

	if (FAILED(pInstance->Ready_VIBuffer(pHeightMap, fInterval)))
	{
		MessageBox(0, L"CBuffer_Terrain Created Failed", L"System Error", MB_OK);
		Engine::Safe_Release(pInstance);
	}
	return pInstance;
}


CComponent * CBuffer_Terrain::Clone_Component(void* pArg)
{
	return new CBuffer_Terrain(*this);
}

CResources* CBuffer_Terrain::Clone(void)
{
	return new CBuffer_Terrain(*this);
}

void CBuffer_Terrain::Free()
{	
	if (false == m_bClone)
	{	
		Engine::Safe_Delete_Array(m_pPixel);
		Engine::Safe_Delete_Array(m_pVtxTexOrigin);
		Engine::Safe_Delete_Array(m_pPolygonVertexIndex);
	}

	CVIBuffer::Free();
}
