#ifndef AniCtrl_h__
#define AniCtrl_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CAniCtrl : public CBase
{
private:
	explicit CAniCtrl(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	explicit CAniCtrl(const CAniCtrl& rhs);
	virtual ~CAniCtrl(void);

public:
	LPD3DXANIMATIONCONTROLLER		Get_AniCtrl(void) { return m_pAniCtrl; }

public:
	HRESULT	Ready_AniCtrl(void);
	void	Set_AnimationSet(const _uint& iIndex);
	void    Set_AnimationTimeSet(const _uint&iIndex, float fTime);
	void	Play_Animation(const _float& fTimeDelta);
	_bool						Is_AnimationSetEnd(void);
	_bool                       is_AnimationSetStart(void);
	void     TimeReset(_double AniTime);
private:
	LPD3DXANIMATIONCONTROLLER			m_pAniCtrl;
	_uint								m_iCurrentTrack;


	_uint								m_iNewTrack;
	_float								m_fAccTime;
	_uint								m_iOldAniIndex;
	_double								m_dPeriod;
public:
	static	CAniCtrl*		Create(LPD3DXANIMATIONCONTROLLER pAniCtrl);
	static  CAniCtrl*		Create(const CAniCtrl& rhs);
	virtual void			Free(void);

};

END
#endif // AniCtrl_h__
