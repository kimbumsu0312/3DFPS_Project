#include "pch.h"
#include "Animatio_Controller.h"
#include "Model.h"
CAnimatio_Controller::CAnimatio_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent(pDevice, pContext)
{
}

HRESULT CAnimatio_Controller::Initialize_Prototype(const string* pAnimFilePath)
{

	if (FAILED(Load_AnimData(pAnimFilePath)))
		return E_FAIL;
	return S_OK;
}

HRESULT CAnimatio_Controller::Initialize(void* pArg)
{
	return S_OK;
}

_bool CAnimatio_Controller::Player_Animation(string szAnimName, CModel* pModel, _float fTimeDelta)
{
	if (m_szCulAnimName != szAnimName)
		m_CulAnim = Find_Anim(szAnimName);

	return pModel->Play_Animation(fTimeDelta, m_eAnim_Status, m_CulAnim);
}

void CAnimatio_Controller::Set_AnimStatus(ANIM_STATUS eStatus)
{
	m_eAnim_Status = eStatus;
}

HRESULT CAnimatio_Controller::Load_AnimData(const string* pAnimFilePath)
{
	ifstream file(pAnimFilePath->c_str());
	if (!file.is_open())
	{
		MSG_BOX(TEXT("애니메이션 데이터 불러오기 실패"));
		return E_FAIL;
	}

	json jDatas;
	file >> jDatas;
	file.close();
	
	auto AnimArray = jDatas["AnimData"];

	for (const auto& jData : AnimArray)
	{
		ANIMEFRAME	AnimData = {};

		string szAnimName = jData["AnimeName"];
		AnimData.iStartFrame = jData["StartFrame"];
		AnimData.iEndFrame = jData["EndFrame"];
		AnimData.fTickPerSecond = jData["TickPerSecond"];

		m_AnimDatas.emplace(szAnimName, AnimData);
	}

	return S_OK;
}

ANIMEFRAME CAnimatio_Controller::Find_Anim(const string pAnimName)
{
	auto    iter = m_AnimDatas.find(pAnimName);
	if (iter == m_AnimDatas.end())
	{
		MSG_BOX(TEXT("해당 애니메이션이 없습니다."));
		return m_CulAnim;
	}
	return iter->second;
}

CAnimatio_Controller* CAnimatio_Controller::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const string* pAnimFilePath)
{
    CAnimatio_Controller* pInstance = new CAnimatio_Controller(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pAnimFilePath)))
    {
        MSG_BOX(TEXT("Failed to Created : CAnimatio_Controller"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CAnimatio_Controller::Clone(void* pArg)
{
    CAnimatio_Controller* pInstance = new CAnimatio_Controller(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Created : CAnimatio_Controller"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAnimatio_Controller::Free()
{
	__super::Free();

    m_AnimDatas.clear();
}
