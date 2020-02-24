#ifndef StaticMesh_h__
#define StaticMesh_h__

#include "Mesh.h"

BEGIN(Engine)

class ENGINE_DLL CStaticMesh : public CMesh
{
private:
	explicit CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh(void);

public:
	_ulong  Get_NumVtx(void) const { return m_dwNumVtx; }
	_ulong  Get_Stride(void) const { return m_dwStride; }
	const _vec3*	Get_VtxPos(void) const { return m_pVtxPos; }




	HRESULT	Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
	void	Render_Mesh(LPD3DXEFFECT pEffect,int iPass);
	void    Render_Mesh();
	void    Render_LightMesh(LPD3DXEFFECT pEffect,wstring temp);

private:
	LPD3DXMESH			m_pMesh;
	LPD3DXBUFFER		m_pAdjacency;
	LPD3DXBUFFER		m_pSubsetBuffer;
	D3DXMATERIAL*		m_pSubsets;
	_ulong				m_dwSubsetCnt;
	LPDIRECT3DTEXTURE9*	m_ppTextures;


    LPDIRECT3DTEXTURE9* m_ppNomalTexture;

	vector <TCHAR*>     m_pTestureName;
	LPD3DXMESH          m_pOriMesh;


	_ulong              m_dwNumVtx;
	_ulong              m_dwStride;
	_vec3*              m_pVtxPos;




public:
	static CStaticMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _tchar* pFilePath,
		const _tchar* pFileName);
	virtual CResources*	Clone(void);
	virtual void		Free(void);
};

END
#endif // StaticMesh_h__
