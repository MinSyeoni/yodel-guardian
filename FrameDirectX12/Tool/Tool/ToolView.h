
// ToolView.h: CToolView 클래스의 인터페이스
//

#pragma once
//#include "Base.h"
#include "GraphicDevice.h"

BEGIN(Engine)
class CComponentMgr;
class CObjectMgr;
class CManagement;
class CRenderer;
END

class CToolDoc;
class CMainFrame;
class CMyForm;

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

public:
	HRESULT			Ready_MainApp();
	_int			Update_MainApp(const _float& fTimeDelta);
	_int			LateUpdate_MainApp(const _float& fTimeDelta);
	HRESULT			Render_MainApp(const _float& fTimeDelta);


	Engine::CGraphicDevice*		m_pDeviceClass = nullptr;
	ID3D12Device*				m_pGraphicDevice = nullptr;
	ID3D12GraphicsCommandList*	m_pCommandList = nullptr;

private:
	HRESULT		SetUp_DefaultSetting(CGraphicDevice::WINMODE eMode, const _uint& dwWidth, const _uint& dwHeight);


private:
	CManagement*	m_pManagement = nullptr;
	CRenderer*		m_pRenderer = nullptr;
	CObjectMgr*		m_pObjectMgr = nullptr;
	CComponentMgr*	m_pComponentMgr = nullptr;

	CMainFrame* m_pMainFrm = nullptr;
	CToolView*	m_pToolView = nullptr;
	CMyForm*	m_pMyForm = nullptr;

protected:

// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

