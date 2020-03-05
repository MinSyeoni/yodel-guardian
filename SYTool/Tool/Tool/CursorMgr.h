#pragma once
class CCursorMgr
{
private:
    CCursorMgr();
    ~CCursorMgr();

public:
    static D3DXVECTOR3 GetCursPos(HWND hWnd);
};

