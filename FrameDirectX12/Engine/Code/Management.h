#pragma once
#include "Engine_Include.h"
#include "Base.h"

BEGIN(Engine)

class CScene;

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement();
	virtual ~CManagement();

public:
	HRESULT	SetUp_CurrentScene(CScene* pNewScene);
public:
	_int	Update_Management(const _float& fTimeDelta);
	_int	LateUpdate_Management(const _float& fTimeDelta);
	void	Render_Management(const _float& fTimeDelta);
	CScene*  Get_CurScene() { return m_pCurrentScene; };
protected:
	CScene* m_pCurrentScene = nullptr;

private:
	virtual void Free();
};

END