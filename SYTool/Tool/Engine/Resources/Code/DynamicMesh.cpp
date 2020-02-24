#include "DynamicMesh.h"

USING(Engine)

Engine::CDynamicMesh::CDynamicMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
	, m_pLoader(nullptr)
	, m_pAniCtrl(nullptr)
{

}

Engine::CDynamicMesh::CDynamicMesh(const CDynamicMesh& rhs)
	: CMesh(rhs)
	,m_pRootFrame(rhs.m_pRootFrame)
	, m_MeshContainerList(rhs.m_MeshContainerList)
	, m_pLoader(rhs.m_pLoader)

{

	m_pAniCtrl = CAniCtrl::Create(*rhs.m_pAniCtrl);
}

Engine::CDynamicMesh::~CDynamicMesh(void)
{

}

HRESULT Engine::CDynamicMesh::Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName)
{
	_tchar			szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	m_pLoader = CHierarchyLoader::Create(m_pGraphicDev, pFilePath);
	if (nullptr == m_pLoader)
		return E_FAIL;

	LPD3DXANIMATIONCONTROLLER  pAniCtrl = NULL;

	FAILED_CHECK_RETURN(D3DXLoadMeshHierarchyFromX(szFullPath,
		D3DXMESH_MANAGED,
		m_pGraphicDev,
		m_pLoader, // Hierarchy Loader
		NULL,
		&m_pRootFrame,
		&pAniCtrl), // aniCtrl
		E_FAIL);

	m_pAniCtrl = CAniCtrl::Create(pAniCtrl);
	if (nullptr == m_pAniCtrl)
		return E_FAIL;

	Safe_Release(pAniCtrl);

	_matrix			matTemp;
	D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f));

	Update_FrameMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);

	SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)m_pRootFrame);

	return S_OK;
}

void Engine::CDynamicMesh::Render_Mesh(void)
{
	auto	iter = m_MeshContainerList.begin();
	auto	iter_end = m_MeshContainerList.end();

	for (; iter != iter_end; ++iter)
	{
		D3DXMESHCONTAINER_DERIVED*			pMeshContainer = (*iter);

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffeSetMatrix[i] *
				(*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix, nullptr, pSrcVtx, pDestVtx);
		// 소프트웨어 스키닝을 수행하는 함수(스키닝 뿐 아니라 애니메이션 변경 시, 뼈대들과 정점 정보들의 변경을 동시에 수행함)
		// 1. 뼈의 최종 변환 상태
		// 2. 원래 상태로 돌리기 위한 상태 행렬 값(본래는 1번 인자의 역행렬을 모두 구해서 넣어줘야 하지만 안넣어줘도 무관)
		// 3. 변하지 않는 원본의 정점 정보
		// 4. 변환된 정점 정보

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			m_pGraphicDev->SetTexture(0, pMeshContainer->ppTexture[i]);
			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}


}

const D3DXFRAME_DERIVED* Engine::CDynamicMesh::Get_FrameByName(const char* pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

D3DXFRAME_DERIVED * CDynamicMesh::Get_FrameByName2(const char * pFrameName)
{
	return (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pFrameName);
}

_bool Engine::CDynamicMesh::Is_AnimationSetEnd(void)
{

	return m_pAniCtrl->Is_AnimationSetEnd();
}

_bool CDynamicMesh::Is_AnimationSetStart(void)
{
	return m_pAniCtrl->is_AnimationSetStart();
}

void Engine::CDynamicMesh::Render_Mesh(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	auto	iter = m_MeshContainerList.begin();
	auto	iter_end = m_MeshContainerList.end();

	for (; iter != iter_end; ++iter)
	{
		D3DXMESHCONTAINER_DERIVED*			pMeshContainer = (*iter);

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			pMeshContainer->pRenderingMatrix[i] = pMeshContainer->pFrameOffeSetMatrix[i] *
				(*pMeshContainer->ppFrameCombinedMatrix[i]);
		}

		void*		pSrcVtx = nullptr;
		void*		pDestVtx = nullptr;

		pMeshContainer->pOriMesh->LockVertexBuffer(0, &pSrcVtx);
		pMeshContainer->MeshData.pMesh->LockVertexBuffer(0, &pDestVtx);

		pMeshContainer->pSkinInfo->UpdateSkinnedMesh(pMeshContainer->pRenderingMatrix, nullptr, pSrcVtx, pDestVtx);

		for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		{
			pEffect->SetTexture("g_BaseTexture", pMeshContainer->ppTexture[i]);
			if(pMeshContainer->ppNormalTexture[i]!=nullptr)
				pEffect->SetTexture("g_NormalTexture", pMeshContainer->ppNormalTexture[i]);
			else
				pEffect->BeginPass(5);

			pEffect->CommitChanges();		// 변경된 사항을 다시 세팅해주는 함수

			pMeshContainer->MeshData.pMesh->DrawSubset(i);
		}
		pMeshContainer->pOriMesh->UnlockVertexBuffer();
		pMeshContainer->MeshData.pMesh->UnlockVertexBuffer();
	}

	Safe_Release(pEffect);
}

void CDynamicMesh::Set_AnimationSet(const _uint & iIndex)
{
	m_pAniCtrl->Set_AnimationSet(iIndex);
}

void CDynamicMesh::Set_AnimationSetTime(const _uint & iIndex, float fTime)
{
	m_pAniCtrl->Set_AnimationTimeSet(iIndex, fTime);
}

void CDynamicMesh::Play_Animation(const _float & fTimeDelta)
{
	m_pAniCtrl->Play_Animation(fTimeDelta);


	_matrix		matTemp;
	D3DXMatrixRotationY(&matTemp, D3DXToRadian(180.f));

	Update_FrameMatrix((D3DXFRAME_DERIVED*)m_pRootFrame, &matTemp);
}

void CDynamicMesh::TimeReset(_double dTime)
{

	m_pAniCtrl->TimeReset(dTime);


}

void Engine::CDynamicMesh::Update_FrameMatrix(D3DXFRAME_DERIVED* pFrame, const _matrix* pParentMatrix)
{
	if (nullptr == pFrame)
		return;

	pFrame->CombinedTransformMatrix = pFrame->TransformationMatrix * (*pParentMatrix);

	if (nullptr != pFrame->pFrameSibling)
		Update_FrameMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameSibling, pParentMatrix);

	if (nullptr != pFrame->pFrameFirstChild)
		Update_FrameMatrix((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild, &pFrame->CombinedTransformMatrix);


}

CDynamicMesh* Engine::CDynamicMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CDynamicMesh*		pInstance = new CDynamicMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Mesh(pFilePath, pFileName)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CDynamicMesh::Clone(void)
{
	return new CDynamicMesh(*this);
}

void Engine::CDynamicMesh::Free(void)
{
	CMesh::Free();

	Safe_Release(m_pAniCtrl);

	if (false == m_bClone)
	{
		m_pLoader->DestroyFrame(m_pRootFrame);
		Safe_Release(m_pLoader);
	}
	m_MeshContainerList.clear();
}

void Engine::CDynamicMesh::SetUp_FrameMatrixPointer(D3DXFRAME_DERIVED* pFrame)
{
	if (nullptr != pFrame->pMeshContainer)
	{
		D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pFrame->pMeshContainer;

		for (_ulong i = 0; i < pMeshContainer->dwNumBones; ++i)
		{
			const char* pBoneName = pMeshContainer->pSkinInfo->GetBoneName(i);
			D3DXFRAME_DERIVED*		pFrame = (D3DXFRAME_DERIVED*)D3DXFrameFind(m_pRootFrame, pBoneName);
			pMeshContainer->ppFrameCombinedMatrix[i] = &pFrame->CombinedTransformMatrix;
		}

		m_MeshContainerList.push_back(pMeshContainer);
	}

	if (nullptr != pFrame->pFrameSibling)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameSibling);

	if (nullptr != pFrame->pFrameFirstChild)
		SetUp_FrameMatrixPointer((D3DXFRAME_DERIVED*)pFrame->pFrameFirstChild);

}

