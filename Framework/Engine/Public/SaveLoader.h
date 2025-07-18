#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CSaveLoader final : public CBase
{
private:
	CSaveLoader();
	virtual ~CSaveLoader() = default;


private:
	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	static CSaveLoader*			Create();
	virtual void				Free() override;
};

NS_END