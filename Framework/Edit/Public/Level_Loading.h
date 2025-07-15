#pragma once
#include "Edit_Defines.h"
#include "Level.h"

NS_BEGIN(Edit)

class CLevel_Loading final : public CLevel
{
private:
	CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Loading() = default;

public:
	virtual HRESULT				Initialize(LEVEL eNextLevelID);
	virtual void				Update(_float fTimeDelta) override;
	virtual HRESULT				Render() override;

private:
	LEVEL						m_eNextLevelID = { LEVEL::END };
	class CLoader*				m_pLoader = { nullptr };

private:
	HRESULT						Ready_GameObjects();
	HRESULT						Ready_LoadingThread();

public:
	static CLevel_Loading*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID);
	virtual void				Free() override;
};

NS_END

