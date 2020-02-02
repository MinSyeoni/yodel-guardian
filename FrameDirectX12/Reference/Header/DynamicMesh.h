#pragma once

#include "Component.h"
#include "Shader.h"

#include "AnimationCotrol.h"
#include "MeshComponent.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

BEGIN(Engine)
class ENGINE_DLL CDynamicMesh : public CComponent
{
	explicit CDynamicMesh(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CDynamicMesh(const CDynamicMesh& rhs);
	virtual ~CDynamicMesh()=default;


	HRESULT         Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
public:
	void			   Render_Mesh(CShader* pMesh);
	HRESULT            Ready_Texture();

public:
	vector< ComPtr<ID3D12Resource>> Get_Texture() { return m_pMeshComponent->Get_Texture(); };

private:
	const aiScene* m_pScene = nullptr;
	Assimp::Importer m_Importer;
	CMeshComponent*  m_pMeshComponent;
	CAnimationComponent* m_pAnimationComponent;

private:
	_tchar				m_szFileName[MAX_PATH] = L"";
	_tchar              m_szFilePath[MAX_PATH] = L"";
public:
	static CDynamicMesh* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const _tchar* pFilePath, const _tchar* pFileName);
public:
	virtual CComponent* Clone();
private:
	virtual void		Free();
};
END