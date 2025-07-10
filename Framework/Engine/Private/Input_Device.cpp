#include "pch.h"
#include "Input_Device.h"

Engine::CInput_Device::CInput_Device(void)
{
	ZeroMemory(m_byCulKeyState, sizeof(m_byCulKeyState));
}

HRESULT Engine::CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL)))
		return E_FAIL;
	// 키보드 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInput_Device::Update(void)
{
	for(_int i = 0; i < 256; ++i)
		m_byPreKeyState[i] = m_byCulKeyState[i];
	
	m_tPreMouseState = m_tCulMouseState;

 	m_pKeyBoard->GetDeviceState(256, m_byCulKeyState);
	m_pMouse->GetDeviceState(sizeof(m_tCulMouseState), &m_tCulMouseState);
}

_bool CInput_Device::IsKeyDown(_ubyte byKeyID)
{
	if (m_byPreKeyState[byKeyID] != m_byCulKeyState[byKeyID] && m_byCulKeyState[byKeyID] & 0x80)
		return true;

	return false;
}

_bool CInput_Device::IsKeyUp(_ubyte byKeyID)
{
	if (m_byPreKeyState[byKeyID] != m_byCulKeyState[byKeyID] && !(m_byCulKeyState[byKeyID] & 0x80))
		return true;

	return false;
}

_bool CInput_Device::IsKeyHold(_ubyte byKeyID)
{
	if(m_byCulKeyState[byKeyID] & 0x80)
		return true;

	return false;
}

_bool CInput_Device::IsMouseDown(MOUSEKEYSTATE eMouse)
{

	if (m_tPreMouseState.rgbButtons[ENUM_CLASS(eMouse)] != m_tCulMouseState.rgbButtons[ENUM_CLASS(eMouse)] 
		&& m_tCulMouseState.rgbButtons[ENUM_CLASS(eMouse)] & 0x80)
		return true;

	return false;
}

_bool CInput_Device::IsMouseUp(MOUSEKEYSTATE eMouse)
{
	if (m_tPreMouseState.rgbButtons[ENUM_CLASS(eMouse)] != m_tCulMouseState.rgbButtons[ENUM_CLASS(eMouse)]
		&& !(m_tCulMouseState.rgbButtons[ENUM_CLASS(eMouse)] & 0x80))
		return true;

	return false;
}

_bool CInput_Device::IsMouseHold(MOUSEKEYSTATE eMouse)
{
	return m_tCulMouseState.rgbButtons[ENUM_CLASS(eMouse)];
}

CInput_Device* CInput_Device::Create(HINSTANCE hInstance, HWND hWnd)
{
	CInput_Device* pInstance = new CInput_Device();

	if (FAILED(pInstance->Initialize(hInstance, hWnd)))
	{
		MSG_BOX(TEXT("Failed to Created : CInput_Device"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CInput_Device::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

