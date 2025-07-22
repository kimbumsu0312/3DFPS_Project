#pragma once
#include "Prototype_Manager.h"
#include "Event_Manager.h"

NS_BEGIN(Engine)
class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)
private:
	CGameInstance();
	virtual ~CGameInstance() = default;

#pragma region ENGINE
public:
	HRESULT			Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void			Update_Engine(_float fTimeDelta);
	HRESULT			Clear_Resources(_uint iClearLevelID);

public:
	void			Render_Begin(const _float4* pClearColor);
	HRESULT			Draw();
	void			Render_End(HWND hWnd = 0);

public:
	_float			Rand_Normal();
	_float			Rand(_float fMin, _float fMax);
#pragma endregion

#pragma region INPUT_DEVICE
public:
	_byte			Get_DIKeyState(_ubyte byKeyID);
	_byte			Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long			Get_DIMouseMove(MOUSEMOVESTATE eMouseState);
	_bool			IsKeyDown(_ubyte byKeyID);
	_bool			IsKeyUp(_ubyte byKeyID);
	_bool			IsKeyHold(_ubyte byKeyID);
	_bool			IsMouseDown(MOUSEKEYSTATE eMouse);
	_bool			IsMouseUp(MOUSEKEYSTATE eMouse);
	_bool			IsMouseHold(MOUSEKEYSTATE eMouse);
#pragma endregion 

#pragma region TIMER_MANAGER
public:
	_float			Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT			Add_Timer(const _wstring& strTimerTag);
	void			Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT			Open_Level(_uint iLevelID, class CLevel* pNewLevel);
#pragma endregion

#pragma region OBJECT_MANAGER
public:
	class CComponent*	Find_Component(_uint iLayerLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	HRESULT				Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
	HRESULT				Add_GameObject_ToLayer(_uint iLayerLevelIndex, const _wstring& strLayerTag, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CGameObject* pGameObject);
	HRESULT				Add_PoolGameObject_ToLayer(class CPoolingObject* pObject, _uint iLayerLevelIndex, const _wstring& strLayerTag);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
public:
	HRESULT				Add_Prototype(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase*		Clone_Prototype(PROTOTYPE ePrototype, _uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region RENDERER
public:
	HRESULT				Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pRenderObject);
	HRESULT				Add_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates, const void* pDesc);
	HRESULT				Switching_RenderState(_wstring szRenderTag, RENDERSTATE eRenderStates);

#pragma endregion

#pragma region EVENT_MANAGER
private:
	class CEvent_Manager* m_pEvent_Manager = { nullptr };
public:
	template<typename EventType>
	using HandlerFunc = function<void(const EventType&)>;
	
	template<typename EventType>
	void Subscribe(HandlerFunc<EventType> handler)	{
		m_pEvent_Manager->Subscribe(handler);
	}

	template<typename EventType>
	void Publish(const EventType& event)	{
		m_pEvent_Manager->Publish(event);
	}
#pragma endregion

#pragma region PIPELINE
public:
	_matrix						Get_Transform_Matrix(D3DTS eTransformState) const;
	const _float4x4*			Get_Transform_Float4x4(D3DTS eTransformState) const;
	_matrix						Get_Transform_Matrix_Inverse(D3DTS eTransformState) const;
	const _float4x4*			Get_Transform_Float4x4_Inverse(D3DTS eTransformState) const;
	const _float4*				Get_CamPosition() const;

	void						Set_Transform(D3DTS eTransformState, _fmatrix Matrix);
	void						Set_Transform(D3DTS eTransformState, const _float4x4& Matrix);

#pragma endregion

#pragma region LIGHT_MANAGER
public:
	const LIGHT_DESC*			Get_LightDesc(_uint iIndex);
	HRESULT						Add_Light(LIGHT_DESC& LightDesc);
#pragma endregion

#pragma region POOLING_MANAGER
public:
	HRESULT						Add_Object_ToPool(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iValue, void* pArg);
	HRESULT						Add_Pool_ToLayer(const _wstring& szPoolingPath, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg);
	HRESULT						Return_Object(class CPoolingObject* pObject, const _wstring& szPoolingPath);
#pragma endregion

#pragma region Garbage_Collector
public:
	void						GarbageSweep(class CGameObject* pObject);
	void						Clear_Garbage();
#pragma endregion

#pragma region Picking
public:
	void						TransformToLocalSpace(class CTransform& pTransformCom);
	_bool						isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3& pOut);
#pragma endregion

#pragma region SaveLoader
public:
	HRESULT						File_Save_TerrainLevel(DATA_TYPE eData, string szFilename, CVIBuffer* pVIBuffer);
	HRESULT						File_Save_Object(string szFilename, MODELTYPE eType, SAVE_MODEL* pData);

	HRESULT						Add_SaveObject(class CGameObject* pObject, _int& pOut);
	HRESULT						Erase_SaveObject(_int iIndex);

	HRESULT						Load_Terrain(string FilePath, SAVE_TERRAIN& pOut);
	HRESULT						Load_Level(string FilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex);

	void						Clear_Object(DATA_TYPE eData);
#pragma endregion
private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CInput_Device*		m_pInput_Device = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };
	class CLevel_Manager*		m_pLevel_Manager = { nullptr };
	class CObject_Manager*		m_pObject_Manager = { nullptr };
	class CPrototype_Manager*	m_pPrototype_Manager = { nullptr };
	class CRenderer*			m_pRenderer = { nullptr };
	class CPipeLine*			m_pPipeLine = { nullptr };
	class CLight_Manager*		m_pLight_Manager = { nullptr };
	class CPooling_Manager*		m_pPooling_Manager = { nullptr };
	class CGarbage_Collector*	m_pGarbage_Collector = { nullptr };
	class CPicking*				m_pPicking = { nullptr };
	class CSaveLoader*			m_pSaveLoader = { nullptr };
public:
	void						Release_Engine();
	virtual void				Free() override;

};

NS_END