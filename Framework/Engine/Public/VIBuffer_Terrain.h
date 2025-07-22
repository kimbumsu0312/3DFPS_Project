#pragma once
#include "VIBuffer.h"

NS_BEGIN(Engine)
class ENGINE_DLL CVIBuffer_Terrain final : public CVIBuffer
{
public:

	typedef struct tagVIBuffer_Terrain
	{
		_uint	iNumverticesX = {};
		_uint	iNumverticesZ = {};
		SAVE_TERRAIN* Terrain_Data = {nullptr};
	}VIBUFFER_TERRAIN_DESC;


private:
	CVIBuffer_Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Terrain(const CVIBuffer_Terrain& Prototype);
	virtual ~CVIBuffer_Terrain() = default;

public:
	virtual HRESULT				Initialize_Prototype();
	virtual HRESULT				Initialize_Prototype(const _tchar* pHeightMapFilePath);
	virtual HRESULT				Initialize();
	virtual HRESULT				Initialize(void* pArg) override;
	
	void						Terrain_Hight(bool raise, _float brushRadius, _float intensity, class CTransform& pTransformCom, _float2 vMinMax);
	const SAVE_TERRAIN&			Save_Terrain();

	_bool						IsPicked(class CTransform& pTransform, _float3& pOut);
private:
	SAVE_TERRAIN				m_TerrainData = {};
private:
	void						UpdateTerrain();

public:
	static CVIBuffer_Terrain*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pHeightMapFilePath);
	static CVIBuffer_Terrain*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*			Clone(void* pArg);
	virtual void				Free() override;
};

NS_END