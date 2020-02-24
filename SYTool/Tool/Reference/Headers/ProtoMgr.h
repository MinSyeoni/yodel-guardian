#ifndef ProtoMgr_h__
#define ProtoMgr_h__

#include "Engine_Defines.h"
#include "Base.h"
#include "Component.h"

#include "Optimization.h"
BEGIN(Engine)

class ENGINE_DLL CProtoMgr : public CBase
{
	DECLARE_SINGLETON(CProtoMgr)

private:
	explicit CProtoMgr(void);
	virtual ~CProtoMgr(void);

public:
	HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
	CComponent*	Clone_Prototype(const _tchar* pProtoTag);

private:
	map<const _tchar*, CComponent*>			m_mapPrototype;

private:
	CComponent*		Find_Prototype(const _tchar* pProtoTag);
public:
	virtual void Free(void);
};

END
#endif // ProtoMgr_h__
