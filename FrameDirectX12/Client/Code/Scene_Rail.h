#pragma once
#include "Include.h"
#include "Scene.h"

class CScene_Rail : public Engine::CScene
{
private:
	explicit CScene_Rail(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CScene_Rail();

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene(const _float& fTimeDelta);
private:
	HRESULT			Ready_GameObjectPrototype();
private:
	HRESULT			Ready_LayerCamera(wstring wstrLayerTag);
	HRESULT			Ready_LayerEnvironment(wstring wstrLayerTag);
	HRESULT			Ready_LayerGameObject(wstring wstrLayerTag);
	HRESULT			Ready_LayerUI(wstring wstrLayerTag);

public:
	static CScene_Rail* Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void		Free();
};

