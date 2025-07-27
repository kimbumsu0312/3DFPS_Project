#include "pch.h"
#include "Animatio_Controller.h"
#include "Model.h"
CAnimatio_Controller::CAnimatio_Controller(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : CComponent(pDevice, pContext)
{
}

HRESULT CAnimatio_Controller::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimatio_Controller::Initialize(void* pArg)
{
	ANIMTION_DESC* pDesc = static_cast<ANIMTION_DESC*>(pArg);

	if (FAILED(Load_AnimData(pDesc->szFile_Path)))
		return E_FAIL;

	m_szCulAnimName = pDesc->szCulAnimName;
	m_CulAnimFrame = Find_Anim(m_szCulAnimName);
	return S_OK;
}

_bool CAnimatio_Controller::Player_Animation(string szAnimName, CModel* pModel, _float fTimeDelta, _int iRootNodeIndex, class CTransform* pTransform)
{
	_bool isAnimChange = false;
	if (m_szCulAnimName != szAnimName)
	{
		m_szCulAnimName = szAnimName;
		m_CulAnimFrame = Find_Anim(szAnimName);
		isAnimChange = true;
		//pModel->AnimChage();
	}
	return pModel->Play_Animation(fTimeDelta, m_eAnim_Status, m_CulAnimFrame, iRootNodeIndex, pTransform, isAnimChange);
}

void CAnimatio_Controller::Set_AnimStatus(ANIM_STATUS eStatus)
{
	m_eAnim_Status = eStatus;
}

void CAnimatio_Controller::Set_Animetion(CModel* pModel, _int iAnimIndex, _bool IsLoop)
{
	m_iCulAnimIndex = iAnimIndex;
	pModel->Set_Animations(m_iCulAnimIndex, IsLoop);
}


HRESULT CAnimatio_Controller::Load_AnimData(string pAnimFilePath)
{
	ifstream file(pAnimFilePath.c_str());
	if (!file.is_open())
	{
		MSG_BOX(TEXT("애니메이션 데이터 불러오기 실패"));
		return E_FAIL;
	}

	json jDatas;
	file >> jDatas;
	file.close();
	auto AnimetionArray = jDatas["Animetions"];

	for (const auto& jAnimtion : AnimetionArray)
	{
		unordered_map<string, ANIMEFRAME> AnimMap;

		auto AnimArray = jAnimtion["Anim"];

		for (const auto& jData : AnimArray)
		{
			ANIMEFRAME	AnimData = {};

			string szAnimName = jData["AnimeName"];
			AnimData.iStartFrame = jData["StartFrame"];
			AnimData.iEndFrame = jData["EndFrame"];
			AnimData.fTickPerSecond = jData["TickPerSecond"];

			AnimMap.emplace(szAnimName, move(AnimData));
		}
		m_AnimDatas.push_back(AnimMap);
	}
	return S_OK;
}

ANIMEFRAME CAnimatio_Controller::Find_Anim(const string pAnimName)
{
	auto    iter = m_AnimDatas[m_iCulAnimIndex].find(pAnimName);
	if (iter == m_AnimDatas[m_iCulAnimIndex].end())
	{
		MSG_BOX(TEXT("해당 애니메이션이 없습니다."));
		return m_CulAnimFrame;
	}
	return iter->second;
}

CAnimatio_Controller* CAnimatio_Controller::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAnimatio_Controller* pInstance = new CAnimatio_Controller(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
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
