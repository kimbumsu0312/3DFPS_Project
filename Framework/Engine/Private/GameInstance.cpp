#include "pch.h"
#include "GameInstance.h"

#include "Graphic_Device.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Renderer.h"
#include "Input_Manager.h"
#include "Event_Manager.h"
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

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;
	
	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pInput_Manager = CInput_Manager::Create();
	if (nullptr == m_pInput_Manager)
		return E_FAIL;

	m_pEvent_Manager = CEvent_Manager::Create();
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInput_Manager->Update(fTimeDelta);
	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pObject_Manager->Update(fTimeDelta);
	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Clear_Resources(_uint iClearLevelID)
{
	m_pPrototype_Manager->Clear(iClearLevelID);

	m_pObject_Manager->Clear(iClearLevelID);

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
	if (nullptr == m_pLevel_Manager || nullptr == m_pRenderer)
		return E_FAIL;

	m_pRenderer->Draw();

	if (FAILED(m_pLevel_Manager->Render()))
		return E_FAIL;

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

HRESULT CGameInstance::Open_Level(_uint iLevelID, CLevel* pNewLevel)
{
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	return m_pLevel_Manager->Open_Level(iLevelID, pNewLevel);
}

CComponent* CGameInstance::Find_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLayerLevelIndex, strLayerTag, strComponentTag, iIndex);
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	return m_pObject_Manager->Add_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, iPrototypeLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	return m_pPrototype_Manager->Add_Prototype(iPrototypeLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	if (nullptr == m_pPrototype_Manager)
		return nullptr;

	return m_pPrototype_Manager->Clone_Prototype(ePrototype, iPrototypeLevelIndex, strPrototypeTag, pArg);
}

HRESULT CGameInstance::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

HRESULT CGameInstance::Add_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates, const void* pDesc)
{
	return m_pRenderer->Add_RenderState(szRenderTag, eRenderStates, pDesc);
}

HRESULT CGameInstance::Switching_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates)
{
	return m_pRenderer->Switching_RenderState(szRenderTag, eRenderStates);
}

void CGameInstance::AddTrackIngKey(int iKey)
{
	m_pInput_Manager->AddTrackIngKey(iKey);
}

_bool CGameInstance::IsKeyDown(int iKey) const
{
	return m_pInput_Manager->IsKeyDown(iKey);
}

_bool CGameInstance::IsKeyUp(int iKey) const
{
	return m_pInput_Manager->IsKeyUp(iKey);
}

_bool CGameInstance::IsKeyHold(int iKey) const
{
	return m_pInput_Manager->IsKeyHold(iKey);
}

_float CGameInstance::GetKeyHoldTime(int iKey) const
{
	return m_pInput_Manager->GetKeyHoldTime(iKey);
}

void CGameInstance::Release_Engine()
{
	Release();
	Safe_Release(m_pInput_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pEvent_Manager);
}

void CGameInstance::Free()
{
	__super::Free();
}
