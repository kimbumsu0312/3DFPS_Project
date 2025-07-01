#include "pch.h"
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

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pDeviceContext);
	return S_OK;
}

HRESULT CGraphic_Device::Clear_BackBuffer_View(const _float4* pClearColor)
{
	if (nullptr == m_pDeviceContext)
		return E_FAIL;

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
	IDXGIDevice* pDevice = nullptr;
	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), ((void**)&pDevice));

	IDXGIAdapter* pAdapter = nullptr;
	pDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&pAdapter);

	IDXGIFactory* pFactory = nullptr;
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);

	//스왑 체인 생성
	DXGI_SWAP_CHAIN_DESC	Swapchain;
	ZeroMemory(&Swapchain, sizeof(DXGI_SWAP_CHAIN_DESC));

	//텍스처 버퍼 생성
	Swapchain.BufferDesc.Width = iWinCX;
	Swapchain.BufferDesc.Height = iWinCY;

	Swapchain.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	Swapchain.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	Swapchain.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	Swapchain.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	Swapchain.BufferCount = 1;

	//스왑 형태
	Swapchain.BufferDesc.RefreshRate.Numerator = 60;
	Swapchain.BufferDesc.RefreshRate.Numerator = 1;

	//멀티 샘플링, 후처리 렌더링
	Swapchain.SampleDesc.Quality = 0;
	Swapchain.SampleDesc.Count = 1;

	Swapchain.OutputWindow = hWnd;
	Swapchain.Windowed = static_cast<BOOL>(isWindowed);
	Swapchain.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	if (FAILED(pFactory->CreateSwapChain(m_pDevice, &Swapchain, &m_pSwapChain)))
		return E_FAIL;
	
	Safe_Release(pFactory);
	Safe_Release(pAdapter);
	Safe_Release(pDevice);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_BackBufferRenderTargetView()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pBackBufferTextture = nullptr;

	if (FAILED(m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBufferTextture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateRenderTargetView(pBackBufferTextture, nullptr, &m_pBackBufferRTV)))
		return E_FAIL;
	
	Safe_Release(pBackBufferTextture);

	return S_OK;
}

HRESULT CGraphic_Device::Ready_DepthStencilView(_uint iWinCX, _uint iWinCY)
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC TextureDesc;
	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = iWinCX;
	TextureDesc.Height = iWinCY;
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;

	TextureDesc.Usage = D3D11_USAGE_DEFAULT;	//동적, 정적 설정
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;	//어떤 용도로 바인드 될지 플래그
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
		return E_FAIL;

	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDepthStencilView)))
		return E_FAIL;

	Safe_Release(pDepthStencilTexture);

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
	if (d3dDebug != nullptr)        
		d3dDebug->Release();
#endif

	Safe_Release(m_pDevice);
}
