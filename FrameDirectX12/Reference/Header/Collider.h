#pragma once

#include "Component.h"
#include "VIBuffer.h"


BEGIN(Engine)

class CMesh;
class CGameObject;
class CShader_DefaultTex;
class ENGINE_DLL CCollider : public CComponent
{
protected:
	explicit CCollider(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CCollider(const CCollider& rhs);
	virtual ~CCollider(void) = default;
public:
	enum COLLSHAPE{COL_BOX,COL_SPHERE,COL_END};


public:
	const COLLSHAPE&	Get_CollShape(void) {
		return m_eShape;
	}
	const _bool			Get_IsCollision(void) {
		return m_bIsCollision;
	}

	const _vec3&		Get_AddPos(void) {
		return m_vAddPos;
	}
	const _vec3*		Get_Min(void) {
		return &m_vMin;
	}
	const _vec3*		Get_Max(void) {
		return &m_vMax;
	}
	const _float&		Get_Radius(void) {
		return m_fRadius;
	}
	const _matrix&		Get_WorldMat(void) {
		return m_matWorld;
	}
	const _vec3&		Get_Angle(void) {
		return m_vAngle;
	}

	CGameObject*		Get_Owner(void) {
		return m_pOwner;
	}
	const _vec3&		Get_WorldPos(void) {
		return *(_vec3*)(&m_matWorld._41);
	}
public:
	void					Set_Radius(float fRadius) { m_fRadius = fRadius; }
	void					Set_AddPosX(float fX) { m_vAddPos.x += fX; }
	void					Set_AddPosY(float fY) { m_vAddPos.y += fY; }
	void					Set_AddPosZ(float fZ) { m_vAddPos.z += fZ; }
	void					Set_AngleX(float fX) { m_vAngle.x += fX; }
	void					Set_AngleY(float fY) { m_vAngle.y += fY; }
	void					Set_AngleZ(float fZ) { m_vAngle.z += fZ; }
	void					Set_Angle(const _vec3* pAngle) { m_vAngle = *pAngle; }
	void					Set_IsCol(const _bool bIsCol) { m_bIsCollision = bIsCol; }
public:
	virtual HRESULT Ready_Collider_ProtoType(const COLLSHAPE& eShape);
	virtual void Update_Collider(const _matrix* pParentMatrix);
	virtual void Render_Collider(CShader_DefaultTex* pShader,_uint offset);
private:
	HRESULT SetOn_ConstantTable(CShader_DefaultTex* pShader, _uint offset);

protected:
	COLLSHAPE m_eShape;
	_bool m_bIsCollision;

	_float m_fRadius;
	_float m_fOriRadius;
	_vec3 m_vAddPos;
	_vec3 m_vAngle;
	_vec3 m_vMin;
	_vec3 m_vMax;
	_matrix m_matWorld;
	_matrix m_matTrans = INIT_MATRIX;
	_matrix m_matScale = INIT_MATRIX;
	CGameObject* m_pOwner = nullptr;
	CVIBuffer* m_pDrawMesh = nullptr;
	BoundingBox m_pBoxInfo;
	BoundingSphere m_pSphereINfo;
public:
	virtual CComponent*		Clone() PURE;
	virtual CComponent*     Clone_Collider(const _bool& bIsAttatch,  CMesh* pMesh, const _vec3& vAddPos, const _vec3& vAngle, const _float& fRadius, const _vec3&vScale, CGameObject* pOwner = nullptr)PURE;

public:
	virtual void		Free();

};
END