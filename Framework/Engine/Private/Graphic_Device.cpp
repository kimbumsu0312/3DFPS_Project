#include "Graphic_Device.h"

CGraphic_Device::CGraphic_Device() : m_pDevice { nullptr }, m_pDeviceContext { nullptr }
{
}

HRESULT CGraphic_Device::Initialize(HWND hWnd, WINMODE isWindowed, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	_uint iFlag = 0;

#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_FEATURE_LEVEL FeatureLV;

	// 그래픽 장치 초기화
	if (FAILED(D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, 0, iFlag, nullptr, 0, D3D11_SDK_VERSION, &m_pDevice, &FeatureLV, &m_pDeviceContext)))
		return E_FAIL;

	if (FAILED(Ready_SwapChain(hWnd, isWindowed, iWinSizeX, iWinSizeY)))
		return E_FAIL;

	if (FAILED(Ready_BackBufferRenderTargetView()))
		return E_FAIL;

	if (FAILED(Ready_DepthStencilView(iWinSizeX, iWinSizeY)))
		return E_FAIL;

	ID3D11RenderTargetView* pRTVs[] = { m_pBackBufferRTV };

	m_pDeviceContext->OMSetRenderTargets(1, pRTVs, m_pDepthStencilView);

	D3D11_VIEWPORT	ViewProtDesc;
	ZeroMemory(&ViewProtDesc, sizeof(D3D11_VIEWPORT));
	ViewProtDesc.TopLeftX = 0;
	ViewProtDesc.TopLeftY = 0;
	ViewProtDesc.Width = (_float)iWinSizeX;
	ViewProtDesc.Height = (_float)iWinSizeY;
	ViewProtDesc.MinDepth = 0.f;
	ViewProtDesc.MaxDepth = 1.f;

	m_pDeviceContext->RSSetViewports(1, &ViewProtDesc);

	*ppDevice = m_pDevice;
	*ppContext = m_pDeviceContext;

	Safe_Release(m_pDevice);
	Safe_Release(m_pDeviceContext);
	return S_OK;
}

HRESULT CGraphic_Device::Clear_BackBuffer_View(const _float4* pClearColor)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	//11
	//백 버퍼 초기화
	m_pDeviceContext->ClearRenderTargetView(m_pBackBufferRTV, reinterpret_cast<const _float*>(pClearColor));

	return S_OK;
}

HRESULT CGraphic_Device::Clear_DepthStencil_View()
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

	//깊이 버퍼, 스텐실 버퍼 초기화
	m_pDeviceContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	return S_OK;
}

HRESULT CGraphic_Device::Present()
{
	if (nullptr == m_pSwapChain)
		return E_FAIL;

	//전면 버퍼와 후면 버퍼를 교체하여 후면 버퍼를 전면으로 보여줌
	return m_pSwapChain->Present(0,0);
}

HRESULT CGraphic_Device::Ready_SwapChain(HWND hWnd, WINMODE isWindowed, _uint iWinCX, _uint iWinCY)
{
	return S_OK;
}

HRESULT CGraphic_Device::Ready_BackBufferRenderTargetView()
{
	return S_OK;
}

HRESULT CGraphic_Device::Ready_DepthStencilView(_uint iWinCX, _uint iWinCY)
{
	return S_OK;
}

CGraphic_Device* CGraphic_Device::Create(HWND hWnd, WINMODE isWindowed, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device** ppDevice, ID3D11DeviceContext** ppDeviceContextOut)
{
	CGraphic_Device* pInstance = new CGraphic_Device();

	if (FAILED(pInstance->Initialize(hWnd, isWindowed, iWinSizeX, iWinSizeY, ppDevice, ppDeviceContextOut)))
	{
		MSG_BOX(TEXT("Failed to Created : CGraphic_Device"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGraphic_Device::Free()
{
	Safe_Release(m_pSwapChain);
	Safe_Release(m_pDepthStencilView);
	Safe_Release(m_pBackBufferRTV);
	Safe_Release(m_pDeviceContext);

#if defined(DEBUG) || defined(_DEBUG)
	ID3D11Debug* d3dDebug;
	HRESULT hr = m_pDevice->QueryInterface(__uuidof(ID3D11Debug), reinterpret_cast<void**>(&d3dDebug));
	if (SUCCEEDED(hr))
	{
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker \r ");
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");

		hr = d3dDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);

		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
		OutputDebugStringW(L"                                                                    D3D11 Live Object ref Count Checker END \r ");
		OutputDebugStringW(L"----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- \r ");
	}
	if (d3dDebug != nullptr)            d3dDebug->Release();
#endif

	Safe_Release(m_pDevice);
}
