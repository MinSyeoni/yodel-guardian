// CameraTool.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CameraTool.h"
#include "afxdialogex.h"
#include "ActionCamera.h"

// CameraTool 대화 상자

IMPLEMENT_DYNAMIC(CCameraTool, CDialogEx)

CCameraTool::CCameraTool(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)
	, m_PosX(0)
	, m_PosY(0)
	, m_PosZ(0)
	, m_CameraName(_T(""))
	, m_fCameraTime(0)
{

}

CCameraTool::~CCameraTool()
{
	for (int i = 0; i < m_vecAtPosList.size(); i++)
	{
		Engine::Safe_Release(m_vecAtPosList[i]);

	}
	for (int i = 0; i < m_vecEyePosList.size(); i++)
	{
		Engine::Safe_Release(m_vecEyePosList[i]);

	}
}

void CCameraTool::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO3, m_AddEyeButton);
	DDX_Control(pDX, IDC_RADIO4, m_AddAtButton);
	DDX_Control(pDX, IDC_RADIO5, m_ModifyButton);
	DDX_Control(pDX, IDC_RADIO6, m_DeleteButton);
	DDX_Text(pDX, IDC_EDIT3, m_PosX);
	DDX_Text(pDX, IDC_EDIT2, m_PosY);
	DDX_Text(pDX, IDC_EDIT16, m_PosZ);
	DDX_Control(pDX, IDC_LIST4, m_CameraAtBox);
	DDX_Control(pDX, IDC_LIST1, m_CameraEyeBox);
	DDX_Text(pDX, IDC_EDIT5, m_CameraName);
	DDX_Control(pDX, IDC_BUTTON2, m_AddCameraButton);
	DDX_Control(pDX, IDC_LIST2, m_CameraMadeList);
	DDX_Text(pDX, IDC_EDIT4, m_fCameraTime);
}


BEGIN_MESSAGE_MAP(CCameraTool, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO3, &CCameraTool::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_BUTTON6, &CCameraTool::OnBnClickedExcute)
	ON_BN_CLICKED(IDC_BUTTON2, &CCameraTool::OnBnClickedAddButton)
	ON_BN_CLICKED(IDC_BUTTON3, &CCameraTool::OnBnClickedPlayCamera)
	ON_BN_CLICKED(IDC_BUTTON5, &CCameraTool::OnBnClickedDeleteButton)
	ON_BN_CLICKED(IDC_BUTTON4, &CCameraTool::OnBnClickedSaveCamera)
	ON_BN_CLICKED(IDC_BUTTON12, &CCameraTool::OnBnClickedLoad)
END_MESSAGE_MAP()


// CameraTool 메시지 처리기


void CCameraTool::OnBnClickedRadio3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.



}

void CCameraTool::UPdateCameraTool(const _float& fTimeDelta)
{
	for (int i = 0; i < m_vecAtPosList.size(); i++)
	{
		m_vecAtPosList[i]->Update_Object(fTimeDelta);

	}
	for (int i = 0; i < m_vecEyePosList.size(); i++)
	{
		m_vecEyePosList[i]->Update_Object(fTimeDelta);

	}



}

void CCameraTool::RenderCameraTool()
{
	for (int i = 0; i < m_vecAtPosList.size(); i++)
	{
		m_vecAtPosList[i]->Render_Object();
	}

	for (int i = 0; i < m_vecEyePosList.size(); i++)
	{
		m_vecEyePosList[i]->Render_Object();
	}
}


void CCameraTool::OnBnClickedExcute()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(FALSE);
	if (m_AddAtButton.GetCheck())
	{
		_vec3 pPos; 
		pPos.x = m_PosX;
		pPos.y = m_PosY;
		pPos.z = m_PosZ;

		CToolPoint* pObj = CToolPoint::Create(CGraphicDev::GetInstance()->GetDevice(), pPos);

		m_vecAtPosList.push_back(pObj);


		int i = m_vecAtPosList.size();
		CString temp;

		temp.Format(L"%d번 At", i);
		m_CameraAtBox.AddString(temp);



	} 
	else if (m_AddEyeButton.GetCheck())
	{
		_vec3 pPos;
		pPos.x = m_PosX;
		pPos.y = m_PosY;
		pPos.z = m_PosZ;



		CToolPoint* pObj = CToolPoint::Create(CGraphicDev::GetInstance()->GetDevice(), pPos);

		m_vecEyePosList.push_back(pObj);

		int i = m_vecEyePosList.size();
		CString temp ;

		temp.Format(L"%d번 Eye", i);
		m_CameraEyeBox.AddString(temp);
	}
	else if (m_DeleteButton.GetCheck())
	{
		int iIndex =m_CameraAtBox.GetCurSel();
		if (iIndex != -1)
		{
			m_CameraAtBox.DeleteString(iIndex);
			m_vecAtPosList.erase(m_vecAtPosList.begin() + iIndex);

		}
		 iIndex = m_CameraEyeBox.GetCurSel();
		if (iIndex != -1)
		{
			m_CameraEyeBox.DeleteString(iIndex);
			m_vecEyePosList.erase(m_vecEyePosList.begin() + iIndex);

		}


	}
	UpdateData(TRUE);

}


void CCameraTool::OnBnClickedAddButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);
	m_CameraMadeList.AddString(m_CameraName.GetString());

	m_CameraAtBox.ResetContent();
	m_CameraEyeBox.ResetContent();

	wstring Temp = m_CameraName;

	CActionCamera::GetInstance()->AddCamera(m_vecAtPosList,m_vecEyePosList, Temp,m_fCameraTime);

	m_vecAtPosList.clear();
	m_vecEyePosList.clear();
	m_vecAtPosList.shrink_to_fit();
	m_vecEyePosList.shrink_to_fit();

	UpdateData(FALSE);
}


void CCameraTool::OnBnClickedPlayCamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	int iIndex=  m_CameraMadeList.GetCurSel();

	if (iIndex < 0)
		return;

	CActionCamera::GetInstance()->PlayCamera(iIndex);

}


void CCameraTool::OnBnClickedDeleteButton()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);


	int iIndex =m_CameraMadeList.GetCurSel();
	if (iIndex == -1)
		return;

	m_CameraMadeList.DeleteString(iIndex);

	CActionCamera::GetInstance()->DeleteCamera(iIndex);

	UpdateData(FALSE);
}


void CCameraTool::OnBnClickedSaveCamera()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	CFileDialog Dlg(FALSE, L"dat", L"제목없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);

	TCHAR szCurPath[256] = L"";

	// GetCurrentDirectory: 현재 작업 경로를 얻어오는 함수.
	GetCurrentDirectory(256, szCurPath);
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data");
	lstrcat(szCurPath, L"\\Camera");
	Dlg.m_ofn.lpstrInitialDir = szCurPath;

	if (IDOK == Dlg.DoModal())
	{

		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_WRITE, 0, 0,
			CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;
		TCHAR strName[128] = L"";
		for (int i = 0; i < CActionCamera::GetInstance()->m_vecCameraSort.size(); i++)
		{
			int iAtSize =CActionCamera::GetInstance()->m_vecCameraSort[i]->m_vecCamAt.size();
			int iEyeSize = CActionCamera::GetInstance()->m_vecCameraSort[i]->m_vecCamEye.size();

			float fPlayTime = CActionCamera::GetInstance()->m_vecCameraSort[i]->m_fTime;
			
			lstrcpy(strName, CActionCamera::GetInstance()->m_vecCameraSort[i]->m_strCameraName.c_str());
			int strLength = lstrlen(strName) + 1;

			WriteFile(hFile, &strLength, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &strName, sizeof(TCHAR)*strLength, &dwByte,nullptr);
			WriteFile(hFile, &fPlayTime, sizeof(float), &dwByte, nullptr);
			WriteFile(hFile, &iEyeSize, sizeof(int), &dwByte, nullptr);
			WriteFile(hFile, &iAtSize, sizeof(int), &dwByte, nullptr);
			for (int j = 0; j < CActionCamera::GetInstance()->m_vecCameraSort[i]->m_vecCamEye.size(); j++)
			{
				_vec3 Pos;

			 Pos=dynamic_cast<CTransform*>(CActionCamera::GetInstance()->m_vecCameraSort[i]->m_vecCamEye[j]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC))->m_vInfo[INFO_POS];
			 WriteFile(hFile, &Pos, sizeof(_vec3),&dwByte, nullptr);



			}
			for (int j = 0; j < CActionCamera::GetInstance()->m_vecCameraSort[i]->m_vecCamAt.size(); j++)
			{
				_vec3 Pos;
				Pos = dynamic_cast<CTransform*>(CActionCamera::GetInstance()->m_vecCameraSort[i]->m_vecCamAt[j]->Get_Component(L"Com_Transform", COMPONENTID::ID_DYNAMIC))->m_vInfo[INFO_POS];
				WriteFile(hFile, &Pos, sizeof(_vec3), &dwByte, nullptr);


			}


		
		}


		CloseHandle(hFile);

	}
	UpdateData(FALSE);
}


void CCameraTool::OnBnClickedLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	CFileDialog Dlg(TRUE, L"dat", L"제목없음.dat", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"Data Files(*.dat)|*.dat||", this);


	TCHAR szCurPath[128] = L"";
	GetCurrentDirectory(256, szCurPath);

	// PathRemoveFileSpec: 현재 경로 상에서 파일명을 제거하는 함수.
	// 제거해야할 파일명이 없을 경우에는 가장 말단 폴더명을 제거한다.
	PathRemoveFileSpec(szCurPath);
	lstrcat(szCurPath, L"\\Data\\Camera");

	Dlg.m_ofn.lpstrInitialDir = szCurPath;


	if (IDOK == Dlg.DoModal())
	{
		CString strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.GetString(), GENERIC_READ, 0, 0,
			OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		DWORD dwByte = 0;

		TCHAR strName[128] = L"";
		int iStrLength = 0;
		float PlayTime = 0;
		int iSpotEye = 0;
		int iSpotAt = 0;
		_vec3 vEye = {};
		_vec3 vAt = {};
		while (1)
		{

			ReadFile(hFile, &iStrLength, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &strName, sizeof(TCHAR)*iStrLength, &dwByte, nullptr);
			ReadFile(hFile, &PlayTime, sizeof(float), &dwByte, nullptr);
			ReadFile(hFile, &iSpotEye, sizeof(int), &dwByte, nullptr);
			ReadFile(hFile, &iSpotAt, sizeof(int), &dwByte, nullptr);

			
			vector<CToolPoint*>		vecEyePosList;
			vector<CToolPoint*>		vecAthPosList;

			for (int i = 0; i < iSpotEye; i++)
			{
				ReadFile(hFile, &vEye, sizeof(_vec3), &dwByte, nullptr);

				CToolPoint* pPoint = CToolPoint::Create(CGraphicDev::GetInstance()->GetDevice(), vEye);
				vecEyePosList.push_back(pPoint);

			}


			for (int i = 0; i < iSpotAt; i++)
			{
				ReadFile(hFile, &vAt, sizeof(_vec3), &dwByte, nullptr);
			
				CToolPoint* pPoint = CToolPoint::Create(CGraphicDev::GetInstance()->GetDevice(), vAt);
				vecAthPosList.push_back(pPoint);

			}

			CActionCamera::GetInstance()->AddCamera(vecAthPosList, vecEyePosList, strName, PlayTime);


			if (0 == dwByte)
			{
				break;
			}
			

			m_CameraMadeList.AddString(strName);


		}
		CloseHandle(hFile);
	}



	UpdateData(FALSE);




}
