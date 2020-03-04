#pragma once
#include "Include.h"
#include "Scene.h"
#include "Loading.h"

class CScene_Logo : public Engine::CScene
{
private:
	explicit CScene_Logo(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	virtual ~CScene_Logo();

public:
	virtual HRESULT Ready_Scene();
	virtual _int	Update_Scene(const _float& fTimeDelta);
	virtual _int	LateUpdate_Scene(const _float& fTimeDelta);
	virtual void	Render_Scene(const _float& fTimeDelta);
	CFont*          Get_Font() { return m_pFont; };
private:
	HRESULT			Ready_GameObjectPrototype();
	HRESULT			Ready_LayerEnvironment(wstring wstrLayerTag);
	HRESULT			Ready_LayerGameObject(wstring wstrLayerTag);
	HRESULT			Ready_LayerUI(wstring wstrLayerTag);
	HRESULT			Ready_LayerCamera(wstring wstrLayerTag);
private:
	CLoading*			m_pLoading = nullptr;
		CFont* m_pFont = nullptr;
public:
	static CScene_Logo*	Create(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
private:
	virtual void		Free();
};

