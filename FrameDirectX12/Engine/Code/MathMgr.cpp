#include "MathMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CMathMgr)

CMathMgr::CMathMgr()
{
}


CMathMgr::~CMathMgr()
{
}


void CMathMgr::Free()
{
#ifdef _DEBUG
	COUT_STR("Destroy MathMgr");
#endif

}
