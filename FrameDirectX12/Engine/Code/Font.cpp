#include "Font.h"
#include "GraphicDevice.h"

USING(Engine)

CFont::CFont(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	: CGameObject(pGraphicDevice, pCommandList)
{
}

CFont::CFont(const CFont & rhs)
	: CGameObject(rhs)
	, m_bIsClone(true)
	, m_pD2D_Context(rhs.m_pD2D_Context)
	, m_pDWriteFactory(rhs.m_pDWriteFactory)
	, m_pTextBrush(rhs.m_pTextBrush)
	, m_pTextFormat(rhs.m_pTextFormat)
	, m_fFontSize(rhs.m_fFontSize)
	, m_vPosOffset(rhs.m_vPosOffset)
{
	if (nullptr != m_pTextBrush)
		m_pTextBrush->AddRef();

	if (nullptr != m_pTextFormat)
		m_pTextFormat->AddRef();
}

HRESULT CFont::Ready_GameObjectPrototype(wstring wstrFontName,
	const _float & fSize,
	const D2D1::ColorF & Color)
{
	m_pD2D_Context = CGraphicDevice::Get_Instance()->Get_D2DContext();
	NULL_CHECK_RETURN(m_pD2D_Context, E_FAIL);

	m_pDWriteFactory = CGraphicDevice::Get_Instance()->Get_DWriteFactory();
	NULL_CHECK_RETURN(m_pDWriteFactory, E_FAIL);

	m_fFontSize = fSize;

	/*__________________________________________________________________________________________________________
	[ 기본 D2D 텍스트 개체 만들기 ]
	- 이제 3D 콘텐츠를 렌더링할 ID3D12Device, 즉 ID3D11On12Device를 통해 12 디바이스와 공유되는
	ID2D1Device(2D 콘텐츠 렌더링에 사용할 수 있음)가 있으며 둘 다 동일한 스왑 체인으로 렌더링하도록 구성됩니다.

	- 이 샘플은 게임이 해당 UI를 렌더링하는 것과 비슷한 방식으로, 간단히 D2D 디바이스를 사용하여 3D 장면 위로 텍스트를 렌더링합니다.
	- 이를 위해 아직 LoadAssets 메서드에서 몇 가지 기본적인 D2D 개체를 만들어야 합니다.
	____________________________________________________________________________________________________________*/
	FAILED_CHECK_RETURN(m_pD2D_Context->CreateSolidColorBrush(Color, &m_pTextBrush), E_FAIL);
	FAILED_CHECK_RETURN(m_pDWriteFactory->CreateTextFormat(wstrFontName.c_str(),      // Font Name
		nullptr,                  // Font Collection
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		fSize,                  // Font Size
		L"ko-KR",               // Local Name
		&m_pTextFormat), E_FAIL);

	// 좌상단을 기준으로 텍스트 정렬.
	FAILED_CHECK_RETURN(m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR), E_FAIL);

	return S_OK;
}

HRESULT CFont::Ready_GameObjectClone(wstring wstrText,
	const _vec2& vPos,
	const D2D1::ColorF& Color)
{
	// Text 문자열 & 위치 & 색상.
	m_wstrText = wstrText;
	m_wstrPreText = m_wstrText;
	m_vPos = vPos;
	m_pTextBrush->SetColor(Color);

	// Font가 Rendering될 영역.
	m_TextRect.left = m_vPos.x;
	m_TextRect.top = m_vPos.y;

	/*__________________________________________________________________________________________________________
	[ Offset 계산 ]
	____________________________________________________________________________________________________________*/
	_tchar szTemp[MAX_STR] = L"";
	lstrcpy(szTemp, m_wstrText.c_str());

	_uint         iCurIdx = 0;
	vector<_vec2>   vecPosOffset;
	vecPosOffset.push_back(_vec2(0.f, 0.f));

	for (_int i = 0; szTemp[i] != '\0'; ++i)
	{
		_tchar ch = szTemp[i];

		// 개행 문자 - TextRect의 Height 증가.
		if (ch == '\n')
		{
			vecPosOffset[0].y += m_fFontSize;

			vecPosOffset.push_back(_vec2(0.f, 0.f));
			++iCurIdx;
		}
		// 공백 문자 - TextRect의 Height 증가.
		else if (ch == ' ')
			vecPosOffset[iCurIdx].x += m_fFontSize * 0.33f;

		// Tap 문자 - TextRect의 Height 증가.
		else if (ch == '\t')
			vecPosOffset[iCurIdx].x += m_fFontSize * 3.5f;

		// 알파벳 대문자 & 소문자 - TextRect의 Width 증가.
		else if ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122))
			vecPosOffset[iCurIdx].x += m_fFontSize * 0.85f;

		// 특수 문자 - TextRect의 Width 증가.
		else if ((ch >= 33 && ch <= 64) || (ch >= 91 && ch <= 96) || (ch >= 123 && ch <= 126))
			vecPosOffset[iCurIdx].x += (m_fFontSize * 0.5f);

		// 한글 문자 - TextRect의 Width 증가.
		else
			vecPosOffset[iCurIdx].x += m_fFontSize;
	}

	/*__________________________________________________________________________________________________________
	[ Offset 중에서 가장 큰 X 값을 찾는다. ]
	____________________________________________________________________________________________________________*/
	for (_uint i = 0; i < vecPosOffset.size(); ++i)
	{
		if (m_vPosOffset.x <= vecPosOffset[i].x)
			m_vPosOffset.x = vecPosOffset[i].x;
	}

	m_vPosOffset.y = vecPosOffset[0].y;

	vecPosOffset.clear();
	vecPosOffset.shrink_to_fit();

	/*__________________________________________________________________________________________________________
	[ 최종 FontRect의 크기 ]
	____________________________________________________________________________________________________________*/
	m_TextRect.right = m_vPos.x + m_vPosOffset.x;
	m_TextRect.bottom = m_vPos.y + m_vPosOffset.y;

	return S_OK;
}

HRESULT CFont::Ready_GameObject()
{
	return S_OK;
}

HRESULT CFont::LateInit_GameObject()
{


	return S_OK;
}

_int CFont::Update_GameObject(const _float & fTimeDelta)
{
	FAILED_CHECK_RETURN(CGameObject::LateInit_GameObject(), E_FAIL);

	/*__________________________________________________________________________________________________________
	[ Change Text - Change Rect Offset ]
	____________________________________________________________________________________________________________*/
	Calc_RectOffset();

	/*__________________________________________________________________________________________________________
	[ Update TextRect ]
	____________________________________________________________________________________________________________*/
	m_TextRect.left = m_vPos.x;
	m_TextRect.top = m_vPos.y;
	m_TextRect.right = m_vPos.x + m_vPosOffset.x;
	m_TextRect.bottom = m_vPos.y + m_vPosOffset.y;

	/*__________________________________________________________________________________________________________
	[ Renderer - Add Render Group ]
	____________________________________________________________________________________________________________*/
	NULL_CHECK_RETURN(m_pRenderer, -1)
		FAILED_CHECK_RETURN(m_pRenderer->Add_Renderer(CRenderer::RENDER_FONT, this), -1);

	return 1;
}

_int CFont::LateUpdate_GameObject(const _float & fTimeDelta)
{
	return 1;
}

void CFont::Render_GameObject(const _float & fTimeDelta)
{
	if (nullptr != m_pD2D_Context)
	{
		m_pD2D_Context->DrawTextW(m_wstrText.c_str(),         // 그릴 문자열.
			lstrlen(m_wstrText.c_str()),   // 그릴 문자열의 길이.
			m_pTextFormat,            // 문자열 텍스트 포멧.
			&m_TextRect,               // 그릴 영역.
			m_pTextBrush);            // 브러쉬.
	}

}

HRESULT CFont::Calc_RectOffset()
{
	/*__________________________________________________________________________________________________________
	- 출력 도중에 문자열의 바뀔 경우, Rect Offset을 다시 지정.
	____________________________________________________________________________________________________________*/
	if (m_wstrPreText != m_wstrText)
	{
		m_wstrPreText = m_wstrText;

		_tchar szTemp[MAX_STR] = L"";
		lstrcpy(szTemp, m_wstrText.c_str());

		_uint         iCurIdx = 0;
		vector<_vec2>   vecPosOffset;
		vecPosOffset.push_back(_vec2(0.f, 0.f));

		for (_int i = 0; szTemp[i] != '\0'; ++i)
		{
			_tchar ch = szTemp[i];

			// 개행 문자 - TextRect의 Height 증가.
			if (ch == '\n')
			{
				vecPosOffset[0].y += m_fFontSize;

				vecPosOffset.push_back(_vec2(0.f, 0.f));
				++iCurIdx;
			}
			// 공백 문자 - TextRect의 Height 증가.
			else if (ch == ' ')
				vecPosOffset[iCurIdx].x += m_fFontSize * 0.33f;

			// Tap 문자 - TextRect의 Height 증가.
			else if (ch == '\t')
				vecPosOffset[iCurIdx].x += m_fFontSize * 3.5f;

			// 알파벳 대문자 & 소문자 - TextRect의 Width 증가.
			else if ((ch >= 65 && ch <= 90) || (ch >= 97 && ch <= 122))
				vecPosOffset[iCurIdx].x += m_fFontSize * 0.85f;

			// 특수 문자 - TextRect의 Width 증가.
			else if ((ch >= 33 && ch <= 64) || (ch >= 91 && ch <= 96) || (ch >= 123 && ch <= 126))
				vecPosOffset[iCurIdx].x += (m_fFontSize * 0.5f);

			// 한글 문자 - TextRect의 Width 증가.
			else
				vecPosOffset[iCurIdx].x += m_fFontSize;
		}

		/*__________________________________________________________________________________________________________
		[ Offset 중에서 가장 큰 X 값을 찾는다. ]
		____________________________________________________________________________________________________________*/
		for (_uint i = 0; i < vecPosOffset.size(); ++i)
		{
			if (m_vPosOffset.x <= vecPosOffset[i].x)
				m_vPosOffset.x = vecPosOffset[i].x;
		}

		m_vPosOffset.y = vecPosOffset[0].y;

		vecPosOffset.clear();
		vecPosOffset.shrink_to_fit();

		/*__________________________________________________________________________________________________________
		[ 최종 FontRect의 크기 ]
		____________________________________________________________________________________________________________*/
		m_TextRect.right = m_vPos.x + m_vPosOffset.x;
		m_TextRect.bottom = m_vPos.y + m_vPosOffset.y;
	}


	return S_OK;
}

CGameObject * CFont::Clone_GameObject(void *pArg)
{
	return new CFont(*this);
}

CFont * CFont::Create_Prototype(ID3D12Device * pGraphicDevice,
	ID3D12GraphicsCommandList * pCommandList,
	wstring wstrFontName,
	const _float & fSize,
	const D2D1::ColorF & Color)
{
	CFont* pInstance = new CFont(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype(wstrFontName, fSize, Color)))
		Safe_Release(pInstance);

	return pInstance;
}

void CFont::Free()
{
	CGameObject::Free();

	Safe_Release(m_pTextBrush);
	Safe_Release(m_pTextFormat);

}