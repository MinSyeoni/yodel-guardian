#include "Frame.h"

USING(Engine)

CFrame::CFrame()
{
}


CFrame::~CFrame()
{
}

_bool CFrame::IsPermit_Call(const _float & fTimeDelta)
{
	/*____________________________________________________________________
	제한된 프레임 수치 만큼 MainApp을 구동시킨다.
	______________________________________________________________________*/
	m_fAccTimeDelta += fTimeDelta;

	if (m_fAccTimeDelta >= m_fCallLimit)
	{
		m_fAccTimeDelta = 0.f;
		return true;
	}

	return false;
}

HRESULT CFrame::Ready_Frame(const _float & fCallLimit)
{
	/*____________________________________________________________________
	제한할 Frame 수치를 입력받아 초기화한다.
	______________________________________________________________________*/
	m_fCallLimit = 1.f / fCallLimit;

	return S_OK;
}

CFrame * CFrame::Create(const _float & fCallLimit)
{
	CFrame* pInstance = new CFrame;

	if (FAILED(pInstance->Ready_Frame(fCallLimit)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CFrame::Free()
{
}
