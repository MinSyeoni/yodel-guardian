#pragma once

struct MeshInfo
{
	wstring MeshTag;
	_vec3 Pos;
	_vec3 Scale;
	_vec3 Rotation;
	_uint iMeshID;
	_uint iDrawID;
};

typedef struct tagMeshData
{
	wchar_t			szFilePath[128] = L"";
	wchar_t			szFileName[128] = L"";
	wchar_t			m_MeshTag[128] = L"";

	_vec3			vDirection = _vec3( 0.f,0.f,0.f );
	_vec3			vPos = _vec3( 0.f,0.f,0.f );
	_vec3			vScale = _vec3( 0.f,0.f,0.f );
	_vec3			vRotate = _vec3( 0.f,0.f,0.f );

	BYTE			byDrawID = 0;
	BYTE			byMeshID = 0;
}MESHDATA;

typedef struct tagCollider
{
	_vec3		vCenter = _vec3(0.f, 0.f, 0.f);
	_vec3		vScale = _vec3(0.f, 0.f, 0.f);
	_vec3		vRotate = _vec3(0.f, 0.f, 0.f);
	_vec3		dwVtxPos = _vec3(0.f, 0.f, 0.f);
	unsigned long	dwStride = 0;
	unsigned long	dwNumVtx = 0;
	int				iType = 0;
	int				iOptionID = 0;
	float			fRadius = 0.f;
	unsigned int	iColID = 0;
	unsigned int	iGroupID = 0;
	_matrix			matWorld;
}COLLIDER;