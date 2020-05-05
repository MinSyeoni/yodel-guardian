#include "SphereTex.h"

USING(Engine)

CSphereTex::CSphereTex(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: CVIBuffer(pGraphicDevice, pCommandList)
{
	ZeroMemory(&m_BoundingInfo, sizeof(BoundingSphere));
}

CSphereTex::CSphereTex(const CSphereTex& rhs)
	: CVIBuffer(rhs)
	,m_BoundingInfo(rhs.m_BoundingInfo)

{
}

CSphereTex::~CSphereTex()
{
}

HRESULT CSphereTex::Ready_Buffer()
{
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_BoundingInfo.Center = _vec3(0.f, 0.f, 0.f);
	m_BoundingInfo.Radius = 1.f;

	VTXRC  topVerTex = VTXRC(_vec3(0.f, +m_BoundingInfo.Radius, 0.0f), _vec2(0.f, 0.f));
	VTXRC bottomTEX = VTXRC(_vec3(0.f, -m_BoundingInfo.Radius, 0.f), _vec2(1.f, 1.f));
	_uint uiStackCnt = 10;
	_uint uiSliceCnt = 10;
	float phiStep = XM_PI / uiStackCnt;
	_float thetaStep = 2.0f * XM_PI / uiSliceCnt;
	vector<VTXRC>vertices;

	vertices.emplace_back(topVerTex);

	for (_uint i = 1; i <= uiStackCnt - 1; ++i)
	{
		float phi = i * phiStep;

		for (_uint j = 0; j <= uiSliceCnt; ++j)
		{
			float theta = j * thetaStep;

			VTXRC vtx;

			vtx.vPos.x = m_BoundingInfo.Radius * sinf(phi) * cosf(theta);
			vtx.vPos.y = m_BoundingInfo.Radius * cosf(phi);
			vtx.vPos.z = m_BoundingInfo.Radius * sinf(phi) * sinf(theta);

			vtx.vTexUV = _vec2( i / 20.f,j / 20.f );
			vertices.emplace_back(vtx);

		}
	}
	vertices.emplace_back(bottomTEX);

	vector<_uint>indices;

	for (_uint i = 1; i <= uiSliceCnt; ++i)
	{
		indices.emplace_back(0);
		indices.emplace_back(i + 1);
		indices.emplace_back(i);
	}

	_uint baseIndex = 1;
	_uint ringVertexCount = uiSliceCnt + 1;

	for (_uint i = 0; i < uiStackCnt - 2; ++i)
	{

		for (_uint j = 0; j < uiSliceCnt; ++j)
		{
			indices.emplace_back(baseIndex + i * ringVertexCount + j);
			indices.emplace_back(baseIndex + i * ringVertexCount + j + 1);
			indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);

			indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j);
			indices.emplace_back(baseIndex + i * ringVertexCount + j + 1);
			indices.emplace_back(baseIndex + (i + 1) * ringVertexCount + j + 1);

		}


	}
	_uint southPoleIndex = (_uint)vertices.size() - 1;

	// - Offset the indices to the index of the first vertex in the last ring.
	baseIndex = southPoleIndex - ringVertexCount;

	for (_uint i = 0; i < uiSliceCnt; ++i)
	{
		indices.emplace_back(southPoleIndex);
		indices.emplace_back(baseIndex + i);
		indices.emplace_back(baseIndex + i + 1);
	}


	const _int uiVB_ByteSize = (_uint)vertices.size() * sizeof(VTXRC);
	const _int uiIB_ByteSize = (_uint)indices.size() * sizeof(_uint);

	FAILED_CHECK_RETURN(D3DCreateBlob(uiVB_ByteSize, &m_pVB_CPU), E_FAIL);
	CopyMemory(m_pVB_CPU->GetBufferPointer(), vertices.data(), uiVB_ByteSize);

	FAILED_CHECK_RETURN(D3DCreateBlob(uiIB_ByteSize, &m_pIB_CPU), E_FAIL);
	CopyMemory(m_pIB_CPU->GetBufferPointer(), indices.data(), uiIB_ByteSize);

	// VB 持失.
	m_pVB_GPU = Create_DefaultBuffer(vertices.data(), uiVB_ByteSize, m_pVB_Uploader);
	NULL_CHECK_RETURN(m_pVB_GPU, E_FAIL);

	// IB 持失.
	m_pIB_GPU = Create_DefaultBuffer(indices.data(), uiIB_ByteSize, m_pIB_Uploader);
	NULL_CHECK_RETURN(m_pIB_GPU, E_FAIL);


	m_uiVertexByteStride = sizeof(VTXRC);
	m_uiVB_ByteSize = uiVB_ByteSize;
	m_uiIB_ByteSize = uiIB_ByteSize;
	m_IndexFormat = DXGI_FORMAT_R32_UINT;

	m_tSubMeshGeometry.uiIndexCount = (_uint)indices.size();
	m_tSubMeshGeometry.uiStartIndexLocation = 0;
	m_tSubMeshGeometry.iBaseVertexLocation = 0;

	return S_OK;
}

void CSphereTex::Begin_Buffer()
{
	CVIBuffer::Begin_Buffer();
}

void CSphereTex::End_Buffer()
{
}

void CSphereTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent* CSphereTex::Clone()
{
	return new CSphereTex(*this);
}

CSphereTex* CSphereTex::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CSphereTex* pInstance = new CSphereTex(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CSphereTex::Free()
{
	CVIBuffer::Free();
}
