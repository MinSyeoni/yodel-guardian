#pragma once
#include "Component.h"
#include "Cell.h"
#include "Shader_ColorBuffer.h"
BEGIN(Engine)

class CNaviBuffer;
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
	void		Render_NaviMesh(CShader_ColorBuffer* pShader);
	void        SetConstantTable(CShader_ColorBuffer* pShader);


	_ulong  GetIndex(void) { return m_dwIndex; };
	CCell* GetCurCell(void) { return m_vecCell[m_dwIndex]; };
	vector<CCell*> GetNaviCell() { return m_vecCell; };
	_vec3 MoveOn_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, const _float& fSpeed = 0.f, _bool bIsJump = false);

private:

	vector<CCell*>				 m_vecCell;
	_ulong						 m_dwIndex;
	CNaviBuffer* m_pNaviBuffer;
public:
	static CNaviMesh* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
public:
	virtual CComponent* Clone();
private:
	virtual void		Free();
};
END