#include "pch.h"
#include "Body_WereWolf.h"

CBody_WereWolf::CBody_WereWolf(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject { pDevice, pContext }
{
}

CBody_WereWolf::CBody_WereWolf(const CBody_WereWolf& Prototype) : CPartObject (Prototype)
{
}

HRESULT CBody_WereWolf::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_WereWolf::Initialize(void* pArg)
{
	BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);
	m_pBlackBoard = pDesc->pBalckBoard;
	Safe_AddRef(m_pBlackBoard);

	m_iRootLodeIndex = 7;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBody_WereWolf::Priority_Update(_float fTimeDelta)
{
}

void CBody_WereWolf::Update(_float fTimeDelta)
{
	CMonster_WereWolf::WEREWOLF_DATA& WereWolf_Data = m_pBlackBoard->Set_Data();
	
	(*WereWolf_Data.bIsAnimFinsh) = m_pAnimCom->Player_Animation((*WereWolf_Data.iAnimState), (*WereWolf_Data.szAnimTag), (*WereWolf_Data.bIsAnimLoop), m_pModelCom, fTimeDelta, m_iRootLodeIndex);

	Update_CombinedMatrix();
}

void CBody_WereWolf::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

}

HRESULT CBody_WereWolf::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; i++)
	{
		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_DiffuseTexture", i, 0, 0)))
			continue;

		if (FAILED(m_pModelCom->Bind_BoneMatrices(m_pShaderCom, "g_BoneMatrices", i)))
			continue;

		m_pShaderCom->Begin(0);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

_float4x4* CBody_WereWolf::Get_BoneMatrix(const _wstring pBoneName)
{
	return m_pModelCom->Get_BoneMatrix(pBoneName);
}

_float3* CBody_WereWolf::Get_MovePos()
{
	return m_pModelCom->Get_PtrMovePos();
}

_float4* CBody_WereWolf::Get_MoveRot()
{
	return m_pModelCom->Get_PtrMoveRot();;
}

void CBody_WereWolf::Reset_MovePos()
{
	m_pModelCom->Reset_MovePos();
}

HRESULT CBody_WereWolf::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Model_Normal_Mon_2"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
		return E_FAIL;

	CAnimatio_Controller::ANIMTION_DESC Desc;
	Desc.szFile_Path = "../Bin/Resources/Models/Mon/Mon_2/Mon_2Anim.Json";
	Desc.szCulAnimName = *m_pBlackBoard->Get_Data().szAnimTag;
	Desc.iAnimIndex = *m_pBlackBoard->Get_Data().iAnimState;
	Desc.pModel = m_pModelCom;

	Desc.IsLoop = true;
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
		TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_WereWolf::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CBody_WereWolf* CBody_WereWolf::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_WereWolf* pInstance = new CBody_WereWolf(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBody_WereWolf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_WereWolf::Clone(void* pArg)
{
	CBody_WereWolf* pInstance = new CBody_WereWolf(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CBody_WereWolf"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_WereWolf::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pAnimCom);

	Safe_Release(m_pBlackBoard);
}
