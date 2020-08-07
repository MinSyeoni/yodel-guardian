#pragma once
#include "Include.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"

class CDynamicCamera;

class CQuestUI : public Engine::CGameObject
{
public:
	enum QUEST_TYPE {
		QUEST_TYPE0, QUEST_TYPE1, QUEST_TYPE2, QUEST_TYPE3, QUEST_TYPE4,
		QUEST_TYPE5, QUEST_TYPE6, QUEST_TYPE7, QUEST_TYPE8
	};

private:
	explicit CQuestUI(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CQuestUI(const CQuestUI& rhs);
	virtual ~CQuestUI();

public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject();
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);

public:
	void						Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }
	void						Set_CurQUEST_TYPE(QUEST_TYPE iType) { m_eQuestType = iType; }	// 이걸로 퀘스트 정해주면 됌

private:
	virtual HRESULT				Add_Component();

private:
	void						Set_ConstantTable(_uint iIdx);

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom[9] = { nullptr, };
	Engine::CTexture*			m_pTexture[9] = { nullptr, };

	QUEST_TYPE					m_eQuestType = QUEST_TYPE0;

	CDynamicCamera*				m_pDynamicCamera = nullptr;
	_bool						m_bIsShow = true;

	_uint						m_iTest = 0;

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static CQuestUI*			Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();
};
