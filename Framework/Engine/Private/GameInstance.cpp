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
#include "Font_Manager.h"
#include "Collision_Manager.h"
#include "Target_Manager.h"
#include "Shadow.h"

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

	m_pShadow = CShadow::Create(EngineDesc.iWinSizeX, EngineDesc.iWinSizeY);
	if (nullptr == m_pShadow)
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

	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pTarget_Manager)
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

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	if (nullptr == m_pFont_Manager)
		return E_FAIL;

	m_pCollision_Manager = CCollision_Manager::Create(EngineDesc.iNumLayerFilter);
	if (nullptr == m_pCollision_Manager)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInput_Device->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pPicking->Update();

	m_pObject_Manager->Update(fTimeDelta);
	m_pPipeLine->Update();
	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pCollision_Manager->Update();
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
#ifdef _DEBUG
	if(FAILED(m_pPicking->Ray_Render()))
		return E_FAIL;
#endif // _DEBUG

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

_uint CGameInstance::Get_CulLevelID()
{
	return m_pLevel_Manager->Get_CulLevelID();
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
void CGameInstance::On_Static_Shadow(_bool IsOn)
{
	m_pRenderer->On_Static_Shadow(IsOn);
}
_bool CGameInstance::Get_MapShadow()
{
	return m_pRenderer->Get_MapShadowOn();
}
#ifdef _DEBUG

void CGameInstance::IsDebugRender(DEBUG_RENDER eTag)
{
	m_pRenderer->IsDebugRender(eTag);
}

HRESULT CGameInstance::Add_DebugComponent(CComponent* pComponent)
{
	return m_pRenderer->Add_DebugComponent(pComponent);
}
#endif // DEBUG

_matrix CGameInstance::Get_Transform_Matrix(D3DTS eTransformState) const
{
	return m_pPipeLine->Get_Transform_Matrix(eTransformState);
}

const _float4x4* CGameInstance::Get_Transform_Float4x4(D3DTS eTransformState) const
{
	return m_pPipeLine->Get_Transform_Float4x4(eTransformState);
}

const _float4x4* CGameInstance::Get_ShadowLight_Transform_Float4x4(D3DTS eTransformState) const
{
	return m_pShadow->Get_Transform_Float4x4(eTransformState);
}

HRESULT CGameInstance::Ready_ShadowLight(SHADOW_LIGHT_DESC LightDesc)
{
	return m_pShadow->Ready_ShadowLight(LightDesc);
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

const LIGHT_DESC* CGameInstance::Get_LightDesc(_wstring LightTag)
{
	return m_pLight_Manager->Get_LightDesc(LightTag);
}

HRESULT CGameInstance::Add_Light(_wstring LightTag, LIGHT_DESC& LightDesc)
{
	return m_pLight_Manager->Add_Light(LightTag, LightDesc);
}

HRESULT CGameInstance::Render_Lights(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render(pShader, pVIBuffer);
}

_bool CGameInstance::Update_LightPotion(_wstring LightTag, _float4 LightPos)
{
	return m_pLight_Manager->Update_LightPotion(LightTag, LightPos);
}

_bool CGameInstance::OnOff_Light(_wstring LightTag, _bool isOnoff)
{
	return m_pLight_Manager->OnOff_Light(LightTag, isOnoff);
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

_bool CGameInstance::isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float& pDist)
{
	return m_pPicking->isPickedInLocalSpace(vPointA, vPointB, vPointC, pDist);
}

_vector CGameInstance::Get_LocalRayPos()
{
	return m_pPicking->Get_LocalRayPos();
}

_vector CGameInstance::Get_LocalRayDir()
{
	return m_pPicking->Get_LocalRayDir();
}

RAY_DESC CGameInstance::Create_FpsRayDesc(_float iOffSet)
{
	return m_pPicking->Create_FpsRayDesc(iOffSet);
}

_bool CGameInstance::isPicking(_float3* pOut)
{
	return m_pPicking->isPicking(pOut);
}

HRESULT CGameInstance::File_Save_TerrainLevel(DATA_TYPE eData, string szFilename, CVIBuffer* pVIBuffer)
{
	return m_pSaveLoader->File_Save_TerrainLevel(eData, szFilename, pVIBuffer);
}

HRESULT CGameInstance::File_Save_Object(string szFilename, MODELTYPE eType, const SAVE_MODEL& pData)
{
	return m_pSaveLoader->File_Save_Object(szFilename, eType, pData);
}

HRESULT CGameInstance::File_Save_AnimData(string szFilename, const vector<vector<SAVE_ANIMDATA>>& AnimDatas)
{
	return m_pSaveLoader->File_Save_AnimData(szFilename, AnimDatas);
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

HRESULT CGameInstance::Load_Terrain_Client(string szFilePath, SAVE_TERRAIN& pOut)
{
	return m_pSaveLoader->Load_Terrain_Client(szFilePath, pOut);;
}

HRESULT CGameInstance::Load_Level(string FilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex)
{
	return m_pSaveLoader->Load_Level(FilePath, iLevelIndex, szLayerTag, iPrototypeLevelIndex);
}

HRESULT CGameInstance::Load_Level(string szFilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex, _wstring szPrototypeTag)
{
	return m_pSaveLoader->Load_Level(szFilePath, iLevelIndex, szLayerTag, iPrototypeLevelIndex, szPrototypeTag);
}

HRESULT CGameInstance::Load_Objcet(string FilePath, _uint iPrototypeLevelIndex, _wstring szPrototypeTag)
{
	return m_pSaveLoader->Load_Objcet(FilePath, iPrototypeLevelIndex, szPrototypeTag);
}

HRESULT CGameInstance::Load_ModelData(string szFilePath, SAVE_MODEL& pModelData)
{
	return m_pSaveLoader->Load_ModelData(szFilePath, pModelData);
}

void CGameInstance::Clear_Object()
{
	m_pSaveLoader->Clear_Object();
}

vector<class CGameObject*>* CGameInstance::Get_Objects()
{
	return m_pSaveLoader->Get_Objects();
}

HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}

void CGameInstance::DrawText(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor, _float fRadian, const _float2& vOrigin, const _float2& vScale)
{
	m_pFont_Manager->DrawText(strFontTag, pText, vPosition, vColor, fRadian, vOrigin, vScale);
}

HRESULT CGameInstance::Add_ColliderCheck(CGameObject* pObject, CCollider* pCollider)
{
	return m_pCollision_Manager->Add_ColliderCheck(pObject, pCollider);
}

HRESULT CGameInstance::Add_ColliderRay(_uint iLayLayer, _uint iObjType, RAY_DESC& RayDesc)
{
	return m_pCollision_Manager->Add_ColliderRay(iLayLayer, iObjType, RayDesc);
}

HRESULT CGameInstance::Set_LayerFilter(_uint iLayerNum, _uint iLayerFilter)
{
	return m_pCollision_Manager->Set_LayerFilter(iLayerNum, iLayerFilter);
}

HRESULT CGameInstance::Add_RenderTarget(const _wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iSizeX, iSizeY, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag, ID3D11DepthStencilView* pDSV, _bool isClear)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag, pDSV, isClear);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

HRESULT CGameInstance::Bind_RT_ShaderResource(const _wstring& strTargetTag, CShader* pShader, const _char* pConstantName)
{
	return m_pTarget_Manager->Bind_ShaderResource(strTargetTag, pShader, pConstantName);
}

HRESULT CGameInstance::Copy_Resource(const _wstring& strTargetTag, ID3D11Texture2D* pSourTexture)
{
	return m_pTarget_Manager->Copy_Resource(strTargetTag, pSourTexture);;
}
#ifdef _DEBUG

HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}

HRESULT CGameInstance::Render_RT_Debug(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render(pShader, pVIBuffer);
}
#endif // _DEBUG

void CGameInstance::Release_Engine()
{
	Release();
	Safe_Release(m_pShadow);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pFont_Manager);
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
