#include "Line.h"
#include "GraphicDevice.h"
USING(Engine)
CLine::CLine(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CComponent(pGraphicDevice,pCommandList)
{
	ZeroMemory(m_vPoint, sizeof(_vec2)*POINT_END);
}

void CLine::RenderLine(CShader_ColorBuffer* pShader)
{
	SetConstantTable(pShader);
	m_pLine->Render_Buffer();
}

void CLine::SetConstantTable(CShader_ColorBuffer* pShader)
{
	_matrix matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	_matrix matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();
	_matrix matWorld = INIT_MATRIX;


	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));


	_matrix matWVP = matWorld * matView * matProj;
	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));

	pShader->Get_UploadBuffer_MatrixInfo()->CopyData(0, tCB_MatrixInfo);

}

HRESULT CLine::Ready_Line(const _vec3 * pPointA, const _vec3 * pPointB)
{
	m_vPoint[POINT_START] = *pPointA;
	m_vPoint[POINT_FINISH] = *pPointB;

	m_vDirection = m_vPoint[POINT_FINISH] - m_vPoint[POINT_START];
	m_vNormal.x = m_vDirection.z*-1.f;
	m_vNormal.y = 0.f;
	m_vNormal.z = m_vDirection.x;
	m_vNormal.Normalize();

	m_pLine = CVlLine::Create(m_pGraphicDevice, m_pCommandList, m_vPoint[POINT_START], m_vPoint[POINT_FINISH], _vec4(1.0f, 0.0f, 0.0f, 1.0f));

	return S_OK;
}

CLine::COMPARE CLine::Compare(const _vec3 * pEndPos)
{
	_vec3 vEndPos = *pEndPos;
	_vec3 vDest = vEndPos - m_vPoint[POINT_START];

	vDest.Normalize();
	_float fResult = vDest.Dot(m_vNormal);

	if (0.f <= fResult)
		return COMPARE_LEFT;
	else
		return COMPARE_RIGHT;
}

_vec2 CLine::Point_Meet(_vec3 * OutPut, _vec3 * pDir)
{
	_float x1 = OutPut->x;
	_float y1 = OutPut->y;
	_float m1;

	if (pDir->x == 0.f)
		m1 = 0.f;
	else
		m1 = pDir->y / pDir->x;

	_float b1 = -(m1 * x1) + y1;

	_float x2 = m_vPoint[POINT_FINISH].x;
	_float y2 = m_vPoint[POINT_FINISH].y;

	_float m2;

	if (m_vDirection.x == 0.f)
		m2 = 0.f;
	else
		m2 = m_vDirection.y / m_vDirection.x;


	_float b2 = -(m2 * x2) + y2;

	_vec2 vResult;
	vResult.x = (b2 - b1) / (m1 - m2);
	vResult.y = m1 * ((b2 - b1) / (m1 - m2)) + b1;

	return vResult;
}

CLine * CLine::Create(const _vec3 * pPointA, const _vec3 * pPointB, ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
{
	CLine*	pInstance = new CLine(pGraphicDevice,pCommandList);

	if (FAILED(pInstance->Ready_Line(pPointA, pPointB)))
		Safe_Release(pInstance);

	return pInstance;
}

void CLine::Free(void)
{
}
