
#pragma once
#include "Base.h"

class BaziorMgr : public CBase
{
	DECLARE_SINGLETON(BaziorMgr)

private:
	explicit BaziorMgr();
	virtual ~BaziorMgr();
public:
	static _vec3 Bazior2(_vec3 Pos1, _vec3 Pos2, float fAccTime);
	static _vec3 Bazior3(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3, float fAccTime);
	static _vec3 Bazior4(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3, _vec3 Pos4,float fAccTime);


private:
	virtual void Free();
};
