#include "NaviBuffer.h"

USING(Engine)


CNaviBuffer::CNaviBuffer(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: CVIBuffer(pGraphicDevice, pCommandList)
{
}

CNaviBuffer::CNaviBuffer(const CNaviBuffer & rhs)
	: CVIBuffer(rhs)
	,m_iVertexCount(rhs.m_iVertexCount)
{
}


CNaviBuffer::~CNaviBuffer()
{
}

HRESULT CNaviBuffer::Ready_Buffer(vector< NAVIINFO>vecNaviInfo)
{
	FAILED_CHECK_RETURN(CVIBuffer::Ready_Buffer(), E_FAIL);

	m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	m_iVertexCount = vecNaviInfo.size() * 3;
	/*____________________________________________________________________
	[ Vertex Buffer ]
	______________________________________________________________________*/

	vector<VTXCOL> vertiecs;
	for (_uint i = 0; i < vecNaviInfo.size(); ++i)
	{
		for (_uint j = 0; j < 3; ++j)
		{
			VTXCOL pCol= { vecNaviInfo[i].vPosition[j],vecNaviInfo[i].vColor };
			if(i%2==1)
			 pCol = { vecNaviInfo[i].vPosition[2-j],vecNaviInfo[i].vColor };
			

			vertiecs.push_back(pCol);
		}

	}
	vertiecs.shrink_to_fit();
	/*____________________________________________________________________
	[ Index Buffer ]
	______________________________________________________________________*/


	const _int uiVB_ByteSize = (_uint)vertiecs.size() * sizeof(VTXCOL);
	
	ThrowIfFailed(D3DCreateBlob(uiVB_ByteSize, &m_pVB_CPU));
	CopyMemory(m_pVB_CPU->GetBufferPointer(), vertiecs.data(), uiVB_ByteSize);


	// VB 생성.
	m_pVB_GPU = Create_DefaultBuffer(vertiecs.data(), uiVB_ByteSize, m_pVB_Uploader);
	NULL_CHECK_RETURN(m_pVB_GPU, E_FAIL);




	m_uiVertexByteStride = sizeof(VTXCOL);
	m_uiVB_ByteSize = uiVB_ByteSize;


	return S_OK;
}

void CNaviBuffer::Begin_Buffer()
{
	m_pCommandList->IASetVertexBuffers(0, 						 // 시작 슬롯. (입력 슬롯은 총 16개)
		1, 						 // 입력 슬롯들에 묶을 정점 버퍼 개수.
		&Get_VertexBufferView()); // 정점 버퍼 뷰의 첫 원소를 가리키는 포인터.

	m_pCommandList->IASetPrimitiveTopology(m_PrimitiveTopology);
}

void CNaviBuffer::End_Buffer()
{
}

void CNaviBuffer::Render_Buffer()
{
	m_pCommandList->DrawInstanced(m_iVertexCount, 1, 0, 0);
}

CComponent * CNaviBuffer::Clone()
{
	return new CNaviBuffer(*this);
}

CNaviBuffer * CNaviBuffer::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, vector< NAVIINFO>vecNaviInfo)
{
	CNaviBuffer* pInstance = new CNaviBuffer(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Buffer(vecNaviInfo)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CNaviBuffer::Free()
{
	CVIBuffer::Free();
}
