#include "GameInstance.h"

#include "Graphic_Device.h"
#include "Timer_Manager.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

#pragma region ENGINE
HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWinMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;
	
	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
}

HRESULT CGameInstance::Clear_Resources(_uint iClearLevelID)
{
	return S_OK;
}

void CGameInstance::Render_Begin(const _float4* pClearColor)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Clear_BackBuffer_View(pClearColor);
	
	m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Draw()
{
	return S_OK;
}

void CGameInstance::Render_End(HWND hWnd)
{
	if (nullptr == m_pGraphic_Device)
		return;

	m_pGraphic_Device->Present();
}

_float CGameInstance::Rand_Normal()
{
	return static_cast<_float>(rand()) / RAND_MAX;
}

_float CGameInstance::Rand(_float fMin, _float fMax)
{
	return fMin + Rand_Normal() * (fMax - fMin);
}
#pragma endregion

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

void CGameInstance::Release_Engine()
{
	Release();
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
}

void CGameInstance::Free()
{
	__super::Free();
}
