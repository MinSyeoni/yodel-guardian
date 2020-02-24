#ifndef Transform_h__
#define Transform_h__

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CTransform : public CComponent
{
private:
	explicit CTransform(void);
	virtual ~CTransform(void);

public:
	const _matrix*		Get_WorldMatrix(void) const { return &m_matWorld; }
	void				Set_Scale(const float& fX, const float& fY, const float& fZ)
	{
		m_vScale.x = fX;
		m_vScale.y = fY;
		m_vScale.z = fZ;
	}
	void				Set_Pos(const float& fX, const float& fY, const float& fZ)
	{
		m_vInfo[INFO_POS].x = fX;
		m_vInfo[INFO_POS].y = fY;
		m_vInfo[INFO_POS].z = fZ;
	}
	void				Set_Pos(const _vec3* pPos) { m_vInfo[INFO_POS] = *pPos; }

	void				Get_Info(INFO eType, _vec3* pInfo)
	{
		memcpy(pInfo, &m_matWorld.m[eType][0], sizeof(_vec3));

	}

	void				Get_InfoVec(INFO eType, _vec3* pInfo)
	{
		*pInfo = m_vInfo[eType];
	}

	void	Rotation(ROTATION eType, const _float& fAngle)
	{
		*(((_float*)&m_vAngle) + eType) += fAngle;
	}
	void	Move_Pos(const _vec3* const vDir)
	{
		m_vInfo[INFO_POS] += *vDir;
	}

	void	Chase_Target(const _vec3* pTargetPos, const _float& fSpeed);
	const _matrix*	Compute_LookAtTarget(const _vec3* pTargetPos);

	void	Get_WorldMatrix2(_matrix* pWorldMatrix) const
	{
		*pWorldMatrix = m_matWorld;
	}

	void	Move_TargetPos(const _vec3* pTargetPos,
		const _float& fSpeed,
		const _float& fTimeDelta);

	const _matrix*		Get_NRotWorldMatrix(_matrix* pWorldMatrix) const
	{
		if (nullptr != pWorldMatrix)
			*pWorldMatrix = m_matNRotWorld;

		return &m_matNRotWorld;
	}

	void	Set_ParentMatrix(const _matrix* pParentMatrix) { m_matWorld *= *pParentMatrix; }


public:
	HRESULT		Ready_Transform(void);
	virtual void Update_Component(const _float& fTimeDelta) override;

public:
	_vec3		m_vInfo[INFO_END];
	_vec3		m_vScale;
	_vec3		m_vAngle;
	_matrix		m_matWorld;
	_matrix		m_matNRotWorld;
public:
	static CTransform*		Create(void);

private:
	virtual void Free(void) override;

};

END
#endif // Transform_h__
