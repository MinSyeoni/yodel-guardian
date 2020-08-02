#pragma once

#include "Component.h"
#include "Shader.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

BEGIN(Engine)
class CMeshComponent :public CComponent
{
public:
	CMeshComponent(const aiScene* scene, ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, _tchar path[MAX_PATH]);
	explicit CMeshComponent(const CMeshComponent& rhs);
	virtual ~CMeshComponent();
public:
	HRESULT            Ready_Texture();
public:
	ID3D12Resource* Create_DefaultBuffer(const void* InitData,
		UINT64 uiByteSize,
		ID3D12Resource*& pUploadBuffer);
private:
	HRESULT InitMesh(int MeshIndex,
		const aiMesh* paiMesh, vector<VTXMESH>& vecVtx, vector<_uint>& vecIndex);
public:
	D3D12_VERTEX_BUFFER_VIEW Get_VertexBufferView(_uint iIndex) const;
	D3D12_INDEX_BUFFER_VIEW  Get_IndexBufferView(_uint iIndex)	const;
	_vec3 Get_MinPos() { return m_vMinPos; };
	_vec3 Get_MaxPos() { return m_vMaxPos; };

	void FoundColliderPosition(_vec3 vtxPos);
	HRESULT Ready_Mesh();
	int Get_SubSetCount() { return m_iSubSetCount; };
public:
	vector< ComPtr<ID3D12Resource>> Get_Texture() { return m_vecResource; };
	vector< ComPtr<ID3D12Resource>> Get_NormalTexture() { return m_vecNormalResource; };
	vector<ComPtr< ID3D12Resource>> Get_SpecularTexture() { return m_vecSpecularResource; };
	vector<ComPtr< ID3D12Resource>> Get_EmissiveTexture() { return m_vecEmissiveResource; };
public:
	void			   Render_Mesh(CShader* pMesh, vector<vector<_matrix>> vecBoneMatrix, _int CBOffset = 0, _int MeshNum = 0, _bool Draw = true);
	void               Render_ShadowMesh(CShader* pMesh, vector<vector<_matrix>> vecBoneMatrix, bool blsBone = false);
	void               Render_LimMesh(CShader* pMesh, vector<vector<_matrix>> vecBoneMatrix, bool blsBone = false);
	void               Render_Destortion(CShader* pShader);
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
	int                     m_iSubSetCount = 0;
	vector<SUBMESH_GEOMETRY> m_vecSubMeshGeometry;
	D3D12_PRIMITIVE_TOPOLOGY m_PrimitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

private:
	_tchar              m_szFilePath[MAX_PATH] = L"";

private:
	vector< ComPtr<ID3D12Resource>> m_vecResource;
	vector< ComPtr<ID3D12Resource>> m_vecUpload;

	vector< ComPtr<ID3D12Resource>> m_vecNormalResource;
	vector< ComPtr<ID3D12Resource>> m_vecNormalUpload;

	vector< ComPtr<ID3D12Resource>> m_vecSpecularResource;
	vector< ComPtr<ID3D12Resource>> m_vecSpecularUpload;


	vector< ComPtr<ID3D12Resource>> m_vecEmissiveResource;
	vector< ComPtr<ID3D12Resource>> m_vecEmissiveUpload;

private:
	const aiScene* m_pScene;
	vector<MeshEntry> m_entries;

	_vec3 m_vMaxPos = _vec3{ 0.f,0.f,0.f };
	_vec3 m_vMinPos = _vec3{ 0.f,0.f,0.f };
};
END