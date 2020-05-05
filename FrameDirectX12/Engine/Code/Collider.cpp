#include "Collider.h"
#include "GraphicDevice.h"
#include "GameObject.h"
#include "Mesh.h"
#include "Shader_DefaultTex.h"
USING(Engine)

CCollider::CCollider(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:CComponent(pGraphicDevice,pCommandList)
{
}

CCollider::CCollider(const CCollider & rhs)
	:CComponent(rhs)
	, m_bIsCollision(rhs.m_bIsCollision), m_eShape(rhs.m_eShape)
	, m_fRadius(rhs.m_fRadius), m_fOriRadius(rhs.m_fOriRadius)
	,m_matScale(rhs.m_matScale), m_matTrans(rhs.m_matTrans)
	,m_pDrawMesh(nullptr)
{

}

HRESULT CCollider::Ready_Collider_ProtoType(const COLLSHAPE & eShape)
{
	return S_OK;
}

void CCollider::Update_Collider(const _matrix * pParentMatrix)
{
	_matrix matRot[3];
	_matrix matRotTot;

	m_bIsCollision = false;


	 matRot[0]=  XMMatrixRotationX(XMConvertToRadians(m_vAngle.x));
	 matRot[1] = XMMatrixRotationY(XMConvertToRadians(m_vAngle.y));
	 matRot[2] = XMMatrixRotationZ(XMConvertToRadians(m_vAngle.z));
	
	 matRotTot = matRot[0] * matRot[1] * matRot[2];

	_matrix matAddWorld;

	matAddWorld = *pParentMatrix;
	matAddWorld._41 += m_vAddPos.x;
	matAddWorld._42 += m_vAddPos.y;
	matAddWorld._43 += m_vAddPos.z;

	m_matWorld =  matRotTot * matAddWorld;

	if (m_eShape == COL_SPHERE)
	{
		_vec3  vScaleMat[3];
		memcpy(&vScaleMat[0], &matAddWorld._11, sizeof(_vec3));
		memcpy(&vScaleMat[1], &matAddWorld._21, sizeof(_vec3));
		memcpy(&vScaleMat[2], &matAddWorld._31, sizeof(_vec3));

		_vec3 vScale = _vec3{ vScaleMat[0].Get_Length(),vScaleMat[1].Get_Length() ,vScaleMat[2].Get_Length() };

		_float fRatio = 0.f;

		fRatio = max(vScale.x, vScale.y);
		fRatio = max(fRatio, vScale.z);

		m_fRadius = m_fOriRadius * fRatio;
	}


}

void CCollider::Render_Collider(CShader_DefaultTex* pShader, _uint offset)
{

	SetOn_ConstantTable(pShader, offset);
	
	_uint uiTexColor = 0;

	if (m_bIsCollision == true)
		uiTexColor = 1;

	pShader->End_Shader(uiTexColor,offset);

	m_pDrawMesh->Begin_Buffer();
	m_pDrawMesh->Render_Buffer();

}

HRESULT CCollider::SetOn_ConstantTable(CShader_DefaultTex* pShader,_uint offset)
{

	_matrix matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	_matrix matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	CB_MATRIX_INFO	tCB_MatrixInfo;
	ZeroMemory(&tCB_MatrixInfo, sizeof(CB_MATRIX_INFO));
	_matrix matScale = m_matScale;



	_matrix matWorld = m_matScale * m_matTrans * m_matWorld;

	if (m_eShape == COL_SPHERE)
	{
		matWorld._11 = m_fRadius;
		matWorld._22 = m_fRadius;
		matWorld._33 = m_fRadius;
	}
	_matrix matWVP = matWorld * matView * matProj;

	XMStoreFloat4x4(&tCB_MatrixInfo.matWVP, XMMatrixTranspose(matWVP));
	XMStoreFloat4x4(&tCB_MatrixInfo.matWorld, XMMatrixTranspose(matWorld));
	XMStoreFloat4x4(&tCB_MatrixInfo.matView, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_MatrixInfo.matProj, XMMatrixTranspose(matProj));



	pShader->Get_UploadBuffer_MatrixInfo()->CopyData(offset, tCB_MatrixInfo);
	


	return S_OK;
}

void CCollider::Free()
{
	CComponent::Free();
	Safe_Release(m_pDrawMesh);
}

