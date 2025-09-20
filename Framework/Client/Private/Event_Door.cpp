#include "pch.h"
#include "Event_Door.h"
#include "Model_Door.h"

CEvent_Door::CEvent_Door(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject(pDevice, pContext)
{
}

CEvent_Door::CEvent_Door(const CEvent_Door& Prototype) : CContainerObject(Prototype)
{
}

HRESULT CEvent_Door::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CEvent_Door::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;
	m_eDoorEvent = Door_Event::END;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(-17.41f, -8.67f, 45.17f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(90.f));

	m_pGameInstance->Subscribe<Event_OpenDoor>([&](const Event_OpenDoor& e) { e.isOpen ? m_eDoorEvent = Door_Event::OPEN : m_eDoorEvent = Door_Event::CLOSE; m_fSpeed = e.fSpeed;  });

	return S_OK;
}

void CEvent_Door::Priority_Update(_float fTimeDelta)
{
	m_pModelObject->Priority_Update(fTimeDelta);
}

void CEvent_Door::Update(_float fTimeDelta)
{
	switch (m_eDoorEvent)
	{
	case CEvent_Door::Door_Event::OPEN:
		m_fAngle -= fTimeDelta * m_fSpeed;
		if (m_fAngle <= XMConvertToRadians(-90.f))
		{
			m_fAngle = XMConvertToRadians(-90.f);
			m_eDoorEvent = Door_Event::END;
		}
		else
			m_pTransformCom->Turn(_vector{ 0.f, 1.f, 0.f, 0.f }, -fTimeDelta * m_fSpeed);
		break;
	case CEvent_Door::Door_Event::CLOSE:
		m_fAngle += fTimeDelta * m_fSpeed;
		if (m_fAngle >= XMConvertToRadians(0.f))
		{
			m_fAngle = XMConvertToRadians(0.f);
			m_eDoorEvent = Door_Event::END;
		}
		else
			m_pTransformCom->Turn(_vector{ 0.f, 1.f, 0.f, 0.f }, fTimeDelta * m_fSpeed);
		break;
	}
	m_pModelObject->Update(fTimeDelta);
}

void CEvent_Door::Late_Update(_float fTimeDelta)
{
	m_pModelObject->Late_Update(fTimeDelta);
}

HRESULT CEvent_Door::Render()
{
	return S_OK;
}

HRESULT CEvent_Door::Ready_PartObjects()
{
	CPartObject::PARTOBJECT_DESC Desc{};
	Desc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	if (FAILED(__super::Add_PartObject(TEXT("Part_Model"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Part_Model_Door"), &Desc)))
		return E_FAIL;

	CModel_Door* pBody = static_cast<CModel_Door*>(Find_PartObject(TEXT("Part_Model")));

	if (pBody == nullptr)
		return E_FAIL;

	m_pModelObject = pBody;
	Safe_AddRef(m_pModelObject);

	return S_OK;
}

CEvent_Door* CEvent_Door::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEvent_Door* pInstance = new CEvent_Door(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CEvent_Door"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEvent_Door::Clone(void* pArg)
{
	CEvent_Door* pInstance = new CEvent_Door(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CEvent_Door"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEvent_Door::Free()
{
	__super::Free();

	Safe_Release(m_pModelObject);
}
