#pragma once
#include "Scene.h"
#include "Include.h"

class CScene_Stage : public Engine::CScene
{
private:
	explicit CScene_Stage(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CScene_Stage();

public:
	HRESULT			Ready_LightInfo();
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene(const _float& fTimeDelta); 

private:
	HRESULT			Ready_GameObjectPrototype();
	HRESULT			Ready_LayerEnvironment(wstring wstrLayerTag);
	HRESULT			Ready_LayerCamera(wstring wstrLayerTag);
	HRESULT			Ready_LayerGameObject(wstring wstrLayerTag);
	HRESULT			Ready_LayerUI(wstring wstrLayerTag);

private:
	void			Load_MonsterPos(const wstring& wstrFilePath);
	void			Load_TriggerPos(const wstring& wstrFilePath);
	void			Load_StageObject(const wstring& wstrFilePath);

	MeshInfo		m_tMeshInfo;

public:
	static CScene_Stage*	Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void			Free();

};

