#pragma once

#include "Edit_Defines.h"
#include "Level.h"

NS_BEGIN(Edit)

class CLevel_Model final : public CLevel
{
private:
	CLevel_Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CLevel_Model() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Lights();
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);

private:
	_bool	m_bInvenOpen = { false };
public:
	static CLevel_Model* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void Free() override;
};

NS_END