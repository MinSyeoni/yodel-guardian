#ifndef ToolEffect_h__
#define ToolEffect_h__

#include "GameObject.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "Export_Function.h"

BEGIN(Engine)
class CTransform;
class CCalculator;
class CStaticMesh;
class CCollider;
class CShader;
END
typedef struct tagEFFECTDATA
{
	_vec3 vOriPos = { 0.f,0.f,0.f };
	_vec3 vOriRot = { 0.f,0.f,0.f };
	_vec3 vOriScale = { 1.f,1.f,1.f };

	_float fStartTime = 0.f;
	_float fEndTime = 1.f;
	_int iRepeat = 1;

}EFFECTDATA;



class CToolEffect : public Engine::CGameObject
{
public:
	enum TEXTURE_STATE { NONE, ALPHABLEND, ALPHATEST };
private:
	explicit CToolEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolEffect(void);

public:
	virtual HRESULT			Ready_Object();
	virtual _int			Update_Object(const _float& fTimeDelta);
	virtual void			Render_Object(void);
	HRESULT					Set_ConstantTable(LPD3DXEFFECT pEffect);

private:
	HRESULT					Add_Component(void);

private:
	virtual void			Free(void);

public:
	static CToolEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	EFFECTDATA m_tEffectData;
private:
	Engine::CTransform* m_pTransCom = nullptr;
	Engine::CCalculator* m_pCalculatorCom = nullptr;
	Engine::CRcTex* m_pBufferCom = nullptr;
	Engine::CTexture* m_pTexCom = nullptr;
	Engine::CShader* m_pShaderCom = nullptr;

};

#endif // ToolEffect_h__
