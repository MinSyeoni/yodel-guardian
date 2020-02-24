#include "FrameMgr.h"
#include "Frame.h"

USING(Engine)
IMPLEMENT_SINGLETON(CFrameMgr)

Engine::CFrameMgr::CFrameMgr(void)
{

}

Engine::CFrameMgr::~CFrameMgr(void)
{
	Free();
}

_bool Engine::CFrameMgr::IsPermit_Call(const _tchar* pFrameTag, const _float& fTimeDelta)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if (nullptr == pFrame)
		return false;

	return pFrame->IsPermit_Call(fTimeDelta);
}

HRESULT Engine::CFrameMgr::Ready_Frame(const _tchar* pFrameTag, const _float& fCallLimit)
{
	CFrame*		pFrame = Find_Frame(pFrameTag);

	if (nullptr != pFrame)
		return E_FAIL;

	pFrame = CFrame::Create(fCallLimit);
	if (nullptr == pFrame)
		return E_FAIL;

	m_mapFrames.emplace(pFrameTag, pFrame);


	return S_OK;
}

Engine::CFrame* Engine::CFrameMgr::Find_Frame(const _tchar* pFrameTag)
{
	auto		iter = find_if(m_mapFrames.begin(), m_mapFrames.end(), CTag_Finder(pFrameTag));

	if (iter == m_mapFrames.end())
		return nullptr;

	return iter->second;
}

void Engine::CFrameMgr::Free(void)
{
	for_each(m_mapFrames.begin(), m_mapFrames.end(), CDeleteMap());
	m_mapFrames.clear();
}

