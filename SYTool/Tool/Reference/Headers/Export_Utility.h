#ifndef Export_Utility_h__
#define Export_Utility_h__

#include "Engine_Defines.h"
#include "Management.h"
#include "Transform.h"
#include "Renderer.h"
#include "Camera.h"
#include "Calculator.h"
#include "LightMgr.h"
#include "Collider.h"
#include "ProtoMgr.h"
#include "RenderTargetMgr.h"
#include "TrailColor.h"
BEGIN(Engine)

// Management
// Get
inline CLayer*			Get_Layer(const _tchar* pLayerTag);
// Set
inline HRESULT		Set_Scene(CScene* pScene);

// General
inline HRESULT	Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement);

// Renderer
// Get
inline CRenderer* Get_Renderer(void);
// Set
// General
inline HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev);

// LightMgr
// Get
inline const D3DLIGHT9*	Get_LightInfo(const _uint& iIndex = 0);
// Set
// General
inline HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev,
	const D3DLIGHT9* pLight,
	const _uint& iIndex,CLight::STYLE _style ,_vec3 Pos);

inline void		Render_Light(LPD3DXEFFECT pEffect);

// ProtoMgr
// Get
// Set
// General
inline HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance);
inline CComponent*	Clone_Prototype(const _tchar* pProtoTag);

// RenderTargetMgr
// Get
// Set
inline void		Set_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName);

// General
inline HRESULT		Ready_RenderTarget(const _tchar* pTargetTag,
	LPDIRECT3DDEVICE9 pGraphicDev,
	const _uint& iWidth,
	const _uint& iHeight,
	D3DFORMAT Format,
	D3DXCOLOR Color);
inline HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag);
inline HRESULT		Begin_MRT(const _tchar* pMRTTag);
inline HRESULT		End_MRT(const _tchar* pMRTTag);
inline HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY);
inline void			Render_DebugBuffer(const _tchar* pMRTTag);

// Destroy

inline void		DestroyUtility(void);

#include "Export_Utility.inl"

END
#endif // Export_Utility_h__
