// ResourcesMgr
// Get
CResources* Clone_Resources(const _ushort& wContainerIdx, const _tchar* pResourceTag)
{
	return CResourcesMgr::GetInstance()->Clone_Resources(wContainerIdx, pResourceTag);
}

// Set
// General
HRESULT	Reserve_ContainerSize(const _ushort& wSize)
{
	return CResourcesMgr::GetInstance()->Reserve_ContainerSize(wSize);
}

HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pBufferTag, BUFFERID eID, const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv)
{
	return CResourcesMgr::GetInstance()->Ready_Buffer(pGraphicDev, wContainerIdx, pBufferTag, eID, dwCntX, dwCntZ, dwVtxItv);
}

HRESULT	Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt)
{
	return CResourcesMgr::GetInstance()->Ready_Texture(pGraphicDev, wContainerIdx, pTextureTag, eType, pPath, iCnt);
}

HRESULT		Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pMeshTag, MESHTYPE eType, const _tchar* pFilePath, const _tchar* pFileName)
{
	return CResourcesMgr::GetInstance()->Ready_Mesh(pGraphicDev, wContainerIdx, pMeshTag, eType, pFilePath, pFileName);
}


void	Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag)
{
	CResourcesMgr::GetInstance()->Render_Buffer(wContainerIdx, pBufferTag);
}

// Destroy
void		DestroyResources(void)
{
	CResourcesMgr::GetInstance()->DestroyInstance();
}
