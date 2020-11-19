#include "stdafx.h"
#include "Effect.h"
#include "EfffectComponent.h"
CEffect::CEffect(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList)
	:CGameObject(pGraphicDevice,pCommandList)

{
}

CEffect::CEffect(const CEffect& rhs)
	: CGameObject(rhs)
	,m_vecEffectData(rhs.m_vecEffectData)
{

}


CEffect::~CEffect()
{

}

HRESULT CEffect::Ready_GameObjectPrototype(wstring strPath)
{

	HANDLE hFile = CreateFile(strPath.c_str(), GENERIC_READ, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	NULL_CHECK_RETURN(hFile, E_FAIL);

	if (INVALID_HANDLE_VALUE == hFile)
		return E_FAIL;

	DWORD dwByte = 0;
	
	while (1)
	{
		EFFECTDATA eData;

		ReadFile(hFile, &eData, sizeof(EFFECTDATA), &dwByte, nullptr);

			if (dwByte == 0)
				break;

			m_vecEffectData.push_back(eData);


	}
	
	CloseHandle(hFile);


	return S_OK;
}

HRESULT CEffect::Ready_GameObject(_vec3 Pos)
{
	
	for (int i = 0; i < m_vecEffectData.size(); ++i)
	{
	CEffectComponent* pObj=	 CEffectComponent::Create(m_pGraphicDevice, m_pCommandList, m_vecEffectData[i], Pos);

	m_pEffectList.push_back(pObj);
		
		

	}




	return S_OK;
}

HRESULT CEffect::LateInit_GameObject()
{
	return S_OK;
}

_int CEffect::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bIsDead)
  return DEAD_OBJ;
	;

	for (int i = 0; i < m_pEffectList.size();)
	{
	   int iDead =m_pEffectList[i]->Update_GameObject(fTimeDelta);
	   if (iDead == true)
	   {
		   Safe_Release(m_pEffectList[i]);
		   m_pEffectList.erase(m_pEffectList.begin() + i);

	   }
	   else
		   i++;

	}

	if (m_pEffectList.size() == 0)
		return DEAD_OBJ;

	return NO_EVENT;
}

_int CEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{


	for (int i = 0; i < m_pEffectList.size();)
	{
		int iDead = m_pEffectList[i]->LateUpdate_GameObject(fTimeDelta);
		if (iDead == true)
		{
			Safe_Release(m_pEffectList[i]);
			m_pEffectList.erase(m_pEffectList.begin() + i);

		}
		else
			i++;

	}


	return S_OK;
}

void CEffect::Render_GameObject(const _float& fTimeDelta)
{
}

void CEffect::SetPos(_vec3 vPos)
{
	for (int i = 0; i < m_pEffectList.size();i++)
	{
		m_pEffectList[i]->SetPos(vPos);
	}


	return;

}

HRESULT CEffect::Add_Component()
{




	return NO_EVENT;
}

CGameObject* CEffect::Clone_GameObject(void* pArg)
{
	CGameObject* pInstance = new CEffect(*this);

	 _vec3 Pos =  *reinterpret_cast<_vec3*>(pArg);

	if (FAILED(static_cast<CEffect*>(pInstance)->Ready_GameObject(Pos)))
		return nullptr;

	return pInstance;
}

CEffect* CEffect::Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, wstring strPath)
{
	CEffect* pInstance = new CEffect(pGraphicDevice, pCommandList);

	if (FAILED(pInstance->Ready_GameObjectPrototype(strPath)))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CEffect::Free()
{
	for (int i = 0; i < m_pEffectList.size(); ++i)
		Safe_Release(m_pEffectList[i]);


	CGameObject::Free();
}
