#include "pch.h"
#include "Button.h"
#include "GameInstance.h"

CButton::CButton(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject{ pDevice, pContext }
{
}

CButton::CButton(const CButton& Prototype) : CUIObject(Prototype)
{
}

HRESULT CButton::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CButton::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

void CButton::Priority_Update(_float fTimeDelta)
{
	__super::Priority_Update(fTimeDelta);
}

void CButton::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CButton::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CButton::Render()
{
	return S_OK;
}

_bool CButton::IsPick()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	RECT rcUI = { LONG(m_vPos.x - m_vSize.x * 0.5f), LONG(m_vPos.y - m_vSize.y * 0.5f), LONG(m_vPos.x + m_vSize.x * 0.5f), LONG(m_vPos.y + m_vSize.y * 0.5f) };

	return PtInRect(&rcUI, ptMouse);
}

_bool CButton::IsClick_Up(_int iKey)
{
	if (IsPick())
		return m_pGameInstance->IsKeyUp(iKey);
	else
		return false;
}

_bool CButton::IsClick_Down(_int iKey)
{
	if (IsPick())
		return m_pGameInstance->IsKeyDown(iKey);
	else
		return false;
}

_bool CButton::IsClick_Holding(_int iKey)
{
	if (IsPick())
		return m_pGameInstance->IsKeyHold(iKey);
	else
		return false;
}

void CButton::Free()
{
	__super::Free();
}
