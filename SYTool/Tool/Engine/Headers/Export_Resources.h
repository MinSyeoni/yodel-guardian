#ifndef Export_Resources_h__
#define Export_Resources_h__

#include "ResourcesMgr.h"
#include "Texture.h"

BEGIN(Engine)

// ResourcesMgr
// Get
inline CResources* Clone_Resources(const _ushort& wContainerIdx, const _tchar* pResourceTag);
// Set
// General
inline HRESULT	Reserve_ContainerSize(const _ushort& wSize);

inline HRESULT	Ready_Buffer(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pBufferTag,
	BUFFERID eID,
	const _tchar* pPath,
	const _ulong& dwCntX = 1,
	const _ulong& dwCntZ = 1,
	const _ulong& dwVtxItv = 1);


inline HRESULT	Ready_Texture(LPDIRECT3DDEVICE9 pGraphicDev, const _ushort& wContainerIdx, const _tchar* pTextureTag, TEXTURETYPE eType, const _tchar* pPath, const _uint& iCnt = 1);

inline HRESULT		Ready_Mesh(LPDIRECT3DDEVICE9 pGraphicDev,
	const _ushort& wContainerIdx,
	const _tchar* pMeshTag,
	MESHTYPE eType,
	const _tchar* pFilePath,
	const _tchar* pFileName);

inline void	Render_Buffer(const _ushort& wContainerIdx, const _tchar* pBufferTag);
// Destroy
inline void		DestroyResources(void);


#include "Export_Resources.inl"


END
#endif // Export_Resources_h__
