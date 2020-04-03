#pragma once

struct MeshInfo
{
	wstring MeshTag;
	_vec3 Pos;
	_vec3 Scale;
	_vec3 Rotation;
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