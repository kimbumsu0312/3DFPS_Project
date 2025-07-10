#include "pch.h"
#include "Input_Device.h"

Engine::CInput_Device::CInput_Device(void)
{
	ZeroMemory(m_byCulKeyState, sizeof(m_byCulKeyState));
}

HRESULT Engine::CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{

	// DInput �İ�ü�� �����ϴ� �Լ�
	if (FAILED(DirectInput8Create(hInst,
		DIRECTINPUT_VERSION,
		IID_IDirectInput8,
		(void**)&m_pInputSDK,
		NULL)))
		return E_FAIL;
	// Ű���� ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr)))
		return E_FAIL;

	// ������ Ű���� ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// ��ġ�� ���� �������� �������ִ� �Լ�, (Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
	m_pKeyBoard->Acquire();


	// ���콺 ��ü ����
	if (FAILED(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr)))
		return E_FAIL;

	// ������ ���콺 ��ü�� ���� ������ �� ��ü���� �����ϴ� �Լ�
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// ��ġ�� ���� �������� �������ִ� �Լ�, Ŭ���̾�Ʈ�� ���ִ� ���¿��� Ű �Է��� ������ ������ �����ϴ� �Լ�
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// ��ġ�� ���� access ������ �޾ƿ��� �Լ�
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

