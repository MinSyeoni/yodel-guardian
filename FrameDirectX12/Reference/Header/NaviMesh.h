#pragma once
#include "Component.h"
#include "Cell.h"
#include "Shader_ColorBuffer.h"
BEGIN(Engine)


class ENGINE_DLL CNaviMesh : public CComponent
{
private:
	explicit CNaviMesh(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh();

private:
	HRESULT     Ready_NaviMesh();
	HRESULT		Link_Cell(void);
public:
	void		Render_NaviMesh(void);
	_vec3		Move_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir);

	_ulong  GetIndex(void) { return m_dwIndex; };
	CCell* GetCurCell(void) { return m_vecCell[m_dwIndex]; };


private:

	vector<CCell*>				 m_vecCell;
	Engine::CShader_ColorBuffer* m_pShaderCom = nullptr;
	_ulong						 m_dwIndex;
public:
	static CNaviMesh* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
public:
	virtual CComponent* Clone();
private:
	virtual void		Free();
};
END