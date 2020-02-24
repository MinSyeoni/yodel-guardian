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
	///////////////////////////////////////////////////////////////////////
	/*LPDIRECT3DTEXTURE9			pTexture = nullptr;

	D3DXCreateTexture(m_pGraphicDev, 129, 129, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pTexture);

	D3DLOCKED_RECT LockRect;

	pTexture->LockRect(0, &LockRect, NULL, 0);

	for (_int i = 0; i < 129; ++i)
	{
		for (_int j = 0; j < 129; ++j)
		{
			_int	iIndex = i * 129 + j;

			*(((_ulong*)LockRect.pBits) + iIndex) = D3DCOLOR_ARGB(255, 0, 0, 0);
		}
	}

	pTexture->UnlockRect(0);

	D3DXSaveTextureToFile(L"../Bin/TestHeight.bmp", D3DXIFF_BMP, pTexture, NULL);*/





	m_dwTriCnt = (dwCntX - 1) * (dwCntZ - 1) * 2;
	m_dwVtxCnt = dwCntX * dwCntZ;

	m_pPos = new _vec3[m_dwVtxCnt];

	m_dwVtxFVF = FVF_TEX;
	m_dwVtxSize = sizeof(VTXTEX);
	
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	
	FAILED_CHECK(CVIBuffer::Ready_Buffer());

	_ulong		dwByte = 0;

	m_hFile = CreateFile(L"../../Data/Terrain/HeightMap.bmp", 
							GENERIC_READ, 
							0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	
	ReadFile(m_hFile, &m_fH, sizeof(BITMAPFILEHEADER), &dwByte, NULL);
	ReadFile(m_hFile, &m_iH, sizeof(BITMAPINFOHEADER), &dwByte, NULL);


	
	m_dwVtxCntZ = 129;
	m_dwVtxCntX = 129;


	_ulong*		pPixel = new _ulong[m_iH.biWidth * m_iH.biHeight];

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
			
			m_pVtxTex[dwIndex].vPos   = _vec3((_float(j) * dwVtxItv), 
				                         ((pPixel[dwIndex]&0x000000ff)/255.f)*7.f,
											(_float(i) * dwVtxItv));

			m_pPos[dwIndex] = m_pVtxTex[dwIndex].vPos;
			
			m_pVtxTex[dwIndex].vNormal = _vec3(0.f, 0.f, 0.f);
			m_pVtxTex[dwIndex].vTexUV = _vec2(_float(j) / (dwCntX - 1),
				_float(i) / (dwCntZ - 1));
		}
	}


	Safe_Delete_Array(pPixel);

	INDEX32*		pIndex = NULL;

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

			m_pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			m_pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			m_pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			++dwTriIdx;

			pIndex[dwTriIdx]._0 = dwIndex + dwCntX;
			pIndex[dwTriIdx]._1 = dwIndex + 1;
			pIndex[dwTriIdx]._2 = dwIndex;

			vDest = m_pVtxTex[pIndex[dwTriIdx]._1].vPos - m_pVtxTex[pIndex[dwTriIdx]._0].vPos;
			vSour = m_pVtxTex[pIndex[dwTriIdx]._2].vPos - m_pVtxTex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			m_pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			m_pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			m_pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			++dwTriIdx;
		}
	}	

	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
		D3DXVec3Normalize(&m_pVtxTex[i].vNormal, &m_pVtxTex[i].vNormal);

	m_pVB->Unlock();
	m_pIB->Unlock();

	return S_OK;
}

HRESULT CTerrainTex::Reset_Buffer(const _ulong & dwCntX, const _ulong & dwCntZ, const _ulong & dwVtxItv)
{
	return E_NOTIMPL;
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

			pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			++dwTriIdx;

			// 외적 연산으로 면 버텍스의 법선벡터 구하기
			vDest = pVtxTex[pIndex[dwTriIdx]._1].vPos - pVtxTex[pIndex[dwTriIdx]._0].vPos;
			vSour = pVtxTex[pIndex[dwTriIdx]._2].vPos - pVtxTex[pIndex[dwTriIdx]._1].vPos;
			D3DXVec3Cross(&vNormal, &vDest, &vSour);

			pVtxTex[pIndex[dwTriIdx]._0].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._1].vNormal += vNormal;
			pVtxTex[pIndex[dwTriIdx]._2].vNormal += vNormal;

			++dwTriIdx;
		}
	}

	// 법선벡터 정규화
	for (_ulong i = 0; i < m_dwVtxCnt; ++i)
	{
		D3DXVec3Normalize(&pVtxTex[i].vNormal, &pVtxTex[i].vNormal);
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
	    //Safe_Delete_Array(m_pVtxTex);
		Safe_Delete_Array(m_pPos);
	}

}

Engine::CResources* Engine::CTerrainTex::Clone(void)
{
	return new CTerrainTex(*this);
}

