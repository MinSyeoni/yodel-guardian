
#ifndef ObjMgr_h__
#define ObjMgr_h__

#include "GameObject.h"

BEGIN(Engine)
class CGameObject;
END

class CObjMgr
{
	DECLARE_SINGLETON(CObjMgr)
public:
	enum OBJ_ID { OBJ_TERRAIN, OBJ_OBJECT, OBJ_POINT, OBJ_CELL, OBJ_COLLIDER,OBJ_EFFECT, OBJ_END};

private:
	CObjMgr(void);
	virtual ~CObjMgr(void);

public:
	void						Load_Object(const _tchar* pFilePath);

	void						AddObject(Engine::CGameObject*pObject, OBJ_ID eId);
	void						Update_Object(const _float& fTimeDelta);
	void						Render_Object();
	void						ReleaseGroup(OBJ_ID _iD);
	
	Engine::CGameObject*		GetGameObject(OBJ_ID eId);
	list<Engine::CGameObject*>	GetGameObjectLst(OBJ_ID eId) { return m_ObjLst[eId]; };

private:
	void						Release();

public:
	list<Engine::CGameObject*>  m_ObjLst[OBJ_END];
};

#endif // ObjMgr_h__