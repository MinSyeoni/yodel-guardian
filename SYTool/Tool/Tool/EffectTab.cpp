// EffectTab.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "EffectTab.h"
#include "afxdialogex.h"
#include "EffectDefault.h"

// EffectTab 대화 상자

IMPLEMENT_DYNAMIC(CEffectTab, CDialogEx)

CEffectTab::CEffectTab(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
	, m_fOriPosX(0)
	, m_fOriPosY(0)
	, m_fOriPosZ(0)
	, m_fOriScaleX(1)
	, m_fOriScaleY(1)
	, m_fOriScaleZ(1)
	, m_fOriRotX(0)
	, m_fOriRotY(0)
	, m_fOriRotZ(0)
	, m_fPosX(0)
	, m_fPosY(0)
	, m_fPosZ(0)
	, m_fScaleX(1)
	, m_fScaleY(1)
	, m_fScaleZ(1)
	, m_fRotX(0)
	, m_fRotY(0)
	, m_fRotZ(0)
	, m_fOriScaleStartTime(0)
	, m_fOriScaleEndTime(0)
	, m_fOriScaleXPat(1)
	, m_fOriScaleYPat(1)
	, m_fOriScaleZPat(1)
	, m_fOriScaleRepeat(0)
	, m_fScaleRepeat(0)
	, m_fScaleStartTIme(0)
	, m_fScaleEndTime(0)
	, m_fScaleXPat(1)
	, m_fScaleYPat(1)
	, m_fScaleZPat(1)
	, m_fOriRotStartTime(0)
	, m_fOriRotEndTime(0)
	, m_fOriRotXPat(0)
	, m_fOriRotYPat(0)
	, m_fOriRotZPat(0)
	, m_fOriRotRepeat(0)
	, m_fRotStartTime(0)
	, m_fRotEndTime(0)
	, m_fRotXPat(0)
	, m_fRotYPat(0)
	, m_fRotZPat(0)
	, m_fRotRepeat(0)
	, m_fAccTime(0)
{
	
}

CEffectTab::~CEffectTab()
{



	
}

void CEffectTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fOriPosX);
	DDX_Text(pDX, IDC_EDIT2, m_fOriPosY);
	DDX_Text(pDX, IDC_EDIT3, m_fOriPosZ);
	DDX_Text(pDX, IDC_EDIT16, m_fOriScaleX);
	DDX_Text(pDX, IDC_EDIT17, m_fOriScaleY);
	DDX_Text(pDX, IDC_EDIT18, m_fOriScaleZ);
	DDX_Text(pDX, IDC_EDIT19, m_fOriRotX);
	DDX_Text(pDX, IDC_EDIT20, m_fOriRotY);
	DDX_Text(pDX, IDC_EDIT21, m_fOriRotZ);
	DDX_Text(pDX, IDC_EDIT4, m_fPosX);
	DDX_Text(pDX, IDC_EDIT5, m_fPosY);
	DDX_Text(pDX, IDC_EDIT6, m_fPosZ);
	DDX_Text(pDX, IDC_EDIT15, m_fScaleX);
	DDX_Text(pDX, IDC_EDIT22, m_fScaleY);
	DDX_Text(pDX, IDC_EDIT23, m_fScaleZ);
	DDX_Text(pDX, IDC_EDIT24, m_fRotX);
	DDX_Text(pDX, IDC_EDIT25, m_fRotY);
	DDX_Text(pDX, IDC_EDIT26, m_fRotZ);
	DDX_Control(pDX, IDC_CHECK1, m_Pass0);
	DDX_Control(pDX, IDC_CHECK2, m_Pass1);
	DDX_Control(pDX, IDC_CHECK3, m_Pass2);
	DDX_Text(pDX, IDC_EDIT12, m_fOriScaleStartTime);
	DDX_Text(pDX, IDC_EDIT34, m_fOriScaleEndTime);
	DDX_Text(pDX, IDC_EDIT35, m_fOriScaleXPat);
	DDX_Text(pDX, IDC_EDIT36, m_fOriScaleYPat);
	DDX_Text(pDX, IDC_EDIT37, m_fOriScaleZPat);
	DDX_Text(pDX, IDC_EDIT48, m_fOriScaleRepeat);
	DDX_Text(pDX, IDC_EDIT50, m_fScaleRepeat);
	DDX_Text(pDX, IDC_EDIT38, m_fScaleStartTIme);
	DDX_Text(pDX, IDC_EDIT39, m_fScaleEndTime);
	DDX_Text(pDX, IDC_EDIT40, m_fScaleXPat);
	DDX_Text(pDX, IDC_EDIT41, m_fScaleYPat);
	DDX_Text(pDX, IDC_EDIT42, m_fScaleZPat);
	DDX_Text(pDX, IDC_EDIT43, m_fOriRotStartTime);
	DDX_Text(pDX, IDC_EDIT44, m_fOriRotEndTime);
	DDX_Text(pDX, IDC_EDIT45, m_fOriRotXPat);
	DDX_Text(pDX, IDC_EDIT46, m_fOriRotYPat);
	DDX_Text(pDX, IDC_EDIT47, m_fOriRotZPat);
	DDX_Text(pDX, IDC_EDIT49, m_fOriRotRepeat);
	DDX_Text(pDX, IDC_EDIT51, m_fRotStartTime);
	DDX_Text(pDX, IDC_EDIT52, m_fRotEndTime);
	DDX_Text(pDX, IDC_EDIT53, m_fRotXPat);
	DDX_Text(pDX, IDC_EDIT54, m_fRotYPat);
	DDX_Text(pDX, IDC_EDIT55, m_fRotZPat);
	DDX_Text(pDX, IDC_EDIT56, m_fRotRepeat);
	DDX_Text(pDX, IDC_EDIT57, m_fAccTime);
	DDX_Control(pDX, IDC_LIST1, m_Temp);
}

void CEffectTab::UpdateEffectTool(const float& fTimeDelte)
{


	if (m_bIsStartEffect == true)
	{
		UpdateData(TRUE);
		m_fAccTime += fTimeDelte;
		UpdateData(FALSE);

		m_pEffectData->SetTime(m_fAccTime);
	}

}


BEGIN_MESSAGE_MAP(CEffectTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEffectTab::OnBnClickedRcTex)
	ON_BN_CLICKED(IDC_CHECK1, &CEffectTab::OnBnClickedPass0)
	ON_BN_CLICKED(IDC_CHECK2, &CEffectTab::OnBnClickedPass1)
	ON_BN_CLICKED(IDC_CHECK3, &CEffectTab::OnBnClickedPass2)
	ON_BN_CLICKED(IDC_BUTTON7, &CEffectTab::PosXUp)
	ON_BN_CLICKED(IDC_BUTTON24, &CEffectTab::PosXDown)
	ON_BN_CLICKED(IDC_BUTTON16, &CEffectTab::PosYUp)
	ON_BN_CLICKED(IDC_BUTTON25, &CEffectTab::PosYDown)
	ON_BN_CLICKED(IDC_BUTTON17, &CEffectTab::PosZUp)
	ON_BN_CLICKED(IDC_BUTTON26, &CEffectTab::PosZDown)
	ON_BN_CLICKED(IDC_BUTTON18, &CEffectTab::ScaleXUp)
	ON_BN_CLICKED(IDC_BUTTON27, &CEffectTab::ScaleXDown)
	ON_BN_CLICKED(IDC_BUTTON19, &CEffectTab::ScaleYUp)
	ON_BN_CLICKED(IDC_BUTTON28, &CEffectTab::ScaleYDown)
	ON_BN_CLICKED(IDC_BUTTON20, &CEffectTab::ScaleZUp)
	ON_BN_CLICKED(IDC_BUTTON29, &CEffectTab::ScaleZDown)
	ON_BN_CLICKED(IDC_BUTTON21, &CEffectTab::RotXUp)
	ON_BN_CLICKED(IDC_BUTTON30, &CEffectTab::RotXDown)
	ON_BN_CLICKED(IDC_BUTTON22, &CEffectTab::RotYUp)
	ON_BN_CLICKED(IDC_BUTTON31, &CEffectTab::RotYDown)
	ON_BN_CLICKED(IDC_BUTTON23, &CEffectTab::RotZUp)
	ON_BN_CLICKED(IDC_BUTTON32, &CEffectTab::RotZDown)
	ON_BN_CLICKED(IDC_BUTTON2, &CEffectTab::ClickInput)
	ON_BN_CLICKED(IDC_BUTTON15, &CEffectTab::ClickToOri)
	ON_BN_CLICKED(IDC_BUTTON33, &CEffectTab::OnBnClickedPatternInput)
	ON_BN_CLICKED(IDC_BUTTON34, &CEffectTab::PlayPattern)
	ON_BN_CLICKED(IDC_BUTTON35, &CEffectTab::ClickPatStop)
END_MESSAGE_MAP()


// EffectTab 메시지 처리기


void CEffectTab::OnBnClickedRcTex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CToolEffect* pDefaultEffect = CToolEffect::Create(CGraphicDev::GetInstance()->GetDevice());
	CObjMgr::GetInstance()->AddObject(pDefaultEffect, CObjMgr::OBJ_EFFECT);


	m_pEffectData = pDefaultEffect;
	InitEffect();

	UpdateData(TRUE);



	UpdateData(FALSE);



}


void CEffectTab::OnBnClickedPass0()
{

	if (m_pEffectData == nullptr)
	{
		m_Pass0.SetCheck(false);
		AfxMessageBox(L"RCTex 선택이나 만들고 눌러라");
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_Pass1.SetCheck(false);
	m_Pass2.SetCheck(false);

	LoadTexture(L"\\Resources\\Texture\\Default\\",CToolEffect::NONE);
	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedPass1()
{

	if (m_pEffectData == nullptr)
	{
		m_Pass1.SetCheck(false);
		AfxMessageBox(L"RCTex 선택이나 만들고 눌러라");
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_Pass0.SetCheck(false);
	m_Pass2.SetCheck(false);


	LoadTexture(L"\\Resources\\Texture\\AlphaTest\\", CToolEffect::ALPHATEST);


	UpdateData(FALSE);
}


void CEffectTab::OnBnClickedPass2()
{

	if (m_pEffectData == nullptr)
	{
		m_Pass2.SetCheck(false);
		AfxMessageBox(L"RCTex 선택이나 만들고 눌러라");
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_Pass0.SetCheck(false);
	m_Pass1.SetCheck(false);

	LoadTexture(L"\\Resources\\Texture\\AlphaBlend\\", CToolEffect::ALPHABLEND);

	UpdateData(FALSE);
}


BOOL CEffectTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_Pass0.SetCheck(false);
	m_Pass1.SetCheck(false);



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CEffectTab::LoadTexture(wstring strPath, CToolEffect::TEXTURE_STATE eState)
{

	CFileDialog Dlg(TRUE, L"", L"", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		L"", this);


	TCHAR szCurPath[256] = L"";

	// GetCurrentDirectory: 현재 작업 경로를 얻어오는 함수.
	GetCurrentDirectory(256, szCurPath);

	// PathRemoveFileSpec: 현재 경로 상에서 파일명을 제거하는 함수.
	// 제거해야할 파일명이 없을 경우에는 가장 말단 폴더명을 제거한다.
	PathRemoveFileSpec(szCurPath);


	 lstrcat(szCurPath, strPath.c_str());

	// 대화상자를 열 때 초기 경로를 설정한다.
	Dlg.m_ofn.lpstrInitialDir = szCurPath;




	if (IDOK == Dlg.DoModal())
	{
		wstring strFileName = Dlg.GetPathName();

		HANDLE hFile = CreateFile(strFileName.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);

		if (INVALID_HANDLE_VALUE == hFile)
			return;

		strFileName.pop_back();
		strFileName.pop_back();
		strFileName.pop_back();
		strFileName.pop_back();
	
		wstring temp =&strFileName.c_str()[strFileName.size()-1];
		int iIndex = 0;
		iIndex = _wtoi(temp.c_str());
		m_pEffectData->SetTexture(iIndex, eState);

	}


}

void CEffectTab::UpdateEffect()
{

	if (m_pEffectData == nullptr)
		return;

	CTransform* pTransForm = dynamic_cast<CTransform*>(m_pEffectData->Get_Component(L"Com_Transform",ID_DYNAMIC));

	pTransForm->m_vInfo[INFO_POS].x = m_fPosX;
	pTransForm->m_vInfo[INFO_POS].y = m_fPosY;
	pTransForm->m_vInfo[INFO_POS].z = m_fPosZ;


	pTransForm->m_vScale.x = m_fScaleX;
	pTransForm->m_vScale.y = m_fScaleY;
	pTransForm->m_vScale.z = m_fScaleZ;

	pTransForm->m_vAngle.x = m_fRotX;
	pTransForm->m_vAngle.y = m_fRotY;
	pTransForm->m_vAngle.z = m_fRotZ;


	m_pEffectData->m_tEffectData.vScalePat = _vec3(m_fOriScaleXPat, m_fOriScaleYPat, m_fOriScaleZPat);
	m_pEffectData->m_tEffectData.vRotPat = _vec3(m_fOriRotXPat,m_fOriRotYPat,m_fOriRotZPat);

	m_pEffectData->m_tEffectData.fStartRot = m_fOriRotStartTime;
	m_pEffectData->m_tEffectData.fEndRot = m_fOriRotEndTime;

	m_pEffectData->m_tEffectData.fStartScale = m_fOriScaleStartTime;
	m_pEffectData->m_tEffectData.fEndScale = m_fOriScaleEndTime;

	m_pEffectData->m_tEffectData.iRotRepeat = m_fOriRotRepeat;
	m_pEffectData->m_tEffectData.iScaleRepeat = m_fOriScaleRepeat;


}

void CEffectTab::InitEffect()
{
	if (m_pEffectData == nullptr)
		return;

	CTransform* pTransForm = dynamic_cast<CTransform*>(m_pEffectData->Get_Component(L"Com_Transform", ID_DYNAMIC));

	EFFECTDATA pData = m_pEffectData->m_tEffectData;

	UpdateData(TRUE);

	 m_fPosX = pTransForm->m_vInfo[INFO_POS].x;
	 m_fPosY = pTransForm->m_vInfo[INFO_POS].y;
	 m_fPosZ = pTransForm->m_vInfo[INFO_POS].z;

	 m_fScaleX = pTransForm->m_vScale.x;
	 m_fScaleY = pTransForm->m_vScale.y;
	 m_fScaleZ = pTransForm->m_vScale.z;


	 m_fRotX = pTransForm->m_vAngle.x;
	 m_fRotY = pTransForm->m_vAngle.y;
	 m_fRotZ = pTransForm->m_vAngle.z;


	 m_fOriPosX = m_fPosX;
	 m_fOriPosY = m_fPosY;
	 m_fOriPosZ = m_fPosZ;

	 m_fOriRotX = m_fRotX;
	 m_fOriRotY = m_fRotY;
	 m_fOriRotZ = m_fRotZ;

	 m_fOriScaleX = m_fScaleX;
	 m_fOriScaleY = m_fScaleY;
	 m_fOriScaleZ = m_fScaleZ;

	 
	 m_fRotXPat = m_pEffectData->m_tEffectData.vRotPat.x;
	 m_fRotYPat = m_pEffectData->m_tEffectData.vRotPat.y;
	 m_fRotZPat = m_pEffectData->m_tEffectData.vRotPat.z;

	 m_fScaleXPat = m_pEffectData->m_tEffectData.vScalePat.x;
	 m_fScaleYPat = m_pEffectData->m_tEffectData.vScalePat.y;
	 m_fScaleZPat = m_pEffectData->m_tEffectData.vScalePat.z;

	 m_fRotStartTime = m_pEffectData->m_tEffectData.fStartRot;
	 m_fRotEndTime = m_pEffectData->m_tEffectData.fEndRot;
	 m_fScaleStartTIme = m_pEffectData->m_tEffectData.fStartScale;
	 m_fScaleEndTime = m_pEffectData->m_tEffectData.fEndScale;




	 m_fOriScaleXPat = m_fScaleXPat;
	 m_fOriScaleYPat = m_fScaleYPat;
	 m_fOriScaleZPat = m_fScaleZPat;
	 m_fOriRotXPat = m_fRotXPat;
	 m_fOriRotYPat = m_fRotYPat;
	 m_fOriRotZPat = m_fRotZPat;


	 m_fOriScaleStartTime = m_fScaleStartTIme;
	 m_fOriScaleEndTime = m_fScaleEndTime;

	 m_fOriRotStartTime = m_fRotStartTime;
	 m_fOriRotEndTime = m_fRotEndTime;


	 UpdateData(FALSE);

}





void CEffectTab::PosXUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fPosX += 0.1f;

	UpdateData(FALSE);
	UpdateEffect();

}


void CEffectTab::PosXDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fPosX -= 0.1f;

	UpdateData(FALSE);

	UpdateEffect();
}


void CEffectTab::PosYUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fPosY += 0.1f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::PosYDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	UpdateData(TRUE);

	m_fPosY -= 0.1f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::PosZUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fPosZ += 0.1f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::PosZDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fPosZ -= 0.1f;

	UpdateData(FALSE);
	UpdateEffect();

}


void CEffectTab::ScaleXUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fScaleX += 0.01f;

	UpdateData(FALSE);
	UpdateEffect();


}


void CEffectTab::ScaleXDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fScaleX -= 0.01f;

	UpdateData(FALSE);
	UpdateEffect();


}


void CEffectTab::ScaleYUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	UpdateData(TRUE);

	m_fScaleY += 0.01f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::ScaleYDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fScaleY -= 0.01f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::ScaleZUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fScaleZ += 0.01f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::ScaleZDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fScaleZ -= 0.01f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::RotXUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fRotX += 0.01f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::RotXDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fRotX -= 0.01f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::RotYUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	UpdateData(TRUE);

	m_fRotY += 0.01f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::RotYDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fRotY -= 0.01f;

	UpdateData(FALSE);
	UpdateEffect();

}


void CEffectTab::RotZUp()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	UpdateData(TRUE);

	m_fRotZ += 0.01f;

	UpdateData(FALSE);

	UpdateEffect();

}


void CEffectTab::RotZDown()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fRotZ -= 0.01f;

	UpdateData(FALSE);
	UpdateEffect();
}


void CEffectTab::ClickInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_fOriPosX = m_fPosX;
	m_fOriPosY = m_fPosY;
	m_fOriPosZ = m_fPosZ;

	m_fOriRotX = m_fRotX;
	m_fOriRotY = m_fRotY;
	m_fOriRotZ = m_fRotZ;

	m_fOriScaleX = m_fScaleX;
	m_fOriScaleY = m_fScaleY;
	m_fOriScaleZ = m_fScaleZ;

	UpdateEffect();

	UpdateData(FALSE);

}


void CEffectTab::ClickToOri()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fPosX = m_fOriPosX;
	m_fPosY = m_fOriPosY;
	m_fPosZ = m_fOriPosZ;
	

	m_fRotX = m_fOriRotX;
	m_fRotY = m_fOriRotY;
	m_fRotZ = m_fOriRotZ;


	m_fScaleX = m_fOriScaleX;
	m_fScaleY = m_fOriScaleY;

	m_fScaleZ = m_fOriScaleZ;


	UpdateData(FALSE);

	UpdateEffect();


}




void CEffectTab::OnBnClickedPatternInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	m_fOriScaleXPat = m_fScaleXPat;
	m_fOriScaleYPat = m_fScaleYPat;
	m_fOriScaleZPat = m_fScaleZPat;
	m_fOriRotXPat = m_fRotXPat;
	m_fOriRotYPat = m_fRotYPat;
	m_fOriRotZPat = m_fRotZPat;


	m_fOriScaleStartTime = m_fScaleStartTIme;
	m_fOriScaleEndTime = m_fScaleEndTime;

	m_fOriRotStartTime = m_fRotStartTime;
	m_fOriRotEndTime = m_fRotEndTime;


	UpdateEffect();

	UpdateData(FALSE);
}


void CEffectTab::PlayPattern()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	m_bIsStartEffect = true;
	m_fAccTime = 0.f;
	if (m_pEffectData != nullptr)
	{
		m_pEffectData->SetCheck(true);
	}



	UpdateData(FALSE);
}


void CEffectTab::ClickPatStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	UpdateData(TRUE);

	m_bIsStartEffect = false;
	m_fAccTime = 0.f;

	if (m_pEffectData != nullptr)
	{
		m_pEffectData->SetCheck(false);
	}


	UpdateData(FALSE);
}
