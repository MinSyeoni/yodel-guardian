#include "AnimationControl.h"

USING(Engine)
CAnimationControl::CAnimationControl(const aiScene * scene)
	:m_pScene(scene)
{
	aiMesh* mesh = scene->mMeshes[0];
	cout << "mesh num: " << scene->mNumMeshes << endl;
	cout << "bone num: " << mesh->mNumBones << endl;


	m_vecBoneTransform.resize(mesh->mNumBones);
	int inumBones = 0;
	for (int i = 0; i < mesh->mNumBones; i++)
	{
		_uint uiboneindex = 0;
		string boneName(mesh->mBones[i]->mName.data);

		assert(m_boneMap.find(boneName) == m_boneMap.end());
		uiboneindex = 0;
		uiboneindex = inumBones;
		inumBones += 1;
		BoneInfo tBoneInfo;
		m_vecBoneInfo.push_back(tBoneInfo);

		m_boneMap[boneName] = uiboneindex;
		m_vecBoneInfo[uiboneindex].matboneOffset = AiToMat4(mesh->mBones[i]->mOffsetMatrix);
	}

}

CAnimationControl::~CAnimationControl()
{

}

vector<_matrix> CAnimationControl::ExtractBoneTransforms(float fanimationTime, const int animationIndex)
{
	if (animationIndex != m_iCurAniIndex)
		m_iPreAniIndex = m_iCurAniIndex;

	m_iCurAniIndex = animationIndex;
	fanimationTime = fmod(fanimationTime, m_pScene->mAnimations[animationIndex]->mDuration);
	_matrix matIdentity = INIT_MATRIX;
	ReadNodeHierarchy(fanimationTime, m_pScene->mRootNode, matIdentity);

	for (int i = 0; i < m_pScene->mMeshes[0]->mNumBones; i++)
	{
		m_vecBoneTransform[i] = m_vecBoneInfo[i].matfinalTransform;
	}

	return m_vecBoneTransform;
}

void CAnimationControl::Update(float fTime)
{
}

aiNodeAnim * CAnimationControl::FindNodeAnim(const aiAnimation * animation, const string nodeName)
{
	for (int i = 0; i < animation->mNumChannels; i++)
	{
		if (animation->mChannels[i]->mNodeName.data == nodeName)
			return animation->mChannels[i];
	}
	return nullptr;

}

void CAnimationControl::ReadNodeHierarchy(float fanimationTime, const aiNode * node, const _matrix & parentTransform)
{
	string nodeName(node->mName.data);

	const aiAnimation* animation = m_pScene->mAnimations[m_iCurAniIndex];
	_matrix nodeTransform = AiToMat4(node->mTransformation);

	const aiNodeAnim* nodeAnim = FindNodeAnim(animation, nodeName);

	if (nodeAnim)
	{
		const aiVector3D& vScale = CalcInterPolatedValueFromKey(fanimationTime, nodeAnim->mNumScalingKeys, nodeAnim->mScalingKeys);
		_matrix matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);

		const aiQuaternion&vRotate = CalcInterPolatedValueFromKey(fanimationTime, nodeAnim->mNumRotationKeys, nodeAnim->mRotationKeys);
		_matrix matRotationX = XMMatrixRotationX(vRotate.x);
		_matrix matRotationY = XMMatrixRotationX(vRotate.y);
		_matrix matRotationZ = XMMatrixRotationX(vRotate.z);

		_matrix matRotate = AiToMat3(vRotate.GetMatrix());

		const aiVector3D& vTrans = CalcInterPolatedValueFromKey(fanimationTime, nodeAnim->mNumPositionKeys, nodeAnim->mPositionKeys);
		_matrix matTrans = XMMatrixTranslation(vTrans.x, vTrans.y, vTrans.z);

		nodeTransform = matScale * matRotate*matTrans;
	}
	_matrix globalTransfomation =  nodeTransform*parentTransform;

	if (m_boneMap.find(nodeName) != m_boneMap.end())
	{
		_uint uiboneIndex = m_boneMap[nodeName];
		m_vecBoneInfo[uiboneIndex].matfinalTransform = m_vecBoneInfo[uiboneIndex].matboneOffset*
			globalTransfomation*AiToMat4(m_pScene->mRootNode->mTransformation);
	}

	for (int i = 0; i < node->mNumChildren; i++)
		ReadNodeHierarchy(fanimationTime, node->mChildren[i], globalTransfomation);


}

aiVector3D CAnimationControl::CalcInterPolatedValueFromKey(float fanimationTime, const int numKeys, const aiVectorKey * vectorKey)
{

	aiVector3D ret;
	if (numKeys == 1)
	{
		ret = vectorKey[0].mValue;
		return ret;

	}
	_uint uiKeyindex = FindKeyIndex(fanimationTime, numKeys, vectorKey);
	_uint uinextKeyIndex = uiKeyindex + 1;

	assert(uinextKeyIndex < numKeys);

	_float fdeltaTime = vectorKey[uinextKeyIndex].mTime - vectorKey[uiKeyindex].mTime;
	_float factor = (fanimationTime - (float)vectorKey[uiKeyindex].mTime) / fdeltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const aiVector3D& startValue = vectorKey[uiKeyindex].mValue;
	const aiVector3D& endValue = vectorKey[uinextKeyIndex].mValue;

	ret.x = startValue.x + (endValue.x - startValue.x) * factor;
	ret.y = startValue.y + (endValue.y - startValue.y) * factor;
	ret.z = startValue.z + (endValue.z - startValue.z) * factor;

	return ret;
}

aiQuaternion CAnimationControl::CalcInterPolatedValueFromKey(float animationTime, const int numKeys, const aiQuatKey * quatKey)
{
	aiQuaternion ret;
	if (numKeys == 1)
	{
		ret = quatKey[0].mValue;
		return ret;
	}

	unsigned int keyIndex = FindKeyIndex(animationTime, numKeys, quatKey);
	unsigned int nextKeyIndex = keyIndex + 1;

	assert(nextKeyIndex < numKeys);

	float deltaTime = quatKey[nextKeyIndex].mTime - quatKey[keyIndex].mTime;
	float factor = (animationTime - (float)quatKey[keyIndex].mTime) / deltaTime;

	assert(factor >= 0.0f && factor <= 1.0f);

	const aiQuaternion& startValue = quatKey[keyIndex].mValue;
	const aiQuaternion& endValue = quatKey[nextKeyIndex].mValue;
	aiQuaternion::Interpolate(ret, startValue, endValue, factor);
	ret = ret.Normalize();

	return ret;
}

_matrix CAnimationControl::AiToMat4(const aiMatrix4x4 & m)
{
	_matrix ret;

	ret.m[0][0] = m[0][0];  ret.m[0][1] = m[1][0];  ret.m[0][2] = m[2][0];  ret.m[0][3] = m[3][0];
	ret.m[1][0] = m[0][1];  ret.m[1][1] = m[1][1];  ret.m[1][2] = m[2][1];  ret.m[1][3] = m[3][1];
	ret.m[2][0] = m[0][2];  ret.m[2][1] = m[1][2];  ret.m[2][2] = m[2][2];  ret.m[2][3] = m[3][2];
	ret.m[3][0] = m[0][3];  ret.m[3][1] = m[1][3];  ret.m[3][2] = m[2][3];  ret.m[3][3] = m[3][3];
	return ret;

}
_matrix CAnimationControl::AiToMat3(const aiMatrix3x3 & m)
{
	_matrix ret;

	ret.m[0][0] = m[0][0];  ret.m[0][1] = m[1][0];  ret.m[0][2] = m[2][0];  ret.m[0][3] = 0;
	ret.m[1][0] = m[0][1];  ret.m[1][1] = m[1][1];  ret.m[1][2] = m[2][1];  ret.m[1][3] = 0;
	ret.m[2][0] = m[0][2];  ret.m[2][1] = m[1][2];  ret.m[2][2] = m[2][2];  ret.m[2][3] = 0;
	ret.m[3][0] =0;  ret.m[3][1] = 0;  ret.m[3][2] = 0;  ret.m[3][3] =1.0f;
	return ret;

}

_uint CAnimationControl::FindKeyIndex(float fanimationTime, int inumkey, const aiVectorKey * vectorkey)
{
	assert(inumkey > 0);
	for (int i = 0; i < inumkey - 1; i++)
		if (fanimationTime < (float)vectorkey[i + 1].mTime)
			return i;
	assert(0);
}

_uint CAnimationControl::FindKeyIndex(const float animationTime, int numKeys, const aiQuatKey * quatKey)
{
	assert(numKeys > 0);
	for (int i = 0; i < numKeys - 1; i++)
		if (animationTime < (float)quatKey[i + 1].mTime)
			return i;
	assert(0);
}
