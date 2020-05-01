#include "VlLine.h"

CVlLine::CVlLine(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CVIBuffer(pGraphicDevice,pCommandList)
{
}

CVlLine::CVlLine(const CVlLine & rhs)
	:CVIBuffer(rhs)
{
}

CVlLine::~CVlLine()
{
}

HRESULT CVlLine::Ready_Buffer(_vec3 vStartPos, _vec3 vEndPos, _vec4 vColor)
{
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_LINELIST;

	/*____________________________________________________________________
	[ Vertex Buffer ]
	______________________________________________________________________*/
	array<VTXCOL, 3> vertices =
	{
		VTXCOL(vStartPos, vColor),
		VTXCOL(vEndPos, vColor),
	};

	const _int uiVB_ByteSize = (_uint)vertices.size() * sizeof(VTXCOL);

	ThrowIfFailed(D3DCreateBlob(uiVB_ByteSize, &m_pVB_CPU));
	CopyMemory(m_pVB_CPU->GetBufferPointer(), vertices.data(), uiVB_ByteSize);

	// VB »ý¼º.
	m_pVB_GPU = Create_DefaultBuffer(vertices.data(), uiVB_ByteSize, m_pVB_Uploader);
	NULL_CHECK_RETURN(m_pVB_GPU, E_FAIL);


	m_uiVertexByteStride = sizeof(VTXCOL);
	m_uiVB_ByteSize = uiVB_ByteSize;
	m_IndexFormat = DXGI_FORMAT_R32_UINT;


	return S_OK;
}

void CVlLine::Begin_Buffer()
{
	m_pCommandList->IASetVertexBuffers(0, 1,&Get_VertexBufferView()); 
}

void CVlLine::End_Buffer()
{
}

void CVlLine::Render_Buffer()
{

	m_pCommandList->IASetVertexBuffers(0, 1, &Get_VertexBufferView());
	m_pCommandList->IASetPrimitiveTopology(m_PrimitiveTopology);

	m_pCommandList->DrawInstanced(3, 1, 0, 0);
}

CComponent * CVlLine::Clone()
{
	return new CVlLine(*this);
}

CVlLine * CVlLine::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, _vec3 vStartPos, _vec3 vEndPos,_vec4 vColor)
{
	CVlLine* pInstance = new CVlLine(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Buffer( vStartPos,  vEndPos,  vColor)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CVlLine::Free()
{
}
