#pragma once

#include "Base.h"
#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CColliderMgr : public CBase
{
	DECLARE_SINGLETON(CColliderMgr)

public:
	enum SHAPE{SPHERE,BOX,SHAPE_END};
	enum COLLIDER_TAG{PLAYER,OBJECT,MONSTER,BULLETDECAL, COMBAT, TRIGGER, TAG_END};


private:
	explicit CColliderMgr();
	virtual ~CColliderMgr() = default;

public:
	const list<CCollider*>& Get_ColliderList(const SHAPE eShape, const COLLIDER_TAG eTag);

public:
	void Clear_Collider(void);
	HRESULT Add_Collider(const COLLIDER_TAG eTag, CCollider* pCollider);

private:
	typedef list<CCollider*> LIST_COL;

	LIST_COL  m_listSphereCol[TAG_END];
	LIST_COL m_listBoxCol[TAG_END];

public:
	virtual void			 Free(void);
};
END
