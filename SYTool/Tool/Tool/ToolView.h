
// ToolView.h: CToolView 클래스의 인터페이스
//

#pragma once

#include "ToolDoc.h"
#include "Export_Function.h"
#include "Terrain.h"
#include "ToolCamera.h"
#include "MapTab.h"
#include "Myform.h"

BEGIN(Engine)
class CManagement;
class CBoxCollider;
class CSphereCollider;
class CToolCollider;
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

public:
	HRESULT				Render_MainApp();
	_int				Update_MainApp(const _float& fTimeDelta);
	HRESULT				Ready_MainApp();
	HRESULT				Ready_Default_Setting(CGraphicDev::WINMODE eMode, const _uint& iWinCX, const _uint& iWinCY);
	
	virtual void		OnInitialUpdate();
	void				Initalize_Object();
	void				Ready_Buffer_Setting();

	LPDIRECT3DDEVICE9&  Get_ToolViewDevice() { return m_pDevice; };

	_vec3				Get_PickMeshPos() { return m_vMeshPos; };
	_vec3				Get_PickMeshScale() { return m_vMeshScale; };
	_vec3				Get_PickMeshRot() { return m_vMeshRot; };

private:
	Engine::CGraphicDev*	m_pGraphicDev = nullptr;
	LPDIRECT3DDEVICE9       m_pDevice = nullptr;
	CManagement*			m_pManagement = nullptr;

	_vec3					m_vMeshPos = { 0.f,0.f,0.f };
	_vec3					m_vMeshScale = { 0.f,0.f,0.f };
	_vec3					m_vMeshRot = { 0.f,0.f,0.f };

	CMapTab*				m_pMapTab = nullptr;

public:
	afx_msg void		OnLButtonDown(UINT nFlags, CPoint point);
	void				Picking_MeshOnStaticObject(bool& retflag);
	void				Picking_MouseOnStaticObject(bool& retflag);
	void				Picking_TerrainOnStaticObject(bool& retflag);
};

#ifndef _DEBUG  // ToolView.cpp의 디버그 버전
inline CToolDoc* CToolView::GetDocument() const
   { return reinterpret_cast<CToolDoc*>(m_pDocument); }
#endif

