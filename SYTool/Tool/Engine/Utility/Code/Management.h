#ifndef Management_h__
#define Management_h__

#include "Engine_Defines.h"
#include "Base.h"

#include "Scene.h"
#include "Shader.h"

BEGIN(Engine)

class ENGINE_DLL CManagement : public CBase
{
	DECLARE_SINGLETON(CManagement)

private:
	explicit CManagement(void);
	virtual ~CManagement(void);

public:
	CLayer*  Get_Layer(const _tchar* pLayertag);

public:
	HRESULT		Set_Scene(CScene* pScene);
	HRESULT		Ready_Management(LPDIRECT3DDEVICE9 pGraphicDev);
	_uint		Update_Scene(const _float& fTimeDelta);
	void		Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	CScene*			m_pScene = nullptr;

public:
	virtual void Free(void);
};

END
#endif // Management_h__


