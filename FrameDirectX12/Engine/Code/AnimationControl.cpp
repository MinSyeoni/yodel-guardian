#include "AnimationControl.h"

USING(Engine)

CAniCtrl::CAniCtrl(const aiScene * pScene)
	: m_pScene(pScene)
{
}

HRESULT CAniCtrl::Ready_AniCtrl()
{


	m_vecBoneTransform.resize(m_pScene->mNumMeshes);
	m_vecBoneNameMap.resize(m_pScene->mNumMeshes);
	m_vecBoneInfo.resize(m_pScene->mNumMeshes);

	for (_uint i = 0; i < m_pScene->mNumMeshes; ++i)
	{
		_int   iNumBones = 0;
		aiMesh* pSubsetMesh = m_pScene->mMeshes[i];
		cout << m_pScene->mMeshes[i]->mNumBones << endl;
		/*__________________________________________________________________________________________________________
		- 뼈 행렬을 담아둘 컨테이너 초기화.
		____________________________________________________________________________________________________________*/
		m_vecBoneTransform[i].resize(pSubsetMesh->mNumBones);

		for (_uint j = 0; j < pSubsetMesh->mNumBones; ++j, ++iNumBones)
		{
			_uint   uiboneindex = 0;
			string   strBoneName(pSubsetMesh->mBones[j]->mName.data);

			uiboneindex = iNumBones;

			BONE_INFO tBoneInfo;
			m_vecBoneInfo[i].push_back(tBoneInfo);

			m_vecBoneNameMap[i][strBoneName] = uiboneindex;
			m_vecBoneInfo[i][uiboneindex].matboneOffset = Convert_AiToMat4(pSubsetMesh->mBones[j]->mOffsetMatrix);
		
		}

	}


	/*__________________________________________________________________________________________________________
	[ Hierarchy 정보 ]
	- BoneNameMap안에 각 Node의 Name이 어디에 들어있는지 판단.
	- Animation Index별로 갹 Node에 aiNodeAnim가 들어있는지 탐색.
	____________________________________________________________________________________________________________*/
	Ready_NodeHierarchy(m_pScene->mRootNode);

	return S_OK;
}

void CAniCtrl::Ready_NodeHierarchy(const aiNode * pNode)
{
	string strNodeName(pNode->mName.data);

	HIERARCHY_INFO* pNodeHierarchy = new HIERARCHY_INFO(pNode);

	// 1. BoneNameMap안에 각 Node의 Name이 어디에 들어있는지 판단.
	for (_uint i = 0; i < m_vecBoneNameMap.size(); ++i)
	{
		if (m_vecBoneNameMap[i].find(strNodeName) != m_vecBoneNameMap[i].end())
		{
			// 속해이는 BoneNameMap의 Index 정보.
			pNodeHierarchy->vecBoneMapIdx.push_back(i);
		}
	}

	// 2. Animation Index별로 각 Node에 aiNodeAnim가 들어있는지 탐색.
	for (_uint i = 0; i < m_pScene->mNumAnimations; ++i)
	{
		const aiAnimation*   pAnimation = m_pScene->mAnimations[i];
		const aiNodeAnim*   pNodeAnimation = Find_NodeAnimation(pAnimation, strNodeName);

		pNodeHierarchy->mapNodeAnim.emplace(i, pNodeAnimation);
	}


	m_mapNodeHierarchy.emplace(strNodeName, pNodeHierarchy);

	/*__________________________________________________________________________________________________________
	[ 모든 자식 노드에 대해 재귀호출 ]
	____________________________________________________________________________________________________________*/
	for (_uint i = 0; i < pNode->mNumChildren; ++i)
		Ready_NodeHierarchy(pNode->mChildren[i]);

}

void CAniCtrl::Set_AnimationKey(_int AniKey)
{
	if (m_uiNewAniIndex != AniKey )
	{
		m_uiNewAniIndex = AniKey;
		m_fBlendAnimationTime = m_fAnimationTime;
		m_fBlendingTime = 1.0f;
		
	}

}


vector<VECTOR_MATRIX> CAniCtrl::Extract_BoneTransform(_float fAnimationTime, STATE eState,_float fAngle)
{

	m_eState = eState;
	m_fAngle = fAngle;
	/*__________________________________________________________________________________________________________
	[ Extract_BoneTransform ]
	- 특정 시간(fAnimationTime)과 특정 애니메이션 인덱스(uiAnimationIdx)를 넘겨주면
	각 Bone의 형태를 Matrix 형식으로 리턴하는 함수.
	- 매 프레임마다 호출됨.
	____________________________________________________________________________________________________________*/
	if  (m_uiCurAniIndex >= m_pScene->mNumAnimations)
		return vector<VECTOR_MATRIX>();

	/*__________________________________________________________________________________________________________
	- 애니메이션이 계속 반복되도록 fmod 연산을 취함.
	____________________________________________________________________________________________________________*/
	m_fAnimationTime += fAnimationTime;

	m_fAnimationTime = (_float)(fmod(m_fAnimationTime, (m_pScene->mAnimations[m_uiCurAniIndex]->mDuration)));



	if (m_uiNewAniIndex != m_uiCurAniIndex)
	{
		m_fAnimationTime = m_fBlendAnimationTime;
		m_fAnimationTime = (_float)(fmod(m_fAnimationTime, (m_pScene->mAnimations[m_uiCurAniIndex]->mDuration)));
		m_fBlendingTime -= 0.002f*fAnimationTime;
	}
	if (m_fBlendingTime <= 0)
	{
		m_fBlendingTime = 0.f;
	}
	/*__________________________________________________________________________________________________________
	- Root Node와 단위 행렬을 인자로 넘겨주면 재귀 호출을 통하여 BONE_INFO에 데이터를 저장하는 함수.
	- Read_NodeHierarchy() 함수 호출이 끝나고 나면, 멤버 변수인 m_vecBoneTransform배열에 데이터를 채우고 리턴.
	____________________________________________________________________________________________________________*/
	_matrix matIdentity = INIT_MATRIX;
	Update_NodeHierarchy(m_fAnimationTime, m_pScene->mRootNode, matIdentity);

	if (m_fBlendingTime <= 0)
	{
		m_fBlendingTime = 0.f;
		m_uiCurAniIndex = m_uiNewAniIndex;
		m_fBlendingTime = 1.f;
		m_fAnimationTime = 0.f;

	}

	return m_vecBoneTransform;
}

void CAniCtrl::Update_NodeHierarchy(_float fAnimationTime,
	const aiNode * pNode,
	const _matrix & matParentTransform)
{
	/*__________________________________________________________________________________________________________
	[ Update_NodeHierarchy ]
	- Bone의 Transform을 구하는 함수.
	- 내부적으로 재귀호출하여 BONE_INFO배열의 matFinalTransform변수에 데이터를 채운다.
	- 재귀호출을 통해 자식 노드로 내려가면서, 각각 매칭된 boneTransformation을 저장.
	____________________________________________________________________________________________________________*/
	string strNodeName(pNode->mName.data);

	auto            iter_find = m_mapNodeHierarchy.find(strNodeName);
	const aiNodeAnim*   pNodeAnimation = iter_find->second->mapNodeAnim[m_uiCurAniIndex];
	const aiNodeAnim*   pNewNodeAnimation = iter_find->second->mapNodeAnim[m_uiNewAniIndex];

	_matrix            matNodeTransform = Convert_AiToMat4(pNode->mTransformation);

	/*__________________________________________________________________________________________________________
	- 애니메이션 정보가 있는 노드일 경우.
	____________________________________________________________________________________________________________*/
	if (pNodeAnimation)
	{
		/*__________________________________________________________________________________________________________
		- 주어진 KeyFrame의 정보와 AnimationTime정보를 이용해 Interpolation(보간)을 하고 값을 저장.
		____________________________________________________________________________________________________________*/
		// Scale
		const aiVector3D&   vScale = Calc_InterPolatedValue_From_Key(fAnimationTime, pNodeAnimation->mNumScalingKeys, pNodeAnimation->mScalingKeys, pNewNodeAnimation->mNumScalingKeys, pNewNodeAnimation->mScalingKeys);

		// Rotation
		const aiQuaternion& vRotate = Calc_InterPolatedValue_From_Key(fAnimationTime, pNodeAnimation->mNumRotationKeys, pNodeAnimation->mRotationKeys, pNewNodeAnimation->mNumRotationKeys, pNewNodeAnimation->mRotationKeys);
	

		// Trans
		const aiVector3D&   vTrans = Calc_InterPolatedValue_From_Key(fAnimationTime, pNodeAnimation->mNumPositionKeys, pNodeAnimation->mPositionKeys, pNewNodeAnimation->mNumPositionKeys, pNewNodeAnimation->mPositionKeys);

		// Scale * Rotation * Trans
		_matrix   matScale = XMMatrixScaling(vScale.x, vScale.y, vScale.z);
		_matrix   matRotate = Convert_AiToMat3(vRotate.GetMatrix());
		_matrix   matTrans = XMMatrixTranslation(vTrans.x, vTrans.y, vTrans.z);

		if (m_eState == PLAYER && strNodeName == "Chest")
		{
			_matrix            matRotationX = XMMatrixRotationX( XMConvertToRadians(m_fAngle));

			matRotate = matRotationX * matRotate;
		}

		/*__________________________________________________________________________________________________________
		- 각각의 vector와 quaternion은 matrix로 변환되고, 이동/회전/크기 변환을 통해 NodeTransform(Bone Transform)이 완성.
		- 이 NodeTransform은 Bone Space에서 정의되었던 정점들을 Parent Bone Space에서 정의되도록 하는 변환임과 동시에,
		Parent Bone Space에서 정의된 정점들에게 취하는 변환이다.

		- 예를 들어 상완의 좌표계에서 특정 정점들에 변환을 취해서 전완에 위치하도록 만드는 것.
		____________________________________________________________________________________________________________*/
		matNodeTransform = matScale * matRotate * matTrans;
	}

	/*__________________________________________________________________________________________________________
	[ 구한 Transformation을 이용해서 Bone Transformation을 설정하고, 자식 Node에 전파하는 부분 ]

	- matGlobalTransform   : Bone Space에서 정의되었던 정점들을 Model Space에서 정의도도록 함.
	- matParentTransform   : 부모 Bone Space에서 정의되었던 정점들을 Model Space에서 정의되도록 함.
	- matNodeTransform      : Bone Space에서 정의되었던 정점들을 부모 Bone Space에서 정의되도록 함.
					  혹은 부모 bone Space를 기준으로 한 일종의 변환.
	____________________________________________________________________________________________________________*/
	_matrix matGlobalTransform = matNodeTransform * matParentTransform;

	if (strNodeName == "root")
	{
		m_matRootFinal = matGlobalTransform* Convert_AiToMat4(m_pScene->mRootNode->mTransformation);
	}

	if (strNodeName == "Prop01")
	{
		m_matWeapon = matGlobalTransform * Convert_AiToMat4(m_pScene->mRootNode->mTransformation);
	}
	/*__________________________________________________________________________________________________________
	- Bone이 있는 노드에 대해서만 Bone Transform을 저장.
	- m_vecBoneNameMap은 map<string, _uint>타입으로, bone의 이름과 index를 저장.
	- mapBone을 살펴보는 이유는, 모든 Bone은 Node이지만, 모든 Node가 Bone은 아니기 때문.
	____________________________________________________________________________________________________________*/
	for (_uint& iIdx : iter_find->second->vecBoneMapIdx)
	{
		_uint uiBoneIdx = m_vecBoneNameMap[iIdx][strNodeName];
		{
			/*__________________________________________________________________________________________________________
			- matBoneOffset은 Model Space에서 정의되었던 정점들을 Bone Space에서 정의되도록 만드는 것.
			- matGlobalTransform = matNodeTransform * matParentTransform -> ParentTransform을 통해 다시 Model Space에서 정의.
			- GlobalTransform을 따로 저장하는 이유는 자식 노드에 ParentTransform으로 보내주기 위함.
			____________________________________________________________________________________________________________*/
			m_vecBoneInfo[iIdx][uiBoneIdx].matfinalTransform = m_vecBoneInfo[iIdx][uiBoneIdx].matboneOffset
				* matGlobalTransform
				* Convert_AiToMat4(m_pScene->mRootNode->mTransformation);

			m_vecBoneTransform[iIdx][uiBoneIdx] = m_vecBoneInfo[iIdx][uiBoneIdx].matfinalTransform;
		}

	}

	/*__________________________________________________________________________________________________________
	[ 모든 자식 노드에 대해 재귀호출 ]
	____________________________________________________________________________________________________________*/
	for (_uint i = 0; i < pNode->mNumChildren; ++i)
		Update_NodeHierarchy(fAnimationTime, pNode->mChildren[i], matGlobalTransform);

}

aiNodeAnim * CAniCtrl::Find_NodeAnimation(const aiAnimation * pAnimation,
	const string strNodeName)
{
	for (_uint i = 0; i < pAnimation->mNumChannels; ++i)
	{
		if (strNodeName == pAnimation->mChannels[i]->mNodeName.data)
			return pAnimation->mChannels[i];
	}

	return nullptr;
}


/*__________________________________________________________________________________________________________
- Key Frame이란, 애니메이션을 표현하는 Frame들 중 몇 개만 추려서 만든 Frame을 뜻한다.

- Animation의 모든 Frame들을 만들기에는 메모리 낭비도 심하고, 시간도 오래걸리기 때문에,
Key Frame만 만들고 자연스러운 움직임을 위해 그 사이의 값들은 보간해서 얻어낸다.

- Translation과 Scale은 vector 형태로 되어있고, 방향을 뜻하는 Rotation은 quaternion 형태로 되어있다.
- vector와 quaternion은 보간 방식이 다르기 때문에, 함수를 오버로딩하여 구현.
____________________________________________________________________________________________________________*/
aiVector3D CAniCtrl::Calc_InterPolatedValue_From_Key(const _float & fAnimationTime,
	const _uint & uiNumKeys,
	const aiVectorKey * pVectorKey, 
	const _uint & uiNewNumKeys,
	const aiVectorKey* pNewVectorKey)
{
	aiVector3D ret;
	aiVector3D Preret;

	_uint uiKeyindex = Find_KeyIndex(fAnimationTime, uiNumKeys, pVectorKey);
	_uint uiNextKeyIndex = uiKeyindex + 1;


	/*__________________________________________________________________________________________________________
	[ Key Frame 사이를 보간하여 특정 시간의 Node의 Transformation을 구하는 부분 ]
	____________________________________________________________________________________________________________*/
	// assert(uiNextKeyIndex < uiNumKeys);

	_float fTimeDelta = (_float)(pVectorKey[uiNextKeyIndex].mTime - pVectorKey[uiKeyindex].mTime);
	_float fFactor = (fAnimationTime - (_float)pVectorKey[uiKeyindex].mTime) / fTimeDelta;

	// assert(fFactor >= 0.0f && fFactor <= 1.0f);

	const aiVector3D& StartValue = pVectorKey[uiKeyindex].mValue;
	const aiVector3D& EndValue = pVectorKey[uiNextKeyIndex].mValue;

	ret.x = StartValue.x + (EndValue.x - StartValue.x) * fFactor;
	ret.y = StartValue.y + (EndValue.y - StartValue.y) * fFactor;
	ret.z = StartValue.z + (EndValue.z - StartValue.z) * fFactor;


	if (m_uiCurAniIndex != m_uiNewAniIndex)
	{

		uiKeyindex = Find_KeyIndex(0, uiNewNumKeys, pNewVectorKey);
		uiNextKeyIndex = uiKeyindex + 1;


		_float fTimeDelta = (_float)(pNewVectorKey[uiNextKeyIndex].mTime - pNewVectorKey[uiKeyindex].mTime);
		_float fFactor = (0 - (_float)pNewVectorKey[uiKeyindex].mTime) / fTimeDelta;

		// assert(fFactor >= 0.0f && fFactor <= 1.0f);

		const aiVector3D& StartValue = pNewVectorKey[uiKeyindex].mValue;
		const aiVector3D& EndValue = pNewVectorKey[uiNextKeyIndex].mValue;

		Preret.x = StartValue.x + (EndValue.x - StartValue.x) * fFactor;
		Preret.y = StartValue.y + (EndValue.y - StartValue.y) * fFactor;
		Preret.z = StartValue.z + (EndValue.z - StartValue.z) * fFactor;


		ret = ret * m_fBlendingTime + Preret * (1.f - m_fBlendingTime);
	}


	return ret;
}

aiQuaternion CAniCtrl::Calc_InterPolatedValue_From_Key(const _float & fAnimationTime,
	const _uint & uiNumKeys,
	const aiQuatKey * pQuatKey,const _uint & uiNewNumKeys, 
	const aiQuatKey* pNewQuatKey)
{
	
	aiQuaternion ret;
	aiQuaternion ret2;
	aiQuaternion ret3;

	_uint uiKeyIndex = Find_KeyIndex(fAnimationTime, uiNumKeys, pQuatKey);
	_uint uiNextKeyIndex = uiKeyIndex + 1;

	// assert(uiNextKeyIndex < uiNumKeys);

	_float fTimeDelta = (_float)(pQuatKey[uiNextKeyIndex].mTime - pQuatKey[uiKeyIndex].mTime);
	_float fFactor = (fAnimationTime - (_float)pQuatKey[uiKeyIndex].mTime) / fTimeDelta;

	// assert(fFactor >= 0.0f && fFactor <= 1.0f);

	const aiQuaternion& StartValue = pQuatKey[uiKeyIndex].mValue;
	const aiQuaternion& EndValue = pQuatKey[uiNextKeyIndex].mValue;

	aiQuaternion::Interpolate(ret, StartValue, EndValue, fFactor);

	ret = ret.Normalize();

	if (m_uiCurAniIndex != m_uiNewAniIndex)
	{
		_uint uiKeyIndex = Find_KeyIndex(0, uiNewNumKeys, pNewQuatKey);
		_uint uiNextKeyIndex = uiKeyIndex + 1;

		// assert(uiNextKeyIndex < uiNumKeys);

		_float fTimeDelta = (_float)(pNewQuatKey[uiNextKeyIndex].mTime - pNewQuatKey[uiKeyIndex].mTime);
		_float fFactor = (0 - (_float)pNewQuatKey[uiKeyIndex].mTime) / fTimeDelta;

		// assert(fFactor >= 0.0f && fFactor <= 1.0f);

		const aiQuaternion& StartValue = pNewQuatKey[uiKeyIndex].mValue;
		const aiQuaternion& EndValue = pNewQuatKey[uiNextKeyIndex].mValue;

		aiQuaternion::Interpolate(ret2, StartValue, EndValue, fFactor);

		ret2 = ret2.Normalize();


		aiQuaternion::Interpolate(ret, ret2 , ret, m_fBlendingTime);

		ret = ret.Normalize();
	}




	return ret;
}

_matrix * CAniCtrl::Find_BoneMatrix(string strBoneName)
{
	/*__________________________________________________________________________________________________________
	[ Key값 탐색 ]
	- vector<MAP_BONENAME> m_vecBoneNameMap을 순회하면서 strBoneName Key값이 있는지 탐색.
	____________________________________________________________________________________________________________*/
	for (_uint i = 0; i < m_vecBoneNameMap.size(); ++i)
	{
		auto iter_find = m_vecBoneNameMap[i].find(strBoneName);

		if (iter_find == m_vecBoneNameMap[i].end())
			continue;

		/*_____________________________________________________
		_____________________________________________________
		[ 탐색 성공 ]
		- Bone의 m_vecBoneInfo에서 FinalTransform 반환.
		____________________________________________________________________________________________________________*/
	   return &(m_vecBoneInfo[i][iter_find->second].matfinalTransform);


	}

	return nullptr;
}

_uint CAniCtrl::Find_KeyIndex(const _float & fAnimationTime,
	const _uint & uiNumKey,
	const aiVectorKey * pVectorKey)
{
	// assert(uiNumKey > 0);

	for (_uint i = 0; i < uiNumKey - 1; ++i)
	{
		if (fAnimationTime < (_float)pVectorKey[i + 1].mTime)
			return i;
	}

	// assert(0);

	return S_OK;
}

_uint CAniCtrl::Find_KeyIndex(const _float & fAnimationTime,
	const _uint & uiNumKey,
	const aiQuatKey * pQuatKey)
{
	// assert(uiNumKey > 0);

	for (_uint i = 0; i < uiNumKey - 1; ++i)
	{
		if (fAnimationTime < (_float)pQuatKey[i + 1].mTime)
			return i;
	}

	// assert(0);

	return S_OK;
}

_matrix CAniCtrl::Convert_AiToMat4(const aiMatrix4x4 & m)
{
	_matrix matResult;

	matResult.m[0][0] = m[0][0];   matResult.m[0][1] = m[1][0];   matResult.m[0][2] = m[2][0];   matResult.m[0][3] = m[3][0];
	matResult.m[1][0] = m[0][1];   matResult.m[1][1] = m[1][1];   matResult.m[1][2] = m[2][1];   matResult.m[1][3] = m[3][1];
	matResult.m[2][0] = m[0][2];   matResult.m[2][1] = m[1][2];   matResult.m[2][2] = m[2][2];   matResult.m[2][3] = m[3][2];
	matResult.m[3][0] = m[0][3];   matResult.m[3][1] = m[1][3];   matResult.m[3][2] = m[2][3];   matResult.m[3][3] = m[3][3];

	return matResult;
}

_matrix CAniCtrl::Convert_AiToMat3(const aiMatrix3x3 & m)
{
	_matrix matResult;

	matResult.m[0][0] = m[0][0];   matResult.m[0][1] = m[1][0];   matResult.m[0][2] = m[2][0];   matResult.m[0][3] = 0;
	matResult.m[1][0] = m[0][1];   matResult.m[1][1] = m[1][1];   matResult.m[1][2] = m[2][1];   matResult.m[1][3] = 0;
	matResult.m[2][0] = m[0][2];   matResult.m[2][1] = m[1][2];   matResult.m[2][2] = m[2][2];   matResult.m[2][3] = 0;
	matResult.m[3][0] = 0;         matResult.m[3][1] = 0;         matResult.m[3][2] = 0;         matResult.m[3][3] = 1.0f;

	return matResult;
}

CAniCtrl * CAniCtrl::Create(const aiScene * pScece)
{
	CAniCtrl* pInstance = new CAniCtrl(pScece);

	if (FAILED(pInstance->Ready_AniCtrl()))
		Engine::Safe_Release(pInstance);

	return pInstance;
}

void CAniCtrl::Free()
{
	for (auto& MyPair : m_mapNodeHierarchy)
		Safe_Delete(MyPair.second);

	m_mapNodeHierarchy.clear();
}