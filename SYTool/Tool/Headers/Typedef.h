#ifndef Typedef_h__
#define Typedef_h__

typedef		bool					_bool;

typedef		signed char				_char;
typedef		signed char				_byte;
typedef		unsigned char			_ubyte;

typedef		wchar_t					_tchar;

typedef		signed short			_short;
typedef		unsigned short			_ushort;

typedef		signed int				_int;
typedef		unsigned int			_uint;

typedef		signed long				_long;
typedef		unsigned long			_ulong;

typedef		float					_float;
typedef		double					_double;

typedef		D3DXVECTOR3				_vec3;
typedef		D3DXVECTOR2				_vec2;
typedef		D3DXVECTOR4				_vec4;

typedef		D3DXMATRIX				_matrix;


typedef struct tagRay
{
	D3DXVECTOR3 vOri;
	D3DXVECTOR3 vDir;
}RAY;

typedef struct tagMeshData
{
	wchar_t			szFilePath[128] = L"";
	wchar_t			szFileName[128] = L"";
	wchar_t			m_MeshTag[128] = L"";

	D3DXVECTOR3			vDirection = { 0.f,0.f,0.f };
	D3DXVECTOR3			vPos = { 0.f,0.f,0.f };
	D3DXVECTOR3			vScale = { 0.f,0.f,0.f };
	D3DXVECTOR3			vRotate = { 0.f,0.f,0.f };

	BYTE			byDrawID = 0;
	BYTE			byMeshID = 0;
}MESHDATA;

#endif // Typedef_h__
