#include "pch.h"
#include "GameInstance.h"

#include "Graphic_Device.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"
#include "Prototype_Manager.h"
#include "Renderer.h"
#include "Event_Manager.h"
#include "PipeLine.h"
#include "Input_Device.h"
#include "Light_Manager.h"
#include "Pooling_Manager.h"
#include "Garbage_Collector.h"
#include "Picking.h"
#include "SaveLoader.h"

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
	
	m_pInput_Device = CInput_Device::Create(EngineDesc.hInst, EngineDesc.hWnd);
	if (nullptr == m_pInput_Device)
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

	m_pEvent_Manager = CEvent_Manager::Create();
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	m_pPipeLine = CPipeLine::Create();
	if (nullptr == m_pPipeLine)
		return E_FAIL;

	m_pLight_Manager = CLight_Manager::Create();
	if (nullptr == m_pLight_Manager)
		return E_FAIL;

	m_pPooling_Manager = CPooling_Manager::Create();
	if (nullptr == m_pPooling_Manager)
		return E_FAIL;

	m_pGarbage_Collector = CGarbage_Collector::Create();
	if (nullptr == m_pGarbage_Collector)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppDevice, *ppContext, EngineDesc.hWnd);
	if (nullptr == m_pPicking)
		return E_FAIL;

	m_pSaveLoader = CSaveLoader::Create(*ppDevice, *ppContext);
	if (nullptr == m_pSaveLoader)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInput_Device->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pPipeLine->Update();
	m_pPicking->Update();

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

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}
_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}
_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);
}

_bool CGameInstance::IsKeyHold(_ubyte byKeyID)
{
	return m_pInput_Device->IsKeyHold(byKeyID);
}

_bool CGameInstance::IsMouseDown(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->IsMouseDown(eMouse);
}

_bool CGameInstance::IsMouseUp(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->IsMouseUp(eMouse);
}

_bool CGameInstance::IsMouseHold(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->IsMouseHold(eMouse);
}

_bool CGameInstance::IsKeyDown(_ubyte byKeyID)
{
	return m_pInput_Device->IsKeyDown(byKeyID);
}

_bool CGameInstance::IsKeyUp(_ubyte byKeyID)
{
	return m_pInput_Device->IsKeyUp(byKeyID);
}

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

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, CGameObject* pGameObject)
{
	return m_pObject_Manager->Add_GameObject_ToLayer(iLayerLevelIndex, strLayerTag, iPrototypeLevelIndex, strPrototypeTag, pGameObject);;
}

HRESULT CGameInstance::Add_PoolGameObject_ToLayer(CPoolingObject* pObject, _uint iLayerLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Add_PoolGameObject_ToLayer(pObject, iLayerLevelIndex, strLayerTag);
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

_matrix CGameInstance::Get_Transform_Matrix(D3DTS eTransformState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eTransformState);
}

const _float4x4* CGameInstance::Get_Transform_Float4x4(D3DTS eTransformState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eTransformState);
}

_matrix CGameInstance::Get_Transform_Matrix_Inverse(D3DTS eTransformState) const
{
	return m_pPipeLine->Get_Transform_Matrix_Inverse(eTransformState);
}

const _float4x4* CGameInstance::Get_Transform_Float4x4_Inverse(D3DTS eTransformState) const
{
	return m_pPipeLine->Get_Transform_Float4x4_Inverse(eTransformState);
}

const _float4* CGameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}

void CGameInstance::Set_Transform(D3DTS eTransformState, _fmatrix Matrix)
{
	m_pPipeLine->Set_Transform(eTransformState, Matrix);
}

void CGameInstance::Set_Transform(D3DTS eTransformState, const _float4x4& Matrix)
{
	m_pPipeLine->Set_Transform(eTransformState, Matrix);
}

const LIGHT_DESC* CGameInstance::Get_LightDesc(_uint iIndex)
{
	return m_pLight_Manager->Get_LightDesc(iIndex);
}

HRESULT CGameInstance::Add_Light(LIGHT_DESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightDesc);
}

HRESULT CGameInstance::Add_Object_ToPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iValue, void* pArg)
{
	return m_pPooling_Manager->Add_Object_ToPool(iPrototypeLevelIndex, strPrototypeTag, iValue, pArg);
}

HRESULT CGameInstance::Add_Pool_ToLayer(const _wstring& szPoolingPath, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pPooling_Manager->Add_Pool_ToLayer(szPoolingPath, iLayerLevelIndex, strLayerTag, pArg);
}

HRESULT CGameInstance::Return_Object(CPoolingObject* pObject, const _wstring& szPoolingPath)
{
	return m_pPooling_Manager->Return_Object(pObject, szPoolingPath);
}

void CGameInstance::GarbageSweep(CGameObject* pObject)
{
	m_pGarbage_Collector->GarbageSweep(pObject);
}

void CGameInstance::Clear_Garbage()
{
	m_pGarbage_Collector->Clear_Garbage();
}

void CGameInstance::TransformToLocalSpace(CTransform& pTransformCom)
{
	m_pPicking->TransformToLocalSpace(pTransformCom);
}

_bool CGameInstance::isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3& pOut)
{
	return m_pPicking->isPickedInLocalSpace(vPointA, vPointB, vPointC, pOut);
}

HRESULT CGameInstance::File_Save_TerrainLevel(DATA_TYPE eData, string szFilename, CVIBuffer* pVIBuffer)
{
	return m_pSaveLoader->File_Save_TerrainLevel(eData, szFilename, pVIBuffer);
}

HRESULT CGameInstance::File_Save_Object(string szFilename, MODELTYPE eType, SAVE_MODEL* pData)
{
	return m_pSaveLoader->File_Save_Object(szFilename, eType, pData);
}

HRESULT CGameInstance::Add_SaveObject(CGameObject* pObject, _int& pOut)
{
	return m_pSaveLoader->Add_SaveObject(pObject, pOut);
}

HRESULT CGameInstance::Erase_SaveObject(_int iIndex)
{
	return m_pSaveLoader->Erase_SaveObject(iIndex);
}

HRESULT CGameInstance::Load_Terrain(string FilePath, SAVE_TERRAIN& pOut)
{
	return m_pSaveLoader->Load_Terrain(FilePath, pOut);
}

HRESULT CGameInstance::Load_Level(string FilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex)
{
	return m_pSaveLoader->Load_Level(FilePath, iLevelIndex, szLayerTag, iPrototypeLevelIndex);
}

void CGameInstance::Clear_Object(DATA_TYPE eData)
{
	m_pSaveLoader->Clear_Object(eData);
}

void CGameInstance::Release_Engine()
{
	Release();
	Safe_Release(m_pSaveLoader);
	Safe_Release(m_pPicking);
	Safe_Release(m_pGarbage_Collector);
	Safe_Release(m_pPooling_Manager);
	Safe_Release(m_pLight_Manager);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pEvent_Manager);
}

void CGameInstance::Free()
{
	__super::Free();
}
