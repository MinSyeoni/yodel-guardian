#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CSphereTex : public CVIBuffer
{
private:
	explicit CSphereTex(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CSphereTex(const CSphereTex& rhs);
	virtual ~CSphereTex();

public:
	HRESULT	Ready_Buffer();
	void	Begin_Buffer();
	void	End_Buffer();
	void	Render_Buffer();
private:
	BoundingSphere m_BoundingInfo;
public:
	virtual CComponent* Clone();
	static	CSphereTex* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void		Free();
};

END