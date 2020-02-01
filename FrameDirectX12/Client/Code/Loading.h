#pragma

#include "Base.h"

class CLoading : public CBase
{
public:
	enum LOADINGID { LOADING_STAGE, LOADING_END };

private:
	explicit CLoading(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CLoading(void);

public:
	LOADINGID			Get_LoadingID(void) const { return m_LoadingID; }
	CRITICAL_SECTION*	Get_Crt(void) { return &m_Crt; }
	_bool				Get_Finish(void) const { return m_bFinish; }

public:
	HRESULT				Ready_Loading(LOADINGID eLoadingID);
	_uint				Loading_ForStage(void);

	HRESULT             Mesh_ForStage(void);
	HRESULT             Texture_ForStage(void);


private:
	HANDLE			         	m_hThread;
	CRITICAL_SECTION	        m_Crt;
	LOADINGID			        m_LoadingID;

	ID3D12Device*	            m_pGraphicDev;
	ID3D12CommandQueue*			m_pCommandQueue = nullptr;
	ID3D12CommandAllocator*		m_pCommandAllocator = nullptr;
	ID3D12GraphicsCommandList*	m_pCommandList = nullptr;
	_bool				        m_bFinish;

public:
	static unsigned int APIENTRY	Thread_Main(void* pArg);
	static CLoading*	Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, LOADINGID eLoadingID);

private:
	virtual void		Free(void);


};

