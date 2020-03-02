#include "HierarchyLoader.h"

USING(Engine)

Engine::CHierarchyLoader::CHierarchyLoader(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
	: m_pGraphicDev(pGraphicDev)
	, m_pPath(pPath)
{
	m_pGraphicDev->AddRef();
}

Engine::CHierarchyLoader::~CHierarchyLoader(void)
{
	
}

STDMETHODIMP Engine::CHierarchyLoader::CreateFrame(THIS_ LPCSTR Name, LPD3DXFRAME *ppNewFrame)
{
	D3DXFRAME_DERIVED*		pNewFrame = new D3DXFRAME_DERIVED;
	ZeroMemory(pNewFrame, sizeof(D3DXFRAME_DERIVED));

	Allocate_Name(&pNewFrame->Name, Name);

	D3DXMatrixIdentity(&pNewFrame->TransformationMatrix);

	pNewFrame->CombinedTransformMatrix = pNewFrame->TransformationMatrix;

	*ppNewFrame = pNewFrame;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::CreateMeshContainer(THIS_ LPCSTR Name, 
															CONST D3DXMESHDATA *pMeshData, 
															CONST D3DXMATERIAL *pMaterials, 
															CONST D3DXEFFECTINSTANCE *pEffectInstances, 
															DWORD NumMaterials, 
															CONST DWORD *pAdjacency, 
															LPD3DXSKININFO pSkinInfo, 
															LPD3DXMESHCONTAINER *ppNewMeshContainer)
{

	D3DXMESHCONTAINER_DERIVED*	pNewMeshContainer = new D3DXMESHCONTAINER_DERIVED;
	ZeroMemory(pNewMeshContainer, sizeof(D3DXMESHCONTAINER_DERIVED));

	Allocate_Name(&pNewMeshContainer->Name, Name);

	pNewMeshContainer->MeshData.Type = D3DXMESHTYPE_MESH;

	LPD3DXMESH	pMesh = pMeshData->pMesh;

	_ulong	dwNumFaces = pMesh->GetNumFaces();	// 폴리곤의 개수를 반환하는 함수

	pNewMeshContainer->pAdjacency = new _ulong[dwNumFaces * 3];
	memcpy(pNewMeshContainer->pAdjacency, pAdjacency, sizeof(_ulong) * dwNumFaces * 3);

	_ulong	dwFVF = pMesh->GetFVF();

	if(!(dwFVF & D3DFVF_NORMAL))
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh);
		D3DXComputeNormals(pNewMeshContainer->MeshData.pMesh, pNewMeshContainer->pAdjacency);
	}

	else
	{
		pMesh->CloneMeshFVF(pMesh->GetOptions(), dwFVF, m_pGraphicDev, &pNewMeshContainer->MeshData.pMesh);
	}

	pNewMeshContainer->NumMaterials = (NumMaterials == 0 ? 1 : NumMaterials);

	pNewMeshContainer->pMaterials = new D3DXMATERIAL[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

	pNewMeshContainer->ppTexture = new LPDIRECT3DTEXTURE9[pNewMeshContainer->NumMaterials];
	ZeroMemory(pNewMeshContainer->ppTexture, sizeof(LPDIRECT3DTEXTURE9) * pNewMeshContainer->NumMaterials);

	if (0 != NumMaterials)
	{
		memcpy(pNewMeshContainer->pMaterials, pMaterials, sizeof(D3DXMATERIAL) * pNewMeshContainer->NumMaterials);

		for (_ulong i = 0; i < pNewMeshContainer->NumMaterials; ++i)
		{
			_tchar		szFullPath[128] = L"";
			_tchar		szFileName[128] = L"";

			MultiByteToWideChar(CP_ACP, 0, pNewMeshContainer->pMaterials[i].pTextureFilename, 
				strlen(pNewMeshContainer->pMaterials[i].pTextureFilename), szFileName, 128);

			lstrcpy(szFullPath, m_pPath);
			lstrcat(szFullPath, szFileName);

			if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &pNewMeshContainer->ppTexture[i])))
			{
				MSG_BOX("DynamicMesh`s Texture Create Failed");
				return E_FAIL;
			} 
		}
	}

	else
	{
		pNewMeshContainer->pMaterials[0].MatD3D.Diffuse = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Specular = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Ambient = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 0.f);
		pNewMeshContainer->pMaterials[0].MatD3D.Power = 0.f;

		pNewMeshContainer->ppTexture[0] = nullptr;
	}

	if (nullptr == pSkinInfo)
		return S_OK;

	pNewMeshContainer->pSkinInfo = pSkinInfo;
	pNewMeshContainer->pSkinInfo->AddRef();

	// 다이나믹 메쉬를 렌더링하기 위해 필요한 메쉬 복제
	pNewMeshContainer->MeshData.pMesh->CloneMeshFVF(pNewMeshContainer->MeshData.pMesh->GetOptions(),
													pNewMeshContainer->MeshData.pMesh->GetFVF(), 
													m_pGraphicDev, 
													&pNewMeshContainer->pOriMesh);

	// 이 메쉬의 영향을 미치는 뼈대의 개수를 얻어온다.
	pNewMeshContainer->dwNumBones = pNewMeshContainer->pSkinInfo->GetNumBones();

	// 최초의 행렬상태 값을 저장하기 위해 동적할당
	pNewMeshContainer->pFrameOffeSetMatrix = new _matrix[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->pFrameOffeSetMatrix, sizeof(_matrix) * pNewMeshContainer->dwNumBones);

	// 뼈대들이 갖고있는 CombinedMatrix를 매번 가져와야한다. 그래서 그 행렬들의 주소를 모두 보관하기 위해 동적할당을 하고 첫번째 주소를 통해 모든 행렬을 순회할 수 있게 만들 계획
	pNewMeshContainer->ppFrameCombinedMatrix = new _matrix*[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->ppFrameCombinedMatrix, sizeof(_matrix*) * pNewMeshContainer->dwNumBones);

	pNewMeshContainer->pRenderingMatrix = new _matrix[pNewMeshContainer->dwNumBones];
	ZeroMemory(pNewMeshContainer->pRenderingMatrix, sizeof(_matrix) * pNewMeshContainer->dwNumBones);

	for (_ulong i = 0; i < pNewMeshContainer->dwNumBones; ++i)
		pNewMeshContainer->pFrameOffeSetMatrix[i] = *pNewMeshContainer->pSkinInfo->GetBoneOffsetMatrix(i);
	
	*ppNewMeshContainer = pNewMeshContainer;

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyFrame(THIS_ LPD3DXFRAME pFrameToFree)
{
	Safe_Delete_Array(pFrameToFree->Name);

	if (nullptr != pFrameToFree->pMeshContainer)
		DestroyMeshContainer(pFrameToFree->pMeshContainer);

	if (nullptr != pFrameToFree->pFrameSibling)
		DestroyFrame(pFrameToFree->pFrameSibling);

	if(nullptr != pFrameToFree->pFrameFirstChild)
		DestroyFrame(pFrameToFree->pFrameFirstChild);


	Safe_Delete(pFrameToFree);

	return S_OK;
}

STDMETHODIMP Engine::CHierarchyLoader::DestroyMeshContainer(THIS_ LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	D3DXMESHCONTAINER_DERIVED*		pMeshContainer = (D3DXMESHCONTAINER_DERIVED*)pMeshContainerToFree;

	for (_ulong i = 0; i < pMeshContainer->NumMaterials; ++i)
		Safe_Release(pMeshContainer->ppTexture[i]);

	Safe_Delete_Array(pMeshContainer->ppTexture);

	Safe_Delete_Array(pMeshContainer->pMaterials);
	Safe_Delete_Array(pMeshContainer->pFrameOffeSetMatrix);
	Safe_Delete_Array(pMeshContainer->ppFrameCombinedMatrix);
	Safe_Delete_Array(pMeshContainer->pRenderingMatrix);

	Safe_Delete_Array(pMeshContainer->Name);
	Safe_Delete_Array(pMeshContainer->pAdjacency);

	Safe_Release(pMeshContainer->pSkinInfo);
	Safe_Release(pMeshContainer->MeshData.pMesh);
	Safe_Release(pMeshContainer->pOriMesh);

	Safe_Delete(pMeshContainer);

	return S_OK;
}

void Engine::CHierarchyLoader::Allocate_Name(char** ppName, const char* pFrameName)
{
	if (nullptr == pFrameName)
		return;

	_uint	iLength = strlen(pFrameName);

	*ppName = new char[iLength + 1];

	strcpy_s(*ppName, iLength + 1, pFrameName);

}

CHierarchyLoader* Engine::CHierarchyLoader::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pPath)
{
	CHierarchyLoader*		pInstance = new CHierarchyLoader(pGraphicDev, pPath);

	return pInstance;
}

_ulong Engine::CHierarchyLoader::Release(void)
{
	Engine::Safe_Release(m_pGraphicDev);

	delete this;

	return 0;
}

