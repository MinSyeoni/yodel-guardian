// Management
// Get

CLayer* Engine::Get_Layer(const _tchar* pLayerTag)
{
	return CManagement::GetInstance()->Get_Layer(pLayerTag);
}

// Set
HRESULT Engine::Set_Scene(CScene* pScene)
{
	return CManagement::GetInstance()->Set_Scene(pScene);
}
// General
HRESULT	Create_Management(LPDIRECT3DDEVICE9 pGraphicDev, CManagement** ppManagement)
{
	CManagement*		pInstance = CManagement::GetInstance();

	if (nullptr == pInstance)
		return E_FAIL;

	if (FAILED(pInstance->Ready_Management(pGraphicDev)))
		return E_FAIL;

	*ppManagement = pInstance;

	return S_OK;
}

// Renderer
// Get
CRenderer* Get_Renderer(void)
{
	return CRenderer::GetInstance();
}
// Set
// General
HRESULT	Ready_Renderer(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return CRenderer::GetInstance()->Ready_Renderer(pGraphicDev);
}


// LightMgr
// Get
const D3DLIGHT9*	Get_LightInfo(const _uint& iIndex)
{
	return CLightMgr::GetInstance()->Get_LightInfo(iIndex);
}
// Set
// General
HRESULT		Ready_Light(LPDIRECT3DDEVICE9 pGraphicDev, const D3DLIGHT9* pLight, const _uint& iIndex,CLight::STYLE _style,_vec3 Pos)
{
	return CLightMgr::GetInstance()->Ready_Light(pGraphicDev, pLight, iIndex,_style,Pos);
}

void		Render_Light(LPD3DXEFFECT pEffect)
{
	CLightMgr::GetInstance()->Render_Light(pEffect);
}

// ProtoMgr
// Get
// Set
// General
HRESULT		Ready_Prototype(const _tchar* pProtoTag, CComponent* pInstance)
{
	return CProtoMgr::GetInstance()->Ready_Prototype(pProtoTag, pInstance);
}
CComponent*	Clone_Prototype(const _tchar* pProtoTag)
{
	return CProtoMgr::GetInstance()->Clone_Prototype(pProtoTag);
}

// RenderTargetMgr
// Get
// Set
void		Set_OnShader(LPD3DXEFFECT pEffect, const _tchar* pTargetTag, const char* pConstantName)
{
	CRenderTargetMgr::GetInstance()->Set_OnShader(pEffect, pTargetTag, pConstantName);
}
// General
HRESULT		Ready_RenderTarget(const _tchar* pTargetTag, LPDIRECT3DDEVICE9 pGraphicDev,
	const _uint& iWidth, const _uint& iHeight, D3DFORMAT Format, D3DXCOLOR Color)
{
	return CRenderTargetMgr::GetInstance()->Ready_RenderTarget(pTargetTag, pGraphicDev, iWidth, iHeight, Format, Color);
}
HRESULT		Ready_MRT(const _tchar* pMRTTag, const _tchar* pTargetTag)
{
	return CRenderTargetMgr::GetInstance()->Ready_MRT(pMRTTag, pTargetTag);
}
HRESULT		Begin_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->Begin_MRT(pMRTTag);
}
HRESULT		End_MRT(const _tchar* pMRTTag)
{
	return CRenderTargetMgr::GetInstance()->End_MRT(pMRTTag);
}
HRESULT		Ready_DebugBuffer(const _tchar* pTargetTag, const _float& fX, const _float& fY, const _float& fSizeX, const _float& fSizeY)
{
	return CRenderTargetMgr::GetInstance()->Ready_DebugBuffer(pTargetTag, fX, fY, fSizeX, fSizeY);
}
void			Render_DebugBuffer(const _tchar* pMRTTag)
{
	CRenderTargetMgr::GetInstance()->Render_DebugBuffer(pMRTTag);
}

// Destroy
void		DestroyUtility(void)
{
	CRenderTargetMgr::GetInstance()->DestroyInstance();

	CProtoMgr::GetInstance()->DestroyInstance();
	CLightMgr::GetInstance()->DestroyInstance();
	CRenderer::GetInstance()->DestroyInstance();
	CManagement::GetInstance()->DestroyInstance();
}