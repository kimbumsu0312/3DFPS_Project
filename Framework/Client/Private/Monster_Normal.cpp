#include "pch.h"
#include "Monster_Normal.h"
#include "Body_NorMon.h"
#include "WeaponObject.h"

#include "Stand_NorMon_1.h"
#include "Scouting_NorMon_1.h"
#include "Damage_NorMon_1.h"
#include "Chase_NorMon_1.h"
#include "Attack_NorMon_1.h"
#include "Body_NorMon.h"

#include "Player_Manager.h"

CMonster_Normal::CMonster_Normal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPoolingContainer(pDevice, pContext)
{
}

CMonster_Normal::CMonster_Normal(const CMonster_Normal& Prototype) : CPoolingContainer (Prototype)
{
}

HRESULT CMonster_Normal::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster_Normal::Initialize(void* pArg)
{
	m_iCulAnimState = ENUM_CLASS(NORMAL_MON_STATE::NORMAL);
	m_szAnimTag = "Stand_Loop";

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	if (FAILED(Ready_StateObjects()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(4.f, 3.f, 4.f, 1.f));
	
	switch (rand() % 4)
	{
	case ENUM_CLASS(NORMAL_MON_WEAPON::SWORD):
		m_pWeaponObject = Find_PartObject(TEXT("Part_Sword"));
		Safe_AddRef(m_pWeaponObject);
		m_iWeaponType = ENUM_CLASS(NORMAL_MON_WEAPON::SWORD);
		break;
	case ENUM_CLASS(NORMAL_MON_WEAPON::SHOTEL):
		m_pWeaponObject = Find_PartObject(TEXT("Part_Shotel"));
		Safe_AddRef(m_pWeaponObject);
		m_iWeaponType = ENUM_CLASS(NORMAL_MON_WEAPON::SHOTEL);
		break;
	case ENUM_CLASS(NORMAL_MON_WEAPON::HALBERD):
		m_pWeaponObject = Find_PartObject(TEXT("Part_Halberd"));
		Safe_AddRef(m_pWeaponObject);
		m_iWeaponType = ENUM_CLASS(NORMAL_MON_WEAPON::HALBERD);
		break;
	case ENUM_CLASS(NORMAL_MON_WEAPON::END):
		m_pWeaponObject = nullptr;
		m_iWeaponType = ENUM_CLASS(NORMAL_MON_WEAPON::END);
		break;

	}

	return S_OK;
}

void CMonster_Normal::Priority_Update(_float fTimeDelta)
{
	m_NorMonState = {};
	
	m_pBodyObject->Priority_Update(fTimeDelta);
	if(m_pWeaponObject != nullptr)
		m_pWeaponObject->Priority_Update(fTimeDelta);
}

void CMonster_Normal::Update(_float fTimeDelta)
{
	State_Check();
	m_pCulStateObject->Update(fTimeDelta, m_NorMonState);
	if (m_szPreStateTag != m_szCulStateTag)
	{
		m_pCulStateObject->Exit();
		Safe_Release(m_pCulStateObject);

		m_pCulStateObject = Find_StateObject(m_szCulStateTag);
		Safe_AddRef(m_pCulStateObject);

		m_pCulStateObject->Enter(m_NorMonState);
		m_szPreStateTag = m_szCulStateTag;
	}

	m_pBodyObject->Update(fTimeDelta);
	if (m_pWeaponObject != nullptr)
		m_pWeaponObject->Update(fTimeDelta);

}

void CMonster_Normal::Late_Update(_float fTimeDelta)
{
	m_pBodyObject->Late_Update(fTimeDelta);
	if (m_pWeaponObject != nullptr)
		m_pWeaponObject->Late_Update(fTimeDelta);
}

HRESULT CMonster_Normal::Render()
{
	return S_OK;
}

void CMonster_Normal::On_Dead()
{
	__super::On_Dead();
}

HRESULT CMonster_Normal::Initialize_Pool(void* pArg)
{
	return S_OK;
}

void CMonster_Normal::Return_Pool()
{
}

HRESULT CMonster_Normal::Ready_Components()
{
	return S_OK;
}

HRESULT CMonster_Normal::Ready_PartObjects()
{
	CBody_NorMon::BODY_DESC BodyDesc{};
	BodyDesc.pAnimState = &m_iCulAnimState;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	BodyDesc.pAnimTag = &m_szAnimTag;
	BodyDesc.pIsAnimLoop = &m_bIsAnimLoop;
	BodyDesc.pIsAnimFinsh = &m_bIsAnimFinsh;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1"), &BodyDesc)))
		return E_FAIL;

	CBody_NorMon* pBody = static_cast<CBody_NorMon*>(Find_PartObject(TEXT("Part_Body")));

	if (pBody == nullptr)
		return E_FAIL;

	m_pBodyObject = pBody;
	Safe_AddRef(m_pBodyObject);

	CWeaponObject::WEAPON_DESC WeaponDesc{};
	WeaponDesc.pCulStateTag = &m_szCulStateTag;
	WeaponDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();
	WeaponDesc.pSocketMatrix = pBody->Get_BoneMatrix(TEXT("R_Wep"));
	if (FAILED(__super::Add_PartObject(TEXT("Part_Sword"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1_Sword"), &WeaponDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Halberd"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1_Halberd"), &WeaponDesc)))
		return E_FAIL;

	if (FAILED(__super::Add_PartObject(TEXT("Part_Shotel"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Body_Monster_Normal_1_Shotel"), &WeaponDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CMonster_Normal::Ready_StateObjects()
{
	CMonState::Mon_STATE_DESC Desc{};
	Desc.pAnimState = &m_iCulAnimState;
	Desc.pAnimTag = &m_szAnimTag;
	Desc.pStateTag = &m_szCulStateTag;
	Desc.pIsAnimLoop = &m_bIsAnimLoop;
	Desc.pIsAnimFinsh = &m_bIsAnimFinsh;

	Add_StateObject(TEXT("Stand"), CStand_NorMon_1::Create(&Desc));
	Add_StateObject(TEXT("Attack"), CAttack_NorMon_1::Create(&Desc));
	Add_StateObject(TEXT("Chase"), CChase_NorMon_1::Create(&Desc));
	Add_StateObject(TEXT("Scouting"), CScouting_NorMon_1::Create(&Desc));
	Add_StateObject(TEXT("Damage"), CDamage_NorMon_1::Create(&Desc));

	m_pCulStateObject = Find_StateObject(TEXT("Stand"));
	Safe_AddRef(m_pCulStateObject);
	return S_OK;
}

HRESULT CMonster_Normal::Add_StateObject(const _wstring& strStateObjectTag, CStateObject* pStateObject)
{
	if (nullptr != Find_PartObject(strStateObjectTag))
		return E_FAIL;

	if (nullptr == pStateObject)
		return E_FAIL;

	m_StateObjects.emplace(strStateObjectTag, pStateObject);

	return S_OK;
}

CStateObject* CMonster_Normal::Find_StateObject(const _wstring& strStateObjectTag)
{
	auto    iter = m_StateObjects.find(strStateObjectTag);
	if (iter == m_StateObjects.end())
		return nullptr;

	return iter->second;
}

void CMonster_Normal::State_Check()
{
	_vector vPlayerPos = CPlayer_Manager::GetInstance()->Get_PlayerPos();
	_float fDis = {};
	vPlayerPos = XMVector3Length(m_pTransformCom->Get_State(STATE::POSITION) - vPlayerPos);
	XMStoreFloat(&fDis, vPlayerPos);

	if(fDis <= 5.f)
		m_NorMonState.isAttack = true;
	if (fDis <= 10.f)
		m_NorMonState.isChase = true;
	if (m_pGameInstance->IsKeyDown(DIK_J))
		m_NorMonState.isDamage = true;
	m_NorMonState.iWeponType = m_iWeaponType;
}

CMonster_Normal* CMonster_Normal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CMonster_Normal* pInstance = new CMonster_Normal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CMonster_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster_Normal::Clone(void* pArg)
{
	CMonster_Normal* pInstance = new CMonster_Normal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CMonster_Normal"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster_Normal::Free()
{
	Safe_Release(m_pBodyObject);
	for (auto& Pair : m_StateObjects)
		Safe_Release(Pair.second);
	
	m_StateObjects.clear();
	Safe_Release(m_pCulStateObject);
	Safe_Release(m_pWeaponObject);
}
