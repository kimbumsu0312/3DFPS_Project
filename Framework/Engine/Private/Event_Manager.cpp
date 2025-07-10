#include "pch.h"
#include "Event_Manager.h"

CEvent_Manager::CEvent_Manager()
{
}

CEvent_Manager* CEvent_Manager::Create()
{
	return new CEvent_Manager();
}

void CEvent_Manager::Free()
{
	m_maplisteners.clear();
	__super::Free();
}

