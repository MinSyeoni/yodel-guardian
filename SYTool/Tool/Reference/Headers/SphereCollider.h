#ifndef SphereCollider_h__
#define SphereCollider_h__

#include "ToolCollider.h"

BEGIN(Engine)

class ENGINE_DLL CSphereCollider : public CToolCollider
{
private:
	explicit CSphereCollider(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CSphereCollider(void);

public:
	virtual HRESULT	Ready_Collider(const _vec3* pPos,
		const _ulong& dwNumVtx,
		const _ulong& dwStride, COLLID eColID);

	void			Render_ShaderCollider();


	virtual HRESULT	Ready_Collider(const _float& fRadius, COLLID eColID);

	virtual void	Update_Collider(const _matrix* pPareMatrix);

	virtual void	Render_Collider(_int iIsCol, const _matrix* pColliderMatrix);
	virtual void	Render_Collider(const _matrix* pColliderMatrix);
	virtual void	Render_Collider(void);

	_vec3			Get_ColliderPos() { return m_vCenterPos; }

private:
	_vec3		m_vCenterPos;

public:
	static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device,
		const _vec3* pPos,
		const _ulong& dwNumVtx,
		const _ulong& dwStride, COLLID eColID);

	static CSphereCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device,
		const _float& fRadius, COLLID eColID);

	virtual void Free(void);
};

END

#endif // SphereCollider_h__
