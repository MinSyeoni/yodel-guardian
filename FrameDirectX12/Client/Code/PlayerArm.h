#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Player.h"

namespace Engine
{
	class CMesh;
	class CShader_Mesh;
}


class CPlayerArm : public Engine::CGameObject
{
public:
	enum ARMTYPE {CLOTH,HEAVY,NONE};
private:
	explicit CPlayerArm(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CPlayerArm(const CPlayerArm& rhs);
	virtual ~CPlayerArm();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject(ARMTYPE eType);
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	void AnimationBlending();
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);


	void SetPlayerTransform(CTransform* pTransform) { m_pTransCom = pTransform; m_pTransCom->AddRef(); };
	vector<vector<_matrix>> GetArmMatrix() { return m_vecMatrix; };
	void SetLegMatrix(vector<vector<_matrix>> Matrix) { m_vecLegMatrix = Matrix;	AnimationBlending();};

	void            Set_Spine(_float Spine) { m_fSpineAngle = Spine; };


public:
	void            Set_Animation(CPlayer::STATE  Animation) { m_eCurAnimationKey = Animation;};
private:
	void            Set_ShadowTable(CShader_Shadow* pShader);
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*         	m_pShaderCom = nullptr;
	float m_fTime = 0.f;
	CPlayer::STATE   m_eCurAnimationKey = CPlayer::IDLE;
	CPlayer::STATE    m_ePreAnimationKey = CPlayer::IDLE;

	_float m_fSpineAngle = 0.f;

private:

	vector<vector<_matrix>> m_vecMatrix;
	vector<vector<_matrix>>m_vecLegMatrix;

	ARMTYPE m_eArmType = HEAVY;


public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CPlayerArm*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

