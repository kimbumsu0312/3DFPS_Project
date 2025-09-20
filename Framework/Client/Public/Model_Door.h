#pragma once
#include "PartObject.h"
#include "Crow.h"

NS_BEGIN(Engine)
class CShader;
class CModel;
class CAnimatio_Controller;
NS_END

NS_BEGIN(Client)
class CModel_Door final : public CPartObject
{
private:
	CModel_Door(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CModel_Door(const CModel_Door& Prototype);
	virtual ~CModel_Door() = default;

public:
	virtual HRESULT							Initialize_Prototype();
	virtual HRESULT							Initialize(void* pArg);
	virtual void							Priority_Update(_float fTimeDelta);
	virtual void							Update(_float fTimeDelta);
	virtual void							Late_Update(_float fTimeDelta);
	virtual HRESULT							Render();

private:
	CShader*								m_pShaderCom = { nullptr };
	CModel*									m_pModelCom = { nullptr };

private:
	HRESULT									Ready_Components();
	HRESULT									Bind_ShaderResources();

public:
	static CModel_Door*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*					Clone(void* pArg) override;
	virtual void							Free() override;


};


NS_END