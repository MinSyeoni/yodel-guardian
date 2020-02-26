#pragma once

#include "GameObject.h"

namespace Engine
{
	class CTransform;
	class CTerrainTex;
	class CTexture;
	class CCalculator;
	class CShader;
}

class CTerrain :public Engine::CGameObject
{
public:
	enum TERRAIN_STATE { STATE_WIRE, STATE_SOLID, STATE_END };

private:
	explicit CTerrain(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTerrain(void);

public:
	virtual HRESULT		Ready_Object(void);
	virtual _int		Update_Object(const _float& fTimeDelta);
	virtual void		Render_Object(void);

private:
	HRESULT				Add_Component(void);
	HRESULT				Set_Material(void);
	HRESULT				Set_ConstantTable(LPD3DXEFFECT pEffect);

public:
	static CTerrain* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free(void);


private:	
	LPDIRECT3DTEXTURE9		m_pTerrainTex = nullptr;
	Engine::CTerrainTex*	m_pBufferCom = nullptr;
	Engine::CTexture*		m_pTextureCom = nullptr;
	Engine::CTransform*		m_pTransCom = nullptr;
	Engine::CCalculator*	m_pCalculCom = nullptr;
	Engine::CShader*		m_pShaderCom = nullptr;

private:
	TERRAIN_STATE			m_eState = STATE_END;

};

