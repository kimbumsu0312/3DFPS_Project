#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CSaveLoader final : public CBase
{
private:
	CSaveLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CSaveLoader() = default;

public:
	HRESULT						File_Save_TerrainLevel(DATA_TYPE eData, string szFilename, CVIBuffer* pVIBuffer);
	HRESULT						File_Save_Object(string szFilename, MODELTYPE eType, const SAVE_MODEL& pData);
	HRESULT						File_Save_AnimData(string szFilename, const vector<vector<SAVE_ANIMDATA>>& AnimDatas);

	HRESULT						Load_Terrain(string szFilePath, SAVE_TERRAIN& pOut);
	HRESULT						Load_Terrain_Client(string szFilePath, SAVE_TERRAIN& pOut);

	HRESULT						Load_Level(string szFilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex );
	HRESULT						Load_Level(string szFilePath, _uint iLevelIndex, _wstring szLayerTag, _uint iPrototypeLevelIndex, _wstring szPrototypeTag);

	HRESULT						Load_Objcet(string szFilePath, _uint iPrototypeLevelIndex, _wstring szPrototypeTag);

	HRESULT						Add_SaveObject(class CGameObject* pObject, _int &pOut);
	HRESULT						Erase_SaveObject(_int Index);
	void						Clear_Object();

	vector<class CGameObject*>* Get_Objects() { return &m_Objects; }
private:
	ID3D11Device*				m_pDevice;
	ID3D11DeviceContext*		m_pContext;
	class CGameInstance*		m_pGameInstance = { nullptr };
	vector<class CGameObject*>	m_Objects;

private:
	HRESULT						TerrainSave(string szFilename, CVIBuffer* pVIBuffer);
	HRESULT						LevelSave(string szFilename);

	HRESULT						Save_NonAnimMesh(string szFilename, const SAVE_MODEL& pData);
	HRESULT						Save_AnimMesh(string szFilename, const SAVE_MODEL& pData);


public:
	static CSaveLoader*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void				Free() override;
};

NS_END