#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer : public CComponent
{
protected:
	explicit CVIBuffer(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CVIBuffer(const CVIBuffer& rhs);
	virtual ~CVIBuffer();

public:
	D3D12_VERTEX_BUFFER_VIEW Get_VertexBufferView() const;
	D3D12_INDEX_BUFFER_VIEW  Get_IndexBufferView()	const;

public:
	HRESULT			Ready_Buffer();
	void			Begin_Buffer();
	void			Render_Buffer();
public:
	 ID3D12Resource* Create_DefaultBuffer(const void* InitData,
										  UINT64 uiByteSize,
										  ID3D12Resource*& pUploadBuffer);
protected:
	/*____________________________________________________________________
	시스템 메모리 복사본. 정점/인덱스 형식이 범용적일 수 있으므로, ID3DBlob를 사용.
	실제로 사용할 때에는 클라이언트에서 적절히 캐스팅해야 한다.
	______________________________________________________________________*/
	ID3DBlob*		m_pVB_CPU				= nullptr;
	ID3DBlob*		m_pIB_CPU				= nullptr;

	ID3D12Resource* m_pVB_GPU				= nullptr;
	ID3D12Resource* m_pIB_GPU				= nullptr;

	ID3D12Resource* m_pVB_Uploader			= nullptr;
	ID3D12Resource* m_pIB_Uploader			= nullptr;

	_uint			m_uiVertexByteStride	= 0;
	_uint			m_uiVB_ByteSize			= 0;
	_uint			m_uiIB_ByteSize			= 0;
	DXGI_FORMAT		m_IndexFormat			= DXGI_FORMAT_R32_UINT;

	SUBMESH_GEOMETRY m_tSubMeshGeometry;
	D3D12_PRIMITIVE_TOPOLOGY m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

public:
	virtual CComponent* Clone(void) PURE;
protected:
	virtual void Free();
};

END