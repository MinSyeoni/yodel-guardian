#pragma once
#include "Include.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"



class CEffectComponent;

enum TEXTURE_STATE { NONE, ALPHABLEND, ALPHATEST };
enum BUFFER_TYPE { TEXTURE, MESH };

typedef struct tagEFFECTDATA
{
	BUFFER_TYPE  eType = TEXTURE;
	TEXTURE_STATE eState = NONE;

	int iDrawId = 0;


	_vec3 vOriPos = _vec3{ 0.f,0.f,0.f };
	_vec3 vOriRot = _vec3{ 0.f,0.f,0.f };
	_vec3 vOriScale = _vec3{ 1.f,1.f,1.f };

	_float fStartTime = 0.f;
	_float fEndTime = 1.f;
	_int iDeadRepeat = 1;



	_vec3 vScalePat = _vec3{ 1.f,1.f,1.f };
	_vec3 vRotPat = _vec3{ 0.f,0.f,0.f };

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
	_vec4 vColor = _vec4{ 1.f,1.f,1.f,1.f };

}EFFECTDATA;
class CEffect : public Engine::CGameObject
{
private:
	explicit CEffect(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CEffect(const CEffect& rhs);
	virtual ~CEffect();

public:
	HRESULT						Ready_GameObjectPrototype(wstring strPath);
	 HRESULT				Ready_GameObject(_vec3 Pos);
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);
	void LoadFile(wstring strPath);

private:
	virtual HRESULT				Add_Component();

	vector< CEffectComponent*> m_pEffectList;
	vector<EFFECTDATA> m_vecEffectData;
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
public:
	virtual CGameObject* Clone_GameObject(void* pArg);
	static CEffect* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList,wstring strPath);
private:
	virtual void				Free();
};
