#include "Mesh.h"

USING(Engine)

CMesh::CMesh(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CComponent(pGraphicDevice, pCommandList)
{
}

CMesh::CMesh(const CMesh & rhs)
	:m_pScene(rhs.m_pScene)
	,m_pMeshComponent(rhs.m_pMeshComponent)
{
	m_pMeshComponent->AddRef();
	if (m_pScene->mNumAnimations)
	{
		m_pAnimationComponent = new CAniCtrl(*rhs.m_pAnimationComponent);
	}
}



vector<vector<_matrix>> CMesh::ExtractBoneTransforms(float fanimationTime)
{
	return m_pAnimationComponent->Extract_BoneTransform(fanimationTime);
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

	m_pScene = m_Importer.ReadFile(szFullPath.c_str(),
		aiProcess_MakeLeftHanded
		| aiPostProcessSteps::aiProcess_FlipWindingOrder
		| aiProcess_FlipUVs);
	
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

vector<vector<_matrix>> CMesh::ExtractBoneTransformsBlend(float fanimationTime, float fanimationTimeSub, _float fAngle)
{
	return m_pAnimationComponent->Extract_BoneBlendingTransform(fanimationTime,fanimationTimeSub,fAngle);
}

void CMesh::Render_Mesh(CShader * pMesh,vector<vector<_matrix>> vecBoneMatrix, _int iCBoffset, _int MeshId , bool Draw )
{
	m_pMeshComponent->Render_Mesh(pMesh,vecBoneMatrix, iCBoffset, MeshId,Draw);

}

void CMesh::Render_ShadowMesh(CShader * pMesh, vector<vector<_matrix>> vecBoneMatrix, bool blsBone)
{
	m_pMeshComponent->Render_ShadowMesh(pMesh, vecBoneMatrix, blsBone);
}

void CMesh::Render_Destortion(CShader * pShader)
{
	m_pMeshComponent->Render_Destortion(pShader);
}

void CMesh::Set_Animation(_int Animation)
{
	if (m_pAnimationComponent != nullptr)
		m_pAnimationComponent->Set_AnimationKey(Animation);
}

void CMesh::Set_AnimationBlend(_int FirstAni, _int SecondAni)
{
	if (m_pAnimationComponent != nullptr)
		m_pAnimationComponent->Set_BlendAnimationKey(FirstAni,SecondAni);
}

_bool CMesh::Set_IsAniFinsh()
{
	return m_pAnimationComponent->Set_IsFinish();
}

_bool CMesh::Set_FindAnimation(_float fTime,_int AniKey)
{
	return m_pAnimationComponent->Set_FindAnimation(fTime,  AniKey);
}

void CMesh::Set_AnimationStart()
{
	m_pAnimationComponent->Set_Start();
	return;
}

_matrix * CMesh::Find_BoneMatrix(string strBoneName)
{

	return m_pAnimationComponent->Find_BoneMatrix(strBoneName);
}

_matrix * CMesh::Find_BoneOffset(string strBoneName)
{
	return m_pAnimationComponent->Find_BoneOffset(strBoneName);
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
