#include "pch.h"
#include "Player_Manager.h"

IMPLEMENT_SINGLETON(CPlayer_Manager)

CPlayer_Manager::CPlayer_Manager()
{
}

HRESULT CPlayer_Manager::Initialize()
{

	return S_OK;
}

void CPlayer_Manager::Free()
{
	__super::Free();
}
