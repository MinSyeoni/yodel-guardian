#include "CubeTex.h"

USING(Engine)

Engine::CCubeTex::CCubeTex(LPDIRECT3DDEVICE9 pGraphicDev)
	: CVIBuffer(pGraphicDev)
{

}

Engine::CCubeTex::CCubeTex(const CCubeTex& rhs)
	:CVIBuffer(rhs)
{

}

Engine::CCubeTex::~CCubeTex(void)
{

}

HRESULT Engine::CCubeTex::Ready_Buffer(void)
{
	m_dwTriCnt = 12;
	m_dwVtxCnt = 8;
	m_dwVtxFVF = FVF_CUBE;
	m_dwVtxSize = sizeof(VTXCUBE);
	
	m_dwIdxSize = sizeof(INDEX32);
	m_IdxFmt = D3DFMT_INDEX32;
	
	FAILED_CHECK(CVIBuffer::Ready_Buffer());

	VTXCUBE*		pVtxCube = nullptr;

	m_pVB->Lock(0, 0, (void**)&pVtxCube, 0);

	pVtxCube[0].vPos = D3DXVECTOR3(-0.5f, 0.5f, -0.5f);
	pVtxCube[0].vTex = pVtxCube[0].vPos;

	pVtxCube[1].vPos = D3DXVECTOR3(0.5f, 0.5f, -0.5f);
	pVtxCube[1].vTex = pVtxCube[1].vPos;

	pVtxCube[2].vPos = D3DXVECTOR3(0.5f, -0.5f, -0.5f);
	pVtxCube[2].vTex = pVtxCube[2].vPos;

	pVtxCube[3].vPos = D3DXVECTOR3(-0.5f, -0.5f, -0.5f);
	pVtxCube[3].vTex = pVtxCube[3].vPos;


	pVtxCube[4].vPos = D3DXVECTOR3(-0.5f, 0.5f, 0.5f);
	pVtxCube[4].vTex = pVtxCube[4].vPos;

	pVtxCube[5].vPos = D3DXVECTOR3(0.5f, 0.5f, 0.5f);
	pVtxCube[5].vTex = pVtxCube[5].vPos;

	pVtxCube[6].vPos = D3DXVECTOR3(0.5f, -0.5f, 0.5f);
	pVtxCube[6].vTex = pVtxCube[6].vPos;

	pVtxCube[7].vPos = D3DXVECTOR3(-0.5f, -0.5f, 0.5f);
	pVtxCube[7].vTex = pVtxCube[7].vPos;

	m_pVB->Unlock();

	INDEX32*	pIndex = NULL;

	m_pIB->Lock(0, 0, (void**)&pIndex, 0);

	// +x
	pIndex[0]._0 = 1;
	pIndex[0]._1 = 5;
	pIndex[0]._2 = 6;

	pIndex[1]._0 = 1;
	pIndex[1]._1 = 6;
	pIndex[1]._2 = 2;

	// -x
	pIndex[2]._0 = 4;
	pIndex[2]._1 = 0;
	pIndex[2]._2 = 3;

	pIndex[3]._0 = 4;
	pIndex[3]._1 = 3;
	pIndex[3]._2 = 7;


	// +y
	pIndex[4]._0 = 4;
	pIndex[4]._1 = 5;
	pIndex[4]._2 = 1;

	pIndex[5]._0 = 4;
	pIndex[5]._1 = 1;
	pIndex[5]._2 = 0;

	// -y	 
	pIndex[6]._0 = 3;
	pIndex[6]._1 = 2;
	pIndex[6]._2 = 6;

	pIndex[7]._0 = 3;
	pIndex[7]._1 = 6;
	pIndex[7]._2 = 7;

	// +z
	pIndex[8]._0 = 7;
	pIndex[8]._1 = 6;
	pIndex[8]._2 = 5;

	pIndex[9]._0 = 7;
	pIndex[9]._1 = 5;
	pIndex[9]._2 = 4;

	// -z
	pIndex[10]._0 = 0;
	pIndex[10]._1 = 1;
	pIndex[10]._2 = 2;

	pIndex[11]._0 = 0;
	pIndex[11]._1 = 2;
	pIndex[11]._2 = 3;

	m_pIB->Unlock();

	return S_OK;
}

CCubeTex* Engine::CCubeTex::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CCubeTex*		pInstance = new CCubeTex(pGraphicDev);

	if (FAILED(pInstance->Ready_Buffer()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void Engine::CCubeTex::Free(void)
{
	CVIBuffer::Free();
}

Engine::CResources* Engine::CCubeTex::Clone(void)
{
	return new CCubeTex(*this);
}

