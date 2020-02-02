#pragma once

#include "Component.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
BEGIN(Engine)
class CAnimationControl:public CComponent
{
public:
	CAnimationControl(const aiScene* scece);
	virtual ~CAnimationControl();

	vector<_matrix> ExtractBoneTransforms(float fanimationTime, const int animationIndex = 0);

	void Update(float fTime);
private:
	aiNodeAnim* FindNodeAnim(const aiAnimation* animation, const string nodeName);
	void ReadNodeHierarchy(float animationTime, const aiNode* node, const _matrix& parentTransform);

	aiVector3D CalcInterPolatedValueFromKey(float animationTime, const int numKeys, const aiVectorKey* vectorKey);
	aiQuaternion CalcInterPolatedValueFromKey(float animationTime, const int numKeys, const aiQuatKey* quatKey);


	_matrix AiToMat4(const aiMatrix4x4& m);
	_matrix AiToMat3(const aiMatrix3x3& m);
	_uint FindKeyIndex(float fanimationTime, int inumkey, const aiVectorKey* vectorkey);
	_uint FindKeyIndex(const float animationTime, int numKeys, const aiQuatKey* quatKey) ;

	vector<_matrix> m_vecBoneTransform;
	map<string, _uint>m_boneMap;
	vector< BoneInfo>m_vecBoneInfo;
	
	const aiScene* m_pScene;

private:
	_int m_iCurAniIndex = 0;
	_int m_iPreAniIndex = 0;

};
END