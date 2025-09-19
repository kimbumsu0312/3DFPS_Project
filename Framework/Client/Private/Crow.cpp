#include "pch.h"
#include "Crow.h"
#include "Body_Crow.h"

CCrow::CCrow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CContainerObject(pDevice, pContext)
{
}

CCrow::CCrow(const CCrow& Prototype) : CContainerObject(Prototype)
{
}

HRESULT CCrow::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CCrow::Initialize(void* pArg)
{
	m_szAnimTag = "Idle2";

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Utility()))
		return E_FAIL;

	if (FAILED(Ready_PartObjects()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, XMVectorSet(0.3f, 0.f, 0.f, 1.f));
	m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 1.f), XMConvertToRadians(220.f));

	m_pGameInstance->Subscribe<Event_Level_Change>([&](const Event_Level_Change& e) {m_bIsNextLevel = true; });

	return S_OK;
}

void CCrow::Priority_Update(_float fTimeDelta)
{
	m_pTransformCom->PrePostion_Update();
	m_pBodyObject->Priority_Update(fTimeDelta);
}

void CCrow::Update(_float fTimeDelta)
{
	if (m_bIsNextLevel == false)
	{
		if (m_bIsAnimFinsh == true && m_szAnimTag == "Idle2")
			m_szAnimTag = "Idle2";
		else	if (m_bIsAnimFinsh == true && m_szAnimTag == "Idle2")
			m_szAnimTag = "Idle2";
	}
	else
	{
		m_szAnimTag = "Fly2";
		m_bIsAnimLoop = false;
	}
	Root_Move();
	m_pBodyObject->Update(fTimeDelta);
}

void CCrow::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

	m_pBodyObject->Late_Update(fTimeDelta);
}

HRESULT CCrow::Render()
{
	return S_OK;
}

HRESULT CCrow::Ready_PartObjects()
{
	CBody_Crow::BODY_DESC BodyDesc{};
	BodyDesc.pBalckBoard = m_BlackBoard;
	BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrixPtr();

	if (FAILED(__super::Add_PartObject(TEXT("Part_Body"), ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_GameObject_Body_Crow"), &BodyDesc)))
		return E_FAIL;

	CBody_Crow* pBody = static_cast<CBody_Crow*>(Find_PartObject(TEXT("Part_Body")));

	if (pBody == nullptr)
		return E_FAIL;

	m_pBodyObject = pBody;
	Safe_AddRef(m_pBodyObject);

	return S_OK;
}

HRESULT CCrow::Ready_Utility()
{
	m_BlackBoard = CBlackBoard<CROW_DATA>::Create();
	m_BlackBoard->Set_Data().bIsAnimFinsh = &m_bIsAnimFinsh;
	m_BlackBoard->Set_Data().bIsAnimLoop = &m_bIsAnimLoop;
	m_BlackBoard->Set_Data().szAnimTag = &m_szAnimTag;

	return S_OK;
}

void CCrow::Root_Move()
{
	//월드 분해
	_vector vScale, vWorldRot, vWorldTrans;
	XMMatrixDecompose(&vScale, &vWorldRot, &vWorldTrans, m_pTransformCom->Get_WorldMatrix());
	_vector vMovePos = XMLoadFloat3(m_pBodyObject->Get_MovePos());
	_vector vMoveRot = XMLoadFloat4(m_pBodyObject->Get_MoveRot());

	//월드 기준으로 방향 보정
	vMovePos = XMVector3Rotate(vMovePos, vWorldRot);

	//이동량 누적
	vWorldTrans += vMovePos;
	//회전량 누적
	vWorldRot = XMQuaternionMultiply(XMVectorSetW(vMoveRot, 1.f), vWorldRot);
	vWorldRot = XMQuaternionNormalize(vWorldRot);

	_matrix ScaleMat = XMMatrixScalingFromVector(vScale);
	_matrix Movemat = XMMatrixTranslationFromVector(vWorldTrans);
	_matrix RotMat = XMMatrixRotationQuaternion(vWorldRot);

	_float4x4 WorldMatrix = {};
	XMStoreFloat4x4(&WorldMatrix, XMMatrixAffineTransformation(vScale, XMVectorSet(0.0f, 0.0f, 0.0f, 1.f), vWorldRot, vWorldTrans));

	m_pTransformCom->Set_WorldMatrix(WorldMatrix);

}

CCrow* CCrow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CCrow* pInstance = new CCrow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Crated : CCrow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCrow::Clone(void* pArg)
{
	CCrow* pInstance = new CCrow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CCrow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCrow::Free()
{
	__super::Free();
	
	Safe_Release(m_BlackBoard);
	Safe_Release(m_pBodyObject);
}
