#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CVlLine : public CVIBuffer
{
private:
	explicit CVlLine(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CVlLine(const CVlLine& rhs);
	virtual ~CVlLine();

public:
	HRESULT	Ready_Buffer(_vec3 vStartPos, _vec3 vEndPos,_vec4 vColor);
	void	Begin_Buffer();
	void	End_Buffer();
	void	Render_Buffer();

public:
	virtual CComponent* Clone();
	static	CVlLine*	Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList,_vec3 vStartPos,_vec3 vEndPos,_vec4 vColor);
	
private:
	virtual void		Free();
};

END