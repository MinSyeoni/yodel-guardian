#pragma once
#include "Engine_Typedef.h"
#include "Engine_Macro.h"

/*____________________________________________________________________
[ UploadBuffer ]
______________________________________________________________________*/
template<typename T>
class CUploadBuffer
{
public:
	CUploadBuffer(ID3D12Device* pGraphicDevice, _uint uiElementCount, _bool bIsConstantBuffer)
		: m_pGraphicDevice(pGraphicDevice)
		, m_bIsConstantBuffer(bIsConstantBuffer)
		, m_uiElementCount(uiElementCount)
	{
		m_uiElementByteSize = sizeof(T);

		/*____________________________________________________________________
		상수 버퍼 원소의 크기는 반드시 256바이트의 배수이어야 한다.
		이는 하드웨어가 m * 256바이트 오프셋에서 시작하는 n * 256바이트 길이의
		상수 자료만 볼 수 있기 때문이다.

		typedef struct D3D12_CONSTANT_BUFFER_VIEW_DESC 
		{
			UINT64 OffsetInBytes;			// 256의 배수.
			UINT   SizeInBytes;				// 256의 배수.
		} D3D12_CONSTANT_BUFFER_VIEW_DESC;
		______________________________________________________________________*/
		if (m_bIsConstantBuffer)
			m_uiElementByteSize = (sizeof(T) + 255) & ~255;

		ThrowIfFailed(m_pGraphicDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
																D3D12_HEAP_FLAG_NONE,
																&CD3DX12_RESOURCE_DESC::Buffer(m_uiElementByteSize*uiElementCount),
																D3D12_RESOURCE_STATE_GENERIC_READ,
																nullptr,
																IID_PPV_ARGS(&m_pUploadBuffer)));

		ThrowIfFailed(m_pUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pMappedData)));

		/*____________________________________________________________________
		자원을 다 사용하기 전에는 대응을 해제할 필요가 없다.
		그러나, 자원을 GPU가 사용하는 중에는 CPU에서 자원을 갱신하지 않아야 한다.
		(따라서, 반드시 동기화 기법을 사용해아 한다.)
		______________________________________________________________________*/
	}
	CUploadBuffer& operator=(const CUploadBuffer& rhs) = delete;
	~CUploadBuffer()
	{
		Free();
	}

	ID3D12Resource* Resource() const
	{
		return m_pUploadBuffer;
	}

	void CopyData(_int iElementIndex, const T& data)
	{
		memcpy(&m_pMappedData[iElementIndex * m_uiElementByteSize], &data, sizeof(T));
		// m_pUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_pMappedData));
	}

	void ReleaseData()
	{
		//if (m_pUploadBuffer != nullptr)
		//	m_pUploadBuffer->Unmap(0, nullptr);
	}

	void Free()
	{
		if (m_pUploadBuffer != nullptr)
			m_pUploadBuffer->Unmap(0, nullptr);

		m_pMappedData = nullptr;
	}

	void Set_IsClone() { m_bIsClone = true; }

private:
	ID3D12Device*	m_pGraphicDevice	= nullptr;
	ID3D12Resource* m_pUploadBuffer		= nullptr;

	_uint			m_uiElementCount	= 0;
	BYTE*			m_pMappedData		= nullptr;
	_uint			m_uiElementByteSize = 0;
	_bool			m_bIsConstantBuffer = false;

	_bool			m_bIsClone			= false;
};
