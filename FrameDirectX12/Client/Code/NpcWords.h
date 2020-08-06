#pragma once
#include "Include.h"
#include "GameObject.h"
#include "ObjectMgr.h"
#include "DynamicCamera.h"
#include "GraphicDevice.h"
#include "DirectInput.h"

namespace Engine
{
	class CFont;
	class CRcTex;
	class CShader_ColorBuffer;
	class CDirectInput;
}

class CDynamicCamera;

class CNpcWords : public Engine::CGameObject
{
public:
	enum WORDS_TYPE { NPC, SHEPARD,ETC, TYPE_END };

private:
	explicit CNpcWords(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CNpcWords(const CNpcWords& rhs);
	virtual ~CNpcWords();

public:
	void						Set_ShowUI(_bool bIsShow) { m_bIsShow = bIsShow; }
	_bool						Get_ShowUI() { return m_bIsShow; }

public:
	void						Set_CurWordsType(WORDS_TYPE eType) { m_eWordsType = eType; }
	void						Ready_NpcWords();
	WORDS_TYPE					Get_CurWordsType() { return m_eWordsType; }		// type get함수 추가함
public:
	HRESULT						Ready_GameObjectPrototype();
	virtual HRESULT				Ready_GameObject(WORDS_TYPE eType);
	virtual HRESULT				LateInit_GameObject();
	virtual _int				Update_GameObject(const _float& fTimeDelta);
	virtual _int				LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void				Render_GameObject(const _float& fTimeDelta);
	
private:
	virtual HRESULT				Add_Component();
	void						Set_ConstantTable();

private:
	void						Init_OthersUI();
	void						Next_Conversation(const _float& fTimeDelta);
	void						Next_ConversationJudje();
	void						Reset_OthersUI(_bool bIsReset);
	void						Show_ConversationWords();
	void                        Check_Interaction();
	void                        Start_Interaction();
	void                        Finish_ConverSation();

private:
	Engine::CRcTex*				m_pBufferCom = nullptr;
	Engine::CShader_UI*			m_pShaderCom = nullptr;
	Engine::CTexture*			m_pTexture = nullptr;
	Engine::CFont*				m_pWordsFont = nullptr;

	CDynamicCamera*				m_pDynamicCamera = nullptr;

public:
	virtual CGameObject*		Clone_GameObject(void* pArg);
	static  CNpcWords*			Create(ID3D12Device* pGraphicDevice,
										ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void				Free();

private:
	WORDS_TYPE					m_eWordsType = TYPE_END;

	vector<wstring>				m_vWords;
	_int						m_iWordsCnt = 0;
	_int						m_iWordsNum = 0;

	_float						m_fAccTime = 0.f;
	_bool						m_bIsNext = false;
	_bool						m_bIsFinish = false;
	_float						m_fNextTime = 0.f;

	_bool						m_bIsShow = false;
};
