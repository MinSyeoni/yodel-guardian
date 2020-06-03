#include "TerrainTex.h"
#include "GraphicDevice.h"
USING(Engine)


CTerrainTex::CTerrainTex(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: CVIBuffer(pGraphicDevice, pCommandList)
{
}

CTerrainTex::CTerrainTex(const CTerrainTex & rhs)
	: CVIBuffer(rhs)
{
}


CTerrainTex::~CTerrainTex()
{
}

HRESULT CTerrainTex::Ready_Buffer(const _uint& iNumVerticesX, const _uint& iNumVerticesZ, const _float& fInterval)
{


	m_fInterval = fInterval;
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);
	std::vector<VTXTEX> Vertices;
	std::vector<_uint> Indices32;
	uint32_t vertexCount = iNumVerticesX * iNumVerticesZ;
	uint32_t faceCount = (iNumVerticesX - 1)*(iNumVerticesZ - 1) * 2;

	Vertices.resize(vertexCount);


	for (uint32_t i = 0; i < iNumVerticesZ; ++i)
	{
		for (uint32_t j = 0; j < iNumVerticesX; ++j)
		{
			_uint		iIndex = i * iNumVerticesX + j;

			Vertices[iIndex].vPos   = _vec3(j * m_fInterval, (float)pHeightMapPixels[j+i*iNumVerticesZ], i * m_fInterval);
			Vertices[iIndex].vNormal = _vec3(0.0f, 0.0f, 0.0f);
			Vertices[iIndex].vTexUV = _vec2((j / (iNumVerticesX - 1.f)) * 5.f, (i / (iNumVerticesZ - 1.f)) * 5.f);

		}
	}
	Safe_Delete_Array(pHeightMapPixels);
	Indices32.resize(faceCount * 3);
	uint32_t k = 0;
	for (uint32_t i = 0; i < iNumVerticesZ - 1; ++i)
	{
		for (uint32_t j = 0; j < iNumVerticesX - 1; ++j)
		{

			size_t iIndex = i * iNumVerticesX + j;

			Indices32[k] = (_uint)iIndex + iNumVerticesX;
			Indices32[k + 1] = (_uint)iIndex + iNumVerticesX + 1;
			Indices32[k + 2] = (_uint)iIndex + 1;

			_vec3		vSour, vDest;
			_vec3		vNormal;

			vSour = Vertices[Indices32[k + 1]].vPos - Vertices[Indices32[k]].vPos;
			vDest = Vertices[Indices32[k + 2]].vPos - Vertices[Indices32[k+1]].vPos;

			vNormal= vDest.Cross_InputDst(vSour);
			
			vNormal.Normalize();

			Vertices[Indices32[k]].vNormal += vNormal;
			Vertices[Indices32[k+1]].vNormal += vNormal;
			Vertices[Indices32[k+2]].vNormal += vNormal;

			k += 3;

			Indices32[k] = (_uint)iIndex + iNumVerticesX;
			Indices32[k + 1] = (_uint)iIndex + 1;
			Indices32[k + 2] = (_uint)iIndex;

			vSour = Vertices[Indices32[k + 1]].vPos - Vertices[Indices32[k]].vPos;
			vDest = Vertices[Indices32[k + 2]].vPos - Vertices[Indices32[k + 1]].vPos;

			vNormal = vDest.Cross_InputDst(vSour);

			vNormal.Normalize();

			Vertices[Indices32[k]].vNormal += vNormal;
			Vertices[Indices32[k+1]].vNormal += vNormal;
			Vertices[Indices32[k+2]].vNormal += vNormal;

			k += 3; // next quad
		}
	}

	for (_uint i = 0; i < vertexCount; i++)
	{
		Vertices[i].vNormal.Normalize();
	}

	const UINT vbByteSize = (UINT)Vertices.size() * sizeof(VTXTEX);
	m_uiVB_ByteSize = vbByteSize;

	const UINT ibByteSize = (UINT)Indices32.size() * sizeof(std::uint32_t);
	m_uiIB_ByteSize = ibByteSize;

	ThrowIfFailed(D3DCreateBlob(vbByteSize, &m_pVB_CPU));
	CopyMemory(m_pVB_CPU->GetBufferPointer(), Vertices.data(), vbByteSize);

	ThrowIfFailed(D3DCreateBlob(ibByteSize, &m_pIB_CPU));
	CopyMemory(m_pIB_CPU->GetBufferPointer(), Indices32.data(), ibByteSize);


	m_pVB_GPU = Create_DefaultBuffer(Vertices.data(), vbByteSize, m_pVB_Uploader);
	NULL_CHECK_RETURN(m_pVB_GPU, E_FAIL);

	//// IB »ý¼º.
	m_pIB_GPU = Create_DefaultBuffer(Indices32.data(), ibByteSize, m_pIB_Uploader);
	NULL_CHECK_RETURN(m_pIB_GPU, E_FAIL);


	
	m_uiVertexByteStride = sizeof(VTXTEX);
	m_uiVB_ByteSize = vbByteSize;
	m_uiIB_ByteSize = ibByteSize;
	m_IndexFormat = DXGI_FORMAT_R32_UINT;

	m_tSubMeshGeometry.uiIndexCount =(_uint)Indices32.size();
	m_tSubMeshGeometry.uiStartIndexLocation = 0;
	m_tSubMeshGeometry.iBaseVertexLocation = 0;

	

	return S_OK;
}

void CTerrainTex::Begin_Buffer()
{
	CVIBuffer::Begin_Buffer();
}

void CTerrainTex::End_Buffer()
{
}

void CTerrainTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent * CTerrainTex::Clone()
{
	return new CTerrainTex(*this);
}

CTerrainTex * CTerrainTex::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const _uint& iNumVerticesX, const _uint& iNumVerticesZ, const _float& fInterval )
{
	CTerrainTex* pInstance = new CTerrainTex(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Buffer(iNumVerticesX,  iNumVerticesZ, fInterval)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CTerrainTex::Free()
{
	CVIBuffer::Free();
}
