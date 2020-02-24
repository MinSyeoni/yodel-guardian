
#include "KeyMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CKeyMgr)

Engine::CKeyMgr::CKeyMgr()
	: m_dwCurKey(0), m_dwKeyPressed(0), m_dwKeyDown(0)
{

}

Engine::CKeyMgr::~CKeyMgr()
{
}

// 매 프레임마다 호출
void Engine::CKeyMgr::KeyCheck()
{
	m_dwCurKey = 0;

	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
		m_dwCurKey |= KEY_LBUTTON;
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
		m_dwCurKey |= KEY_RBUTTON;
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_dwCurKey |= KEY_LEFT;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_dwCurKey |= KEY_RIGHT;
	if (GetAsyncKeyState(VK_UP) & 0x8000)
		m_dwCurKey |= KEY_UP;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		m_dwCurKey |= KEY_DOWN;
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		m_dwCurKey |= KEY_SPACE;
	if (GetAsyncKeyState('Q') & 0x8000)
		m_dwCurKey |= KEY_Q;
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_dwCurKey |= KEY_RETURN;
	if (GetAsyncKeyState('R') & 0x8000)
		m_dwCurKey |= KEY_R;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurKey |= KEY_W;
	if (GetAsyncKeyState('1') & 0x8000)
		m_dwCurKey |= KEY_1;
	if (GetAsyncKeyState('2') & 0x8000)
		m_dwCurKey |= KEY_2;
	if (GetAsyncKeyState('3') & 0x8000)
		m_dwCurKey |= KEY_3;
	if (GetAsyncKeyState('4') & 0x8000)
		m_dwCurKey |= KEY_4;
	if (GetAsyncKeyState('5') & 0x8000)
		m_dwCurKey |= KEY_5;
	if (GetAsyncKeyState('6') & 0x8000)
		m_dwCurKey |= KEY_6;
	if (GetAsyncKeyState('7') & 0x8000)
		m_dwCurKey |= KEY_7;
	if (GetAsyncKeyState('8') & 0x8000)
		m_dwCurKey |= KEY_8;
	if (GetAsyncKeyState('A') & 0x8000)
		m_dwCurKey |= KEY_A;
	if (GetAsyncKeyState('S') & 0x8000)
		m_dwCurKey |= KEY_S;
	if (GetAsyncKeyState('W') & 0x8000)
		m_dwCurKey |= KEY_W;
	if (GetAsyncKeyState('D') & 0x8000)
		m_dwCurKey |= KEY_D;
	if (GetAsyncKeyState('F') & 0x8000)
		m_dwCurKey |= KEY_F;
	if (GetAsyncKeyState('E') & 0x8000)
		m_dwCurKey |= KEY_E;
	if (GetAsyncKeyState('H') & 0x8000)
		m_dwCurKey |= KEY_H;
}

bool Engine::CKeyMgr::KeyUp(DWORD dwKey)
{
	if (m_dwCurKey & dwKey)
	{
		m_dwKeyPressed |= dwKey;
		return false;
	}
	else if (m_dwKeyPressed & dwKey)
	{
		m_dwKeyPressed ^= dwKey;
		return true;
	}

	return false;
}

bool Engine::CKeyMgr::KeyDown(DWORD dwKey)
{
	// 이전에 누른적 없고 현재 눌렸을 때
	if (!(m_dwKeyDown & dwKey) && (m_dwCurKey & dwKey))
	{
		m_dwKeyDown |= dwKey;
		return true;
	}
	// 현재 누른적 없으나 기존에 눌렸을 때
	else if (!(m_dwCurKey & dwKey) && (m_dwKeyDown & dwKey))
	{
		m_dwKeyDown ^= dwKey;
		return false;
	}

	return false;
}

bool Engine::CKeyMgr::KeyPressing(DWORD dwKey)
{
	if (m_dwCurKey & dwKey)
		return true;

	return false;
}

void Engine::CKeyMgr::Free(void)
{

}

// 두 키가 순서대로 눌렸을 경우
