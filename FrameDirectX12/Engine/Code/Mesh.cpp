#include "Mesh.h"

USING(Engine)

CMesh::CMesh(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CComponent(pGraphicDevice, pCommandList)
{
}

CMesh::CMesh(const CMesh & rhs)
	:m_pScene(rhs.m_pScene)
	,m_pMeshComponent(rhs.m_pMeshComponent)
	, m_pAnimationComponent(rhs.m_pAnimationComponent)
{
	m_pMeshComponent->AddRef();
	if(m_pAnimationComponent!=nullptr)
	m_pAnimationComponent->AddRef();
}



vector<vector<_matrix>> CMesh::ExtractBoneTransforms(float fanimationTime, CAniCtrl::STATE eState, _float fAngle)
{
	return m_pAnimationComponent->Extract_BoneTransform(fanimationTime,eState,fAngle);
}


HRESULT CMesh::Ready_Mesh(const _tchar * pFilePath, const _tchar * pFileName)
{
	lstrcpy(m_szFileName, pFilePath);
	lstrcat(m_szFileName, pFileName);
	lstrcpy(m_szFilePath, pFilePath);

	int len = int(wcslen((wchar_t*)m_szFileName));
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)m_szFileName, 2 * len + 1);
	string szFullPath = psz;
	delete[] psz;

	m_pScene = m_Importer.ReadFile(szFullPath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
		| aiPostProcessSteps::aiProcess_FlipWindingOrder);
	
	if (m_pScene->mNumMeshes)
	{
		m_pMeshComponent = new CMeshComponent(m_pScene, m_pGraphicDevice, m_pCommandList, m_szFilePath);

	}
	if (m_pScene->mNumAnimations)
	{
		m_pAnimationComponent = CAniCtrl::Create(m_pScene);
	}

	return S_OK;
}

void CMesh::Render_Mesh(CShader * pMesh,vector<vector<_matrix>> vecBoneMatrix, _int iCBoffset, _int MeshId , bool Draw )
{
	m_pMeshComponent->Render_Mesh(pMesh,vecBoneMatrix, iCBoffset, MeshId,Draw);

}

void CMesh::Render_ShadowMesh(CShader * pMesh, vector<vector<_matrix>> vecBoneMatrix, bool blsBone)
{
	m_pMeshComponent->Render_ShadowMesh(pMesh, vecBoneMatrix, blsBone);
}

void CMesh::Set_Animation(_int Animation)
{
	if (m_pAnimationComponent != nullptr)
		m_pAnimationComponent->Set_AnimationKey(Animation);
}

_matrix * CMesh::Find_BoneMatrix(string strBoneName)
{

	return m_pAnimationComponent->Find_BoneMatrix(strBoneName);
}



CMesh * CMesh::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, const _tchar * pFilePath, const _tchar * pFileName)
{
	CMesh* pInstance = new CMesh(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Mesh(pFilePath, pFileName)))
		Engine::Safe_Release(pInstance);

	return pInstance;

}

CComponent * CMesh::Clone()
{
	return new CMesh(*this);
}

void CMesh::Free()
{
	if(m_pAnimationComponent!=nullptr)
	Safe_Release(m_pAnimationComponent);

	Safe_Release(m_pMeshComponent);

}
