#pragma once
#include "Include.h"
#include "Scene.h"

class CScene_Boss : public Engine::CScene
{
private:
	explicit CScene_Boss(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CScene_Boss();

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene(const _float& fTimeDelta);
private:
	HRESULT			Ready_GameObjectPrototype();
private:
	HRESULT         Ready_ComponentPrototype(); //시영아 여기서 네비 바꿔주면되~~
	HRESULT			Ready_LayerCamera(wstring wstrLayerTag);
	HRESULT			Ready_LayerEnvironment(wstring wstrLayerTag);
	HRESULT			Ready_LayerGameObject(wstring wstrLayerTag);
	HRESULT			Ready_LayerUI(wstring wstrLayerTag);
	HRESULT         Ready_Light();

private:
	void			Load_StageObject(const wstring& wstrFilePath, wstring wstrLayerTag);
	void			Load_TriggerPos(const wstring& wstrFilePath, wstring wstrLayerTag);
	void			Load_MonsterPos(const wstring& wstrFilePath, wstring wstrLayerTag);
	void			InitMesh_FromFile(const std::wstring& wstrFilePath);
	void			DeleteAll_GameObject(wstring& wstrLayerTag, wstring wstrObjTag);

public:
	static CScene_Boss* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void		Free();

private:
	MeshInfo		m_tMeshInfo;
};

