#include "TrailColor.h"

USING(Engine)
CTrailColor::CTrailColor(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CTrailColor::~CTrailColor(void)
{
}

void CTrailColor::Set_Trail(const _vec3 * pPos, 
										   const _vec3 * pDir, 
										   const _float & fLength,
										   D3DXCOLOR Color, 
										   _ulong dwCount)
{
	m_pVtxCol = new VTXCOL[dwCount];
	ZeroMemory(m_pVtxCol, sizeof(VTXCOL) * dwCount);

	_ulong dwHalfCnt = _ulong(dwCount * 0.5f);

	for (_ulong i = 0; i < dwHalfCnt; ++i)
	{
		_vec3 vPos		= pPos[i];
		_vec3 vDir		= pDir[i];

		_float fLen		= fLength * (i * 2.f / (_float)dwHalfCnt);

		m_pVtxCol[i * 2].vPos					= vPos - vDir * fLen;
		m_pVtxCol[i * 2].dwColor			= D3DXCOLOR(Color.r, Color.g, Color.b, i / (_float)dwHalfCnt);
		m_pVtxCol[i * 2 + 1].vPos			= vPos + vDir * fLen;
		m_pVtxCol[i * 2 + 1].dwColor	= D3DXCOLOR(Color.r, Color.g, Color.b, i / (_float)dwHalfCnt);
	}

	m_dwVtxCnt = dwCount - 2;
}

HRESULT CTrailColor::Initialize_TrailColor()
{
	m_dwVtxFVF = FVF_COL;
	m_dwVtxSize = sizeof(VTXCOL);

	return S_OK;
}

void CTrailColor::Render_TrailColor()
{
	m_pGraphicDev->SetFVF(m_dwVtxFVF);
	m_pGraphicDev->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, m_dwVtxCnt, m_pVtxCol, m_dwVtxSize);
	Safe_Delete_Array(m_pVtxCol);
}

CTrailColor * CTrailColor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTrailColor* pInstance = new CTrailColor(pGraphicDev);

	if (FAILED(pInstance->Initialize_TrailColor()))
		Safe_Release(pInstance);

	return pInstance;
}

CComponent * CTrailColor::Clone(void)
{
	return new CTrailColor(*this);
}

void CTrailColor::Free(void)
{
	Safe_Release(m_pGraphicDev);
	Safe_Delete_Array(m_pVtxCol);
}
