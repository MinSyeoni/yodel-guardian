#pragma once
#include "Component.h"
#include "Shader.h"

#include "AnimationControl.h"
#include "MeshComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

	BEGIN(Engine)

	class ENGINE_DLL CMesh : public CComponent
{
	explicit CMesh(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CMesh(const CMesh& rhs);
	virtual ~CMesh() = default;

	HRESULT         Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
public:
	vector<vector<_matrix>> ExtractBoneTransforms(float fanimationTime);
	vector<vector<_matrix>> ExtractBoneTransformsBlend(float fanimationTime,float fanimationTimeSub, _float fAngle = 0.f);

	void			   Render_Mesh(CShader* pMesh, vector<vector<_matrix>> vecBoneMatrix= vector<vector<_matrix>>(),_int iCBoffset=0,_int MeshId=0,bool Draw =true);
	void               Render_ShadowMesh(CShader* pMesh, vector<vector<_matrix>> vecBoneMatrix = vector<vector<_matrix>>(), bool blsBone = false);
	void               Render_Destortion(CShader* pMesh);


	void Set_Animation(_int Animation);
	void Set_AnimationBlend(_int FirstAni, _int SecondAni);

	_bool Set_IsAniFinsh();
	void  Set_AnimationStart();

	_matrix*           Get_RootFrame() { return m_pAnimationComponent->Get_RootFrame(); };
public:
	_matrix*            Find_BoneMatrix(string strBoneName);
	_matrix*            Find_BoneOffset(string strBoneName);

public:
	vector< ComPtr<ID3D12Resource>> Get_Texture() { return m_pMeshComponent->Get_Texture(); };
	vector< ComPtr<ID3D12Resource>> Get_NormalTexture() { return m_pMeshComponent->Get_NormalTexture(); };
	vector< ComPtr<ID3D12Resource>> Get_SpecularTexture() { return m_pMeshComponent->Get_SpecularTexture(); };
	vector< ComPtr<ID3D12Resource>> Get_EmissiveTexture() { return m_pMeshComponent->Get_EmissiveTexture(); };

	CMeshComponent* Get_MeshComponent() { return m_pMeshComponent; };
	CAniCtrl* Get_AnimationComponent() { return m_pAnimationComponent; };
private:
	const aiScene* m_pScene = nullptr;
	Assimp::Importer m_Importer;
	CMeshComponent*  m_pMeshComponent;
	CAniCtrl* m_pAnimationComponent;

private:
	_tchar				m_szFileName[MAX_PATH] = L"";
	_tchar              m_szFilePath[MAX_PATH] = L"";
public:
	static CMesh* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const _tchar* pFilePath, const _tchar* pFileName);
public:
	virtual CComponent* Clone();
private:
	virtual void		Free();
};
END