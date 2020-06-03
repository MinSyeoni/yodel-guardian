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

typedef struct tagTerrainData
{
	wchar_t			szFilePath[128] = L"";
	wchar_t			szFileName[128] = L"";
	wchar_t			m_TerrainTag[128] = L"";

	D3DXVECTOR3		vPos = { 0.f,0.f,0.f };

	_ulong			dwVtxCntZ = 0;
	_ulong			dwVtxCntX = 0;
	_ulong			dwInterval = 0;
	_float			fHeight = 0.f;
}TERRAIN_DATA;

typedef struct tagSphere
{
	D3DXVECTOR3		vCenter;
	unsigned long	dwStride;
	unsigned long	dwNumVtx;
	float			fRadius;
}SPHERE, * LPSPHERE;

typedef struct tagCollider
{
	D3DXVECTOR3		vCenter = { 0.f,0.f,0.f };
	D3DXVECTOR3		vScale = { 0.f,0.f,0.f };
	D3DXVECTOR3		vRotate = { 0.f,0.f,0.f };
	D3DXVECTOR3		dwVtxPos = { 0.f,0.f,0.f };
	unsigned long	dwStride = 0;
	unsigned long	dwNumVtx = 0;
	int				iType = 0;
	int				iOptionID = 0;
	float			fRadius = 0.f;
	unsigned int	iColID = 0;
	unsigned int	iGroupID = 0;
	_matrix			matWorld;
}COLLIDER;

typedef struct tagCell
{
	D3DXVECTOR3  PointA;
	D3DXVECTOR3  PointB;
	D3DXVECTOR3  PointC;
	int			 Option = 0;
	int			 DrawID = 0;
}NAVICELL;

#endif // Typedef_h__
