#pragma once


// CameraTool 대화 상자

class CCameraTool : public CDialogEx
{
	DECLARE_DYNAMIC(CCameraTool)

public:
	CCameraTool(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraTool();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	void DrawCameraPoint();

	DECLARE_MESSAGE_MAP()
public:
	CButton m_AddEyeButton;
	afx_msg void OnBnClickedRadio3();

	void UPdateCameraTool(const _float& fTimeDelta);
	void RenderCameraTool();

	CButton m_AddAtButton;
	CButton m_ModifyButton;
	CButton m_DeleteButton;
	afx_msg void OnBnClickedExcute();
	float m_PosX;
	float m_PosY;
	float m_PosZ;
	CListBox m_CameraAtBox;
	CListBox m_CameraEyeBox;


	vector<CToolPoint*>		m_vecAtPosList;
	vector<CToolPoint*>		m_vecEyePosList;


	CString m_CameraName;
	CButton m_AddCameraButton;
	CListBox m_CameraMadeList;

	afx_msg void OnBnClickedAddButton();
	float m_fCameraTime;//카메라시간
	afx_msg void OnBnClickedPlayCamera();
	afx_msg void OnBnClickedDeleteButton();
	afx_msg void OnBnClickedSaveCamera();
	afx_msg void OnBnClickedLoad();
};
