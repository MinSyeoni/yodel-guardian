#include "RcTex.h"

USING(Engine)


CRcTex::CRcTex(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: CVIBuffer(pGraphicDevice, pCommandList)
{
}

CRcTex::CRcTex(const CRcTex & rhs)
	: CVIBuffer(rhs)
{
}


CRcTex::~CRcTex()
{
}

HRESULT CRcTex::Ready_Buffer()
{
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	/*____________________________________________________________________
	[ Vertex Buffer ]
	______________________________________________________________________*/
	array<VTXRC, 4> vertices =
	{
		VTXRC(_vec3(-1.0f, 1.0f, 0.f), _vec2(0.0f,0.0f)),	// 0
		VTXRC(_vec3(1.0f, 1.0f, 0.f), _vec2(1.0f,0.0f)),	// 1
		VTXRC(_vec3(1.0f, -1.0f, 0.f), _vec2(1.0f,1.0f)),	// 2
		VTXRC(_vec3(-1.0f, -1.0f, 0.f),_vec2(0.0f,1.0f))	// 3
	};

	/*____________________________________________________________________
	[ Index Buffer ]
	______________________________________________________________________*/
	array<_uint, 6> indices =
	{
		0, 1, 2,	// 첫 번째 삼각형.
		0, 2, 3,	// 두 번째 삼각형
	};

	const _int uiVB_ByteSize = (_uint)vertices.size() * sizeof(VTXCOL);
	const _int uiIB_ByteSize = (_uint)indices.size() * sizeof(_uint);

	ThrowIfFailed(D3DCreateBlob(uiVB_ByteSize, &m_pVB_CPU));
	CopyMemory(m_pVB_CPU->GetBufferPointer(), vertices.data(), uiVB_ByteSize);

	ThrowIfFailed(D3DCreateBlob(uiIB_ByteSize, &m_pIB_CPU));
	CopyMemory(m_pIB_CPU->GetBufferPointer(), indices.data(), uiIB_ByteSize);

	// VB 생성.
	m_pVB_GPU = Create_DefaultBuffer(vertices.data(), uiVB_ByteSize, m_pVB_Uploader);
	NULL_CHECK_RETURN(m_pVB_GPU, E_FAIL);

	// IB 생성.
	m_pIB_GPU = Create_DefaultBuffer(indices.data(), uiIB_ByteSize, m_pIB_Uploader);
	NULL_CHECK_RETURN(m_pIB_GPU, E_FAIL);


	m_uiVertexByteStride	= sizeof(VTXRC);
	m_uiVB_ByteSize			= uiVB_ByteSize;
	m_uiIB_ByteSize			= uiIB_ByteSize;
	m_IndexFormat			= DXGI_FORMAT_R32_UINT;

	m_tSubMeshGeometry.uiIndexCount			= (_uint)indices.size();
	m_tSubMeshGeometry.uiStartIndexLocation = 0;
	m_tSubMeshGeometry.iBaseVertexLocation	= 0;

	return S_OK;
}

void CRcTex::Begin_Buffer()
{
	CVIBuffer::Begin_Buffer();
}

void CRcTex::End_Buffer()
{
}

void CRcTex::Render_Buffer()
{
	CVIBuffer::Render_Buffer();
}

CComponent * CRcTex::Clone()
{
	return new CRcTex(*this);
}

CRcTex * CRcTex::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CRcTex* pInstance = new CRcTex(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CRcTex::Free()
{
	CVIBuffer::Free();
}
