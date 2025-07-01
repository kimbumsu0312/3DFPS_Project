#include "pch.h"
#include "Input_Manager.h"

CInput_Manager::CInput_Manager()
{
}

HRESULT CInput_Manager::Initialize()
{
	m_mapPreState.clear();
	m_mapCurState.clear();
	m_mapKeyHoldTime.clear();
	m_vecTrackingKeys.clear();

	return S_OK;
}

void CInput_Manager::Update(float fTimeDelta)
{
	for (_int iK : m_vecTrackingKeys)
	{
		bool bCurKey = (GetAsyncKeyState(iK) & 0x8000) != 0;

		m_mapPreState[iK] = m_mapCurState[iK];
		m_mapCurState[iK] = bCurKey;

		if (bCurKey)
			m_mapKeyHoldTime[iK] += fTimeDelta;
		else
			m_mapKeyHoldTime[iK] = 0.0f;
	}
}

void CInput_Manager::AddTrackIngKey(int iKey)
{
	if (find(m_vecTrackingKeys.begin(), m_vecTrackingKeys.end(), iKey) == m_vecTrackingKeys.end())
		m_vecTrackingKeys.push_back(iKey);
}

_bool CInput_Manager::IsKeyDown(int iKey) const
{
	auto iterPre_Key = m_mapPreState.find(iKey);
	auto iterCul_Key = m_mapCurState.find(iKey);

	return iterCul_Key != m_mapCurState.end() && iterCul_Key->second && (!iterPre_Key->second);
}

_bool CInput_Manager::IsKeyUp(int iKey) const
{
	auto iterPre_Key = m_mapPreState.find(iKey);
	auto iterCul_Key = m_mapCurState.find(iKey);

	return iterCul_Key != m_mapCurState.end() && !iterCul_Key->second && iterPre_Key->second;
}

_bool CInput_Manager::IsKeyHold(int iKey) const
{
	auto iterCul_Key = m_mapCurState.find(iKey);

	return iterCul_Key != m_mapCurState.end() && iterCul_Key->second;
}

_float CInput_Manager::GetKeyHoldTime(int iKey) const
{
	auto iter = m_mapKeyHoldTime.find(iKey);
	if (iter != m_mapKeyHoldTime.end())
		return iter->second;

	return 0.0f;
}

CInput_Manager* CInput_Manager::Create()
{
	CInput_Manager* pInstance = new CInput_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CInput_Manager"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CInput_Manager::Free()
{
	__super::Free();
}
