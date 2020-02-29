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
	const _vec3*		Get_VtxPos(void) const {	return m_pPos;	}
	_vec3*              Get_VtxPos2(void) { return m_pPos; };
	_ulong				Get_VtxCntX(void) const { return m_iH.biWidth; }
	_ulong				Get_VtxCntZ(void) const { return m_iH.biHeight; }
public:
	void		Copy_Index(Engine::INDEX32* pIndex, const _ulong& dwTriCnt);
public:
	_ulong	Get_TriCnt(void) const { return m_dwTriCnt; }

public:
	HRESULT	Ready_Buffer(const _ulong& dwCntX, 
						 const _ulong& dwCntZ, 
						 const _ulong& dwVtxItv);
	
	HRESULT	Ready_Buffer(const _tchar* pHeightMap, 
						const _ulong& dwVtxItv);

	HRESULT Reset_Buffer(const _ulong& dwCntX, const _ulong& dwCntZ, const _ulong& dwVtxItv);

	void  SetTextureHeight(_vec3* pPos, float fRange);
	void  SetHeight(_int iIndex, _float fHeight);





private:
	HANDLE				m_hFile;
	BITMAPFILEHEADER	m_fH;
	BITMAPINFOHEADER	m_iH;

	_vec3*				m_pPos;
	_bool				m_bClone;

	_ulong				m_dwVtxCntZ;
	_ulong				m_dwVtxCntX;
	_ulong				m_dwVtxItv;

private:
	VTXTEX*		m_pVtxTex = nullptr;

public:
	static CTerrainTex*		Create(LPDIRECT3DDEVICE9 pGraphicDev, 
									const _ulong& dwCntX,
									const _ulong& dwCntZ, 
									const _ulong& dwVtxItv);

	virtual CResources*		Clone(void);
	virtual void			Free(void);
};

END
#endif // TerrainTex_h__
