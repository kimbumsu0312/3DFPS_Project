#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

NS_BEGIN(Engine)

class  CInput_Device : public CBase
{
private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;
	
public:
	_byte	Get_DIKeyState(_ubyte byKeyID)	{ 
		return m_byCulKeyState[byKeyID]; }

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)	{ 	
		return m_tCulMouseState.rgbButtons[static_cast<_uint>(eMouse)]; }

	// 현재 마우스의 특정 축 좌표를 반환
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)	{	
		return *((reinterpret_cast<_int*>(&m_tCulMouseState)) + static_cast<_uint>(eMouseState));	}
	_bool IsKeyDown(_ubyte byKeyID);
	_bool IsKeyUp(_ubyte byKeyID);
	_bool IsKeyHold(_ubyte byKeyID);
	_bool IsMouseDown(MOUSEKEYSTATE eMouse);
	_bool IsMouseUp(MOUSEKEYSTATE eMouse);
	_bool IsMouseHold(MOUSEKEYSTATE eMouse);

public:
	HRESULT					Initialize(HINSTANCE hInst, HWND hWnd);
	void					Update(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = { nullptr };

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = { nullptr };
	LPDIRECTINPUTDEVICE8	m_pMouse = { nullptr };
	
private:
	_byte					m_byCulKeyState[256] = {};	
	_byte					m_byPreKeyState[256] = {};

	DIMOUSESTATE			m_tCulMouseState = {};
	DIMOUSESTATE			m_tPreMouseState = {};

public:
	static CInput_Device*	Create(HINSTANCE hInstance, HWND hWnd);
	virtual void			Free(void);

};

NS_END
#endif // InputDev_h__


