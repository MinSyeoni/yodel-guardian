#include "Light.h"
#include "ComponentMgr.h"
#include "GraphicDevice.h"
#include "LightMgr.h"
CLight::CLight(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList)
	:m_pGraphicDevice(pGraphicDevice),m_pCommandList(pCommandList)
{
}

HRESULT CLight::Ready_Light(D3DLIGHT tagLightInfo)
{
	m_tagLightInfo = tagLightInfo;

	m_pBuffer = CRcTex::Create(m_pGraphicDevice, m_pCommandList);

	if(tagLightInfo.m_eType==LIGHTTYPE::D3DLIGHT_DIRECTIONAL)
	m_pShader = dynamic_cast<CShader_LightAcc*>( CComponentMgr::Get_Instance()->Clone_Component(L"Prototype_Shader_LightDirect", ID_STATIC));


	if(tagLightInfo.m_eType==LIGHTTYPE::D3DLIGHT_POINT)
		m_pShader= dynamic_cast<CShader_LightAcc*>(CComponentMgr::Get_Instance()->Clone_Component(L"Prototype_Shader_LightPoint", ID_STATIC));

	m_LightIndex= CLight_Manager::Get_Instance()->Get_LightIndex();
	m_LightIndex += 1;
	return S_OK;
}

void CLight::SetUp_ConstateTable()
{
	_matrix matView = INIT_MATRIX;
	_matrix matProj = INIT_MATRIX;

	CB_LIGHT_INFO	tCB_LightInfo;

	tCB_LightInfo.g_vLightDir = m_tagLightInfo.m_vDirection;
	tCB_LightInfo.g_vLightDiffuse = m_tagLightInfo.m_vDiffuse;
	tCB_LightInfo.g_vLightAmibient = m_tagLightInfo.m_vAmbient;
	tCB_LightInfo.g_vLightSpecular = m_tagLightInfo.m_vSpecular;
	tCB_LightInfo.g_vLightPos = m_tagLightInfo.m_vPosition;
	tCB_LightInfo.g_fRange = m_tagLightInfo.m_fRange;
	

	m_pShader->Get_UploadBuffer_LightInfo()->CopyData(0, tCB_LightInfo);


	matView = CGraphicDevice::Get_Instance()->GetViewMatrix();
	matProj = CGraphicDevice::Get_Instance()->GetProjMatrix();

	matView = XMMatrixInverse(nullptr, matView);
	matProj = XMMatrixInverse(nullptr, matProj);

	_vec4 vCamPos = _vec4(0.f, 0.f, 0.f,0.f);
	memcpy(&vCamPos, &matView._41, sizeof(_vec4));

	CB_INVERSEMATRIX_INFO tCB_InverseInfo;

	XMStoreFloat4x4(&tCB_InverseInfo.matViewInv, XMMatrixTranspose(matView));
	XMStoreFloat4x4(&tCB_InverseInfo.matProjInv, XMMatrixTranspose(matProj));

	tCB_InverseInfo.vCamPos = vCamPos;
	m_pShader->Get_UploadBuffer_InverseInfo()->CopyData(0, tCB_InverseInfo);


}

HRESULT CLight::Render_Light(vector<ComPtr<ID3D12Resource>> pTargetTexture)
{
	if (!m_bIsInit)
	{
		m_pShader->Set_Shader_Texture(pTargetTexture);
		m_bIsInit = true;
		return S_OK;
	}
	SetUp_ConstateTable();

	m_pShader->Begin_Shader();
	m_pBuffer->Begin_Buffer();
	m_pShader->End_Shader(0);
	m_pBuffer->Render_Buffer();


	return S_OK;
}

CLight * CLight::Create(ID3D12Device * pGraphicDevice, ID3D12GraphicsCommandList * pCommandList, D3DLIGHT LightInfo)
{
	CLight* pInstance = new CLight(pGraphicDevice,pCommandList);

	if (FAILED(pInstance->Ready_Light(LightInfo)))
		return nullptr;

	return pInstance;
}

void CLight::Free()
{
	Safe_Release(m_pShader);



	Safe_Release(m_pBuffer);
}
