#include "pch.h"
#include "Body_Crow.h"

CBody_Crow::CBody_Crow(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CPartObject{ pDevice, pContext }
{
}

CBody_Crow::CBody_Crow(const CBody_Crow& Prototype) : CPartObject(Prototype)
{
}

HRESULT CBody_Crow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Crow::Initialize(void* pArg)
{
	BODY_DESC* pDesc = static_cast<BODY_DESC*>(pArg);
	m_pBlackBoard = pDesc->pBalckBoard;
	Safe_AddRef(m_pBlackBoard);

	m_iRootLodeIndex = 3;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CBody_Crow::Priority_Update(_float fTimeDelta)
{
}

void CBody_Crow::Update(_float fTimeDelta)
{
	CCrow::CROW_DATA& Crow_Data = m_pBlackBoard->Set_Data();

	(*Crow_Data.bIsAnimFinsh) = m_pAnimCom->Player_Animation(0, (*Crow_Data.szAnimTag), (*Crow_Data.bIsAnimLoop), m_pModelCom, fTimeDelta, m_iRootLodeIndex);

	Update_CombinedMatrix();
}

void CBody_Crow::Late_Update(_float fTimeDelta)
{
	if (FAILED(m_pGameInstance->Add_RenderGroup(RENDERGROUP::NONBLEND, this)))
		return;

}

HRESULT CBody_Crow::Render()
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

		if (FAILED(m_pModelCom->Bind_Materials(m_pShaderCom, "g_NormalTexture", i, 1, 0)))
			continue;
		m_pShaderCom->Begin(2);

		m_pModelCom->Render(i);
	}

	return S_OK;
}

_float4x4* CBody_Crow::Get_BoneMatrix(const _wstring pBoneName)
{
	return m_pModelCom->Get_BoneMatrix(pBoneName);
}

_float3* CBody_Crow::Get_MovePos()
{
	return m_pModelCom->Get_PtrMovePos();
}

_float4* CBody_Crow::Get_MoveRot()
{
	return m_pModelCom->Get_PtrMoveRot();;
}

void CBody_Crow::Reset_MovePos()
{
	m_pModelCom->Reset_MovePos();
}

HRESULT CBody_Crow::Ready_Components()
{
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		TEXT("Com_Shader"), reinterpret_cast<CComponent**>(&m_pShaderCom), nullptr)))
		return E_FAIL;

	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::LOGO), TEXT("Prototype_Model_Crow"),
		TEXT("Com_Model"), reinterpret_cast<CComponent**>(&m_pModelCom), nullptr)))
		return E_FAIL;

	CAnimatio_Controller::ANIMTION_DESC Desc;
	Desc.szFile_Path = "../Bin/Resources/Models/Crow/CrowAnim.json";
	Desc.szCulAnimName = *m_pBlackBoard->Get_Data().szAnimTag;
	Desc.iAnimIndex = 0;
	Desc.pModel = m_pModelCom;

	Desc.IsLoop = true;
	if (FAILED(CGameObject::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Animatio_Controller"),
		TEXT("Com_AnimCom"), reinterpret_cast<CComponent**>(&m_pAnimCom), &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBody_Crow::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::VIEW))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", m_pGameInstance->Get_Transform_Float4x4(D3DTS::PROJ))))
		return E_FAIL;

	return S_OK;
}

CBody_Crow* CBody_Crow::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Crow* pInstance = new CBody_Crow(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CBody_Crow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CBody_Crow::Clone(void* pArg)
{
	CBody_Crow* pInstance = new CBody_Crow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Created : CBody_Crow"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBody_Crow::Free()
{
	__super::Free();
	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pAnimCom);

	Safe_Release(m_pBlackBoard);
}
