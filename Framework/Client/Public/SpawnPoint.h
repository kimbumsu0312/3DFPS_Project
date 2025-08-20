#pragma once
#include "PartObject.h"
NS_BEGIN(Engine)
class CShader;
class CVIBuffer_Cube;
NS_END

NS_BEGIN(Client)
class CSpawnPoint final : public CPartObject
{
public:
	typedef struct SpawnPointDesc : public CPartObject::PARTOBJECT_DESC {
		_vector		vPostion;
	}SPAWNPOINTDESC;
private:
	CSpawnPoint(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CSpawnPoint(const CSpawnPoint& Prototype);
	virtual ~CSpawnPoint() = default;

public:
	virtual HRESULT			Initialize_Prototype();
	virtual HRESULT			Initialize(void* pArg);
	virtual void			Priority_Update(_float fTimeDelta);
	virtual void			Update(_float fTimeDelta);
	virtual void			Late_Update(_float fTimeDelta);
	virtual HRESULT			Render();

private:
	CShader*				m_pShaderCom = { nullptr };
	CVIBuffer_Cube*			m_pVIBufferCom = { nullptr };

private:
	HRESULT					Ready_Components();
	HRESULT					Bind_ShaderResources();

public:
	static CSpawnPoint*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*	Clone(void* pArg) override;
	virtual void			Free() override;
};

NS_END