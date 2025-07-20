#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CSaveLoader final : public CBase
{
private:
	CSaveLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSaveLoader() = default;

public:
	HRESULT						File_Save(DATA_TYPE eData, string szFilename);
	HRESULT						Load_Terrain(string FilePath, SAVE_TERRAIN& pOut);
	
	HRESULT						Save_Object(string szFilename, const SAVE_MODEL& pData);
	void						Add_OBjcet(DATA_TYPE eData, CGameObject* pGameObject);
	void						Clear_Object(DATA_TYPE eData);
private:
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	
	class CGameInstance*		m_pGameInstance = { nullptr };


	vector<class CGameObject*>	m_vecObject[ENUM_CLASS(DATA_TYPE::END)];

	HRESULT						Save_Terrain(string FilePath);

public:
	static CSaveLoader*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};

NS_END