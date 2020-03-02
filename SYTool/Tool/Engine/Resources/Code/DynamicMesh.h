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
	_bool Is_AnimationSetEnd(void);	

public:
	HRESULT		Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
	void		Render_Mesh(void);
	void		Render_Mesh(LPD3DXEFFECT pEffect);

	void	Set_AnimationSet(const _uint& iIndex);
	void	Play_Animation(const _float& fTimeDelta);

private:
	void	Update_FrameMatrix(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix);
	void	SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame);

private:
	D3DXFRAME*				m_pRootFrame;
	CHierarchyLoader*		m_pLoader;
	CAniCtrl*				m_pAniCtrl;
	list<D3DXMESHCONTAINER_DERIVED*>		m_MeshContainerList;


public:
	static CDynamicMesh*	Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName);
	virtual CResources*		Clone(void);
	virtual void			Free(void);
};



END
#endif // DynamicMesh_h__


//typedef struct _D3DXFRAME
//{
//	LPSTR                   Name;			// 뼈의 이름(x파일에서 읽어온 뼈의 이름)
//	D3DXMATRIX              TransformationMatrix; // 최초 로드 상태에서의 뼈가 지닌 상태 행렬
//
//	LPD3DXMESHCONTAINER     pMeshContainer;	//	메쉬를 구성하는 부분 뼈대 또는 메쉬
//
//	struct _D3DXFRAME       *pFrameSibling;			// 형제 뼈의 주소를 저장하기 위한 포인터
//	struct _D3DXFRAME       *pFrameFirstChild;		// 자식 뼈의 주소를 저장하기 위한 포인터
//} D3DXFRAME, *LPD3DXFRAME;
//
//typedef struct _D3DXMESHCONTAINER
//{
//	LPSTR                   Name;				// 이름 : 거의 사용되어 지지 않음		
//
//	D3DXMESHDATA            MeshData;			// 실제적인 메쉬 컴객체 생성 후 정보를 보관해두는 구조체
//
//	LPD3DXMATERIAL          pMaterials;			// 메쉬가 지닌 재질 정보를 저장하기 위한 컴객체
//	LPD3DXEFFECTINSTANCE    pEffects;			// 사용하지 않음
//	DWORD                   NumMaterials;		// 재질의 개수(서브셋의 개수)
//	DWORD                  *pAdjacency;			// 이웃하는 정보(첫 주소로 모든 정점을 순회하기 위해 포인터로 할당함)
//
//	LPD3DXSKININFO          pSkinInfo;			// 읽어들여온 메쉬의 정보를 기준으로 스키닝에 필요한 각종 멤버 함수를 제공
//
//	struct _D3DXMESHCONTAINER *pNextMeshContainer;
//} D3DXMESHCONTAINER, *LPD3DXMESHCONTAINER;
//
//typedef struct _D3DXMESHDATA
//{
//	D3DXMESHDATATYPE Type;
//
//	// current mesh data interface
//	union
//	{
//		LPD3DXMESH              pMesh;
//		LPD3DXPMESH             pPMesh;
//		LPD3DXPATCHMESH         pPatchMesh;
//	};
//} D3DXMESHDATA, *LPD3DXMESHDATA;