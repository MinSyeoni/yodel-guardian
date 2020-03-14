#ifndef ToolPoint_h__
#define ToolPoint_h__

#include "GameObject.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "Export_Function.h"
#include "SphereCollider.h"

BEGIN(Engine)
class CTransform;
class CCalculator;
class CStaticMesh;
class CToolCollider;
class CSphereCollider;
END

class CToolPoint : public Engine::CGameObject
{
private:
	explicit CToolPoint(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CToolPoint(void);

public:
	virtual HRESULT					Ready_Object(_vec3 Pos);
	virtual _int					Update_Object(const _float& fTimeDelta);
	virtual void					Render_Object(void);

	void							Set_CheckPoint(_bool bIsCheck) { m_bIsCheck = bIsCheck; }
	void							Set_NaviPos(_vec3 vPos) { m_vPos = vPos; }
	void							Set_IsShowPoint(_bool bIsShow) { m_bIsShow = bIsShow; }

private:
	HRESULT							Add_Component(_vec3 Pos);

public:
	Engine::CTransform*				m_pTransCom = nullptr;
	Engine::CCalculator*			m_pCalculCom = nullptr;
	Engine::CToolCollider*			m_pSphereCol = nullptr;

private:
	virtual void Free(void);

public:
	static CToolPoint*	Create(LPDIRECT3DDEVICE9 pGraphicDev,_vec3 Pos);
	
private:
	_matrix							m_matColWorld;
	_matrix							m_matColScale;
	_matrix							m_matColTrans;

	_vec3							m_vPos = {};
	_vec3							m_vScale = {};

	bool							m_bIsCheck = false;
	_bool							m_bIsShow = true;
};

#endif // ToolPoint_h__
