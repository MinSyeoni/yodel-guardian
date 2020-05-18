#pragma once


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
};
