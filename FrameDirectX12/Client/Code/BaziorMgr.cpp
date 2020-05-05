#include "stdafx.h"
#include "BaziorMgr.h"

IMPLEMENT_SINGLETON(BaziorMgr)

BaziorMgr::BaziorMgr()
{
}

BaziorMgr::~BaziorMgr()
{
}

_vec3 BaziorMgr::Bazior2(_vec3 Pos1, _vec3 Pos2, float fAccTime)
{
	_vec3 Temp = (1 - fAccTime) * Pos1 + fAccTime * Pos2;

	return Temp;
}

_vec3 BaziorMgr::Bazior3(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3, float fAccTime)
{
	_vec3 Temp = ((1.f - fAccTime) * (1.f - fAccTime)) * Pos1 + (2.f * fAccTime) * (1.f - fAccTime) * Pos2 + (fAccTime * fAccTime * Pos3);

	return Temp;
}

_vec3 BaziorMgr::Bazior4(_vec3 Pos1, _vec3 Pos2, _vec3 Pos3, _vec3 Pos4, float fAccTime)
{
	_vec3 Temp = (Pos1 * pow(1.f - fAccTime, 3)) + ((3.f * Pos2) * fAccTime * pow(1.f - fAccTime, 2)) +
		3.f * Pos3 * pow(fAccTime, 2) * (1.f - fAccTime) + Pos4 * pow(fAccTime, 3);

	return Temp;
}

void BaziorMgr::Free()
{
}

