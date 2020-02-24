#ifndef GameObject_h__
#define GameObject_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~CGameObject(void);


public:
	CComponent*	Get_Component(const _tchar* pComponentTag, Engine::COMPONENTID eID);

public:
	_float		Get_ViewZ(void) { return m_fViewZ; }
	void		Compute_ViewZ(const _vec3* pPos);

public:
	virtual HRESULT Ready_Object(void);
	virtual _int	Update_Object(const _float& fTimeDelta);
	virtual void	Render_Object();

protected:
	LPDIRECT3DDEVICE9					m_pGraphicDev = nullptr;
	map<const _tchar*, CComponent*>		m_mapComponent[ID_END];
	_float								m_fViewZ;

private:
	CComponent*	Find_Component(const _tchar* pComponentTag, Engine::COMPONENTID eID);

public:
	virtual void Free(void);
	_bool m_bisDead = false;
	_bool m_bisInit = false;
};
END
#endif // GameObject_h__
