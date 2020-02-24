#ifndef Resources_h__
#define Resources_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CResources : public CComponent
{
protected:
	explicit CResources(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CResources(const CResources& rhs);
	virtual ~CResources(void);

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

public:
	virtual CResources* Clone(void)PURE;
	virtual void		Free(void);
};
END

#endif // Resources_h__
