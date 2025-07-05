#include "pch.h"
#include "Logo_Button.h"

//#include "Logo_Button_Line.h"
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
	m_vLocalPos.x = g_iWinSizeX >> 1;
	m_vLocalPos.y = 300.f;
	m_vLocalSize.x = 512.f;
	m_vLocalSize.y = 512.f;

	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_Children()))
		return E_FAIL;

	return S_OK;
}

void CLogo_Button::Priority_Update(_float fTimeDelta)
{

}

void CLogo_Button::Update(_float fTimeDelta)
{
}

void CLogo_Button::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::UI, this)))
		return;
	__super::Late_Update(fTimeDelta);
}

HRESULT CLogo_Button::Render()
{
	__super::Bind_Shader_Resourec(m_pShaderCom);

	m_pVIBufferCom->Bind_Resources();
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CLogo_Button::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxPosTex"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLogo_Button::Ready_Children_Prototype()
{
	//if (FAILED(m_pGameInstance->Add_Prototype(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button_Line"),
	//	CLogo_Button_Line::Create(m_pDevice, m_pContext))))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CLogo_Button::Ready_Children()
{
	CUIObject* pGameObject = nullptr;

	//pGameObject = dynamic_cast<CUIObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Logo_Button_Line")));
	//if (nullptr == pGameObject)
	//	return E_FAIL;
	//Add_Child(this, pGameObject);

	return S_OK;
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
	Safe_Release(m_pShaderCom);
}
