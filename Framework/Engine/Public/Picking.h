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
	void					TransformToLocalSpace(class CTransform& pTransformCom);
	_bool					isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3& pOut);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	class CGameInstance*	m_pGameInstance = { nullptr };

	HWND					m_hWnd = {};

	_float4x4				m_ViewMatrix = {};
	_uint					m_iWinSizeX{}, m_iWinSizeY{};

	_vector					m_vWorldRayDir{}, m_vWorldRayPos{};
	_vector					m_vLocalRayPos{}, m_vLocalRayDir{};
public:
	static CPicking*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	virtual void			Free();
};

NS_END