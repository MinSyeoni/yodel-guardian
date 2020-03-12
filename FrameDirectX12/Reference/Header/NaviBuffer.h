#pragma once
#include "VIBuffer.h"

BEGIN(Engine)

class ENGINE_DLL CNaviBuffer : public CVIBuffer
{
public:
	typedef struct tagNAVIINFO
	{
		_vec3 vPosition[3];
		_vec4 vColor;

	}NAVIINFO;
private:
	explicit CNaviBuffer(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CNaviBuffer(const CNaviBuffer& rhs);
	virtual ~CNaviBuffer();

public:
	HRESULT	Ready_Buffer(vector< NAVIINFO>vecNaviInfo);
	void	Begin_Buffer();
	void	End_Buffer();
	void	Render_Buffer();
private:
	_uint m_iVertexCount;
public:
	virtual CComponent* Clone();
	static	CNaviBuffer*		Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList,vector< NAVIINFO>vecNaviInfo);
private:
	virtual void		Free();
};

END