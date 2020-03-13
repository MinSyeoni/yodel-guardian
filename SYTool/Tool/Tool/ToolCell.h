#ifndef Toolcell_h__
#define Toolcell_h__

#include "GameObject.h"
#include "ToolPoint.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "Export_Function.h"

BEGIN(Engine)
class CCalculator;
END

class CToolPoint;
class CToolCell : public Engine::CGameObject
{
private:
	explicit CToolCell(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolCell(void);

public:
	_int				Get_CellIndex() { return m_iCellindex; }
	_int				Get_CellOption() { return m_iCellOption; }
	void				Set_CellIndex(_int iIdx) { m_iCellindex = iIdx; }
	void				Set_CellOption(_int iOption) { m_iCellOption = iOption; }

public:
	virtual HRESULT     Ready_Object();
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);

	void				Render_FontOnCell();

private:
	HRESULT				Add_Component(_vec3 Pos);
	virtual void		Free(void);

public:
	static CToolCell*	Create(LPDIRECT3DDEVICE9 pGraphicDev, 
		CToolPoint* PosA, CToolPoint* PosB, CToolPoint* PosC);

public:
	CToolPoint*					m_pPointA = nullptr;
	CToolPoint*					m_pPointB = nullptr;
	CToolPoint*					m_pPointC = nullptr;
	
	LPD3DXLINE					m_pD3DXLine;
	D3DVIEWPORT9				m_ViewPort;
	Engine::CCalculator*		m_pCalculCom = nullptr;

private: 
	_int						m_iCellindex = 0;
	_int						m_iCellOption = 0;

	_vec3						m_vPtPos[4] = {};
	_vec3						m_vFontPos = {};
	_vec2						m_vViewPortPos = {};

	_matrix						m_matProj;
	_matrix						m_matView;
	_matrix						m_matWorld;
	_matrix						m_matViewPort;
};

#endif // ToolCell_h__