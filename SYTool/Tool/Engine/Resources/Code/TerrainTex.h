#ifndef TerrainTex_h__
#define TerrainTex_h__

#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CTerrainTex : public CVIBuffer
{
private:
	explicit CTerrainTex(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTerrainTex(const CTerrainTex& rhs);
	virtual ~CTerrainTex(void);

public:
	const _vec3* Get_VtxPos(void) const { return m_pPos; }
	_ulong				Get_VtxCntX(void) const { return m_dwVtxCntX; }
	_ulong				Get_VtxCntZ(void) const { return m_dwVtxCntZ; }
	void				Set_TerrainHeight(_float fRange, _float fHeight, _vec3 vPos, _int iMode);
	_float				Get_TerrainHeight(_ulong dwIndex);

	VTXTEX* m_pVtxTex = nullptr;
	VTXTEX* m_pVtxTexOrigin = nullptr;

public:
	void		Copy_Index(Engine::INDEX32* pIndex, const _ulong& dwTriCnt);

public:
	HRESULT	Ready_Buffer(const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv);
	HRESULT	Ready_Buffer(const _tchar* pPath,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv);

private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_vec3* m_pPos = nullptr;
	_bool				m_bClone;

	_ulong				m_dwVtxCntZ;
	_ulong				m_dwVtxCntX;

	_float				m_fHeight = 0.f;
public:
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv);

	virtual CResources* Clone(void);
	static CTerrainTex* Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pPath,
		const _ulong& dwCntX,
		const _ulong& dwCntZ,
		const _ulong& dwVtxItv);

	virtual void			Free(void);
};

END
#endif // TerrainTex_h__
