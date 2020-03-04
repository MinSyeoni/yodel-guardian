#pragma once
#include "Component.h"
#include "VlLine.h"
#include "Shader_ColorBuffer.h"
BEGIN(Engine)

class ENGINE_DLL CLine : public CComponent
{
public:
	enum POINT { POINT_START, POINT_FINISH, POINT_END };
	enum COMPARE { COMPARE_LEFT, COMPARE_RIGHT };

private:
	explicit CLine(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CLine(void)=default;

public:
	void RenderLine(CShader_ColorBuffer* pShader);
	void SetConstantTable(CShader_ColorBuffer* pShader);
public:
	HRESULT	Ready_Line(const _vec3* pPointA, const _vec3* pPointB);
	COMPARE	Compare(const _vec3* pEndPos);
	_vec2 Point_Meet(_vec3 * OutPut, _vec3 * pDir);
	_vec3 Get_LineDir() { return m_vPoint[POINT_FINISH] - m_vPoint[POINT_START]; }
private:
	_vec3			m_vPoint[POINT_END];
	_vec3			m_vDirection;
	_vec3			m_vNormal;

	CVlLine*      m_pLine = nullptr;
public:
	static CLine*		Create(const _vec3* pPointA, const _vec3* pPointB, ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual void		Free(void);
};

END