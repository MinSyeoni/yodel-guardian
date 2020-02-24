#ifndef Layer_h__
#define Layer_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "GameObject.h"

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
private:
	explicit	CLayer(void);
	virtual		~CLayer(void);

public:
	HRESULT				Add_GameObject(const _tchar* pObjTag, CGameObject* pGameObject);
	CGameObject*        Get_GameObj(const _tchar* pObjTag);
	list<CGameObject*>*  Get_GameObjLst(const _tchar* pObjTag);

public:

	virtual HRESULT		Ready_Layer(void);
	virtual _int		Update_Layer(const _float& fTimeDelta);
	virtual void		Render_Layer(void);

private:
	typedef list<CGameObject*>    OBJLIST;
	map<wstring, OBJLIST> m_mapOBjList;

public:
	static CLayer*		Create(void);
	virtual void		Free(void);
};

END
#endif // Layer_h__
