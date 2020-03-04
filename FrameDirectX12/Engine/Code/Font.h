#pragma once
#include "Engine_Include.h"
#include "GameObject.h"

/*__________________________________________________________________________________________________________
[ 한 개의 Font를 관리하는 클래스 ]
____________________________________________________________________________________________________________*/

BEGIN(Engine)

class ENGINE_DLL CFont : public CGameObject
{
private:
	explicit CFont(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CFont(const CFont& rhs);
	virtual ~CFont() = default;

public:
	void         Set_Text(wstring wstrText) { m_wstrText = wstrText; }
	void         Set_Pos(const _vec2& vPos) { m_vPos = vPos; }
	void         Set_Color(const D2D1::ColorF& Color) { m_pTextBrush->SetColor(Color); }
public:
	HRESULT         Ready_GameObjectPrototype(wstring wstrFontName,
		const _float& fSize,
		const D2D1::ColorF& Color);
	HRESULT         Ready_GameObjectClone(wstring wstrText,
		const _vec2& vPos,
		const D2D1::ColorF& Color = D2D1::ColorF::White);
public:
	virtual HRESULT   Ready_GameObject();
	virtual HRESULT   LateInit_GameObject();
	virtual _int   Update_GameObject(const _float& fTimeDelta);
	virtual _int   LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void   Render_GameObject(const _float& fTimeDelta);
private:
	HRESULT         Calc_RectOffset();
private:
	/*__________________________________________________________________________________________________________
	[ DirectX 11 Interface ]
	____________________________________________________________________________________________________________*/
	ID2D1DeviceContext2*   m_pD2D_Context = nullptr;
	IDWriteFactory2*      m_pDWriteFactory = nullptr;

	/*__________________________________________________________________________________________________________
	[ Font Value ]
	____________________________________________________________________________________________________________*/
	ID2D1SolidColorBrush*   m_pTextBrush = nullptr;
	IDWriteTextFormat*      m_pTextFormat = nullptr;

	wstring               m_wstrText;
	wstring               m_wstrPreText;
	D2D1_RECT_F            m_TextRect;

	_vec2               m_vPos = _vec2(0.f, 0.f);
	_float               m_fFontSize = 0.f;
	_vec2               m_vPosOffset = _vec2(0.f, 0.f);

	_bool               m_bIsClone = false;

public:
	virtual CGameObject*   Clone_GameObject(void * parg);
	static CFont*         Create_Prototype(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList,
		wstring wstrFontName,
		const _float& fSize,
		const D2D1::ColorF& Color);
private:
	virtual void         Free();
};

END