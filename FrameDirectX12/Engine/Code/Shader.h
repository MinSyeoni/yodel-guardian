#pragma once
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
protected:
	explicit CShader(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CShader(const CShader& rhs);
	virtual ~CShader();

public:
	ID3D12PipelineState*		Get_PipelineState()						{ return m_pPipelineState; }
public:
	virtual void Begin_Shader();
	virtual void End_Shader(_uint Texnum = 0,_int Index=0);

public:
	ID3D12DescriptorHeap*			Get_CBVDescriptorHeap() { return m_pCBV_DescriptorHeap; }
	CUploadBuffer<CB_MATRIX_INFO>*	Get_UploadBuffer_MatrixInfo() { return m_pCB_MatrixInfo; }

protected:
	ID3DBlob* Compile_Shader(const wstring& wstrFilename,
							 const D3D_SHADER_MACRO* tDefines,
							 const string& strEntrypoint,
							 const string& strTarget);
protected:
	virtual HRESULT						Create_DescriptorHeaps()	PURE;
	virtual HRESULT						Create_ConstantBufferView()	PURE;
	virtual HRESULT						Create_PipelineState()		PURE;
protected:
	virtual D3D12_RASTERIZER_DESC		Create_RasterizerState()	PURE;
	virtual D3D12_BLEND_DESC			Create_BlendState()			PURE;
	virtual D3D12_DEPTH_STENCIL_DESC	Create_DepthStencilState()	PURE;
	virtual D3D12_INPUT_LAYOUT_DESC		Create_InputLayout()		PURE;

protected:


	/*____________________________________________________________________
	[ 파이프라인 상태 객체(Pipeline State Object, PSO) ]
	- 파이프라인 상태를 나타내는 인터페이스 포인터.
	- PSO 검증과 생성에는 많은 시간이 걸릴 수 있으므로, 초기화 시점에서 생성해야 한다.
	- 성능을 위해서는 PSO 상태 변경을 최소화해야 한다.
	- 같은 PSO를 사용할 수 있는 물체들은 모두 함께 그려야 마땅하다.
	- 그리기 호출마다 PSO를 변경하지는 말아야 한다.
	______________________________________________________________________*/
	ID3D12PipelineState* m_pPipelineState = nullptr;

	ID3DBlob* m_pVS_ByteCode = nullptr;
	ID3DBlob* m_pPS_ByteCode = nullptr;

protected:
	ID3D12DescriptorHeap*			m_pCBV_DescriptorHeap = nullptr;
	CUploadBuffer<CB_MATRIX_INFO>*	m_pCB_MatrixInfo = nullptr;


public:
	virtual CComponent* Clone() PURE;
protected:
	virtual void		Free();
};

END