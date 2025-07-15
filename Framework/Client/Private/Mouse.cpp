#include "pch.h"
#include "Mouse.h"
#include "Mouse_Click_Fx.h"

CMouse::CMouse(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CUIObject(pDevice, pContext)
{
}

CMouse::CMouse(const CMouse& Prototype) : CUIObject (Prototype)
{
}

HRESULT CMouse::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMouse::Initialize(void* pArg)
{
	m_vLocalPos.x = g_iWinSizeX * 0.5f;
	m_vLocalPos.y = g_iWinSizeY * 0.5f;
	m_vLocalSize.x = 46.f * 0.4f;
	m_vLocalSize.y = 72.f * 0.4f;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	_float fMaxSizeX = 256;
	_float fMaxSizeY = 256;
	m_vDefaultMinUV = { 156.f / fMaxSizeX, 32.f / fMaxSizeY };
	m_vDefaultMaxUV = { 202.f / fMaxSizeX, 104.f / fMaxSizeY };
	m_vSeleteMinUV = { 37.f / fMaxSizeX, 32.f / fMaxSizeY };
	m_vSeleteMaxUV = { 83.f / fMaxSizeX, 104.f / fMaxSizeY };

	return S_OK;
}

void CMouse::Priority_Update(_float fTimeDelta)
{
	if (m_bIsFPS)
		FPS_Mode();
}

void CMouse::Update(_float fTimeDelta)
{
	if (m_pGameInstance->IsKeyDown(DIK_N) && !m_bIsFPS)
		m_bIsFPS = true;
	else if (m_pGameInstance->IsKeyDown(DIK_N) && m_bIsFPS)
		m_bIsFPS = false;
	
	POINT			ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(g_hWnd, &ptMouse);

	m_vPos.x = ptMouse.x + m_vSize.x * 0.5f ;
	m_vPos.y = ptMouse.y + m_vSize.y * 0.5f;

	if (m_bIsFPS)
		return;

	Mouse_Input();
}

void CMouse::Late_Update(_float fTimeDelta)
{
	if (m_bIsFPS)
		return;

	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::LAST, this)))
		return;

}

HRESULT CMouse::Render()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float4 vVector = { 0.f, g_iWinSizeX, 0.f, g_iWinSizeY };

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &vVector, sizeof(_float4))))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_Shader_Resource(m_pShaderCom, "g_Texture", 0)))
		return E_FAIL;

	Bind_ShaderTransform_Resourc(2);

	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CMouse::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex_UI"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Text_Mouse"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

void CMouse::FPS_Mode()
{
	RECT clientRect;
	POINT centerPoint;

	// 클라이언트 영역 크기 얻기
	GetClientRect(g_hWnd, &clientRect);

	// 중심 계산
	centerPoint.x = (clientRect.right - clientRect.left) / 2;
	centerPoint.y = (clientRect.bottom - clientRect.top) / 2;

	// 클라이언트 좌표를 스크린 좌표로 변환
	ClientToScreen(g_hWnd, &centerPoint);

	// 커서 이동
	SetCursorPos(centerPoint.x, centerPoint.y);
}

void CMouse::Mouse_Input()
{
	if (m_pGameInstance->IsMouseDown(MOUSEKEYSTATE::LB))
	{
		CMouse_Click_Fx::CLICKFX_POOL_DESC pDesc{};

		pDesc.vPos = { m_vPos };
		m_pGameInstance->Add_Pool_ToLayer(TEXT("Pool_Click_Fx"), ENUM_CLASS(LEVEL::STATIC), TEXT("Layer_UI"), &pDesc);
	}

	if (m_pGameInstance->IsMouseHold(MOUSEKEYSTATE::LB))
	{
		m_vMinUV = m_vSeleteMinUV;
		m_vMaxUV = m_vSeleteMaxUV;
	}
	else
	{
		m_vMinUV = m_vDefaultMinUV;
		m_vMaxUV = m_vDefaultMaxUV;
	}
	ShowCursor(FALSE);
}

CMouse* CMouse::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMouse* pInstance = new CMouse(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CMouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMouse::Clone(void* pArg)
{
	CMouse* pInstance = new CMouse(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMouse"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouse::Free()
{
	__super::Free();
	Safe_Release(m_pVIBufferCom);
}
