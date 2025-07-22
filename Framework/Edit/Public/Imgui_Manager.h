#pragma once
#include "Base.h"

NS_BEGIN(Edit)

class CImgui_Manger final : public CBase
{
	DECLARE_SINGLETON(CImgui_Manger)
private:
	CImgui_Manger();
	virtual ~CImgui_Manger() = default;

public:
	HRESULT					Initalize(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	void					Render_Begin();
	void					Update_Logo();
	void					Update_Map();
	void					Update_Model();
	HRESULT					Redner_End();

	void					Selete_Object(class CMapObject& pObject, CTransform& pTransform);
	void					PosSet(_float4 fPos);
	void					ScaleSet(_float3 fScale);
	void					RotSet(_float3 fRot);
	_wstring				Get_ModelPath();
	void					Erase_Model();

	void					Move_Model(_float3 fPos);

	_float					Get_Brash() { return m_fBrash; }
	_float					Get_Height() { return m_fHeight; }
	_bool					Get_HeightUp() { return m_bIsHeightUP; }
	_float2					Get_MinMaxHeight() { return m_fMinMaxHeight; }

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	ID3D11RasterizerState*	m_pWireframeRS = { nullptr };
	ID3D11RasterizerState*	m_pSolidframeRS = { nullptr };
	CGameInstance*			m_pGameInstance = { nullptr };

	bool                    m_SwapChainOccluded = false;
	_uint                   m_ResizeWidth = 0, g_ResizeHeight = 0;

	bool					show_another_window = false;
	//저장할 파일들
	class CMapObject*		m_pModel = { nullptr };
	char					m_szSeleteModel[MAX_PATH];
	class CTransform*		m_pTransform = { nullptr };

	class CTerrain*			m_pTerrain = {nullptr };

	_wstring				m_pSaveLoader;
	_float3					m_vPos = {};
	_float3					m_vScale = {};
	_float3					m_vRot = {};
	LEVEL					m_eLevel = {};

	_int					m_iCurModel_Index = {};
	string					m_szModelPath = {};

	_float					m_fBrash = {10.f};
	_float					m_fHeight = {10.f};
	_bool					m_bIsHeightUP = { true };
	_float2					m_fMinMaxHeight = {0.f, 400.f};

	_bool					m_bISWireFream = {false};

	//세이브 타입
	_int					m_iMapSaveType = {0};
	const char*				m_szMapSaveType[ENUM_CLASS(DATA_TYPE::END)] = { "TERRAIN", "LEVEL"};
	
	DATA_TYPE				m_eSaveType = { DATA_TYPE::END };
	char					m_szFileName[MAX_PATH] = {};

public:
	virtual void			Free();
};
NS_END

