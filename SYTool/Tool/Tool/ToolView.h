
// ToolView.h: CToolView 클래스의 인터페이스
//

#pragma once

#include "ToolDoc.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "ToolCamera.h"

BEGIN(Engine)
class CManagement;
END

class CToolView : public CView
{
protected: // serialization에서만 만들어집니다.
	CToolView() noexcept;
	DECLARE_DYNCREATE(CToolView)

// 특성입니다.
public:
	CToolDoc* GetDocument() const;

// 작업입니다.
public:

// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 구현입니다.
public:
	virtual ~CToolView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

public:
	HRESULT			Render_MainApp();
	_int			Update_MainApp(const _float& fTimeDelta);
	HRESULT			Ready_MainApp();
	HRESULT			Ready_Default_Setting(CGraphicDev::WINMODE eMode, const _uint& iWinCX, const _uint& iWinCY);
	
	virtual void	OnInitialUpdate();
	void			Initalize_Object();
	void			Ready_Buffer_Setting();

	LPDIRECT3DDEVICE9& Get_ToolViewDevice() { return m_pDevice; };

private:
	Engine::CGraphicDev*	m_pGraphicDev = nullptr;
	LPDIRECT3DDEVICE9       m_pDevice = nullptr;
	CManagement*			m_pManagement = nullptr;

};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

