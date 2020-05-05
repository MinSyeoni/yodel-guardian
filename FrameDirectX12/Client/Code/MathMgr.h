
#pragma once
#include "Base.h"

class MathMgr : public CBase
{
	DECLARE_SINGLETON(MathMgr)

private:
	explicit MathMgr();
	virtual ~MathMgr();
public:
	static _vec3 Bazior2(_vec3 Pos1, _vec3 Pos2, float fAccTime);
	static _vec3 Bazior3(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3, float fAccTime);
	static _vec3 Bazior4(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3, _vec3 Pos4,float fAccTime);


private:
	virtual void Free();
};
