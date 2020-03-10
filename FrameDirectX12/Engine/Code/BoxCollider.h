#pragma once

#include "Collider.h"

BEGIN(Engine)

class ENGINE_DLL CBoxCollider : public CCollider
{
private:
	explicit CBoxCollider(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CBoxCollider(const CBoxCollider&rhs);
	virtual ~CBoxCollider(void) = default;

public:
	virtual HRESULT			Ready_Collider_ProtoType(const COLLSHAPE& eColShape);
	virtual HRESULT			Ready_Collider( CMesh* pMesh, const _vec3& vAddPos, const _vec3& vAngle, CGameObject* pOwner = nullptr);
	virtual HRESULT			Ready_Collider(const _vec3& vAddPos, const _vec3& vAngle, const _vec3& vScale, CGameObject* pOwner = nullptr);

private:
	_vec3 m_vCenterPos;

public:

public:
	virtual CComponent* Clone();
	virtual CComponent*     Clone_Collider(const _bool& bIsAttatch,  CMesh* pMesh, const _vec3& vAddPos, const _vec3& vAngle, const _float& fRadius, const _vec3&vScale, CGameObject* pOwner = nullptr);
	static	CBoxCollider*		Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList, const COLLSHAPE& eColShape);
	virtual void Free(void);

};
END