#pragma once
#include "Engine_Include.h"
#include "Base.h"
#include "Transform.h"
#include "Info.h"
#include "RcTex.h"
#include "CubeTex.h"
#include "TerrainTex.h"
#include "Shader_ColorBuffer.h"
#include "Shader_Default.h"
#include "Shader_LightAcc.h"
#include "Shader_Terrain.h"
#include "Texture.h"
#include "Shader_DefaultTex.h"
#include "Shader_Destortion.h"
#include "Shader_Mesh.h"
#include "Mesh.h"
#include "AnimationControl.h"
#include "MeshComponent.h"
#include "NaviMesh.h"
#include "Collider.h"
#include "BoxCollider.h"


BEGIN(Engine)

class ENGINE_DLL CComponentMgr : public CBase
{
	DECLARE_SINGLETON(CComponentMgr)

private:
	explicit CComponentMgr();
	virtual ~CComponentMgr();

public:
	CComponent*	Get_Component(wstring wstrPrototypeTag, COMPONENTID eID);
	HRESULT		Add_ComponentPrototype(wstring wstrPrototypeTag, const COMPONENTID& eID, CComponent* pComponent);
	CComponent* Clone_Component(wstring wstrPrototypeTag, const COMPONENTID& eID);
	CComponent* Clone_Collider(wstring wstrPrototypeTag, const COMPONENTID& eID, const CCollider::COLLSHAPE& eColShape, const _bool& bIsAttatch,  CMesh* pMesh, const _vec3& vAddPos,
		const _vec3& vAngle, const _float& fRadius,
		const _vec3& vScale, CGameObject* pOwner = nullptr);
private:
	CComponent*	Find_Component(wstring wstrPrototypeTag, const COMPONENTID& eID);

private:
	unordered_map<wstring, CComponent*>	m_mapComponent[COMPONENTID::ID_END];

private:
	virtual void Free();
};

END