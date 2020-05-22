#include "TerrainTex.h"

USING(Engine)

Engine::CTerrainTex::CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
	, m_pPos(nullptr)
	, m_bClone(false)
{

}

Engine::CTerrainTex::CTerrainTex(const CTerrainTex& rhs)
	:CVIBuffer(rhs)
	, m_pPos(rhs.m_pPos)
	, m_bClone(true)
	, m_fH(rhs.m_fH)
	, m_iH(rhs.m_iH)
	, m_pVtxTex(rhs.m_pVtxTex)
	, m_pVtxTexOrigin(rhs.m_pVtxTexOrigin)
	, m_dwVtxCntX(rhs.m_dwVtxCntX)
	, m_dwVtxCntZ(rhs.m_dwVtxCntZ)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

void CTerrainTex::Copy_Index(Engine::INDEX32* pIndex, const _ulong& dwTriCnt)
{
	Engine::INDEX32* pOriIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriIndex, 0);

	memcpy(pOriIndex, pIndex, sizeof(Engine::INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

HRESULT CTerrainTex::Ready_Buffer(
	const _ulong& dwCntX, 
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;
	m_pVtxTexOrigin = new VTXTEX[m_dwVtxCnt];

	m_pPos = new _vec3[m_dwVtxCnt];

	m_dwVtxFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < dwCntZ; i++)
	{
		for (size_t j = 0; j < dwCntZ; j++)
		{
			_uint		iIndex = i * dwCntX + j;

			pVertices[iIndex].vPos = _vec3(j * dwVtxItv, 0.0f, i * dwVtxItv);
			pVertices[iIndex].vTexUV = _vec2(j / (dwCntX - 1.f), i / (dwCntZ - 1.f));

			m_pVtxTexOrigin[iIndex].vPos = pVertices[iIndex].vPos;
		}
	}

	INDEX32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint			iPolygonIndex = 0;

	for (size_t i = 0; i < dwCntZ - 1; i++)
	{
		for (size_t j = 0; j < dwCntX - 1; j++)
		{
			size_t iIndex = i * dwCntX + j;

			// 우.상
			pIndices[iPolygonIndex]._0 = iIndex + dwCntX;
			pIndices[iPolygonIndex]._1 = iIndex + dwCntX + 1;
			pIndices[iPolygonIndex]._2 = iIndex + 1;

			_vec3		vSour, vDest;
			_vec3		vNormal;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPos - pVertices[pIndices[iPolygonIndex]._0].vPos;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPos - pVertices[pIndices[iPolygonIndex]._1].vPos;

			++iPolygonIndex;

			// 좌.하
			pIndices[iPolygonIndex]._0 = iIndex + dwCntX;
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

HRESULT Engine::CTerrainTex::Ready_Buffer(
	_tchar* pPath,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_pPos = new _vec3[m_dwVtxCnt];

	m_dwVtxFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK(CVIBuffer::Ready_Buffer());

	_ulong		dwByte = 0;

	m_hFile = CreateFile(pPath,
		GENERIC_READ,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);

	m_dwVtxCntZ = 768;
	m_dwVtxCntX = 768;
	// 나중에 꼭 수정
	m_iH.biWidth = 768;
	m_iH.biHeight = 768;

	_ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, NULL);

	CloseHandle(m_hFile);

	m_pVB->Lock(0, 0, (void**)&m_pVtxTex, 0);

	_ulong		dwIndex = 0;

	for (_ulong i = 0; i < dwCntZ; ++i)
	{
		for (_ulong j = 0; j < dwCntX; ++j)
		{
			dwIndex = i * dwCntX + j;

			m_pVtxTex[dwIndex].vPos = _vec3((_float(j) * dwVtxItv),
				((pPixel[dwIndex] & 0x000000ff) / 255.f) * 7.f,
				(_float(i) * dwVtxItv));

			m_pPos[dwIndex] = m_pVtxTex[dwIndex].vPos;

			m_pVtxTex[dwIndex].vTexUV = _vec2(_float(j) / (dwCntX - 1),
				_float(i) / (dwCntZ - 1));
		}
	}

	Safe_Delete_Array(pPixel);

	INDEX32* pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong	dwTriIdx = 0;

	for (_ulong i = 0; i < dwCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < dwCntX - 1; ++j)
		{
			dwIndex = i * dwCntX + j;

			_vec3	vDest, vSour, vNormal;

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + dwCntX + 1;
			pIndex[dwTriIdx]._2 = dwIndex + 1;

			vDest = m_pVtxTex[pIndex[dwTriIdx]._1].vPos - m_pVtxTex[pIndex[dwTriIdx]._0].vPos;
			vSour = m_pVtxTex[pIndex[dwTriIdx]._2].vPos - m_pVtxTex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			++dwTriIdx;

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = m_pVtxTex[pIndex[dwTriIdx]._1].vPos - m_pVtxTex[pIndex[dwTriIdx]._0].vPos;
			vSour = m_pVtxTex[pIndex[dwTriIdx]._2].vPos - m_pVtxTex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			++dwTriIdx;
		}
	}

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ulong& dwCntX,
	const _ulong& dwCntZ,
	const _ulong& dwVtxItv)
{
	CTerrainTex* pInstance = new CTerrainTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer(dwCntX, dwCntZ, dwVtxItv)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CTerrainTex::Free(void)
{
	CVIBuffer::Free();
	if (false == m_bClone)
	{
		Safe_Delete_Array(m_pVtxTex);
		Safe_Delete_Array(m_pPos);
	}
}

Engine::CResources* Engine::CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

