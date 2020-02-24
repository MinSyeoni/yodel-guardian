#include "StaticMesh.h"

USING(Engine)

Engine::CStaticMesh::CStaticMesh(LPDIRECT3DDEVICE9 pGraphicDev)
	: CMesh(pGraphicDev)
{
	m_bClone = false;
}

Engine::CStaticMesh::CStaticMesh(const CStaticMesh& rhs)
	: CMesh(rhs)
	, m_pMesh(rhs.m_pMesh)
	, m_pAdjacency(rhs.m_pAdjacency)
	, m_pSubsetBuffer(rhs.m_pSubsetBuffer)
	, m_pSubsets(rhs.m_pSubsets)
	, m_dwSubsetCnt(rhs.m_dwSubsetCnt)
	, m_pOriMesh(rhs.m_pOriMesh)
	, m_pVtxPos(rhs.m_pVtxPos)
	, m_dwNumVtx(rhs.m_dwNumVtx)
	, m_dwStride(rhs.m_dwStride)
	,m_pTestureName(rhs.m_pTestureName)
	
{
	m_ppTextures = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	m_ppNomalTexture = new LPDIRECT3DTEXTURE9[rhs.m_dwSubsetCnt];
	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppTextures[i] = rhs.m_ppTextures[i];
		m_ppTextures[i]->AddRef();
	}
	for (_ulong i = 0; i < rhs.m_dwSubsetCnt; ++i)
	{
		m_ppNomalTexture[i] = rhs.m_ppNomalTexture[i];
		if(m_ppNomalTexture[i]!=nullptr)
		m_ppNomalTexture[i]->AddRef();
	}



	m_bClone = true;
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

	if (!(dwFVF & D3DFVF_TEX0))
	{
		int i = 0;
	}

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
	m_ppNomalTexture = new LPDIRECT3DTEXTURE9[m_dwSubsetCnt];

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		wstring      wstrNomal = L"";
		_tchar*		pszFileName = new TCHAR[128];
		_tchar  szNomalFileName[128] = L"";
		_tchar Temp[128] = L"";
		lstrcpy(szFullPath, pFilePath);

		MultiByteToWideChar(CP_ACP,
			0,
			m_pSubsets[i].pTextureFilename,
			strlen(m_pSubsets[i].pTextureFilename),
			Temp,
			128);


		lstrcpy(pszFileName, Temp);
		lstrcat(szFullPath, pszFileName);
		m_pTestureName.push_back(pszFileName);

		if (FAILED(D3DXCreateTextureFromFile(m_pGraphicDev, szFullPath, &m_ppTextures[i])))
		{
			m_ppTextures[i] = nullptr;
		
		}

	}

	m_pMesh->UnlockVertexBuffer();

	return S_OK;
}

void Engine::CStaticMesh::Render_Mesh(LPD3DXEFFECT pEffect,int iPass)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		
      pEffect->SetTexture("g_NormalTexture", m_ppNomalTexture[i]);
		pEffect->SetTexture("g_BaseTexture", m_ppTextures[i]);
		pEffect->CommitChanges();
		m_pMesh->DrawSubset(i);
	}


}

void CStaticMesh::Render_Mesh()
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);

	}



}

void CStaticMesh::Render_LightMesh(LPD3DXEFFECT pEffect, wstring temp)
{
	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
	{
		if (m_pTestureName[i] == temp)
			pEffect->BeginPass(4);
		else
			pEffect->BeginPass(0);


		m_pGraphicDev->SetTexture(0, m_ppTextures[i]);
		m_pMesh->DrawSubset(i);

		pEffect->EndPass();

	}


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

	for (_ulong i = 0; i < m_dwSubsetCnt; ++i)
		Safe_Release(m_ppNomalTexture[i]);
	if (m_bClone == false)
	{
		for (auto& pSrc : m_pTestureName)
		{
			delete[] pSrc;
		}
	}

	Safe_Delete_Array(m_ppTextures);

	if (false == m_bClone)
		Safe_Delete_Array(m_pVtxPos);

	Safe_Release(m_pOriMesh);
	Safe_Release(m_pMesh);
	Safe_Release(m_pSubsetBuffer);
	Safe_Release(m_pAdjacency);



}

