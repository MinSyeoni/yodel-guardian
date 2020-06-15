#pragma once

#include "EffectDefault.h"
// EffectTab2 대화 상자

class EffectTab2 : public CDialogEx
{
	DECLARE_DYNAMIC(EffectTab2)

public:
	EffectTab2(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EffectTab2();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_Effect2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
public:
	void FindEffectData();
	void UpdateEffectTool2(const float& fTimeDelta);


	CToolEffect* m_pEffectData = nullptr;

	DECLARE_MESSAGE_MAP()
	float m_fOriFadeInStart;
	float m_fFadeInStart;
	float m_fOriFadeInEnd;
	float m_fFadeinEnd;
	CButton m_FadeInButton;
	CButton m_FadeOutButton;
	float m_fOriFadeOutStart;
	float m_fFadeOutStart;
	float m_fOriFadeOutEnd;
	float m_fFadeOutEnd;


	_bool m_bIsPlayAni = false;




	afx_msg void ClickInput();
	float m_fAccTime;
	afx_msg void PlayEffect();
	afx_msg void ClickStop();
	float m_fOriWidth;
	float m_fWidth;
	float m_fOriHeight;
	float m_fHeight;
	float m_fOriUvRepeat;
	float m_fUvRepeat;
	CButton m_UvSpriteButton;
	float m_fOriUvSpeed;
	float m_fUvSpeed;
	CMFCColorButton m_ColorList;
	CButton m_InputColorButton;
};
