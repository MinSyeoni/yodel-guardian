#ifndef Light_h__
#define Light_h__

#include "Engine_Defines.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CBase
{
public:
	enum STYLE {SPOT,DIRECTION,POINT};
private:
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLight(void);

public:
	const D3DLIGHT9*	Get_LightInfo(void) const { return &m_tLightInfo; }
	HRESULT	Ready_Light(const D3DLIGHT9* pLightInfo, const _uint& iIndex);

public:
	void		Render(LPD3DXEFFECT pEffect);

private:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	D3DLIGHT9				m_tLightInfo;

	_uint					m_iIndex;
	LPDIRECT3DVERTEXBUFFER9		m_pVB;
	LPDIRECT3DINDEXBUFFER9		m_pIB;
	STYLE m_eStyle;
public:
	_bool  m_bisRender = false;
public:
	_vec3 m_vecLightPos = {};
public:
	static CLight*		Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const D3DLIGHT9* pLightInfo,
		const _uint& iIndex,STYLE _style,_vec3 Pos);
private:
	virtual void		Free(void);

};

END
#endif // Light_h__
