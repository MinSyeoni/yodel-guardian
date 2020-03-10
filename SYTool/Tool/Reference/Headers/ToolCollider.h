#ifndef CToolCollider_h__
#define CToolCollider_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CToolCollider : public CComponent
{
public:
	enum COLLID { COLID_COLLIDER, COLID_TERRAIN, COLID_COMBAT, COLID_TRIGGER, COLID_NAVI, COLID_END };
	enum COLLKIND { COLID_CLIFF, COLID_LADDER, COLID_ACTIONCAM, COLLKIND_END };
	enum COLLTYPE { COL_FALSE, COL_TRUE, COL_END };
	enum COLLSHAPE { COL_BOX, COL_SPHERE, SHAPE_END };

public:
	const COLLID&		Get_ColID(void)			{ return m_eCollID; }
	const COLLTYPE&		Get_CollType(void)		{ return m_eCollType; }
	const COLLSHAPE&	Get_CollShape(void)		{ return m_eShape; }
	const COLLKIND&		Get_CollKind(void) { return m_eCollKind; }
	void				Set_CollKind(COLLKIND eID) { m_eCollKind = eID; }

	const _bool&		Get_IsFit(void)			{ return m_bIsFit; }
	
	const _vec3&		Get_AddPos(void)		{ return m_vAddPos; }
	const _vec3&		Get_AddAngle(void)		{ return m_vAngle; }
	const _vec3&		Get_Scale(void)			{ return m_vScale; }
	const _vec3*		Get_Min(void);
	const _vec3*		Get_Max(void);

	const _matrix&		Get_WorldMat(void)		{ return m_matWorld; }
	const _matrix&		Get_PareOriWorld(void) { return m_matPariOriWorld; }
	//¿¹¿Ü
	const _vec3&		Get_Size(void) { return m_vFSize; }
	const _vec3&		Get_Angle(void) { return m_vAngle; }
	const _float&		Get_Radius(void) { return m_fRadius; }


	const LPD3DXMESH*	Get_ColliderMesh(void) { return &m_pMesh; }

	const _vec3			Get_VtxPos(void) { return m_pVtxPos; }
	const _ulong		Get_dwStride(void) { return m_dwStride; }
	const _ulong		Get_dwNumVtx(void) { return m_dwNumVtx; }

public:
	void Set_ColType(COLLTYPE eColType)		{ m_eCollType = eColType; }

	void Set_Radius(float fRadius) { m_fRadius = fRadius; }
	void Set_Scale(_vec3 vScale) { m_vScale = vScale; }

	void Set_AddPosX(float fX)				{ m_vAddPos.x += fX; }
	void Set_AddPosY(float fY)				{ m_vAddPos.y += fY; }
	void Set_AddPosZ(float fZ)				{ m_vAddPos.z += fZ; }

	void Set_AngleX(float fX)				{ m_vAngle.x = fX; }
	void Set_AngleY(float fY)				{ m_vAngle.y = fY; }
	void Set_AngleZ(float fZ)				{ m_vAngle.z = fZ; }
	void Set_Angle(const _vec3* pAngle)		{ m_vAngle = *pAngle; }
	void Set_WorldMat(_matrix pWorldMat) { m_matWorld = pWorldMat; }
	void Set_PareOriWorld(const _matrix& matPareWorld) { m_matPariOriWorld = matPareWorld; }
	void Reset_AddPos(void)					{ ZeroMemory(&m_vAddPos, sizeof(_vec3)); }
	void Load_AddPos(const _vec3& vAddPos)	{ m_vAddPos = vAddPos; }

	void	Set_ColliderGroupID(_uint iGroupID) { m_iGroupID = iGroupID; }
	_uint	Get_ColliderGroupID() { return m_iGroupID; }

protected:
	explicit CToolCollider(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CToolCollider(void);

public:
	virtual HRESULT	Ready_Collider(const _vec3* pPos, const _ulong& dwNumVtx, const _ulong& dwStride,COLLID eColID) PURE;

	virtual void	Update_Collider(const _matrix* pPareMatrix);

	virtual void	Render_Collider(_int iIsCol,const _matrix* pColliderMatrix) PURE;
	virtual void	Render_Collider(const _matrix* pColliderMatrix) PURE;
	virtual void	Render_Collider(void) PURE;

protected:
	COLLID					m_eCollID;
	COLLTYPE				m_eCollType;
	COLLSHAPE				m_eShape;
	COLLKIND				m_eCollKind;
	
	_uint					m_iGroupID;

	LPD3DXMESH				m_pMesh;
	LPDIRECT3DTEXTURE9		m_pTexture[COL_END];
	LPDIRECT3DDEVICE9		m_pGraphicDev;

	_bool					m_bIsFit = false;
	_float					m_fRadius;
	_float					m_fOriRadius;
	_vec3					m_vFSize;
	_vec3					m_vAddPos;
	_vec3					m_vScale;
	_vec3					m_vAngle;
	_vec3					m_vMin;
	_vec3					m_vMax;
	_matrix					m_matWorld;
	_matrix					m_matPariOriWorld;

	_ulong					m_dwNumVtx;
	_ulong					m_dwStride;
	_vec3					m_pVtxPos;
//	virtual CComponent* Clone_Component(void* pArg);
public:
	virtual void		Free(void);

};

END

#endif // Collider_h__
