#pragma once
#include "Base.h"
#include "Edit_Defines.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Edit)

class CEditMain final : public CBase
{
private:
	CEditMain();
	virtual ~CEditMain() = default;

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

public:
	static CEditMain*				Create();
	virtual void					Free() override;
};

NS_END
