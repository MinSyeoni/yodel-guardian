// EffectTab2.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "EffectTab2.h"
#include "afxdialogex.h"


// EffectTab2 대화 상자

IMPLEMENT_DYNAMIC(EffectTab2, CDialogEx)

EffectTab2::EffectTab2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_Effect2, pParent)
	, m_fOriFadeInStart(0)
	, m_fFadeInStart(0)
	, m_fOriFadeInEnd(0)
	, m_fFadeinEnd(0)
	, m_fOriFadeOutStart(0)
	, m_fFadeOutStart(0)
	, m_fOriFadeOutEnd(0)
	, m_fFadeOutEnd(0)
	, m_fAccTime(0)
	, m_fOriWidth(0)
	, m_fWidth(0)
	, m_fOriHeight(0)
	, m_fHeight(0)
	, m_fOriUvRepeat(0)
	, m_fUvRepeat(0)
	, m_fOriUvSpeed(0)
	, m_fUvSpeed(0)
{

}

EffectTab2::~EffectTab2()
{
}

void EffectTab2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_fOriFadeInStart);
	DDX_Text(pDX, IDC_EDIT17, m_fFadeInStart);
	DDX_Text(pDX, IDC_EDIT2, m_fOriFadeInEnd);
	DDX_Text(pDX, IDC_EDIT16, m_fFadeinEnd);
	DDX_Control(pDX, IDC_CHECK1, m_FadeInButton);
	DDX_Control(pDX, IDC_CHECK2, m_FadeOutButton);
	DDX_Text(pDX, IDC_EDIT18, m_fOriFadeOutStart);
	DDX_Text(pDX, IDC_EDIT58, m_fFadeOutStart);
	DDX_Text(pDX, IDC_EDIT19, m_fOriFadeOutEnd);
	DDX_Text(pDX, IDC_EDIT20, m_fFadeOutEnd);
	DDX_Text(pDX, IDC_EDIT3, m_fAccTime);
	DDX_Text(pDX, IDC_EDIT60, m_fOriWidth);
	DDX_Text(pDX, IDC_EDIT65, m_fWidth);
	DDX_Text(pDX, IDC_EDIT61, m_fOriHeight);
	DDX_Text(pDX, IDC_EDIT66, m_fHeight);
	DDX_Text(pDX, IDC_EDIT62, m_fOriUvRepeat);
	DDX_Text(pDX, IDC_EDIT67, m_fUvRepeat);
	DDX_Control(pDX, IDC_CHECK5, m_UvSpriteButton);
	DDX_Text(pDX, IDC_EDIT63, m_fOriUvSpeed);
	DDX_Text(pDX, IDC_EDIT76, m_fUvSpeed);
}

void EffectTab2::FindEffectData()
{
	for (auto& pSRC : CObjMgr::GetInstance()->m_ObjLst[CObjMgr::OBJ_EFFECT])
	{	
		if (dynamic_cast<CToolEffect*>(pSRC)->m_bIsCheck == true)
			m_pEffectData = dynamic_cast<CToolEffect*>(pSRC);
	}

	if (m_pEffectData == nullptr)
		return;



	UpdateData(TRUE);

	m_fFadeInStart = m_pEffectData->m_tEffectData.fFadeInStartTime;

	m_fFadeinEnd = m_pEffectData->m_tEffectData.fFadeInEndTime;

	m_fFadeOutStart = m_pEffectData->m_tEffectData.fFadeOutStartTime;

	m_fFadeOutEnd = m_pEffectData->m_tEffectData.fFadeOutEndTime;

	m_fOriFadeInStart = m_fFadeInStart;
	m_fOriFadeInEnd = m_fFadeinEnd;

	m_fOriFadeOutStart = m_fFadeOutStart;
	m_fOriFadeOutEnd = m_fFadeOutEnd;

	m_FadeOutButton.SetCheck(m_pEffectData->m_tEffectData.bIsFadeOut);

	m_FadeInButton.SetCheck(m_pEffectData->m_tEffectData.bIsFadeIn);

	m_fHeight = m_pEffectData->m_tEffectData.iUvHeight;
	m_fWidth = m_pEffectData->m_tEffectData.iUvWidth;



	m_UvSpriteButton.SetCheck(m_pEffectData->m_tEffectData.bIsUvSprite);

	m_fOriWidth = m_fWidth;
	m_fOriHeight = m_fHeight;






	UpdateData(FALSE);


}

void EffectTab2::UpdateEffectTool2(const float& fTimeDelta)
{
	if (m_bIsPlayAni == true)
	{
		UpdateData(TRUE);
		m_fAccTime += fTimeDelta;
		UpdateData(FALSE);

		if (m_pEffectData != nullptr)
			m_pEffectData->m_fAccTime = m_fAccTime;

	}
	else
	{
		UpdateData(TRUE);
		m_fAccTime = 0.f;
		if (m_pEffectData != nullptr)
			m_pEffectData->SetCheck(false);
		UpdateData(FALSE);
	}

}


BEGIN_MESSAGE_MAP(EffectTab2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &EffectTab2::ClickInput)
	ON_BN_CLICKED(IDC_BUTTON3, &EffectTab2::PlayEffect)
	ON_BN_CLICKED(IDC_BUTTON4, &EffectTab2::ClickStop)
END_MESSAGE_MAP()


// EffectTab2 메시지 처리기


void EffectTab2::ClickInput()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);

	if (m_pEffectData == nullptr)
	return;


	m_pEffectData->m_tEffectData.bIsFadeIn = m_FadeInButton.GetCheck();
	m_pEffectData->m_tEffectData.bIsFadeOut = m_FadeOutButton.GetCheck();


	m_pEffectData->m_tEffectData.fFadeInStartTime = m_fFadeInStart;
	m_pEffectData->m_tEffectData.fFadeInEndTime = m_fFadeinEnd;


	m_pEffectData->m_tEffectData.fFadeOutStartTime = m_fFadeOutStart;
	m_pEffectData->m_tEffectData.fFadeOutEndTime= m_fFadeOutEnd;


	m_pEffectData->m_tEffectData.iUvWidth = m_fWidth;
	m_pEffectData->m_tEffectData.iUvHeight = m_fHeight;
	m_pEffectData->m_tEffectData.bIsUvSprite = m_UvSpriteButton.GetCheck();
	m_pEffectData->m_tEffectData.fSpriteSpeed = m_fUvSpeed;

	m_fOriFadeInEnd = m_fFadeinEnd;
	m_fOriFadeInStart = m_fFadeInStart;


	m_fOriFadeOutStart = m_fFadeOutStart;
	m_fOriFadeOutEnd = m_fFadeOutEnd;


	m_fOriWidth = m_fWidth;
	m_fOriHeight = m_fHeight;

	m_fOriUvSpeed = m_fUvSpeed;


	UpdateData(FALSE);
}


void EffectTab2::PlayEffect()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_bIsPlayAni = true;
	if (m_pEffectData != nullptr)
		m_pEffectData->m_bIsPlayPattern = true;


}


void EffectTab2::ClickStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_bIsPlayAni = false;
}
