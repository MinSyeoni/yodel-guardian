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
}


BEGIN_MESSAGE_MAP(CEffectTab, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CEffectTab::OnBnClickedRcTex)
END_MESSAGE_MAP()


// EffectTab 메시지 처리기


void CEffectTab::OnBnClickedRcTex()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CToolEffect* pDefaultEffect = CToolEffect::Create(CGraphicDev::GetInstance()->GetDevice());
	CObjMgr::GetInstance()->AddObject(pDefaultEffect, CObjMgr::OBJ_EFFECT);


}
