#include "pch.h"
#include "Picking.h"
#include "Engine_Defines.h"
#include "Transform.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext) : m_pDevice{ pDevice }, m_pContext{pContext}, m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	D3D11_VIEWPORT Viewport{};
	_uint iNumViewports = { 1 };
	m_pContext->RSGetViewports(&iNumViewports, &Viewport);

	m_iWinSizeX = Viewport.Width;
	m_iWinSizeY = Viewport.Height;

	return S_OK;
}

void CPicking::Update()
{
	POINT	ptMouse{};
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	_float3	vMousePos{};

	vMousePos.x = ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
	vMousePos.y = 1.f - ptMouse.y / (m_iWinSizeY * 0.5f);
	vMousePos.z = 0.f;
	XMVECTOR vMouseNDC = XMLoadFloat3(&vMousePos);

	vMouseNDC = XMVector3TransformCoord(vMouseNDC, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::PROJ));

	XMVECTOR vWolrdPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	
	vMouseNDC = XMVector3TransformNormal(vMouseNDC, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::VIEW));
	m_vWorldRayPos = XMVector3TransformCoord(vWolrdPos, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::VIEW));
	m_vWorldRayDir = XMVector3Normalize(vMouseNDC);

}

void CPicking::TransformToLocalSpace(CTransform& pTransformCom)
{
	m_vLocalRayPos = XMVector3TransformCoord(m_vWorldRayPos, pTransformCom.Get_WorldMatrix_Inverse());
	m_vLocalRayDir = XMVector3TransformNormal(m_vWorldRayDir, pTransformCom.Get_WorldMatrix_Inverse());

	m_vLocalRayDir = XMVector3Normalize(m_vLocalRayDir);
}

_bool CPicking::isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float3& pOut)
{
	_float fDist{};
	_bool isPicked = DirectX::TriangleTests::Intersects(m_vLocalRayPos, m_vLocalRayDir, XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);

	if (true == isPicked)
		XMStoreFloat3(&pOut, m_vLocalRayPos + m_vLocalRayDir * fDist);
	
	return isPicked;
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	CPicking* pInstance = new CPicking(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX(TEXT("Failed to Created : CPicking"));
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CPicking::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
