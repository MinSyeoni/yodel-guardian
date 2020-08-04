#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CMesh;
	class CShader_Default;
}

class CDynamicCamera;
class CPlayer;
class CSalone : public Engine::CGameObject
{
	enum STATE { IDLE, EYEBLINK, RUN, WALK };

	enum CUTSCENESTATE{NONE,FIRST,SECOND};

private:
	explicit CSalone(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CSalone(const CSalone& rhs);
	virtual ~CSalone();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);
	virtual void    Render_LimLight(CShader_LimLight* pShader);

	void            FIRTSTTALKCEHCK();
	void            ShowEquipUI(bool bIsRender);
           
private:
	void            Set_ShadowTable(CShader_Shadow* pShader);
	void            Set_LimTable(CShader_LimLight* pShader);
private:
	virtual HRESULT Add_Component();
private:
	void			Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CMesh* m_pMeshCom = nullptr;
	Engine::CShader_Mesh* m_pShaderCom = nullptr;


	vector<vector<_matrix>> m_vecMatrix;

public:
	virtual CGameObject* Clone_GameObject(void* prg);
	static CSalone* Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);
private:
	STATE m_eCurState = EYEBLINK;

	CPlayer* m_pPlayer = nullptr;

	bool m_bIsLimLIght = false;
	bool m_bIsStartTalking = false;
	bool m_bIsStartTalking2 = false;
private:
	virtual void			Free();
};

