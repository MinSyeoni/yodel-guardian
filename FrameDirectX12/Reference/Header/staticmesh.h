#pragma once

#include "Component.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

BEGIN(Engine)
class ENGINE_DLL CStaticMesh : public CComponent
{
private:
	explicit CStaticMesh(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CStaticMesh(const CStaticMesh& rhs);
	virtual ~CStaticMesh() = default;

	HRESULT            Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName);
	HRESULT            Ready_Texture();
public:
	void			   Render_Mesh(CShader* pMesh, vector<vector<_matrix>> vecBoneMatrix);
public:
	ID3D12Resource* Create_DefaultBuffer(const void* InitData,
		UINT64 uiByteSize,
		ID3D12Resource*& pUploadBuffer);

	void InitMesh(int MeshIndex,
		const aiMesh* paiMesh,
		std::vector<_vec3>& vecPositions,
		std::vector<_vec3>& vecNormals,
		std::vector<_vec2>& vecTexCoords,
		std::vector<unsigned int>& _uiIndices);
	
public:
	D3D12_VERTEX_BUFFER_VIEW Get_VertexBufferView(_uint iIndex) const;
	D3D12_INDEX_BUFFER_VIEW  Get_IndexBufferView(_uint iIndex)	const;
public:
	vector< ComPtr<ID3D12Resource>> Get_Texture() { return m_vecResource; };
	 
private:
	const aiScene* m_pScene = nullptr;
	Assimp::Importer m_Importer;
	vector<MeshEntry> m_entries;
	

private:
	vector<ID3DBlob*>		m_vecVB_CPU;
	vector<ID3DBlob*>		m_vecIB_CPU;

	vector<ID3D12Resource*> m_vecVB_GPU;
	vector<ID3D12Resource*> m_vecIB_GPU;

	vector<ID3D12Resource*> m_vecVB_Uploader;
	vector<ID3D12Resource*> m_vecIB_Uploader;
private:
	_uint			        m_uiVertexByteStride = 0;
	vector<_uint>			m_uiVB_ByteSize;
	vector<_uint>			m_uiIB_ByteSize;
	DXGI_FORMAT		        m_IndexFormat = DXGI_FORMAT_R32_UINT;

	vector<SUBMESH_GEOMETRY> m_vecSubMeshGeometry;
	D3D12_PRIMITIVE_TOPOLOGY m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;


private:
	vector< ComPtr<ID3D12Resource>> m_vecResource;
	vector< ComPtr<ID3D12Resource>> m_vecUpload;

private:
	_tchar					m_szFileName[MAX_PATH] = L"";
	_tchar                  m_szFilePath[MAX_PATH] = L"";
public:
	static CStaticMesh* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList,const _tchar* pFilePath,const _tchar* pFileName, const _uint& iCnt = 1);
public:
	virtual CComponent* Clone();

private:
	virtual void		Free();


};
END