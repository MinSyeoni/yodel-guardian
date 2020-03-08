#ifndef BoxCollider_h__
#define BoxCollider_h__

#include "ToolCollider.h"

BEGIN(Engine)

class ENGINE_DLL CBoxCollider: public CToolCollider
{
public:
	enum ROTATION { ROT_X, ROT_Y, ROT_Z, ROT_END };

private:
	explicit CBoxCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CBoxCollider(void);


public:
	virtual HRESULT	Ready_Collider(const _vec3* pPos,
		const _ulong& dwNumVtx,
		const _ulong& dwStride, COLLID eColID);

	virtual HRESULT	Ready_Collider(const _float& fX,
		const _float& fY,
		const _float& fZ
		, COLLID eColID);

	virtual void		Render_Collider(_int iIsCol, const _matrix* pColliderMatrix);
	virtual void		Render_Collider(const _matrix* pColliderMatrix);
	virtual void		Render_Collider(void);
	virtual void		Render_Collider(_int iIsCol);
	virtual HRESULT		Ready_Collider(const _vec3& vPos, const _float& fX, const _float& fY, const _float& fZ, COLLID eColID);

	virtual HRESULT		Ready_OffsetCollider(const _vec3* pVtxPos,
		const _ulong& dwNumVtx,
		const _ulong& dwStride,
		const _float& fX,
		const _float& fY,
		const _float& fZ
		, COLLID eColID);

	void					Update_Collider(const _matrix * pPareMatrix);

private:
	_vec3		m_vAddMin;
	_vec3		m_vAddMax;


public:
	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device,
		const _vec3* pPos,
		const _ulong& dwNumVtx,
		const _ulong& dwStride, COLLID eColID);

	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device,
		const _float& fX,
		const _float& fY,
		const _float& fZ, COLLID eColID);

	static CBoxCollider* Create(LPDIRECT3DDEVICE9 pGraphic_Device,
		const _vec3& vPos,
		const _float& fX,
		const _float& fY,
		const _float& fZ, COLLID eColID);

	static CBoxCollider* Create_Offset(LPDIRECT3DDEVICE9 pGraphic_Device,
		const _vec3* pVtxPos,
		const _ulong& dwNumVtx,
		const _ulong& dwStride,
		const _float& fX,
		const _float& fY,
		const _float& fZ
		, COLLID eColID);

	virtual void Free(void);
};

END

#endif // BoxCollider_h__
