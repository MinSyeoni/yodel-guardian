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
	,m_fH(rhs.m_fH)
	,m_iH(rhs.m_iH)
	,m_pVtxTex(nullptr)
	,m_dwVtxCntX(rhs.m_dwVtxCntX)
	,m_dwVtxCntZ(rhs.m_dwVtxCntZ)
{

}

Engine::CTerrainTex::~CTerrainTex(void)
{

}

void CTerrainTex::Copy_Index(Engine::INDEX32 * pIndex, const _ulong & dwTriCnt)
{
	Engine::INDEX32*			pOriIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pOriIndex, 0);

	memcpy(pOriIndex, pIndex, sizeof(Engine::INDEX32) * dwTriCnt);

	m_pIB->Unlock();

	m_dwTriCnt = dwTriCnt;
}

HRESULT Engine::CTerrainTex::Ready_Buffer(const _ulong& dwCntX,
										  const _ulong& dwCntZ,
										  const _ulong& dwVtxItv)
{
	m_dwVtxCntX = dwCntX;
	m_dwVtxCntZ = dwCntZ;
	m_dwVtxItv = dwVtxItv;

	m_dwVtxCnt = m_dwVtxCntX * m_dwVtxCntZ;

	m_pVtxTex = new VTXTEX[m_dwVtxCnt];

	m_dwVtxSize = sizeof(VTXTEX);
	m_dwVtxFVF = FVF_TEX/* | D3DFVF_TEXCOORDSIZE2(0)*/;
	m_dwTriCnt = (m_dwVtxCntX - 1) * (m_dwVtxCntZ - 1) * 2;

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	if (FAILED(CVIBuffer::Ready_Buffer()))
		return E_FAIL;

	VTXTEX* pVertices = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVertices, 0);

	for (size_t i = 0; i < m_dwVtxCntZ; i++)
	{
		for (size_t j = 0; j < m_dwVtxCntX; j++)
		{
			_uint		iIndex = i * m_dwVtxCntX + j;

			pVertices[iIndex].vPos = _vec3(j * m_dwVtxItv, 0.0f, i * m_dwVtxItv);
			pVertices[iIndex].vTexUV = _vec2(j / (m_dwVtxCntX - 1.f), i / (m_dwVtxCntZ - 1.f));

			m_pVtxTex[iIndex].vPos = pVertices[iIndex].vPos;
		}
	}

	Safe_Delete_Array(m_pVtxTex);

	INDEX32* pIndices = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndices, 0);

	_uint			iPolygonIndex = 0;

	for (size_t i = 0; i < m_dwVtxCntZ - 1; i++)
	{
		for (size_t j = 0; j < m_dwVtxCntX - 1; j++)
		{
			size_t iIndex = i * m_dwVtxCntX + j;

			// 우.상
			pIndices[iPolygonIndex]._0 = iIndex + m_dwVtxCntX;
			pIndices[iPolygonIndex]._1 = iIndex + m_dwVtxCntX + 1;
			pIndices[iPolygonIndex]._2 = iIndex + 1;

			_vec3		vSour, vDest;
			_vec3		vNormal;

			vSour = pVertices[pIndices[iPolygonIndex]._1].vPos - pVertices[pIndices[iPolygonIndex]._0].vPos;
			vDest = pVertices[pIndices[iPolygonIndex]._2].vPos - pVertices[pIndices[iPolygonIndex]._1].vPos;

			++iPolygonIndex;

			// 좌.하
			pIndices[iPolygonIndex]._0 = iIndex + m_dwVtxCntX;
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

HRESULT CTerrainTex::Ready_Buffer(const _tchar* pHeightMap, const _ulong& dwVtxItv)
{
	_ulong dwCntX = 129;
	_ulong dwCntZ = 129;

	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_pPos = new _vec3[m_dwVtxCnt];

	m_dwVtxFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);

	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;

	FAILED_CHECK(CVIBuffer::Ready_Buffer());

	_ulong		dwByte = 0;

	m_hFile = CreateFile(pHeightMap,
		GENERIC_READ,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);



	m_dwVtxCntZ = 129;
	m_dwVtxCntX = 129;


	_ulong* pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

	ReadFile(m_hFile, pPixel, sizeof(_ulong) * m_iH.biHeight * m_iH.biWidth, &dwByte, NULL);

	CloseHandle(m_hFile);


	//m_pVtxTex =  new VTXTEX[dwCntX*dwCntZ];

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

HRESULT CTerrainTex::Reset_Buffer(const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{

	return S_OK;
}

void CTerrainTex::SetTextureHeight(_vec3* pPos, float fRange)
{

	VTXTEX*	pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	_ulong dwIndex = 0;

	for (_ulong i = 0; i < m_dwVtxCntZ; ++i)
	{
		for (_ulong j = 0; j < m_dwVtxCntX; ++j)
		{
			dwIndex = i * m_dwVtxCntX + j;

			if (pPos->x - fRange <= pVtxTex[dwIndex].vPos.x &&
				pPos->x + fRange >= pVtxTex[dwIndex].vPos.x &&
				pPos->z - fRange <= pVtxTex[dwIndex].vPos.z &&
				pPos->z + fRange >= pVtxTex[dwIndex].vPos.z)
			{
				if (0 == dwIndex)
				{
					_float fPosY =
						(
							pVtxTex[dwIndex + 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX + 1].vPos.y
							) / 3;

					pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
					m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

					continue;
				}

				if (m_dwVtxCntX - 1 == dwIndex)
				{
					_float fPosY =
						(
							pVtxTex[dwIndex - 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX - 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX].vPos.y
							) / 3;

					pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
					m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

					continue;
				}

				if (m_dwVtxCntX * (m_dwVtxCntZ - 1) == dwIndex)
				{
					_float fPosY =
						(
							pVtxTex[dwIndex + 1].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX + 1].vPos.y
							) / 3;

					pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
					m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

					continue;
				}

				if ((m_dwVtxCntX * m_dwVtxCntZ) - 1 == dwIndex)
				{
					_float fPosY =
						(
							pVtxTex[dwIndex - 1].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX - 1].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX].vPos.y
							) / 3;

					pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
					m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

					continue;
				}

				if (m_dwVtxCntX > dwIndex)
				{
					_float fPosY =
						(
							pVtxTex[dwIndex - 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX - 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX + 1].vPos.y +
							pVtxTex[dwIndex + 1].vPos.y
							) / 5;

					pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
					m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

					continue;
				}

				if (i * m_dwVtxCntX == dwIndex)
				{
					_float fPosY =
						(
							pVtxTex[dwIndex - m_dwVtxCntX].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX + 1].vPos.y +
							pVtxTex[dwIndex + 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX + 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX].vPos.y
							) / 5;

					pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
					m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

					continue;
				}

				if ((i * m_dwVtxCntX) + (m_dwVtxCntX - 1) == dwIndex)
				{
					_float fPosY =
						(
							pVtxTex[dwIndex - m_dwVtxCntX].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX - 1].vPos.y +
							pVtxTex[dwIndex - 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX - 1].vPos.y +
							pVtxTex[dwIndex + m_dwVtxCntX].vPos.y
							) / 5;

					pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
					m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

					continue;
				}

				if ((m_dwVtxCntX * (m_dwVtxCntZ - 1)) - 1 < dwIndex)
				{
					_float fPosY =
						(
							pVtxTex[dwIndex - 1].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX - 1].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX].vPos.y +
							pVtxTex[dwIndex - m_dwVtxCntX + 1].vPos.y +
							pVtxTex[dwIndex + 1].vPos.y
							) / 5;

					pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
					m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;

					continue;
				}

				_float fPosY =
					(
						pVtxTex[dwIndex - 1].vPos.y +
						pVtxTex[dwIndex - m_dwVtxCntX - 1].vPos.y +
						pVtxTex[dwIndex - m_dwVtxCntX].vPos.y +
						pVtxTex[dwIndex - m_dwVtxCntX + 1].vPos.y +
						pVtxTex[dwIndex + 1].vPos.y +
						pVtxTex[dwIndex + m_dwVtxCntX + 1].vPos.y +
						pVtxTex[dwIndex + m_dwVtxCntX].vPos.y +
						pVtxTex[dwIndex + m_dwVtxCntX - 1].vPos.y
						) / 8;

				pVtxTex[dwIndex].vPos.y = (pVtxTex[dwIndex].vPos.y + fPosY) * 0.5f;
				m_pPos[dwIndex] = pVtxTex[dwIndex].vPos;
			}
		}
	}

	INDEX32* pIndex = nullptr;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	_ulong	dwTriIdx = 0;

	_vec3 vDest, vSour, vNormal;

	for (_ulong i = 0; i < m_dwVtxCntZ - 1; ++i)
	{
		for (_ulong j = 0; j < m_dwVtxCntX - 1; ++j)
		{
			dwIndex = (i * m_dwVtxCntX) + j;

			// 외적 연산으로 면 버텍스의 법선벡터 구하기
			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPos - pVtxTex[pIndex[dwTriIdx]._0].vPos;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPos - pVtxTex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			++dwTriIdx;

			// 외적 연산으로 면 버텍스의 법선벡터 구하기
			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPos - pVtxTex[pIndex[dwTriIdx]._0].vPos;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPos - pVtxTex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			++dwTriIdx;
		}
	}


	m_pIB->Unlock();
	m_pVB->Unlock();
}

void CTerrainTex::SetHeight(_int iIndex, _float fHeight)
{
	VTXTEX*	pVtxTex = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxTex, 0);

	pVtxTex[iIndex].vPos.y = fHeight;
	m_pPos[iIndex].y = fHeight;



	m_pVB->Unlock();

}

CTerrainTex* Engine::CTerrainTex::Create(LPDIRECT3DDEVICE9 pGraphicDev, 
										const _ulong& dwCntX,
										const _ulong& dwCntZ,
										const _ulong& dwVtxItv)
{
	CTerrainTex*		pInstance = new CTerrainTex(pGraphicDev);

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

