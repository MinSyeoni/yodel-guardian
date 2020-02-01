#include "FrameMgr.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameMgr)

CFrameMgr::CFrameMgr()
{
}


CFrameMgr::~CFrameMgr()
{
}

_bool CFrameMgr::IsPermit_Call(wstring wstrFrameTag, const _float & fTimeDelta)
{
	CFrame*	pInstance = Find_Frame(wstrFrameTag);
	NULL_CHECK_RETURN(pInstance, false);

	return pInstance->IsPermit_Call(fTimeDelta);
}

HRESULT CFrameMgr::Ready_Frame(wstring wstrFrameTag, const _float & fCallLimit)
{
	/*____________________________________________________________________
	새로 생성하기 전에, FrameTag값을 탐색한다.
	이미 존재하는 값이면 새로 할당할 필요 없으므로 종료.
	______________________________________________________________________*/
	CFrame* pInstance = Find_Frame(wstrFrameTag);

	if (nullptr != pInstance)
		return E_FAIL;

	/*____________________________________________________________________
	없으면 새로 할당한 후, map컨테이너에 추가해준다.
	______________________________________________________________________*/
	pInstance = CFrame::Create(fCallLimit);
	NULL_CHECK_RETURN(pInstance, E_FAIL);

	m_mapFrame.emplace(wstrFrameTag, pInstance);

	return S_OK;
}

CFrame * CFrameMgr::Find_Frame(wstring wstrFrameTag)
{
	/*____________________________________________________________________
	찾고자 하는 Frame Tag를 탐색한다.
	없다면 nullptr 반환.
	______________________________________________________________________*/
	auto iter_find = m_mapFrame.find(wstrFrameTag);

	if (iter_find == m_mapFrame.end())
		return nullptr;


	return iter_find->second;
}

void CFrameMgr::Free()
{
	for_each(m_mapFrame.begin(), m_mapFrame.end(), CDeleteMap());

	m_mapFrame.clear();
}
