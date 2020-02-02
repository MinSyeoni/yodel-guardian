#include "MeshComponent.h"
#include "GraphicDevice.h"

USING(Engine)
CMeshComponent::CMeshComponent(const aiScene * scene, ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList,_tchar path[MAX_PATH])
	:m_pScene(scene),CComponent(pGraphicDevice, pCommandList)
{
	lstrcpy(m_szFilePath, path);
	Ready_Mesh();
}

CMeshComponent::CMeshComponent(const CMeshComponent & rhs)
	: CComponent(rhs)
	, m_vecVB_CPU(rhs.m_vecVB_CPU)
	, m_vecIB_CPU(rhs.m_vecIB_CPU)
	, m_vecVB_GPU(rhs.m_vecVB_GPU)
	, m_vecIB_GPU(rhs.m_vecIB_GPU)
	, m_vecVB_Uploader(rhs.m_vecVB_Uploader)
	, m_vecIB_Uploader(rhs.m_vecIB_Uploader)
	, m_uiVertexByteStride(rhs.m_uiVertexByteStride)
	, m_uiVB_ByteSize(rhs.m_uiVB_ByteSize)
	, m_uiIB_ByteSize(rhs.m_uiIB_ByteSize)
	, m_IndexFormat(rhs.m_IndexFormat)
	, m_vecSubMeshGeometry(rhs.m_vecSubMeshGeometry)
	, m_PrimitiveTopology(rhs.m_PrimitiveTopology)
	, m_entries(rhs.m_entries)
	, m_vecResource(rhs.m_vecResource)
	, m_vecUpload(rhs.m_vecUpload)
{
	for (int i = 0; i < m_entries.size(); i++)
	{
		m_vecIB_GPU[i]->AddRef();
		m_vecVB_GPU[i]->AddRef();

		m_vecIB_Uploader[i]->AddRef();
		m_vecVB_Uploader[i]->AddRef();
	}

	for (auto& pTexture : m_vecResource)
		pTexture->AddRef();


	for (auto& pTexture : m_vecUpload)
		pTexture->AddRef();
}

CMeshComponent::~CMeshComponent()
{

	for (_ulong i = 0; i < m_vecVB_CPU.size(); ++i)
		Safe_Release(m_vecVB_CPU[i]);

	for (_ulong i = 0; i < m_vecIB_CPU.size(); ++i)
		Safe_Release(m_vecIB_CPU[i]);

	for (_ulong i = 0; i < m_vecIB_GPU.size(); ++i)
		Safe_Release(m_vecIB_GPU[i]);

	for (_ulong i = 0; i < m_vecVB_GPU.size(); ++i)
		Safe_Release(m_vecVB_GPU[i]);
	for (_ulong i = 0; i < m_vecIB_Uploader.size(); ++i)
		Safe_Release(m_vecIB_Uploader[i]);

	for (_ulong i = 0; i < m_vecVB_Uploader.size(); ++i)
		Safe_Release(m_vecVB_Uploader[i]);

}

HRESULT CMeshComponent::Ready_Texture()
{


	if (m_pScene->HasMaterials())
	{


		for (_uint i = 0; i < m_pScene->mNumMaterials; i++)
		{
			aiString textureName;//택스처이름
			const aiMaterial* material = m_pScene->mMaterials[i];

			m_entries[i].m_blsTexture = true;

			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &textureName) == AI_SUCCESS)
			{
				wstring strTextureName;//택스처이름
				wstring strPathName;//파일경로
				string  Name = textureName.C_Str();
				strTextureName.assign(Name.begin(), Name.end());

				strPathName = m_szFilePath + strTextureName;


				ComPtr<ID3D12Resource> pResource = nullptr;
				ComPtr<ID3D12Resource> pUpload = nullptr;
				ThrowIfFailed(CreateDDSTextureFromFile12(m_pGraphicDevice, CGraphicDevice::Get_Instance()->Get_CommandListThread(), strPathName.c_str(), pResource, pUpload));


				m_vecResource.push_back(pResource);
				m_vecUpload.push_back(pUpload);
			}
		}

	}


	return S_OK;
}

ID3D12Resource * CMeshComponent::Create_DefaultBuffer(const void * InitData, UINT64 uiByteSize, ID3D12Resource *& pUploadBuffer)
{
	

	ID3D12Resource* pDefaultBuffer = nullptr;

	/*____________________________________________________________________
	실제 기본 버퍼 자원을 생성한다.
	______________________________________________________________________*/
	ThrowIfFailed(DEVICE->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uiByteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&pDefaultBuffer)));

	/*____________________________________________________________________
	CPU 메모리의 자료를 기본 버퍼에 복사하려면, 임시 업로드 힙을 만들어야 한다.
	______________________________________________________________________*/
	ThrowIfFailed(DEVICE->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uiByteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&pUploadBuffer)));

	/*____________________________________________________________________
	기본 버퍼에 복사할 자료를 서술한다.
	______________________________________________________________________*/
	D3D12_SUBRESOURCE_DATA subResourceData = {};
	subResourceData.pData = InitData;
	subResourceData.RowPitch = uiByteSize;
	subResourceData.SlicePitch = subResourceData.RowPitch;

	/*____________________________________________________________________
	기본 버퍼 자원으로의 자료 복사를 요청한다.
	계략적으로 말하자면, 보조 함수 UpdateSubresources는 CPU 메모리를 임시 업로드 힙에 복사하고,
	ID3D12CommandList::CopySubresourceRegion을 이용해서 임시 업로드 힙의 자료를 mBuffer에 복사한다.
	______________________________________________________________________*/
	CGraphicDevice::Get_Instance()->Get_CommandListThread()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(pDefaultBuffer,
			D3D12_RESOURCE_STATE_COMMON,
			D3D12_RESOURCE_STATE_COPY_DEST));
	UpdateSubresources<1>(CGraphicDevice::Get_Instance()->Get_CommandListThread(), pDefaultBuffer, pUploadBuffer, 0, 0, 1, &subResourceData);

	CGraphicDevice::Get_Instance()->Get_CommandListThread()->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(pDefaultBuffer,
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_GENERIC_READ));

	/*____________________________________________________________________
	[ 주의 ]
	위의 함수 호출 이후에도, UploadBuffer를 계속 유지해야 한다.
	실제로 복사를 수행하는 명령 목록이 아직 실행되지 않았기 때문이다.
	복사가 완료되었음이 확실해진 후에 호출자가 UploadBuffer를 해제하면 된다.
	______________________________________________________________________*/


	return pDefaultBuffer;
}

HRESULT CMeshComponent::InitMesh(int MeshIndex, const aiMesh * paiMesh, vector<VTXMESH>& vecVtx, vector<_uint>& vecIndex)
{

		VTXMESH vertex;
		ZeroMemory(&vertex, sizeof(VTXMESH));
		for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
		{
			vertex.Pos.x = paiMesh->mVertices[i].x;
			vertex.Pos.y = paiMesh->mVertices[i].y;
			vertex.Pos.z = paiMesh->mVertices[i].z;

			if (paiMesh->HasNormals())
			{
				vertex.Normal.x = paiMesh->mNormals[i].x;
				vertex.Normal.y = paiMesh->mNormals[i].y;
				vertex.Normal.z = paiMesh->mNormals[i].z;
			}
			if (paiMesh->HasTextureCoords(0))
			{
				vertex.TexCoord.x = paiMesh->mTextureCoords[0][i].x;
				vertex.TexCoord.y = paiMesh->mTextureCoords[0][i].y;
			}
			vecVtx.push_back(vertex);
		}

	
	for (int i = 0; i < paiMesh->mNumFaces; i++)
	{
		aiFace face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		vecIndex.push_back(face.mIndices[2]);
		vecIndex.push_back(face.mIndices[1]);
		vecIndex.push_back(face.mIndices[0]);
	}

	int inumBones = 0;
	for (int i = 0; i < paiMesh->mNumBones; i++)
	{

		unsigned int boneIndex = inumBones;
		inumBones += 1;

		for (int j = 0; j < paiMesh->mBones[i]->mNumWeights; j++)
		{
			unsigned int vertexld = paiMesh->mBones[i]->mWeights[j].mVertexId;
			float weight = paiMesh->mBones[i]->mWeights[j].mWeight;
			if (vertexld == 7)
				int i = 0;
			for (int k = 0; k < 8; k++)
			{
				unsigned int uivectorId = k / 4;
				unsigned int uielementId = k % 4;
				if (uielementId == 0)
				{
					if (vecVtx[vertexld].BoneWeights[uivectorId].x == 0.0f)
					{
						vecVtx[vertexld].BoneIds[uivectorId].x = boneIndex;
						vecVtx[vertexld].BoneWeights[uivectorId].x = weight;
						break;
					}
				}
				if (uielementId == 1)
				{
					if (vecVtx[vertexld].BoneWeights[uivectorId].y == 0.0f)
					{
						vecVtx[vertexld].BoneIds[uivectorId].y = boneIndex;
						vecVtx[vertexld].BoneWeights[uivectorId].y = weight;
						break;
					}

				}
				if (uielementId == 2)
				{
					if (vecVtx[vertexld].BoneWeights[uivectorId].z == 0.0f)
					{
						vecVtx[vertexld].BoneIds[uivectorId].z = boneIndex;
						vecVtx[vertexld].BoneWeights[uivectorId].z = weight;
						break;
					}

				}
				if (uielementId == 3)
				{
					if (vecVtx[vertexld].BoneWeights[uivectorId].w == 0.0f)
					{
						vecVtx[vertexld].BoneIds[uivectorId].w = boneIndex;
						vecVtx[vertexld].BoneWeights[uivectorId].w = weight;
						break;
					}

				}
			}



		}

	}
		return S_OK;
}


HRESULT CMeshComponent::Ready_Mesh()

{
	Engine::CGraphicDevice::Get_Instance()->Begin_ResetCmdListThread();
	if (m_pScene)
	{

		m_entries.resize(m_pScene->mNumMeshes);

		for (size_t i = 0; i < m_entries.size(); i++) {

			_int numVertices = 0;
			_int numIndices = 0;

			m_entries[i].MaterialIndex = m_pScene->mMeshes[i]->mMaterialIndex;
			m_entries[i].NumIndices = m_pScene->mMeshes[i]->mNumFaces * 3;
			m_entries[i].BaseVertex = numVertices;
			m_entries[i].BaseIndex = numIndices;
			numVertices = m_pScene->mMeshes[i]->mNumVertices;
			numIndices = m_entries[i].NumIndices;

			vector<VTXMESH> vecVertex;
			vector<_uint> vecIndex;



			vector<VTXMESH> Vertices;
			Vertices.resize(vecVertex.size());
			const aiMesh* paiMesh = m_pScene->mMeshes[i];
			InitMesh(i, paiMesh, Vertices, vecIndex);


			m_uiIB_ByteSize.resize(m_entries.size());
			m_uiVB_ByteSize.resize(m_entries.size());
			m_vecIB_CPU.resize(m_entries.size());
			m_vecIB_GPU.resize(m_entries.size());
			m_vecVB_CPU.resize(m_entries.size());
			m_vecVB_GPU.resize(m_entries.size());
			m_vecIB_Uploader.resize(m_entries.size());;
			m_vecVB_Uploader.resize(m_entries.size());
			m_vecSubMeshGeometry.resize(m_entries.size());

			const UINT vbByteSize = (UINT)Vertices.size() * sizeof(VTXMESH);
			m_uiVB_ByteSize[i] = vbByteSize;

			const UINT ibByteSize = (UINT)vecIndex.size() * sizeof(_uint);
			m_uiIB_ByteSize[i] = ibByteSize;

			ThrowIfFailed(D3DCreateBlob(vbByteSize, &m_vecVB_CPU[i]));
			CopyMemory(m_vecVB_CPU[i]->GetBufferPointer(), Vertices.data(), vbByteSize);

			ThrowIfFailed(D3DCreateBlob(ibByteSize, &m_vecIB_CPU[i]));
			CopyMemory(m_vecIB_CPU[i]->GetBufferPointer(), vecIndex.data(), ibByteSize);

			m_vecVB_GPU[i] = Create_DefaultBuffer(Vertices.data(), vbByteSize, m_vecVB_Uploader[i]);
			NULL_CHECK_RETURN(m_vecVB_GPU[i], E_FAIL);

			//// IB 생성.
			m_vecIB_GPU[i] = Create_DefaultBuffer(vecIndex.data(), ibByteSize, m_vecIB_Uploader[i]);
			NULL_CHECK_RETURN(m_vecIB_GPU[i], E_FAIL);

			m_uiVertexByteStride = sizeof(VTXMESH);
			m_uiVB_ByteSize[i] = vbByteSize;
			m_uiIB_ByteSize[i] = ibByteSize;
			m_IndexFormat = DXGI_FORMAT_R32_UINT;

			m_vecSubMeshGeometry[i].uiIndexCount = (_uint)vecIndex.size();
			m_vecSubMeshGeometry[i].uiStartIndexLocation = 0;
			m_vecSubMeshGeometry[i].iBaseVertexLocation = 0;

		}

		Ready_Texture();
		CGraphicDevice::Get_Instance()->Wait_ForGpuComplete();
		Engine::CGraphicDevice::Get_Instance()->End_ResetCmdListThread();
		return S_OK;
	}
}

void CMeshComponent::Render_Mesh(CShader * pShader)
{
	for (int i = 0; i < m_entries.size(); i++)
	{
		if (m_entries[i].m_blsTexture == false)
			continue;

		m_pCommandList->IASetVertexBuffers(0, 1, &Get_VertexBufferView(i));
		m_pCommandList->IASetIndexBuffer(&Get_IndexBufferView(i));

		m_pCommandList->IASetPrimitiveTopology(m_PrimitiveTopology);

		pShader->End_Shader(i);


		m_pCommandList->DrawIndexedInstanced(m_vecSubMeshGeometry[i].uiIndexCount,
			1,
			0,
			0,
			0);

	}
}
D3D12_VERTEX_BUFFER_VIEW CMeshComponent::Get_VertexBufferView(_uint iIndex) const
{
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
	VertexBufferView.BufferLocation = m_vecVB_GPU[iIndex]->GetGPUVirtualAddress();
	VertexBufferView.StrideInBytes = m_uiVertexByteStride;
	VertexBufferView.SizeInBytes = m_uiVB_ByteSize[iIndex];

	return VertexBufferView;
}

D3D12_INDEX_BUFFER_VIEW CMeshComponent::Get_IndexBufferView(_uint iIndex) const
{
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	IndexBufferView.BufferLocation = m_vecIB_GPU[iIndex]->GetGPUVirtualAddress();
	IndexBufferView.Format = m_IndexFormat;
	IndexBufferView.SizeInBytes = m_uiIB_ByteSize[iIndex];

	return IndexBufferView;
}