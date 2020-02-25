#ifndef Export_System_h__
#define Export_System_h__

#include "Engine_Defines.h"

#include "GraphicDev.h"
#include "InputDev.h"
#include "TimerMgr.h"
#include "FrameMgr.h"
#include "FontMgr.h"

BEGIN(Engine)

// GraphicDev
// Get
// Set
// General
inline HRESULT		Ready_GraphicDev(HWND hWnd,
	Engine::CGraphicDev::WINMODE eMode,
	const _uint& iSizeX,
	const _uint& iSizeY,
	Engine::CGraphicDev** ppGraphicDev);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// TimeMgr
// Get
inline _float		Get_TimeDelta(const _tchar* pTimerTag);
// Set
// General
inline HRESULT Add_Timer(const _tchar* pTimerTag);
inline _float Compute_Timer(const _tchar* pTimerTag);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FrameMgr
// Get
inline _bool	IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta);
// Set
// General
inline HRESULT	Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// FontMgr
// Get
// Set
// General
inline HRESULT		Ready_Font(LPDIRECT3DDEVICE9 pGraphicDev,
	const _tchar* pFontTag,
	const _tchar* pFontType,
	const _uint& iWidth,
	const _uint& iHeight,
	const _uint& iWeight);

inline void		Render_Font(const _tchar* pFontTag,
	const _tchar* pString,
	const _vec2* pPos,
	D3DXCOLOR Color);


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// InputDev
// Get
inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _bool	Get_DIKeyUp(_ubyte byKeyID);
inline _bool	Get_DIKeyDown(_ubyte byKeyID);
inline _bool	Get_DIKeyPressing(_ubyte byKeyID);
inline _bool	Get_DIKeyCombined(_ubyte byFirst, _ubyte bySecond);

inline _byte	Get_DIKeyState(_ubyte byKeyID);
inline _byte	Get_DIMouseState(CInputDev::MOUSEKEYSTATE eMouseBtID);
inline _long	Get_DIMouseMove(CInputDev::MOUSEMOVESTATE eMouseMoveID);

// Set
// General
inline HRESULT Ready_Input_Device(HINSTANCE hInst, HWND hWnd);
inline HRESULT Inquire_Input_State();




////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Destroy
inline void		DestroySystem(void);

#include "Export_System.inl"

END
#endif // Export_System_h__
