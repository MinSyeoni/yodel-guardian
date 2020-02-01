#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CCubeTex : public CVIBuffer
{
private:
	explicit CCubeTex(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CCubeTex(const CCubeTex& rhs);
	virtual ~CCubeTex();

public:
	HRESULT	Ready_Buffer();
	void	Begin_Buffer();
	void	End_Buffer();
	void	Render_Buffer();

public:
	virtual CComponent* Clone();
	static	CCubeTex*	Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void		Free();
};

END