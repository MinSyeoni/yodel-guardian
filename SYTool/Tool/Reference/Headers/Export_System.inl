// GraphicDev
// Get
// Set
// General
HRESULT		Ready_GraphicDev(HWND hWnd,
	Engine::CGraphicDev::WINMODE eMode,
	const _uint& iSizeX,
	const _uint& iSizeY,
	Engine::CGraphicDev** ppGraphicDev)
{
	return CGraphicDev::GetInstance()->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppGraphicDev);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeMgr
// Get
_float		Get_TimeDelta(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Get_TimeDelta(pTimerTag);
}
// Set
// General
HRESULT Add_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Add_Timer(pTimerTag);
}
_float Compute_Timer(const _tchar* pTimerTag)
{
	return CTimerMgr::GetInstance()->Compute_Timer(pTimerTag);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FrameMgr
// Get
_bool	IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	return CFrameMgr::GetInstance()->IsPermit_Call(pFrameTag, fTimeDelta);
}
// Set
// General
HRESULT	Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	return CFrameMgr::GetInstance()->Ready_Frame(pFrameTag, fCallLimit);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FontMgr
// Get
// Set
// General
HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFontTag, const _tchar* pFontType, const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
	return CFontMgr::GetInstance()->Ready_Font(pGraphicDev, pFontTag, pFontType, iWidth, iHeight, iWeight);
}
void		Render_Font(const _tchar* pFontTag, const _tchar* pString, const _vec2* pPos, D3DXCOLOR Color)
{
	CFontMgr::GetInstance()->Render_Font(pFontTag, pString, pPos, Color);
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InputDev
// Get
_bool	Get_DIKeyUp(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyUp(byKeyID);
}
_bool	Get_DIKeyDown(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyDown(byKeyID);
}
_bool	Get_DIKeyPressing(_ubyte byKeyID)
{
	return CInputDev::GetInstance()->Get_DIKeyPressing(byKeyID);
}
_bool	Get_DIKeyCombined(_ubyte byFirst, _ubyte bySecond)
{
	return CInputDev::GetInstance()->Get_DIKeyCombined(byFirst, bySecond);
}
_byte Get_DIKeyState(_ubyte byKeyID)
{
	return  CInputDev::GetInstance()->Get_DIKeyState(byKeyID);
}
_byte Get_DIMouseState(CInputDev::MOUSEKEYSTATE eMouseBtID)
{
	return CInputDev::GetInstance()->Get_DIMouseState(eMouseBtID);
}
_long Get_DIMouseMove(CInputDev::MOUSEMOVESTATE eMouseMoveID)
{
	return  CInputDev::GetInstance()->Get_DIMouseMove(eMouseMoveID);
}

// Set
// General
HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd)
{
	return CInputDev::GetInstance()->Ready_InputDev(hInst, hWnd);
}

HRESULT Inquire_Input_State()
{
	return CInputDev::GetInstance()->Inquire_Input_State();
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destroy
void		DestroySystem(void)
{
	CInputDev::GetInstance()->DestroyInstance();
	CFontMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CTimerMgr::GetInstance()->DestroyInstance();
}
