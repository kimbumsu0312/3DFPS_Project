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

private:
	ID3D11Device*			m_pDevice = nullptr;
	ID3D11DeviceContext*	m_pContext = nullptr;
	CGameInstance*			m_pGameInstance = { nullptr };

	bool                    m_SwapChainOccluded = false;
	_uint                   m_ResizeWidth = 0, g_ResizeHeight = 0;

	bool					show_another_window = false;
	
	class CMapObject*			m_pModel = { nullptr };
	char					m_szSeleteModel[MAX_PATH];

	class CTransform*		m_pTransform = { nullptr };
	_float3					m_vPos = {};
	_float3					m_vScale = {};
	_float3					m_vRot = {};
	LEVEL					m_eLevel = {};

	_int					m_iCurModel_Index = {};
	string					m_szModelPath = {};
public:
	virtual void			Free();
};
NS_END

