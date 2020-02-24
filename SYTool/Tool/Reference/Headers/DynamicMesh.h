#ifndef DynamicMesh_h__
#define DynamicMesh_h__

#include "Mesh.h"
#include "HierarchyLoader.h"
#include "AniCtrl.h"

BEGIN(Engine)

class ENGINE_DLL CDynamicMesh : public CMesh
{
private:
	explicit CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh(void);

public:
	const D3DXFRAME_DERIVED*		Get_FrameByName(const char* pFrameName);
	 D3DXFRAME_DERIVED*		Get_FrameByName2(const char* pFrameName);
	_bool Is_AnimationSetEnd(void);
	_bool Is_AnimationSetStart(void);

public:
	HRESULT		Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Mesh(void);
	void		Render_Mesh(LPD3DXEFFECT pEffect);

	void	Set_AnimationSet(const _uint& iIndex);
	void    Set_AnimationSetTime(const _uint& iIndex, float fTime);
	void	Play_Animation(const _float& fTimeDelta);
	list<D3DXMESHCONTAINER_DERIVED*>	   Get_Dynamic_Constainer() { return m_MeshContainerList; }
	void  TimeReset(_double dTime);
private:
	void	Update_FrameMatrix(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void	SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*				m_pRootFrame;
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;
	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainerList;

	_ulong              m_dwNumVtx;
	_ulong              m_dwStride;
	_vec3*              m_pVtxPos;

public:
	static CDynamicMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CResources*		Clone(void);
	virtual void			Free(void);
};



END
#endif