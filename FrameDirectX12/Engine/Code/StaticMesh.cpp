#include "StaticMesh.h"
#include "GraphicDevice.h"

USING(Engine)

CStaticMesh::CStaticMesh(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CComponent(pGraphicDevice,pCommandList)
{

}

CStaticMesh::CStaticMesh(const CStaticMesh & rhs)
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
	,m_entries(rhs.m_entries)
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

HRESULT CStaticMesh::Ready_Mesh(const _tchar* pFilePath, const _tchar* pFileName)
{
	_bool ret = false;


	lstrcpy(m_szFileName, pFilePath);
	lstrcat(m_szFileName, pFileName);

	lstrcpy(m_szFilePath, pFilePath);
   
	int len = wcslen((wchar_t*)m_szFileName);
	char* psz = new char[2 * len + 1];
	wcstombs(psz, (wchar_t*)m_szFileName, 2 * len + 1);
	string szFullPath = psz;
	delete[] psz;

	m_pScene = m_Importer.ReadFile(szFullPath.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals
		| aiProcess_FlipUVs
		| aiProcess_JoinIdenticalVertices
		| aiPostProcessSteps::aiProcess_FlipWindingOrder);

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


			vector<_vec3> vecPositions;
			vector<_vec3> vecNormals;
			vector<_vec2> vecTexCoords;
			vector<_uint> vecIndices;

			vecPositions.reserve(numVertices);
			vecNormals.reserve(numVertices);
			vecTexCoords.reserve(numVertices);
			vecIndices.reserve(numIndices);

			const aiMesh* paiMesh = m_pScene->mMeshes[i];
			InitMesh(i, paiMesh, vecPositions, vecNormals, vecTexCoords, vecIndices);

			vector<VTXTEX> Vertices;
			Vertices.resize(vecPositions.size());


			for (size_t i = 0; i < vecPositions.size(); i++)
			{

				VTXTEX vertex;
				_vec3 pos = vecPositions[i];
				vertex.vPos = _vec3(pos.x, pos.y, pos.z);
				_vec3 norm = vecNormals[i];
				vertex.vNormal = _vec3(norm.x, norm.y, norm.z);
				_vec2 tex = vecTexCoords[i];
				vertex.vTexUV = _vec2(tex.x, tex.y);
				Vertices[i] = vertex;

			}

			m_uiIB_ByteSize.resize(m_entries.size());
			m_uiVB_ByteSize.resize(m_entries.size());
			m_vecIB_CPU.resize(m_entries.size());
			m_vecIB_GPU.resize(m_entries.size());
			m_vecVB_CPU.resize(m_entries.size());
			m_vecVB_GPU.resize(m_entries.size());
			m_vecIB_Uploader.resize(m_entries.size());;
			m_vecVB_Uploader.resize(m_entries.size());
			m_vecSubMeshGeometry.resize(m_entries.size());

			const UINT vbByteSize = (UINT)Vertices.size() * sizeof(VTXTEX);
			m_uiVB_ByteSize[i] = vbByteSize;

			const UINT ibByteSize = (UINT)vecIndices.size() * sizeof(_uint);
			m_uiIB_ByteSize[i] = ibByteSize;

			ThrowIfFailed(D3DCreateBlob(vbByteSize, &m_vecVB_CPU[i]));
			CopyMemory(m_vecVB_CPU[i]->GetBufferPointer(), Vertices.data(), vbByteSize);

			ThrowIfFailed(D3DCreateBlob(ibByteSize, &m_vecIB_CPU[i]));
			CopyMemory(m_vecIB_CPU[i]->GetBufferPointer(), vecIndices.data(), ibByteSize);

			m_vecVB_GPU[i] = Create_DefaultBuffer(Vertices.data(), vbByteSize, m_vecVB_Uploader[i]);
			NULL_CHECK_RETURN(m_vecVB_GPU[i], E_FAIL);

			//// IB 생성.
			m_vecIB_GPU[i] = Create_DefaultBuffer(vecIndices.data(), ibByteSize, m_vecIB_Uploader[i]);
			NULL_CHECK_RETURN(m_vecIB_GPU[i], E_FAIL);

			m_uiVertexByteStride = sizeof(VTXTEX);
			m_uiVB_ByteSize[i] = vbByteSize;
			m_uiIB_ByteSize[i] = ibByteSize;
			m_IndexFormat = DXGI_FORMAT_R32_UINT;

			m_vecSubMeshGeometry[i].uiIndexCount = (_uint)vecIndices.size();
			m_vecSubMeshGeometry[i].uiStartIndexLocation = 0;
			m_vecSubMeshGeometry[i].iBaseVertexLocation = 0;

		}
		
		Ready_Texture();

		return S_OK;
	}
	else
		return E_FAIL;


}

HRESULT CStaticMesh::Ready_Texture()
{
	Engine::CGraphicDevice::Get_Instance()->Begin_ResetCmdListThread();

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
				
				strPathName = m_szFilePath+strTextureName;


				ComPtr<ID3D12Resource> pResource = nullptr;
				ComPtr<ID3D12Resource> pUpload = nullptr;
				ThrowIfFailed(CreateDDSTextureFromFile12(m_pGraphicDevice, CGraphicDevice::Get_Instance()->Get_CommandListThread(),strPathName.c_str(), pResource, pUpload));


				m_vecResource.push_back(pResource);
				m_vecUpload.push_back(pUpload);
			}
		}

	}


	Engine::CGraphicDevice::Get_Instance()->End_ResetCmdListThread();

	return S_OK;
}



void CStaticMesh::Render_Mesh(CShader* pShader)
{
	for (int i = 0; i < m_entries.size(); i++)
	{
		if (m_entries[i].m_blsTexture == false)
			continue;

		m_pCommandList->IASetVertexBuffers(0,1,&Get_VertexBufferView(i));
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

ID3D12Resource * CStaticMesh::Create_DefaultBuffer(const void * InitData, UINT64 uiByteSize, ID3D12Resource *& pUploadBuffer)
{
	Engine::CGraphicDevice::Get_Instance()->Begin_ResetCmdListThread();

	ID3D12Resource* pDefaultBuffer = nullptr;

	/*____________________________________________________________________
	실제 기본 버퍼 자원을 생성한다.
	______________________________________________________________________*/
	ThrowIfFailed(m_pGraphicDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uiByteSize),
		D3D12_RESOURCE_STATE_COMMON,
		nullptr,
		IID_PPV_ARGS(&pDefaultBuffer)));

	/*____________________________________________________________________
	CPU 메모리의 자료를 기본 버퍼에 복사하려면, 임시 업로드 힙을 만들어야 한다.
	______________________________________________________________________*/
	ThrowIfFailed(m_pGraphicDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
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
	CGraphicDevice::Get_Instance()->Get_CommandListThread() ->ResourceBarrier(1,
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
	CGraphicDevice::Get_Instance()->Wait_ForGpuComplete();
	Engine::CGraphicDevice::Get_Instance()->End_ResetCmdListThread();

	return pDefaultBuffer;
}

void CStaticMesh::InitMesh(int MeshIndex, const aiMesh * paiMesh, std::vector<_vec3>& vecPositions, std::vector<_vec3>& vecNormals, std::vector<_vec2>& vecTexCoords, std::vector<unsigned int>& uiIndices)
{
	const aiVector3D Zero3D(0.0f, 0.0f, 0.0f);

	// Populate the vertex attribute vectors
	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++) {
		const aiVector3D* pPos = &(paiMesh->mVertices[i]);
		const aiVector3D* pNormal = &(paiMesh->mNormals[i]);
		const aiVector3D* pTexCoord = paiMesh->HasTextureCoords(0) ? &(paiMesh->mTextureCoords[0][i]) : &Zero3D;

		vecPositions.push_back(_vec3(pPos->x, pPos->y, pPos->z));
		vecNormals.push_back(_vec3(pNormal->x, pNormal->y, pNormal->z));
		vecTexCoords.push_back(_vec2(pTexCoord->x, pTexCoord->y));
	}

	// Populate the index buffer
	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++) {
		const aiFace& face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		uiIndices.push_back(face.mIndices[2]);
		uiIndices.push_back(face.mIndices[1]);
		uiIndices.push_back(face.mIndices[0]);
	}

}

D3D12_VERTEX_BUFFER_VIEW CStaticMesh::Get_VertexBufferView(_uint iIndex) const
{
	D3D12_VERTEX_BUFFER_VIEW VertexBufferView;
	VertexBufferView.BufferLocation = m_vecVB_GPU[iIndex]->GetGPUVirtualAddress();
	VertexBufferView.StrideInBytes = m_uiVertexByteStride;
	VertexBufferView.SizeInBytes = m_uiVB_ByteSize[iIndex];

	return VertexBufferView;
}

D3D12_INDEX_BUFFER_VIEW CStaticMesh::Get_IndexBufferView(_uint iIndex) const
{
	D3D12_INDEX_BUFFER_VIEW IndexBufferView;
	IndexBufferView.BufferLocation = m_vecIB_GPU[iIndex]->GetGPUVirtualAddress();
	IndexBufferView.Format = m_IndexFormat;
	IndexBufferView.SizeInBytes = m_uiIB_ByteSize[iIndex];

	return IndexBufferView;
}

CStaticMesh * CStaticMesh::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, const _tchar* pFilePath, const _tchar* pFileName, const _uint & iCnt)
{
	CStaticMesh* pInstance = new CStaticMesh(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_Mesh( pFilePath,pFileName)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CComponent * CStaticMesh::Clone()
{
	return new CStaticMesh(*this);
}

void CStaticMesh::Free()
{

	CComponent::Free();


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
