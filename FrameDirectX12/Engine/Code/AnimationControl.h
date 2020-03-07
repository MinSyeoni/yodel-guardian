#pragma once
#include "Component.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

BEGIN(Engine)

typedef unordered_map<string, _uint>   MAP_BONENAME;
typedef vector<BONE_INFO>            VECTOR_BONEINFO;
typedef vector<_matrix>               VECTOR_MATRIX;

/*__________________________________________________________________________________________________________
[ aiNode 구조체 재정의 ]
____________________________________________________________________________________________________________*/
typedef struct tagHierarchyInfo
{
	tagHierarchyInfo(const aiNode* pNode)
	{
		pAiNode = pNode;
	}
	~tagHierarchyInfo()
	{
		vecBoneMapIdx.clear();
		vecBoneMapIdx.shrink_to_fit();
		mapNodeAnim.clear();
	}

	const aiNode*               pAiNode = nullptr;   // AiNode 정보.
	vector<_uint>               vecBoneMapIdx;      // 영향을 받는 BoneMap Index 정보.
	map<_uint, const aiNodeAnim*>   mapNodeAnim;      // <AnimationIdx, aiNodeAnim*>

} HIERARCHY_INFO;



class ENGINE_DLL CAniCtrl : public CComponent
{
public:
	enum STATE{NONE,PLAYER};

private:
	explicit CAniCtrl(const aiScene* pScene);
	virtual ~CAniCtrl() = default;

public:
	HRESULT               Ready_AniCtrl();
	void               Ready_NodeHierarchy(const aiNode* pNode);

	void               Set_AnimationKey(_int AniKey);

	vector<VECTOR_MATRIX>   Extract_BoneTransform(_float fAnimationTime, STATE eState =NONE, _float fAngle=0.f);
	_matrix*           Get_RootFrame() { return &m_matRootFinal; };
	_matrix*           Get_WeaponMatrix() { return &m_matWeapon; };
private:
	void               Update_NodeHierarchy(_float fAnimationTime,
		const aiNode* pNode,
		const _matrix& matParentTransform);
private:
	aiNodeAnim*            Find_NodeAnimation(const aiAnimation* pAnimation, const string strNodeName);
	aiVector3D            Calc_InterPolatedValue_From_Key(const _float& fAnimationTime,
		const _uint& uiNumKeys,
		const aiVectorKey* pVectorKey ,
		const _uint & uiPreNumKeys,
		const aiVectorKey* pPreVectorKey);


	aiQuaternion         Calc_InterPolatedValue_From_Key(const _float& fAnimationTime,
		const _uint& uiNumKeys,
		const aiQuatKey* pQuatKey,
		const _uint & uiPreNumKeys,
		const aiQuatKey* pPreVectorKey);
public:
	_matrix*            Find_BoneMatrix(string strBoneName);
private:
	_uint               Find_KeyIndex(const _float& fAnimationTime, const _uint& uiNumKey, const aiVectorKey* pVectorKey);
	_uint               Find_KeyIndex(const _float& fAnimationTime, const _uint& uiNumKey, const aiQuatKey* pQuatKey);
private:
	_matrix               Convert_AiToMat4(const aiMatrix4x4& m);
	_matrix               Convert_AiToMat3(const aiMatrix3x3& m);

private:
	const aiScene*         m_pScene = nullptr;               // 모든 Mesh의 정보를 갖고있는 구조체.

	vector<MAP_BONENAME>   m_vecBoneNameMap;               // 뼈의 이름이 몇 번째인지 알려주는 컨테이너.
	vector<VECTOR_BONEINFO>   m_vecBoneInfo;                  // 뼈의 행렬 정보를 갖고있는 컨테이너.
	vector<VECTOR_MATRIX>   m_vecBoneTransform;               // Mesh의 애니메이션 최종 변환 행렬.

	_uint               m_uiCurAniIndex = 0;            // 현재 애니메이션의 Index
	_uint               m_uiNewAniIndex = 0;            // 새로운 애니메이션이 Index


	_float              m_fBlendingTime = 1.f;
	_float              m_fBlendAnimationTime = 0.f;
	_float              m_fAnimationTime = 0.f;
	unordered_map<string, HIERARCHY_INFO*>   m_mapNodeHierarchy;   // Node Hierarchy 정보.

	_matrix            m_matRootFinal;
	_matrix            m_matWeapon;
	_float  m_fAngle = 0.f;

private:
	STATE m_eState=NONE;

public:
	static CAniCtrl*   Create(const aiScene* pScece);
private:
	virtual void      Free();
};
END
