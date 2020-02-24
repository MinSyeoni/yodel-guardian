#ifndef CSCollider_h__
#define CSCollider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CSCollider : public CComponent
{

private:
	explicit CSCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSCollider(void);

public:
	const _vec3*		Get_Min(void) const { return &m_vMin; }
	const _vec3*		Get_Max(void) const { return &m_vMax; }
	const _matrix*		Get_ColliderWorldMatrix(void) const { return &m_matWorld; }

public:
	_matrix Get_ColliderWorldMatrix2(void) { return m_matWorld; };
	_vec3 	Get_Min2(void) const { return m_vMin; }
	_vec3 	Get_Max2(void) const { return m_vMax; }
public:
	HRESULT	Ready_Collider(const _vec3* pPos,
		const _vec3* vSize);

	void	Render_Collider(COLLTYPE eType,
		const _matrix* pColliderMatrix);

private:
	_vec3			m_vMin, m_vMax;
	_matrix			m_matWorld;


public:
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXMESH              m_pMesh;
	LPDIRECT3DTEXTURE9		m_pTexture[COL_END];



public:
	static CSCollider*	Create(LPDIRECT3DDEVICE9 pGraphicDev,
		const _vec3* pPos,const _vec3* vSize);

	virtual void		Free(void);

};

END
#endif // Collider_h__
