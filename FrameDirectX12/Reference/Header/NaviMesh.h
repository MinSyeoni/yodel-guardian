#pragma once
#include "Component.h"
#include "Cell.h"
#include "Shader_ColorBuffer.h"
BEGIN(Engine)

class CNaviBuffer;
class ENGINE_DLL CNaviMesh : public CComponent
{
private:
	explicit CNaviMesh(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const wstring& wstrFilePath);
	explicit CNaviMesh(const CNaviMesh& rhs);
	virtual ~CNaviMesh();


private:
	HRESULT						Link_Cell(void);
	HRESULT						Ready_PrototypeNaviMesh();

public:
	void						Set_CurNaviFile(const wstring& wstrFilePath) { m_wstrFilePath = wstrFilePath; }
	void						Render_NaviMesh(CShader_ColorBuffer* pShader);
	void						SetConstantTable(CShader_ColorBuffer* pShader);
	void						SetFirstNavi(_vec3 vPos);
	_ulong						GetIndex(void) { return m_dwIndex; };
	CCell*						GetCurCell(void) { return m_vecCell[m_dwIndex]; };
	_vec3						MoveOn_NaviMesh(const _vec3* pTargetPos, const _vec3* pTargetDir, const _float& fSpeed = 0.f, _bool bIsJump = false);
	_uint                        FoundIndex(_vec3 vPos);
	vector<CCell*>				 GetNaviCell() { return m_vecCell; };
private:
	vector<CCell*>				 m_vecCell;
	_ulong						 m_dwIndex;
	CNaviBuffer*				 m_pNaviBuffer = nullptr;

	wstring						 m_wstrFilePath = L"";

public:
	static CNaviMesh* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const wstring& wstrFilePath);
public:
	virtual CComponent* Clone();
private:
	virtual void		Free();
};
END