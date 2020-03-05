#pragma once

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CBuffer_Terrain final : public CVIBuffer
{
private:
	explicit CBuffer_Terrain(LPDIRECT3DDEVICE9 pGraphic_Device);
	explicit CBuffer_Terrain(const CBuffer_Terrain& rhs);
	virtual ~CBuffer_Terrain() = default;
public:
	HRESULT Ready_VIBuffer(const _uint& iNumVerticesX, const _uint& iNumVerticesZ, const _float& fInterval);
	HRESULT Ready_VIBuffer(const _tchar* pHeightMap, const _float& fInterval);
	void Render_VIBuffer();
	_float Compute_HeightOnTerrain(const _vec3* pPosition);	

	VTXTEX*				m_pVtxTex = nullptr;
	VTXTEX*				m_pVtxTexOrigin = nullptr;

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_vec3* m_pPos = nullptr;
	_bool				m_bClone;

	_ulong				m_dwVtxCntZ;
	_ulong				m_dwVtxCntX;

	_uint				m_iNumVerticesX = 0;
	_uint				m_iNumVerticesZ = 0;
	_float				m_fInterval = 0.f;
	_ulong*				m_pPixel = nullptr;

private:
	INDEX32*			m_pPolygonVertexIndex = nullptr;

public:
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _uint& iNumVerticesX, const _uint& iNumVerticesZ, const _float& fInterval = 1.f);
	static CBuffer_Terrain* Create(LPDIRECT3DDEVICE9 pGraphic_Device, const _tchar* pHeightMap, const _float& fInterval = 1.f);
	virtual CComponent* Clone_Component(void* pArg = nullptr);

	virtual CResources*		Clone(void);
	virtual void			Free(void);
};

END