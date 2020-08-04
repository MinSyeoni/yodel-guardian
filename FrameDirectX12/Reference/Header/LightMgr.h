#pragma once 

#include "Base.h"
BEGIN(Engine)

class CLight;
class ENGINE_DLL CLight_Manager : public CBase
{
  DECLARE_SINGLETON(CLight_Manager)

private:
	explicit CLight_Manager();
	virtual ~CLight_Manager() = default;

public:
	const D3DLIGHT *  Get_LightInfo(const _uint& iIndex);
	HRESULT Set_LightInfo(const _uint& iIndex, D3DLIGHT LightInfo);
	HRESULT Set_LightOnOff(const _uint&iIdex, _bool blsLight);
	void   ClearLigth();
	_uint Get_LightIndex() { return (_uint)(m_LightList.size()-1); };
public:
	HRESULT Add_Light(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const D3DLIGHT* tagLight);
	HRESULT Render_Light(vector<ComPtr<ID3D12Resource>> pTargetTexture);

private:
	list<CLight*>     m_LightList;

protected:
	virtual void Free();

};
END