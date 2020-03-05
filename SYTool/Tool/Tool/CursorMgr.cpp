#include "pch.h"
#include "CursorMgr.h"

CCursorMgr::CCursorMgr()
{
}


CCursorMgr::~CCursorMgr()
{
}

D3DXVECTOR3 CCursorMgr::GetCursPos(HWND hWnd)
{
    POINT pt{};
    GetCursorPos(&pt);
    ScreenToClient(hWnd, &pt);

    return D3DXVECTOR3(float(pt.x), float(pt.y), 0.f);
}
