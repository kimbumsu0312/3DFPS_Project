#pragma once
#include "Engine_Defines.h"
#include "Base.h"

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

#pragma region TIMER_MANAGER
public:
	_float			Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT			Add_Timer(const _wstring& strTimerTag);
	void			Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

private:
	class CGraphic_Device*		m_pGraphic_Device = { nullptr };
	class CTimer_Manager*		m_pTimer_Manager = { nullptr };

public:
	void			Release_Engine();
	virtual void	Free() override;

};

NS_END