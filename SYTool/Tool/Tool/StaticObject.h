#ifndef StaticObject_h__
#define StaticObject_h__

#include "GameObject.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "MyForm.h"
#include "Export_Function.h"

BEGIN(Engine)
class CTransform;
class CCalculator;
class CStaticMesh;
class CCollider;
class CShader;
END

class CStaticObject : public Engine::CGameObject
{
public:
	enum STATIC_STATE { STATIC_WIRE, STATIC_SOLID, STATIC_END };
private:
	explicit CStaticObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CStaticObject(void);

public:
	virtual HRESULT			Ready_Object();
	virtual _int			Update_Object(const _float& fTimeDelta);
	virtual void			Render_Object(void);
	HRESULT					Set_ConstantTable(LPD3DXEFFECT pEffect);
	void					Set_StaticCurState(STATIC_STATE eState) { m_eState = eState; };

	Engine::CTransform*		Get_StaticTranscom() { return m_pTransCom; };
	const LPD3DXMESH		Get_Mesh(void) const { return m_pMeshCom->Get_StaticMesh(); }
	Engine::CStaticMesh*    Get_StaticMesh() const { return m_pMeshCom; }

	_tchar					m_szMeshTag[128] = L"";

private:
	HRESULT					Add_Component(void);

private:
	virtual void			Free(void);

public:
	static CStaticObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev);
	static CStaticObject*	Create(LPDIRECT3DDEVICE9 pGraphicDev, _vec3 pPos, _vec3 pRot, _vec3 pScale, _tchar* pTag);

private:
	Engine::CTransform*		m_pTransCom = nullptr;
	Engine::CCalculator*	m_pCalculatorCom = nullptr;
	//Engine::CCollider*		m_pColliderCom = nullptr;
	Engine::CStaticMesh*	m_pMeshCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

	MESHDATA				m_tMeshData;
	_tchar*					m_szFileTag = L"";

	_vec3					m_vMeshPos = { 0.f,0.f,0.f };
	_vec3					m_vMeshRot = { 0.f,0.f,0.f };
	_vec3					m_vMeshScale = { 0.f,0.f,0.f };

	STATIC_STATE			m_eState = STATIC_END;
};

#endif // StaticObject_h__
