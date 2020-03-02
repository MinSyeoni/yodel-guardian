#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{

}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	:CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pSubsets(rhs.m_pSubsets)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_dwNumVtx(rhs.m_dwNumVtx)
	, m_dwStride(rhs.m_dwStride)
{
	m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];

	for(_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		m_ppTextures[i]->AddRef();
	}

	m_pAdjacency->AddRef();
	m_pSubsetBuffer->AddRef();
	m_pMesh->AddRef();
	m_pOriMesh->AddRef();
}

Engine::CStaticMesh::~CStaticMesh(void)
{

}

HRESULT Engine::CStaticMesh::Ready_Mesh(const _tchar* pFilePath, 
										const _tchar* pFileName)
{
	_tchar		szFullPath[128] = L"";

	lstrcpy(szFullPath, pFilePath);
	lstrcat(szFullPath, pFileName);

	FAILED_CHECK_RETURN(D3DXLoadMeshFromX(szFullPath, 
											D3DXMESH_MANAGED, 
											m_pGraphicDev,
											&m_pAdjacency,
											&m_pSubsetBuffer, 
											NULL,
											&m_dwSubsetCnt, 
											&m_pOriMesh), 
											E_FAIL);

	_ulong	dwFVF = m_pOriMesh->GetFVF();

	if (!(dwFVF & D3DFVF_NORMAL))
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF | D3DFVF_NORMAL, m_pGraphicDev, &m_pMesh);

		// 메쉬의 모든 정점을 순회하면서 normal을 계산해서 삽입해주는 함수
		D3DXComputeNormals(m_pMesh, (_ulong*)m_pAdjacency->GetBufferPointer());
	}
	else
	{
		m_pOriMesh->CloneMeshFVF(m_pOriMesh->GetOptions(), dwFVF, m_pGraphicDev, &m_pMesh);
	}

	void*		pVertex = NULL;

	m_dwNumVtx = m_pMesh->GetNumVertices();
	m_pVtxPos = new _vec3[m_dwNumVtx];

	m_pMesh->LockVertexBuffer(0, &pVertex);

	D3DVERTEXELEMENT9		Decl[MAX_FVF_DECL_SIZE];
	ZeroMemory(&Decl, sizeof(D3DVERTEXELEMENT9) * MAX_FVF_DECL_SIZE);

	m_pMesh->GetDeclaration(Decl);

	_ubyte byOffset = 0;

	for (_ulong i = 0; i < MAX_FVF_DECL_SIZE; ++i)
	{
		if (Decl[i].Usage == D3DDECLUSAGE_POSITION)
		{
			byOffset = (_ubyte)Decl[i].Offset;
			break;
		}
	}

	m_dwStride = D3DXGetFVFVertexSize(dwFVF);

	for (_ulong i = 0; i < m_dwNumVtx; ++i)
	{
		m_pVtxPos[i] = *((_vec3*)(((_ubyte*)pVertex) + (m_dwStride * i + byOffset)));
	}	

	// d3dxbuffer가 담고 있는 정점의 가장 앞 주소를 리턴시켜 보관
	m_pSubsets = (D3DXMATERIAL*)m_pSubsetBuffer->GetBufferPointer();

	m_ppTextures = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_tchar		szFileName[128] = L"";

		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP, 
							0, 
							m_pSubsets[i].pTextureFilename, 
							strlen(m_pSubsets[i].pTextureFilename), 
							szFileName, 
							128);

		lstrcat(szFullPath, szFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
			return E_FAIL;

	}

	m_pMesh->UnlockVertexBuffer();
	
	return S_OK;
}

void Engine::CStaticMesh::Render_Mesh()
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);
	}
}

void Engine::CStaticMesh::Render_Mesh(LPD3DXEFFECT pEffect)
{
	pEffect->AddRef();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		_bool		bAlpha = false;
		_uint		iPassCnt = 0;

		if (bAlpha = Find_Alpha(m_pSubsets[i].pTextureFilename))
			iPassCnt = 1;


		pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
		pEffect->CommitChanges();

		pEffect->BeginPass(iPassCnt);

		m_pMesh->DrawSubset(i);

		pEffect->EndPass();
	}

	Safe_Release(pEffect);
}

CStaticMesh* Engine::CStaticMesh::Create(LPDIRECT3DDEVICE9 pGraphicDev, const _tchar* pFilePath, const _tchar* pFileName)
{
	CStaticMesh*	pInstance = new CStaticMesh(pGraphicDev);

	if (FAILED(pInstance->Ready_Mesh(pFilePath, pFileName)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CResources* Engine::CStaticMesh::Clone(void)
{
	return new CStaticMesh(*this);
}

void Engine::CStaticMesh::Free(void)
{
	CMesh::Free();

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppTextures[i]);

	Safe_Delete_Array(m_ppTextures);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Release(m_pOriMesh);
	Safe_Release(m_pMesh);
	Safe_Release(m_pSubsetBuffer);
	Safe_Release(m_pAdjacency);



}

_bool Engine::CStaticMesh::Find_Alpha(const char* pFileName)
{
	_uint	iLength = strlen(pFileName);

	for (_uint i = 0; i < iLength + 1; ++i)
	{
		if (pFileName[i] == '.')
		{
			if (pFileName[i - 1] == 'A')
				return true;
		}
	}

	return false;

}

