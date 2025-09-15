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

	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = m_iWinSizeX;
	TextureDesc.Height = m_iWinSizeY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_STAGING;

	TextureDesc.BindFlags = 0;
	TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D10_CPU_ACCESS_READ;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
		return E_FAIL;

	m_pPixels = new _float4[m_iWinSizeX * m_iWinSizeY];

#ifdef _DEBUG
	m_pBatch = new PrimitiveBatch<VertexPositionColor>(m_pContext);
	m_pEffect = new BasicEffect(m_pDevice);

	m_pEffect->SetVertexColorEnabled(true);

	const void* pShaderByteCode = { nullptr };
	size_t iShaderByteCodeLength = {};

	m_pEffect->GetVertexShaderBytecode(&pShaderByteCode, &iShaderByteCodeLength);

	if (FAILED(m_pDevice->CreateInputLayout(VertexPositionColor::InputElements, VertexPositionColor::InputElementCount,
		pShaderByteCode, iShaderByteCodeLength, &m_pInputLayout)))
		return E_FAIL;
#endif

	return S_OK;
}

void CPicking::Update()
{
	GetCursorPos(&m_ptMouse);
	ScreenToClient(m_hWnd, &m_ptMouse);

	_float3	vMousePos{};

	vMousePos.x = m_ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
	vMousePos.y = 1.f - m_ptMouse.y / (m_iWinSizeY * 0.5f);
	vMousePos.z = 0.f;
	_vector vMouseNDC = XMLoadFloat3(&vMousePos);

	vMouseNDC = XMVector3TransformCoord(vMouseNDC, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::PROJ));

	_vector vWolrdPos = XMVectorSet(0.f, 0.f, 0.f, 1.f);
	
	vMouseNDC = XMVector3TransformNormal(vMouseNDC, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::VIEW));
	m_vWorldRayPos = XMVector3TransformCoord(vWolrdPos, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::VIEW));
	m_vWorldRayDir = XMVector3Normalize(vMouseNDC);

	if (FAILED(m_pGameInstance->Copy_Resource(TEXT("Target_Depth"), m_pTexture2D)))
		return;

	D3D11_MAPPED_SUBRESOURCE SubResource{};

	if (FAILED(m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &SubResource)))
		return;

	memcpy(m_pPixels, SubResource.pData, sizeof(_float4) * m_iWinSizeX * m_iWinSizeY);

	m_pContext->Unmap(m_pTexture2D, 0);

}

RAY_DESC CPicking::Create_FpsRayDesc(_float fOffSet)
{
	RAY_DESC Desc;

	POINT	ptMouse{};
	
	ptMouse.x = (m_iWinSizeX >> 1);
	ptMouse.y = (m_iWinSizeY >> 1);
	
	_float RayX = m_pGameInstance->Rand(ptMouse.x - (fOffSet * 0.5f), ptMouse.x + (fOffSet * 0.5f));
	_float RayY = m_pGameInstance->Rand(ptMouse.y - (fOffSet * 0.5f), ptMouse.y + (fOffSet * 0.5f));

	_float3	vMousePos{};
	vMousePos.x = ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
	vMousePos.y = 1.f - ptMouse.y / (m_iWinSizeY * 0.5f);
	vMousePos.z = 0.f;

	_float3	vRayPos{};
	vRayPos.x = RayX / (m_iWinSizeX * 0.5f) - 1.f;
	vRayPos.y =  1.f - RayY / (m_iWinSizeY * 0.5f);
	vRayPos.z = 1.f;

	_vector vRayNDC = XMLoadFloat3(&vRayPos) - XMLoadFloat3(&vMousePos);
	vRayNDC = XMVectorSetW(vRayNDC, 0.f);

	vRayNDC = XMVector3TransformCoord(vRayNDC, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::PROJ));

	_vector vWolrdPos = XMVectorSet(0.f, 0.f, 1.f, 1.f);

	vRayNDC = XMVector3TransformNormal(vRayNDC, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::VIEW));
	Desc.RayPos = XMVector3TransformCoord(vWolrdPos, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::VIEW));
	Desc.RayDIr = XMVector3Normalize(vRayNDC);

	Desc.Time = 2.f;
#ifdef _DEBUG
	m_RayDescs.push_back(Desc);
#endif // DEBUG
	return Desc;
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

_bool CPicking::isPickedInLocalSpace(_float3 vPointA, _float3 vPointB, _float3 vPointC, _float& pDist)
{
	_float fDist{};
	_bool isPicked = false;
	isPicked = DirectX::TriangleTests::Intersects(m_vLocalRayPos, m_vLocalRayDir, XMLoadFloat3(&vPointA), XMLoadFloat3(&vPointB), XMLoadFloat3(&vPointC), fDist);

	if (true == isPicked)
		pDist = fDist;

	return isPicked;
}

_bool CPicking::isPicking(_float3* pOut)
{
	_uint iIndex = m_ptMouse.y * m_iWinSizeX + m_ptMouse.x;

	if (0.f == m_pPixels[iIndex].w)
		return false;

	_vector vPosition = {};

	vPosition = XMVectorSetX(vPosition, m_ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f);
	vPosition = XMVectorSetY(vPosition, m_ptMouse.y / (m_iWinSizeX * -0.5f) + 1.f);
	vPosition = XMVectorSetZ(vPosition, m_pPixels[iIndex].x);
	vPosition = XMVectorSetW(vPosition, 1.f);

	vPosition = XMVector3TransformCoord(vPosition, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::PROJ));
	vPosition = XMVector3TransformCoord(vPosition, m_pGameInstance->Get_Transform_Matrix_Inverse(D3DTS::VIEW));

	XMStoreFloat3(pOut, vPosition);

	return true;
}

#ifdef _DEBUG
HRESULT CPicking::Ray_Render()
{
	m_pEffect->SetWorld(XMMatrixIdentity());
	m_pEffect->SetView(m_pGameInstance->Get_Transform_Matrix(D3DTS::VIEW));
	m_pEffect->SetProjection(m_pGameInstance->Get_Transform_Matrix(D3DTS::PROJ));

	m_pContext->IASetInputLayout(m_pInputLayout);
	m_pEffect->Apply(m_pContext);

	m_pBatch->Begin();
	
	if (m_RayDescs.size() > 0)
	{
		for (auto iter = m_RayDescs.begin(); iter != m_RayDescs.end(); )
		{
			DX::DrawRay(m_pBatch, (*iter).RayPos, (*iter).RayDIr * 1000.f, false);
			(*iter).Time -= 0.016f;

			if ((*iter).Time <= 0.f)
			{
				iter = m_RayDescs.erase(iter);
			}
			else
			{
				++iter;
			}
		}
	}
	m_pBatch->End();

	return S_OK;
}
#endif // _DEBUG

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
	Safe_Delete_Array(m_pPixels);

	Safe_Release(m_pTexture2D);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

#ifdef _DEBUG
	Safe_Release(m_pInputLayout);
	Safe_Delete(m_pBatch);
	Safe_Delete(m_pEffect);
	m_RayDescs.clear();
#endif

}
