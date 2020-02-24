#ifndef Shader_h__
#define Shader_h__

#include "Engine_Defines.h"
#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CShader : public CComponent
{
private:
	explicit CShader(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CShader(const CShader& rhs);
	virtual ~CShader(void);

public:
	LPD3DXEFFECT Get_EffectHandle(void) { return m_pEffect; }

public:
	HRESULT		Ready_Shader(const _tchar* pFilePath);

private:
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	LPD3DXEFFECT				m_pEffect;
	LPD3DXBUFFER				m_pErrMsg;	// 버퍼자료형 : 단순한 임의의 정보를 저장하기 위한 메모리 공간, hlsl코드 중 문제가 있는 것을 알리고 출력하기 위한 메세지를 저장하는 용도

public:
	static CShader* Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath);
	virtual CComponent*	Clone(void);
	virtual void Free(void);

};

END
#endif // Shader_h__
