#include "pch.h"
#include "ToolCamera.h"


IMPLEMENT_SINGLETON(CToolCamera)

CToolCamera::CToolCamera(void)
{
	Initialize_Camera();
}

CToolCamera::~CToolCamera(void)
{
}

void CToolCamera::Initialize_Camera()
{
	m_vPos = { 50.f, 10.f, 1.f };

	ZeroMemory(&m_vRight, sizeof(_vec3));
	ZeroMemory(&m_vLook, sizeof(_vec3));
	ZeroMemory(&m_vUp, sizeof(_vec3));

	m_vRight.x = 1.f;
	m_vUp.y = 1.f;
	m_vLook.z = 1.f;
}

void CToolCamera::Update_Camera(const _float& fTimeDelta)
{
	if (GetAsyncKeyState('W') && 0x8000)
		m_vPos.y += 20 * fTimeDelta;
	if (GetAsyncKeyState('S') && 0x8000)
		m_vPos.y -= 20 * fTimeDelta;;
	if (GetAsyncKeyState('A') && 0x8000)
		m_vPos -= m_vRight * 20.f * fTimeDelta;
	if (GetAsyncKeyState('D') && 0x8000)
		m_vPos += m_vRight * 20.f * fTimeDelta;

	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		POINT	pt = { g_iWinCX >> 1, g_iWinCY >> 1 };
		ClientToScreen(g_hWnd, &pt);
		SetCursorPos(pt.x, pt.y);

		if (GetAsyncKeyState('W') && 0x8000)
			Camera_Pitch(-50.f * fTimeDelta);
		if (GetAsyncKeyState('S') && 0x8000)
			Camera_Pitch(50.f * fTimeDelta);
		if (GetAsyncKeyState('A') && 0x8000)
			Camera_Yaw(-50.f * fTimeDelta);
		if (GetAsyncKeyState('D') && 0x8000)
			Camera_Yaw(50.f * fTimeDelta);
	}

	if (GetAsyncKeyState('Q') & 0x8000)
	{
		m_vPos.x += (m_vLook.x) * 40.f * fTimeDelta;
		m_vPos.z += (m_vLook.z) * 40.f * fTimeDelta;
	}
	if (GetAsyncKeyState('E') & 0x8000)
	{
		m_vPos.x -= (m_vLook.x) * 40.f * fTimeDelta;
		m_vPos.z -= (m_vLook.z) * 40.f * fTimeDelta;
	}

	Change_CameraMat(&m_matView);
	if (m_bIsCameraExecute)
		m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matView);
}

void CToolCamera::Move_Camera(_vec3 vDir)
{
	m_vPos += vDir;
}

_vec3& CToolCamera::Get_CameraPos()
{
	return m_vPos;
}

void CToolCamera::Set_CameraPos(_vec3* pPos)
{
	m_vPos = *pPos;
}

void CToolCamera::Rotation_Axis(const _vec3* pAxis, const _float& fRadian, const _float& fTimeDelta)
{
	_matrix			matRot;
	D3DXMatrixRotationAxis(&matRot, pAxis, fRadian * fTimeDelta);

	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[Engine::INFO_RIGHT][0], (_vec3*)&m_matWorld.m[Engine::INFO_RIGHT][0], &matRot);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[Engine::INFO_UP][0], (_vec3*)&m_matWorld.m[Engine::INFO_UP][0], &matRot);
	D3DXVec3TransformNormal((_vec3*)&m_matWorld.m[Engine::INFO_LOOK][0], (_vec3*)&m_matWorld.m[Engine::INFO_LOOK][0], &matRot);
}

void CToolCamera::Camera_Pitch(_float fAngle)
{
	D3DXMatrixRotationAxis(&m_matWorld, &m_vRight, D3DXToRadian(fAngle));
	D3DXVec3TransformCoord(&m_vUp, &m_vUp, &m_matWorld);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &m_matWorld);
}

void CToolCamera::Camera_Yaw(_float fAngle)
{
	D3DXMatrixRotationY(&m_matWorld, D3DXToRadian(fAngle));
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &m_matWorld);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &m_matWorld);
}

void CToolCamera::Camera_Roll(_float fAngle)
{
	D3DXMatrixRotationY(&m_matWorld, fAngle);
	D3DXVec3TransformCoord(&m_vRight, &m_vRight, &m_matWorld);
	D3DXVec3TransformCoord(&m_vLook, &m_vLook, &m_matWorld);
}

_matrix& CToolCamera::Change_CameraMat(_matrix* pMatView)
{
	// TODO: 여기에 return 문을 삽입합니다.
	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Cross(&m_vUp, &m_vLook, &m_vRight);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXVec3Cross(&m_vRight, &m_vUp, &m_vLook);
	D3DXVec3Normalize(&m_vRight, &m_vRight);

	float x = -D3DXVec3Dot(&m_vRight, &m_vPos);
	float y = -D3DXVec3Dot(&m_vUp, &m_vPos);
	float z = -D3DXVec3Dot(&m_vLook, &m_vPos);

	(*pMatView)(0, 0) = m_vRight.x;
	(*pMatView)(0, 1) = m_vUp.x;
	(*pMatView)(0, 2) = m_vLook.x;
	(*pMatView)(0, 3) = 0.f;

	(*pMatView)(1, 0) = m_vRight.y;
	(*pMatView)(1, 1) = m_vUp.y;
	(*pMatView)(1, 2) = m_vLook.y;
	(*pMatView)(1, 3) = 0.f;

	(*pMatView)(2, 0) = m_vRight.z;
	(*pMatView)(2, 1) = m_vUp.z;
	(*pMatView)(2, 2) = m_vLook.z;
	(*pMatView)(2, 3) = 0.f;

	(*pMatView)(3, 0) = x;
	(*pMatView)(3, 1) = y;
	(*pMatView)(3, 2) = z;
	(*pMatView)(3, 3) = 1.f;

	return *pMatView;
}
