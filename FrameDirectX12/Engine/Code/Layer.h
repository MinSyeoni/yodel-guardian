#pragma once
#include "GameObject.h"

/*____________________________________________________________________
GameObject List를 관리할 Layer 클래스.
______________________________________________________________________*/
typedef list<CGameObject*>	OBJLIST;
typedef OBJLIST::iterator	OBJITER;

BEGIN(Engine)

class ENGINE_DLL CLayer : public CBase
{
public:
	explicit CLayer();
	virtual ~CLayer();

public:
	OBJLIST*		Get_OBJLIST(wstring wstrObjTag);
	CGameObject*	Get_GameObject(wstring wstrObjTag, _int iIdx = 0);
public:
	HRESULT			Add_GameObject(wstring wstrObjTag, CGameObject* pGameObject);
	HRESULT			Delete_GameObject(wstring wstrObjTag, _int iIdx = 0);
	HRESULT			Clear_OBJLIST(wstring wstrObjTag);
public:
	HRESULT			Ready_Layer();
	_int			Update_Layer(const _float& fTimeDelta);
	_int			LateUpdate_Layer(const _float& fTimeDelta);
	void			Render_Layer(const _float& fTimeDelta);

private:
	unordered_map<wstring, OBJLIST> m_mapObjLst;

public:
	static CLayer*	Create();
private:
	virtual void	Free();
};

END