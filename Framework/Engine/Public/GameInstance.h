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
	_uint			Get_CulLevelID();
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
	
#ifdef _DEBUG
	void				IsDebugRender(DEBUG_RENDER eTag);
	HRESULT				Add_DebugComponent(class CComponent* pComponent);
#endif // DEBUG

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
	const LIGHT_DESC*			Get_LightDesc(_wstring LightTag);
	HRESULT						Add_Light(_wstring LightTag, LIGHT_DESC& LightDesc);
	HRESULT						Render_Lights(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
	_bool						Update_LightPotion(_wstring LightTag, _float4 LightPos);
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
	_bool						isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float& pDist);
	_vector						Get_LocalRayPos();
	_vector						Get_LocalRayDir();

	RAY_DESC					Create_FpsRayDesc(_float iOffSet);
#pragma endregion

#pragma region SaveLoader
public:
	HRESULT						File_Save_TerrainLevel(DATA_TYPE eData, string szFilename, CVIBuffer* pVIBuffer);
	HRESULT						File_Save_Object(string szFilename, MODELTYPE eType, const SAVE_MODEL& pData);
	HRESULT						File_Save_AnimData(string szFilename, const vector<vector<SAVE_ANIMDATA>>& AnimDatas);

	HRESULT						Add_SaveObject(class CGameObject* pObject, _int& pOut);
	HRESULT						Erase_SaveObject(_int iIndex);

	HRESULT						Load_Terrain(string FilePath, SAVE_TERRAIN& pOut);
	HRESULT						Load_Terrain_Client(string szFilePath, SAVE_TERRAIN& pOut);

	HRESULT						Load_Level(string FilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex);
	HRESULT						Load_Level(string szFilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex, _wstring szPrototypeTag);

	HRESULT						Load_Objcet(string FilePath, _uint iPrototypeLevelIndex, _wstring szPrototypeTag);

	void						Clear_Object();
	vector<class CGameObject*>* Get_Objects();
#pragma endregion

#pragma region Font_Manager
	HRESULT						Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	void						DrawText(const _wstring& strFontTag, const _tchar* pText, const _float2& vPosition, _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), _float fRadian = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), const _float2& vScale = _float2(1.f, 1.f));
#pragma endregion

#pragma region Collision_Manager
	HRESULT						Add_ColliderCheck(class CGameObject* pObject, class CCollider* pCollider);
	HRESULT						Add_ColliderRay(_uint iLayLayer, _uint iObjType, RAY_DESC& RayDesc);

	HRESULT						Set_LayerFilter(_uint iLayerNum, _uint iLayerFilter);
#pragma endregion 

#pragma region TARGET_MANAGER
	HRESULT						Add_RenderTarget(const _wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor);
	HRESULT						Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag);
	HRESULT						Begin_MRT(const _wstring& strMRTTag);
	HRESULT						End_MRT();
	HRESULT						Bind_RT_ShaderResource(const _wstring& strTargetTag, class CShader* pShader, const _char* pConstantName);

#ifdef _DEBUG
	HRESULT						Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT						Render_RT_Debug(class CShader* pShader, class CVIBuffer_Rect* pVIBuffer);
#endif
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
	class CFont_Manager*		m_pFont_Manager = { nullptr };
	class CCollision_Manager*	m_pCollision_Manager = { nullptr };
	class CTarget_Manager*		m_pTarget_Manager = { nullptr };

public:
	void						Release_Engine();
	virtual void				Free() override;

};

NS_END