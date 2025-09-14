#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CPicking final : public CBase
{
private:
	CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CPicking() = default;

public:
	HRESULT					Initialize(HWND hWnd);

	void					Update();

	RAY_DESC				Create_FpsRayDesc(_float fOffSet);
	void					TransformToLocalSpace(class CTransform& pTransformCom);
	_bool					isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3& pOut);
	_bool					isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float& pDist);

	_bool					isPicking(_float3* pOut);
	_vector					Get_LocalRayPos() { return m_vLocalRayPos; }
	_vector					Get_LocalRayDir() { return m_vLocalRayDir; }
#ifdef _DEBUG
	HRESULT					Ray_Render();
#endif // _DEBUG
private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

	HWND					m_hWnd = {};

	_float4x4				m_ViewMatrix = {};
	_uint					m_iWinSizeX{}, m_iWinSizeY{};

	_vector					m_vWorldRayDir{}, m_vWorldRayPos{};
	_vector					m_vLocalRayPos{}, m_vLocalRayDir{};

	ID3D11Texture2D*		m_pTexture2D = { nullptr };
	_float4*				m_pPixels = { nullptr };
	POINT					m_ptMouse = {};

#ifdef _DEBUG
	PrimitiveBatch<
		VertexPositionColor>*	m_pBatch = { nullptr };
	BasicEffect*				m_pEffect = { nullptr };
	ID3D11InputLayout*			m_pInputLayout = { nullptr };
	vector<RAY_DESC>			m_RayDescs;
#endif // _DEBUG
public:
	static CPicking*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void			Free();
};

NS_END