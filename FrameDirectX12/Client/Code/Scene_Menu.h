#pragma once
#include "Include.h"
#include "Scene.h"

class CScene_Menu : public Engine::CScene
{
private:
	explicit CScene_Menu(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CScene_Menu();

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene(const _float& fTimeDelta);
private:
	HRESULT			Ready_GameObjectPrototype();
private:
	HRESULT			Ready_LayerEnvironment(wstring wstrLayerTag);
	HRESULT			Ready_LayerGameObject(wstring wstrLayerTag);
	HRESULT			Ready_LayerUI(wstring wstrLayerTag);

public:
	static CScene_Menu*	Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void		Free();
};

