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

enum TEXTURE_STATE { NONE, ALPHABLEND, ALPHATEST };
enum BUFFER_TYPE{TEXTURE,MESH};

typedef struct tagEFFECTDATA
{
	BUFFER_TYPE  eType = TEXTURE;
	TEXTURE_STATE eState = NONE;

	int iDrawId = 0;


	_vec3 vOriPos = { 0.f,0.f,0.f };
	_vec3 vOriRot = { 0.f,0.f,0.f };
	_vec3 vOriScale = { 1.f,1.f,1.f };

	_float fStartTime = 0.f;
	_float fEndTime = 1.f;
	_int iDeadRepeat = 1;



	_vec3 vScalePat = { 1.f,1.f,1.f };
	_vec3 vRotPat = { 0.f,0.f,0.f };

	_float fStartScale = 0.f;//시작시간
	_float fEndScale = 0.f;

	_float fStartRot = 0.f;
	_float fEndRot = 0.f;

	_int iScaleRepeat = 1;
	_int iRotRepeat = 1;


	_bool bIsFadeOut = false;
	_bool bIsFadeIn = false;

	_float fFadeInStartTime = 0.f;
	_float fFadeInEndTime = 0.f;
	_float fFadeOutStartTime = 0.f;
	_float fFadeOutEndTime = 0.f;

	int iUvWidth = 1;
	int iUvHeight = 1;

	_float fSpriteSpeed = 0.f;
	_bool bIsUvSprite = false;
	_bool bIsUvInfinite = false;

	_bool bIsColorInput = false;
	_vec4 vColor = { 1.f,1.f,1.f,1.f };

}EFFECTDATA;



class CToolEffect : public Engine::CGameObject
{
private:
	explicit CToolEffect(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolEffect(void);

public:
	virtual HRESULT			Ready_Object();
	virtual _int			Update_Object(const _float& fTimeDelta);
	virtual void			Render_Object(void);
	HRESULT					Set_ConstantTable(LPD3DXEFFECT pEffect);
	HRESULT SetTexture(int iDrawId, TEXTURE_STATE eState);
	CSphereCollider* Get_Collider() { return m_pSphereCol; };
	void UvAnimation();
	void SetTime(float fTime) { m_fAccTime = fTime; };
	void SetCheck(_bool bIsCheck);

	HRESULT SetEffectData(EFFECTDATA tData);
private:
	HRESULT					Add_Component(void);

private:
	virtual void			Free(void);

public:
	static CToolEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	bool m_bIsCheck = true;//이게 지금 툴에서 사용하는 이펙트인지?
	EFFECTDATA m_tEffectData;
	TEXTURE_STATE m_eState = NONE;
	_float m_fAccTime = 0.f;

	int m_iDrawId = 0;
	float m_fChapterX = 0;
	float m_fChapterY = 0;


	_bool m_bIsPlayPattern = false; //저장된 패턴 실행 . 

	_float m_fAlpha = 1.f;

	_float m_fSpriteTime = 0.f;
	_int m_iSpriteRow = 0;

	_bool m_bIsUvStop = false;


	void UpdateAnimation();
	void UpdateScaleAni();
	void UpdateRotAni();
	void UpdateFadeOut();
	void UpdateFadeIn();


private:
	Engine::CTransform* m_pTransCom = nullptr;
	Engine::CCalculator* m_pCalculatorCom = nullptr;
	Engine::CRcTex* m_pBufferCom = nullptr;
	Engine::CTexture* m_pTexCom = nullptr;
	Engine::CShader* m_pShaderCom = nullptr;
	Engine::CSphereCollider* m_pSphereCol = nullptr;
};

#endif // ToolEffect_h__
