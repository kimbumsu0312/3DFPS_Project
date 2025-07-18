#include "pch.h"
#include "SaveLoader.h"

CSaveLoader::CSaveLoader() : m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

CSaveLoader* CSaveLoader::Create()
{
	return new CSaveLoader();
}

void CSaveLoader::Free()
{
	__super::Free();
}
