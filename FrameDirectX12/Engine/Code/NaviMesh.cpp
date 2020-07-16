#include "NaviMesh.h"
#include "NaviBuffer.h"
#include "GraphicDevice.h"
CNaviMesh::CNaviMesh(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	:CComponent(pGraphicDevice, pCommandList)
{
}

CNaviMesh::CNaviMesh(const CNaviMesh& rhs)
	: m_vecCell(rhs.m_vecCell)
	, m_dwIndex(rhs.m_dwIndex)
	, m_pNaviBuffer(rhs.m_pNaviBuffer)
{
	for (auto iter : m_vecCell)
		iter->AddRef();

	m_pNaviBuffer->AddRef();
}

CNaviMesh::~CNaviMesh()
{
}

HRESULT CNaviMesh::Ready_NaviMesh()
{
	for (auto iter : m_vecCell)
		Safe_Release(iter);

	m_vecCell.clear();
	m_vecCell.shrink_to_fit();

	_vec3 vecPointA, vecPointB, vecPointC;
	_int iOption = 0;

	// 나중에 파일도 받아올 수 있도록 해야함.
	HANDLE hFile = CreateFile(L"../../../SYTool/Tool/Data/Navi/map1addoutside.dat", GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	NULL_CHECK_RETURN(hFile, E_FAIL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	NAVICELL tNaviData = {};
	CCell* pCell = nullptr;

	while (true)
	{
		ReadFile(hFile, &tNaviData, sizeof(NAVICELL), &dwByte, nullptr);

		if (dwByte == 0)
			break;

		int iPointCnt = 0;

		vecPointA = tNaviData.PointA;
		iPointCnt++;
		vecPointB = tNaviData.PointB;
		iPointCnt++;
		vecPointC = tNaviData.PointC;
		iPointCnt++;

		if (3 == iPointCnt)
		{
			pCell = CCell::Create(m_pGraphicDevice, m_pCommandList, _ulong(m_vecCell.size()), &vecPointA, &vecPointB, &vecPointC, 0);
			m_vecCell.push_back(pCell);

			iPointCnt = 0;
		}
	}
	CloseHandle(hFile);

	vector<CNaviBuffer::NAVIINFO> vecInfo;
	vecInfo.reserve(vecInfo.size());
	for (int i = 0; i < m_vecCell.size(); ++i)
	{
		CNaviBuffer::NAVIINFO tNaviInfo;
		tNaviInfo.vPosition[0] = *m_vecCell[i]->Get_Point(CCell::POINT_A);
		tNaviInfo.vPosition[1] = *m_vecCell[i]->Get_Point(CCell::POINT_B);
		tNaviInfo.vPosition[2] = *m_vecCell[i]->Get_Point(CCell::POINT_C);

		_uint iOption = m_vecCell[i]->Get_Option();
		_vec4 vColor;
		if (iOption == 0)
			vColor = _vec4(1.f, 0.f, 0.f, 1.0f);
		tNaviInfo.vColor = vColor;

		vecInfo.push_back(tNaviInfo);
	}
	m_pNaviBuffer = CNaviBuffer::Create(m_pGraphicDevice, m_pCommandList, vecInfo);
	if (m_pNaviBuffer == nullptr)
		return E_FAIL;

	Link_Cell();

	return S_OK;
}

_vec3 CNaviMesh::MoveOn_NaviMesh(const _vec3* pTargetPos,
	const _vec3* pTargetDir,
	const _float& fSpeed,
	_bool bIsJump)
{
	_vec3 vEndPos = _vec3(*pTargetPos) + _vec3(*pTargetDir) * fSpeed;

	_vec3 vSliding = _vec3(0.f, 0.f, 0.f);
	_vec3 vTargetDir = *pTargetDir;
	_vec3 vTargetPos = *pTargetPos;

	if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex, fSpeed, &vTargetPos, &vTargetDir, &vSliding))
	{
		return vEndPos;
	}
	else if (CCell::COMPARE_SLIDING == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex, fSpeed, &vTargetPos, &vTargetDir, &vSliding))
	{

		vEndPos = _vec3(*pTargetPos) + _vec3(vSliding) * fSpeed;

		return vEndPos;
	}
	else if (CCell::COMPARE_STOP == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex, fSpeed, &vTargetPos, &vTargetDir, &vSliding))
	{
		vEndPos = _vec3(*pTargetPos) + vTargetDir * fSpeed;
		if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex, fSpeed, &vTargetPos, &vTargetDir, &vSliding))
		{

			return vEndPos;
		}
		vEndPos = _vec3(*pTargetPos) + vTargetDir * fSpeed;
		if (CCell::COMPARE_MOVE == m_vecCell[m_dwIndex]->Compare(&vEndPos, &m_dwIndex, fSpeed, &vTargetPos, &vTargetDir, &vSliding))
		{

			return vEndPos;
		}
		_vec3 vA = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_A);
		_vec3 vB = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_B);
		_vec3 vC = *m_vecCell[m_dwIndex]->Get_Point(CCell::POINT_C);

		_vec3 src = _vec3(vA + vB + vC) * 0.3333333f;
		_vec3 dir = src - *pTargetPos;
		dir.Normalize();
		src = _vec3(*pTargetPos) + dir * fSpeed;
		//m_dwCurrentIdx = m_dstIdx;
		return src;

	}



}
HRESULT CNaviMesh::Link_Cell(void)
{
	for (_ulong i = 0; i < m_vecCell.size(); ++i)


	{
		for (_ulong j = 0; j < m_vecCell.size(); ++j)
		{
			if (i == j)
				continue;

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_AB) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_A),
					m_vecCell[i]->Get_Point(CCell::POINT_B),
					m_vecCell[i]))

			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_AB, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_BC) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_B),
					m_vecCell[i]->Get_Point(CCell::POINT_C),
					m_vecCell[i]))

			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_BC, m_vecCell[j]);
				continue;
			}

			if (nullptr == m_vecCell[i]->Get_Neighbor(CCell::NEIGHBOR_CA) &&
				true == m_vecCell[j]->Compare_Point(m_vecCell[i]->Get_Point(CCell::POINT_C),
					m_vecCell[i]->Get_Point(CCell::POINT_A),
					m_vecCell[i]))

			{
				m_vecCell[i]->Set_Neighbor(CCell::NEIGHBOR_CA, m_vecCell[j]);
				continue;
			}
		}
	}

	return S_OK;
}

void CNaviMesh::Render_NaviMesh(CShader_ColorBuffer* pShader)
{
	SetConstantTable(pShader);

	pShader->End_Shader();
	m_pNaviBuffer->Begin_Buffer();
	m_pNaviBuffer->Render_Buffer();

}

void CNaviMesh::SetConstantTable(CShader_ColorBuffer* pShader)
{
	_matrix matWorld = INIT_MATRIX;
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_MATRIX_INFO	tCB_MatrixInfo;

	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));

	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();


	_matrix matWVP = matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));

	pShader->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);



}



CNaviMesh* CNaviMesh::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CNaviMesh* pInstance = new CNaviMesh(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_NaviMesh()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

CComponent* CNaviMesh::Clone()
{
	return new CNaviMesh(*this);
}

void CNaviMesh::Free()
{
	for_each(m_vecCell.begin(), m_vecCell.end(), CDeleteObj());
	m_vecCell.clear();
	Safe_Release(m_pNaviBuffer);
}
