#pragma once
#include "Include.h"
#include "GameObject.h"


namespace Engine
{
	class CTerrainTex;
	class CShader_Default;
	class CTexture;
}

class CDynamicCamera;

class CTerrain : public Engine::CGameObject
{
private:
	explicit CTerrain(ID3D12Device* pGraphicDevice, ID3D12GraphicsCommandList* pCommandList);
	explicit CTerrain(const CTerrain& rhs);
	virtual ~CTerrain();

public:
	HRESULT			Ready_GameObjectPrototype();
	virtual HRESULT	Ready_GameObject();
	virtual HRESULT	LateInit_GameObject();
	virtual _int	Update_GameObject(const _float& fTimeDelta);
	virtual _int	LateUpdate_GameObject(const _float& fTimeDelta);
	virtual void	Render_GameObject(const _float& fTimeDelta);
	virtual void    Render_ShadowDepth(CShader_Shadow* pShader);
private:
	virtual HRESULT Add_Component();
private:
	void            Set_ShadowTable(CShader_Shadow* pShader);
	void			Set_ConstantTable();
private:
	/*____________________________________________________________________
	[ Component ]
	______________________________________________________________________*/
	Engine::CTerrainTex*				m_pBufferCom = nullptr;
	Engine::CShader_Terrain*         	m_pShaderCom = nullptr;

	Engine::CTexture*                   m_pDiffuseTexture = nullptr;
	Engine::CTexture*                   m_pNormalTexture = nullptr;


public:
	virtual CGameObject*	Clone_GameObject(void* prg);
	static CTerrain*		Create(ID3D12Device* pGraphicDevice,
		ID3D12GraphicsCommandList* pCommandList);

private:
	virtual void			Free();
};

