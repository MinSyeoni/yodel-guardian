#include "Shader.h"

USING(Engine)

CShader::CShader(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	: CComponent(pGraphicDevice, pCommandList)
{
}

CShader::CShader(const CShader & rhs)
	: CComponent(rhs)
	, m_pPipelineState(rhs.m_pPipelineState)
	, m_pVS_ByteCode(rhs.m_pVS_ByteCode)
	, m_pPS_ByteCode(rhs.m_pPS_ByteCode)
{
	m_pVS_ByteCode->AddRef();
	m_pPS_ByteCode->AddRef();
	m_pPipelineState->AddRef();
}


CShader::~CShader()
{
}


void CShader::Begin_Shader()
{
}

void CShader::End_Shader(_uint Texnum)
{
}

ID3DBlob * CShader::Compile_Shader(const wstring & wstrFilename, 
								   const D3D_SHADER_MACRO * tDefines,
								   const string & strEntrypoint, 
								   const string & strTarget)
{
	_uint uiCompileFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)  
	uiCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif

	ID3DBlob* pByteCode = nullptr;
	ID3DBlob* pErrors;

	HRESULT hResult = D3DCompileFromFile(wstrFilename.c_str(),
										 tDefines, 
										 D3D_COMPILE_STANDARD_FILE_INCLUDE,
										 strEntrypoint.c_str(), 
										 strTarget.c_str(),
										 uiCompileFlags, 
										 0,
										 &pByteCode,
										 &pErrors);

	if (pErrors != nullptr)
		OutputDebugStringA((char*)pErrors->GetBufferPointer());

	ThrowIfFailed(hResult);

	return pByteCode;
}


void CShader::Free()
{
	CComponent::Free();

	Engine::Safe_Release(m_pPipelineState);

	Engine::Safe_Release(m_pVS_ByteCode);
	Engine::Safe_Release(m_pPS_ByteCode);
}
