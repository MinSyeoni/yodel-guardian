#pragma once
#include "Include.h"
#include "GameObject.h"
#include "Player.h"

namespace Engine
{
	class CMesh;
	class CShader_Mesh;
}

class CPlayerLeg : public Engine::CGameObject
{


private:
	explicit CPlayerLeg(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CPlayerLeg(const CPlayerLeg& rhs);
	virtual ~CPlayerLeg();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	void SetPlayerTransform(CTransform* pTransform) { m_pTransCom = pTransform; m_pTransCom->AddRef();};
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);
public:
	void            Set_Animation(CPlayer::STATE  Animation) { m_eCurAnimationKey = Animation; };
	void            Set_Spine(_float Spine) { m_fSpineAngle = Spine; };
	vector<vector<_matrix>> GetLegMatrix() { return m_vecMatrix; };
	void SetArmMatrix(vector<vector<_matrix>> Matrix) { m_vecArmMatrix = Matrix; };

private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
	void            Set_ShadowTable(CShader_Shadow* pShader);
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh*				m_pMeshCom = nullptr;
	Engine::CShader_Mesh*         	m_pShaderCom = nullptr;

	CPlayer::STATE   m_eCurAnimationKey = CPlayer::RIFLEIDLE;
	CPlayer::STATE    m_ePreAnimationKey = CPlayer::RIFLEIDLE;

	float m_fTime = 0.f;
	_float m_fSpineAngle = 0.f;
	vector<vector<_matrix>> m_vecMatrix;
	vector<vector<_matrix>>m_vecArmMatrix;


public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CPlayerLeg*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

