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
	vector<_matrix> ExtractBoneTransforms(float fanimationTime, const int animationIndex = 0);
	void			   Render_Mesh(CShader* pMesh);
public:
	vector< ComPtr<ID3D12Resource>> Get_Texture() { return m_pMeshComponent->Get_Texture(); };
	CAnimationControl* Get_AnimationComponent() { return m_pAnimationComponent; };
private:
	const aiScene* m_pScene = nullptr;
	Assimp::Importer m_Importer;
	CMeshComponent*  m_pMeshComponent;
	CAnimationControl* m_pAnimationComponent;

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