#pragma once
#include "Engine_Defines.h"
#include "ContainerObject.h"
#include "Client_Struct.h"
#include "BlackBoard.h"

NS_BEGIN(Engine)
class CCollider;
NS_END

NS_BEGIN(Client)
class CEvent_Door final : public CContainerObject
{
	enum class Door_Event{OPEN, CLOSE, END};
private:
	CEvent_Door(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CEvent_Door(const CEvent_Door& Prototype);
	virtual ~CEvent_Door() = default;

public:
	virtual HRESULT					Initialize_Prototype();
	virtual HRESULT					Initialize(void* pArg);
	virtual void					Priority_Update(_float fTimeDelta);
	virtual void					Update(_float fTimeDelta);
	virtual void					Late_Update(_float fTimeDelta);
	virtual HRESULT					Render();

private:
	class CModel_Door*				m_pModelObject = { nullptr };

	Door_Event						m_eDoorEvent = {};
	_float							m_fAngle = {};
	_float							m_fSpeed = {};
private:
	HRESULT							Ready_PartObjects();

public:
	static CEvent_Door* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject* Clone(void* pArg);
	virtual void					Free();

};

NS_END