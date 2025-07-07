#pragma once
#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CMainApp final : public CBase
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT							Initialize();
	void							Update(_float fTimeDelta);
	HRESULT							Render();

private:
	CGameInstance*					m_pGameInstance = { nullptr };
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };
	
private:
	HRESULT							Ready_Prototype_ForStatic();
	HRESULT							Start_Level(LEVEL eStartLevelID);

	void							Ready_Key_Setting();
public:
	static CMainApp*				Create();
	virtual void					Free() override;
};

NS_END
