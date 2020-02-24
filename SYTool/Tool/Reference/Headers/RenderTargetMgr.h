#ifndef RenderTargetMgr_h__
#define RenderTargetMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "RenderTarget.h"

BEGIN(Engine)

class ENGINE_DLL CRenderTargetMgr : public CBase
{
	DECLARE_SINGLETON(CRenderTargetMgr)

private:
	explicit CRenderTargetMgr(void);
	virtual ~CRenderTargetMgr(void);

public:
	HRESULT		Ready_RenderTarget(const _tchar* pTargetTag,
		LPDIRECT3DDEVICE9 pGraphicDev,
		const _uint& iWidth,
		const _uint& iHeight,
		D3DFORMAT Format,
		D3DXCOLOR Color);

	HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag,
		const _float& fX,
		const _float& fY,
		const _float& fSizeX,
		const _float& fSizeY);

	void		Render_DebugBuffer(const _tchar* pMRTTag);


	HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);

	HRESULT		Begin_MRT(const _tchar* pMRTTag);
	HRESULT		End_MRT(const _tchar* pMRTTag);

	void		Set_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName);


private:
	CRenderTarget*		Find_RenderTarget(const _tchar* pTargetTag);
	list<CRenderTarget*>*	Find_MRT(const _tchar* pMRTTag);

private:
	map<const _tchar*, CRenderTarget*>		m_mapRenderTarget;

	map<const _tchar*, list<CRenderTarget*>>	m_mapMRT; // 새로운 렌더타겟 추가용도가 아님, 기존에 생성한 렌더타겟들을 그룹지어 관리하는 것이 주목적




private:
	virtual void		Free(void);

};

END
#endif // RenderTargetMgr_h__
