#include "AniCtrl.h"

USING(Engine)

Engine::CAniCtrl::CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl)
	: m_pAniCtrl(pAniCtrl)
	, m_iCurrentTrack(0)
	, m_iNewTrack(1)
	, m_fAccTime(0.f)
	, m_iOldAniIndex(99)
	, m_dPeriod(0.0)
{
	m_pAniCtrl->AddRef();
}

Engine::CAniCtrl::CAniCtrl(const CAniCtrl& rhs)
	: m_iCurrentTrack(rhs.m_iCurrentTrack)
	, m_iNewTrack(rhs.m_iNewTrack)
	, m_fAccTime(rhs.m_fAccTime)
	, m_iOldAniIndex(rhs.m_iOldAniIndex)
	, m_dPeriod(0.0)
{
	// 애니메이션은 공유가 되어선 안되기 때문에 함수를 통해 복제를 한다.
	rhs.m_pAniCtrl->CloneAnimationController(rhs.m_pAniCtrl->GetMaxNumAnimationOutputs(),  // 복제 시 원본 객체에서 제공되고 있는 애니메이션의 개수
											rhs.m_pAniCtrl->GetMaxNumAnimationSets(),      // 구동 가능한 애니메이션의 최대 개수(1인자와 2인자의 값이 같다)
											rhs.m_pAniCtrl->GetMaxNumTracks(),			   // 재생 가능한 최대 트랙의 개수(대부분 트랙을 한 개 사용함, 정말 많아봐야 두 개를 사용)
											rhs.m_pAniCtrl->GetMaxNumEvents(), // 메쉬가 지니고 있는 각종 이벤트, 메쉬의 독특한 효과(현재 우리는 사용하지 못한다)
											&m_pAniCtrl);			   
	// 트랙 위에 구동하고자 하는 하나의 동작을 올리고, 트랙을 재생하라고 명령하는 시스템.
	// 하나의 동작을 가리켜 애니메이션 set이라 말한다. 정수형의 값

}

Engine::CAniCtrl::~CAniCtrl(void)
{

}

HRESULT Engine::CAniCtrl::Ready_AniCtrl(void)
{
	return S_OK;
}

void Engine::CAniCtrl::Set_AnimationSet(const _uint& iIndex)
{
	if (m_iOldAniIndex == iIndex)
		return;

	m_iNewTrack = m_iCurrentTrack == 0 ? 1 : 0;

	LPD3DXANIMATIONSET		pAS = NULL;
	// 인덱스에 해당하는 애니메이션을 얻어와 객체 넣어주는 함수
	m_pAniCtrl->GetAnimationSet(iIndex, &pAS); 
	// m_pAniCtrl->GetAnimationSetByName(); 문자열을 이용하여 애니메이션 정보를 얻어오는 함수
	
	
	// 시간 값을 반환(현재 애니메이션 트랙의 포지션과 일치하는 값 == 현재 애니메이션 셋의 전체 재생 시간) 
	m_dPeriod = pAS->GetPeriod();
	
	
	// 트랙 위에 재생하고자하는 애니메이션 셋을 설정하는 함수
	m_pAniCtrl->SetTrackAnimationSet(m_iNewTrack, pAS);

	// 사용하지 않는 않고 있는 이벤트 정보들을 아예 해제한다.(이유 : 이벤트 정보 때문에 간혹 보간이 되지 않는 경우 발생)
	m_pAniCtrl->UnkeyAllTrackEvents(m_iCurrentTrack);
	m_pAniCtrl->UnkeyAllTrackEvents(m_iNewTrack);

	// 키 프레임 : 애니메이션 재생 도중 특정 동작을 의미, 예를 들면 공격 모션 중에 함수가 걸리는 시점의 동작을 하나의 키 프레임이라 말할 수 있다.
	// 애니메이션 메쉬의 경우 모든 키 프레임을 정의할 수는 없다(워낙 많아서)
	// 몇 몇 동작만 키 프레임으로 남겨둬서 구간 별 보간을 통해 자연스러운 애니메이션이 구동된다.

	// 현재 이 함수를 통해 재생되어지던 모션을 꺼버리는 함수 (3인자 : 언제부터 트랙에서 애니메이션을 해제할 것인가)
	// 이제부터 모든 실수는 double형을 사용
	m_pAniCtrl->KeyTrackEnable(m_iCurrentTrack, FALSE, m_fAccTime + 0.25);

	// 애니메이션 재생 속도(위의 코드 근거로는 해제 속도를 설정하는 함수, 속도의 상수 값은 1)
	m_pAniCtrl->KeyTrackSpeed(m_iCurrentTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	// 합성 시에 어떤 트랙에 있는 키 프레임에 가깝게 보간할 것인지 설정하는 함수(2인자 : 가중치 값, 절대 값은 1)
	m_pAniCtrl->KeyTrackWeight(m_iCurrentTrack, 0.1f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);


	m_pAniCtrl->SetTrackEnable(m_iNewTrack, TRUE);
	m_pAniCtrl->KeyTrackSpeed(m_iNewTrack, 1.f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);
	m_pAniCtrl->KeyTrackWeight(m_iNewTrack, 0.9f, m_fAccTime, 0.25, D3DXTRANSITION_LINEAR);

	m_pAniCtrl->ResetTime();
	m_fAccTime = 0.f;

	// 트랙을 활성화 하는 함수(애니메이션 셋을 재생하는 함수가 아니다. )
	m_pAniCtrl->SetTrackPosition(m_iNewTrack, 0.0);

	m_iOldAniIndex = iIndex;

	m_iCurrentTrack = m_iNewTrack;


	//pAS->GetPeriod(); // 시간 값을 반환하는 함수(현재 애니메이션 셋이 트랙의 포지션과 일치하는 값)
	//m_pAniCtrl->GetTrackDesc(); // 현재의 트랙 정보를 반환하는 함수

}

void Engine::CAniCtrl::Play_Animation(const _float& fTimeDelta)
{
	// 애니메이션 셋을 재생하는 함수
	m_pAniCtrl->AdvanceTime(fTimeDelta, NULL); // 2인자 : 애니메이션 동작에 따른 사운드나 이펙트를 담당하는 객체 주소(fmod처럼 직접 만들어서 사용하는 것이 좋다. 사용의 제약이 심하고 오히려 코드의 가중만 커진다)
	m_fAccTime += fTimeDelta;
}

CAniCtrl* Engine::CAniCtrl::Create(LPD3DXANIMATIONCONTROLLER pAniCtrl)
{
	CAniCtrl*			pInstance = new CAniCtrl(pAniCtrl);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

CAniCtrl* Engine::CAniCtrl::Create(const CAniCtrl& rhs)
{
	CAniCtrl*	pInstance = new CAniCtrl(rhs);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Safe_Release(pInstance);

	return pInstance;
}

void Engine::CAniCtrl::Free(void)
{
	Safe_Release(m_pAniCtrl);
}

_bool Engine::CAniCtrl::Is_AnimationSetEnd(void)
{
	D3DXTRACK_DESC			TrackInfo;
	ZeroMemory(&TrackInfo, sizeof(D3DXTRACK_DESC));

	// 현재 트랙의 정보를 가져와 주는 함수

	m_pAniCtrl->GetTrackDesc(m_iCurrentTrack, &TrackInfo);

	if (TrackInfo.Position >= m_dPeriod - 0.1)
		return true;

	return false;
}

