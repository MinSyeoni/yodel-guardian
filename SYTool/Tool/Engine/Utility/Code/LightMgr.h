#ifndef LightMgr_h__
#define LightMgr_h__

#include "Engine_Defines.h"
#include "Base.h"

#include "Light.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr : public CBase
{
	DECLARE_SINGLETON(CLightMgr)
public:
	enum LIGHTENUM { DANNY,JONAS};
private:
	explicit CLightMgr(void);
	virtual ~CLightMgr(void);

public:
	const D3DLIGHT9*	Get_LightInfo(const _uint& iIndex = 0);

public:
	HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
		const D3DLIGHT9* pLight,
		const _uint& iIndex,CLight::STYLE _style,_vec3 Pos);

	vector<CLight*> GetLightLst() { return m_LightList; };

	void		Render_Light(LPD3DXEFFECT pEffect);

private:
	vector<CLight*>			m_LightList;

private:
	virtual void Free(void);

};
END
#endif // LightMgr_h__
