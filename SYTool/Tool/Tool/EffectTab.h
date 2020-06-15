#pragma once
#include "EffectDefault.h"

// EffectTab 대화 상자

class CEffectTab : public CDialogEx
{
	DECLARE_DYNAMIC(CEffectTab)

public:
	CEffectTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CEffectTab();

	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	void UpdateEffectTool(const float& fTimeDelte);
	float m_fOriPosX;
	float m_fOriPosY;
	float m_fOriPosZ;
	float m_fOriScaleX;
	float m_fOriScaleY;
	float m_fOriScaleZ;
	float m_fOriRotX;
	float m_fOriRotY;
	float m_fOriRotZ;
	afx_msg void OnBnClickedRcTex();
	float m_fPosX;
	float m_fPosY;
	float m_fPosZ;
	float m_fScaleX;
	float m_fScaleY;
	float m_fScaleZ;
	float m_fRotX;
	float m_fRotY;
	float m_fRotZ;

	CToolEffect* m_pEffectData = nullptr;

	CButton m_Pass0;
	CButton m_Pass1;
	CButton m_Pass2;
	afx_msg void OnBnClickedPass0();
	afx_msg void OnBnClickedPass1();
	afx_msg void OnBnClickedPass2();
	virtual BOOL OnInitDialog();


	void LoadTexture(wstring strPath, TEXTURE_STATE eState);

	void UpdateEffect();
	void InitEffect();

	afx_msg void PosXUp();
	afx_msg void PosXDown();
	afx_msg void PosYUp();
	afx_msg void PosYDown();
	afx_msg void PosZUp();
	afx_msg void PosZDown();
	afx_msg void ScaleXUp();
	afx_msg void ScaleXDown();
	afx_msg void ScaleYUp();
	afx_msg void ScaleYDown();
	afx_msg void ScaleZUp();
	afx_msg void ScaleZDown();
	afx_msg void RotXUp();
	afx_msg void RotXDown();
	afx_msg void RotYUp();
	afx_msg void RotYDown();
	afx_msg void RotZUp();
	afx_msg void RotZDown();
	afx_msg void ClickInput();
	afx_msg void ClickToOri();

	float m_fOriScaleStartTime;
	float m_fOriScaleEndTime;
	float m_fOriScaleXPat;
	float m_fOriScaleYPat;
	float m_fOriScaleZPat;
	float m_fOriScaleRepeat;
	float m_fScaleRepeat;
	float m_fScaleStartTIme;
	float m_fScaleEndTime;
	float m_fScaleXPat;
	float m_fScaleYPat;
	float m_fScaleZPat;
	float m_fOriRotStartTime;
	float m_fOriRotEndTime;
	float m_fOriRotXPat;
	float m_fOriRotYPat;
	float m_fOriRotZPat;
	float m_fOriRotRepeat;
	float m_fRotStartTime;
	float m_fRotEndTime;
	float m_fRotXPat;
	float m_fRotYPat;
	float m_fRotZPat;
	float m_fRotRepeat;



	_bool m_bIsStartEffect = false;

	afx_msg void OnBnClickedPatternInput();
	float m_fAccTime;
	afx_msg void PlayPattern();
	afx_msg void ClickPatStop();
	float m_fDeadStart;
	float m_fDeadEnd;
	float m_fOriStartTime;
	float m_fOriEndTIme;
	afx_msg void OnBnClickedInputStartEnd();
	CListBox m_MadeEffectList;
	afx_msg void EffectChangeClick();
	afx_msg void ClickDeleteButton();
	afx_msg void ClickCopy();
	afx_msg void OnBnClickedSaveEffect();
	afx_msg void OnBnClickedEffectLoad();
	CButton m_ButtonSelectAll;
};
