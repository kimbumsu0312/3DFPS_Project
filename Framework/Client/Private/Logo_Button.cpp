#include "pch.h"
#include "Logo_Button.h"

#include "Logo_Button_Line.h"
CLogo_Button::CLogo_Button(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CButton{ pDevice, pContext }
{
}

CLogo_Button::CLogo_Button(const CLogo_Button& Prototype) : CButton(Prototype)
{
}

HRESULT CLogo_Button::Initialize_Prototype()
{
	if (FAILED(Ready_Children_Prototype()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogo_Button::Initialize(void* pArg)
{
	if (pArg == nullptr)
		return E_FAIL;

	UIOBJECT_DESC* Desc = static_cast<UIOBJECT_DESC*>(pArg);

	m_vBackGroundColor = { 0.3f, 0.3f, 0.3f, 0.7f };
	m_vLocalPos.x = Desc->vPos.x;
	m_vLocalSize = Desc->vSize;
	m_iIndex = Desc->iIndex;
	m_iMaxIndex = Desc->iMaxIndex;

	m_vLocalPos.y = Desc->vPos.y + Desc->iIndex * Desc->OffsetY - (Desc->iMaxIndex - 1) * Desc->OffsetY / 2;;

	if (m_iIndex == 1)
		m_bIsSelete = true;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Children()))
		return E_FAIL;

	m_pGameInstance->Subscribe<Event_NonSelete_LogoButton>([&](const Event_NonSelete_LogoButton& e) {m_bIsSelete = false; });
	m_pGameInstance->Subscribe<Event_Selete_LogoButton>([&](const Event_Selete_LogoButton& e) { if (e.iIndex == m_iIndex) { m_bIsSelete = true; } });

	return S_OK;
}

void CLogo_Button::Priority_Update(_float fTimeDelta)
{

}

void CLogo_Button::Update(_float fTimeDelta)
{
	Button_Event();
	Button_Selete();
}

void CLogo_Button::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
		return;
	if (m_bIsSelete)
	{
		__super::Late_Update(fTimeDelta);
	}
}

HRESULT CLogo_Button::Render()
{

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Vector", &m_vBackGroundColor, sizeof(_float4))))
		return E_FAIL;

	__super::Bind_ShaderTransform_Resourc(1);

	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CLogo_Button::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogo_Button::Ready_Children_Prototype()
{
	if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button_Line"),
		CLogo_Button_Line::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogo_Button::Ready_Children()
{
	CUIObject* pGameObject = nullptr;
	
	UIOBJECT_DESC Desc;
	Desc.vSize = { 255.f, 18.f };
	Desc.vPos = { 0, 20 };
	Desc.vMinUV = { 500.f / 1024.f, 0.f };
	Desc.vMaxUV = { 755.f / 1024.f, 18.f / 512.f };


	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button_Line"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

	Desc.vSize = { 255.f, 20.f };
	Desc.vPos = { 0, 0 };
	Desc.vMinUV = { 500.f / 1024.f, 20.f / 512.f};
	Desc.vMaxUV = { 755.f / 1024.f, 40.f / 512.f };

	pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button_Line"), &Desc));
	if (nullptr == pGameObject)
		return E_FAIL;
	Add_Child(this, pGameObject, m_pShaderCom, m_pTextureCom);

	return S_OK;
}

void CLogo_Button::Button_Event()
{
	if (IsClick_Down(MOUSEKEYSTATE::LB) || m_pGameInstance->IsKeyDown(DIK_F) && m_bIsSelete)
	{
		switch (m_iIndex)
		{
		case 1:
			m_pGameInstance->Publish(Event_Level_Change{});
			break;
		case 2:
			break;
		case 3:
			break;
		case 4:
			break;
		}
	}
}

void CLogo_Button::Button_Selete()
{
	if (IsPick())
	{
		m_pGameInstance->Publish(Event_NonSelete_LogoButton{});
		m_pGameInstance->Publish(Event_Selete_LogoButton_Index{ {m_iIndex} });

		m_bIsSelete = true;
	}


}

CLogo_Button* CLogo_Button::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLogo_Button* pInstance = new CLogo_Button(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CLogo_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CLogo_Button::Clone(void* pArg)
{
	CLogo_Button* pInstance = new CLogo_Button(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CLogo_Button"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLogo_Button::Free()
{
	__super::Free();

	Safe_Release(m_pVIBufferCom);
}
