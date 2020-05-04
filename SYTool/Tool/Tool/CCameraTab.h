#pragma once


// CCameraTab 대화 상자

class CCameraTab : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraTab)

public:
	CCameraTab(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraTab();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:


	void UPdateCameraTool(const _float& fTimeDelta);
	void RenderCameraTool();



	vector<CToolPoint*>		m_vecAtPosList;
	vector<CToolPoint*>		m_vecEyePosList;


	float m_PosX;
	float m_PosY;
	float m_PosZ;
	CListBox m_CameraAtBox;
	CListBox m_CameraEyeBox;
	CButton m_AddEyeButton;
	CButton m_AddAtButton;
	CButton m_ModifyButton;
	CButton m_DeleteButton;
	float m_fCameraTime;
	CString m_CameraName;
	CButton m_AddCameraButton;
	CListBox m_CameraMadeList;
	afx_msg void OnBnClickedExcute();
	afx_msg void OnBnClickedAddButton();
	afx_msg void OnBnClickedPlayCamera();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedSaveCamera();
	afx_msg void OnBnClickedButton14();
};
