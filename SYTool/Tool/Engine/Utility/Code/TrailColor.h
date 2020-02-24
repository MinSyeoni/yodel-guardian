#ifndef TrailColor_h__
#define TrailColor_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTrailColor : public CComponent
{
private:
	explicit		CTrailColor(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		~CTrailColor(void);

public:
	void			Set_Trail(const _vec3* pPos, 
								   const _vec3* pDir,
								   const _float& fLength,
								   D3DXCOLOR Color, 
								   _ulong dwCount);

public:
	HRESULT	Initialize_TrailColor();
	void			Render_TrailColor();


private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	VTXCOL*					m_pVtxCol;
	_ulong						m_dwVtxSize;
	_ulong						m_dwVtxFVF;
	_ulong 					m_dwVtxCnt;
	
public:
	static		CTrailColor*			Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual		CComponent*		Clone(void);
	virtual		void						Free(void);

};

END

#endif // !TrailColor_h__
