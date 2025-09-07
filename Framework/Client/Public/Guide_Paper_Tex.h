#pragma once
#include "UIObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CGuide_Paper_Tex final : public CUIObject
{
public:
	enum class TEXT_TYPE { EVENT1, EVENT2, EVENT3, EVENT4, END };
private:
	CGuide_Paper_Tex(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGuide_Paper_Tex(const CGuide_Paper_Tex& Prototype);
	virtual ~CGuide_Paper_Tex() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize(void* pArg);
	virtual void				Priority_Update(_float fTimeDelta);
	virtual void				Update(_float fTimeDelta);
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render();

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	_int						m_iTexType = {};

private:
	HRESULT						Ready_Components();
	void						Render_Font_EVENT_1();
	void						Render_Font_EVENT_2();
	void						Render_Font_EVENT_3();
	void						Render_Font_EVENT_4();

public:
	static CGuide_Paper_Tex*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*		Clone(void* pArg) override;
	virtual void				Free() override;
};

NS_END