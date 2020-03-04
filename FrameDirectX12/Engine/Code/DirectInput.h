#ifndef DirectInput_h__
#define DirectInput_h__


#include "Engine_Include.h"
#include "Base.h"

#define MAX_VKEY 256

BEGIN(Engine)

class ENGINE_DLL CDirectInput : public CBase
{
	DECLARE_SINGLETON(CDirectInput)
public:
	enum MOUSEMOVE { DIMM_X, DIMM_Y, DIMM_H, DIMM_END };
private:
	explicit CDirectInput(void);
	virtual ~CDirectInput(void);

public:
	_byte	Get_DIKeyState(_ubyte byKeyID)					{ return m_byKeyState[byKeyID]; }
	_byte	Get_DIMouseState(MOUSEBUTTON eMouseID)			{ return m_MouseState.rgbButtons[eMouseID]; }
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseMoveState) { return *(((_ulong*)&m_MouseState) + eMouseMoveState); }

public:
	HRESULT	Ready_InputDevice(HINSTANCE hInst, HWND hWnd);
	void	SetUp_InputState();

public:
	bool Key_Pressing(const byte& byKey);
	bool Key_Up(const byte& byKey);
	bool Key_Down(const byte& byKey);
	bool Key_Combine(const byte& byFirstKey, const byte& bySecondKey);
	bool Key_CombineUp(const byte& byFirstKey, const byte& bySecondKey);
	bool Key_CombineDown(const byte& byFirstKey, const byte& bySecondKey);

	bool Mouse_KeyPressing(const MOUSEBUTTON& eMouseKey);
	bool Mouse_KeyUp(const MOUSEBUTTON& eMouseKey);
	bool Mouse_KeyDown(const MOUSEBUTTON& eMouseKey);

	_long Get_DIMouseMove(MOUSEMOVE eMouseMoveID)
	{
		return *((_long*)&m_MouseState + eMouseMoveID);
	}


private:
	LPDIRECTINPUT8			m_pInputSDK = nullptr;
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = nullptr;
	LPDIRECTINPUTDEVICE8	m_pMouse = nullptr;

	// KeyBoard
	byte			m_byKeyState[256];
	byte			m_byKeyPressed[256];
	byte			m_byKeyPressedTemp[256];
	byte			m_byKeyDown[256];
	byte			m_byKeyDownTemp[256];

	// Mouse
	DIMOUSESTATE	m_MouseState;
	DIMOUSESTATE	m_MousePressed;
	DIMOUSESTATE	m_MousePressedTemp;
	DIMOUSESTATE	m_MouseDown;
	DIMOUSESTATE	m_MouseDownTemp;

public:
	virtual void Free();
};

END

#endif // DirectInput_h__
