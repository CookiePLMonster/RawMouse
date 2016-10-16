#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define WINVER 0x0502
#define _WIN32_WINNT 0x0502

#include <windows.h>
#include <cassert>
#include "MemoryMgr.h"
#include "MenuManager.h"

struct PsGlobalType
{
	HWND	window;
	DWORD	instance;
	DWORD	fullscreen;
	DWORD	lastMousePos_X;
	DWORD	lastMousePos_Y;
	DWORD	unk;
	DWORD	diInterface;
	DWORD	diMouse;
	void*	diDevice1;
	void*	diDevice2;
};

struct RsGlobalType
{
	const char*		AppName;
#ifndef _GTA_SA
	unsigned int	unkWidth, unkHeight;
#endif
	signed int		MaximumWidth;
	signed int		MaximumHeight;
	unsigned int	frameLimit;
	BOOL			quit;
	PsGlobalType*	ps;
	void*			keyboard;
	void*			mouse;
	void*			pad;
};

class CMouseControllerState
{
public:
	bool	lmb;
	bool	rmb;
	bool	mmb;
	bool	wheelUp;
	bool	wheelDown;
	bool	bmx1;
	bool	bmx2;
	float	Z;
	float	X;
	float	Y;

public:
	inline bool		CheckForInput() const
		{ return lmb || rmb || mmb || wheelUp || wheelDown || bmx1 || bmx2 || X != 0.0f || Y != 0.0f; }
};

class CMousePointerStateHelper
{
public:
	bool				m_bVerticalInvert;
	bool				m_bHorizontalInvert;

public:
	CMouseControllerState*	GetMouseSetUp( CMouseControllerState* mouseCaps );
};

class CControllerState
{
public:
	short LEFTSTICKX;
	short LEFTSTICKY;
	short RIGHTSTICKX;
	short RIGHTSTICKY;

	short LEFTSHOULDER1;
	short LEFTSHOULDER2;
	short RIGHTSHOULDER1;
	short RIGHTSHOULDER2;

	short DPADUP;
	short DPADDOWN;
	short DPADLEFT;
	short DPADRIGHT;

	short START;
	short SELECT;

	short SQUARE;
	short TRIANGLE;
	short CROSS;
	short CIRCLE;

	short LEFTSHOCK;
	short RIGHTSHOCK;

	short HOME;

	short m_bPedWalk;
	short m_bVehicleMouseLook;
	short m_bRadioTrackSkip;
};

class CPad
{
public:
	CControllerState	NewState;
	CControllerState	OldState;
	WORD				SteeringLeftRightBuffer[10];
	DWORD				DrunkDrivingBufferUsed;
	CControllerState	PCTempKeyState;
	CControllerState	PCTempJoyState;
	CControllerState	PCTempMouseState;
	BYTE				Phase;
	WORD				Mode;
	short				ShakeDur;
	WORD				DisablePlayerControls;
	BYTE				ShakeFreq;
	BYTE				bHornHistory[5];
	BYTE				iCurrHornHistory;
	bool				JustOutOfFrontEnd;
	bool				bApplyBrakes;
	bool				bDisablePlayerEnterCar;
	bool				bDisablePlayerDuck;
	bool				bDisablePlayerFireWeapon;
	bool				bDisablePlayerFireWeaponWithL1;
	bool				bDisablePlayerCycleWeapon;
	bool				bDisablePlayerJump;
	bool				bDisablePlayerDisplayVitalStats;
	int					LastTimeTouched;
	DWORD				AverageWeapon;
	DWORD				AverageEntries;
	DWORD				NoShakeBeforeThis;
	BYTE				NoShakeFreq;

private:
	static CMouseControllerState&	PCTempMouseControllerState;
	static CMouseControllerState&	NewMouseControllerState;
	static CMouseControllerState&	OldMouseControllerState;

public:
	inline bool								CrossJustDown() { return NewState.CROSS && !OldState.CROSS; }
	inline bool								RightShockJustDown() { return NewState.RIGHTSHOCK && !OldState.RIGHTSHOCK; }
	static inline CMouseControllerState&	GetMouseStateBuffer() { return PCTempMouseControllerState; }

	CControllerState	ReconcileTwoControllersInput(const CControllerState& rDevice1, const CControllerState& rDevice2);
	void				UpdateMouse();

	static CPad*		GetPad(int nPad);
	static void			UpdatePads();

	// Temp
	static void			Inject();
};


CMouseControllerState&	CPad::PCTempMouseControllerState = *(CMouseControllerState*)0xB73404;
CMouseControllerState&	CPad::NewMouseControllerState = *(CMouseControllerState*)0xB73418;
CMouseControllerState&	CPad::OldMouseControllerState = *(CMouseControllerState*)0xB7342C;

CMousePointerStateHelper&	MousePointerStateHelper = *(CMousePointerStateHelper*)0xBA6744;

CMenuManager&					FrontEndMenuManager = **AddressByVersion<CMenuManager**>(0x4054DC, 0, 0);

RsGlobalType&	RsGlobal = **AddressByVersion<RsGlobalType**>(0x619604, 0, 0);

int&	snTimeInMilliseconds =  **AddressByVersion<int**>(0x53F51C, 0, 0);

inline bool IsForeground()
{
	return *(BOOL*)0x8D621C != FALSE;
}

CMouseControllerState* CMousePointerStateHelper::GetMouseSetUp( CMouseControllerState* mouseCaps )
{
	mouseCaps->lmb = false;
	mouseCaps->rmb = false;
	mouseCaps->mmb = false;
	mouseCaps->wheelUp = false;
	mouseCaps->wheelDown = false;
	mouseCaps->X = mouseCaps->Y = mouseCaps->Z = 0.0f;
	mouseCaps->bmx1 = false;
	mouseCaps->bmx2 = false;

	UINT numRIDevices = 0;
	GetRawInputDeviceList( nullptr, &numRIDevices, sizeof(RAWINPUTDEVICELIST) );
	if ( numRIDevices > 0 )
	{
		PRAWINPUTDEVICELIST pDevices = new RAWINPUTDEVICELIST[ numRIDevices ];
		GetRawInputDeviceList( pDevices, &numRIDevices, sizeof(RAWINPUTDEVICELIST) );
		for( size_t i = 0; i < numRIDevices; ++i )
		{
			if ( pDevices[i].dwType == RIM_TYPEMOUSE )
			{	
				RID_DEVICE_INFO deviceInfo;
				UINT cbSize = sizeof(deviceInfo);
				GetRawInputDeviceInfo( pDevices[i].hDevice, RIDI_DEVICEINFO, &deviceInfo, &cbSize );

				if ( !mouseCaps->lmb ) mouseCaps->lmb = deviceInfo.mouse.dwNumberOfButtons >= 1;
				if ( !mouseCaps->rmb ) mouseCaps->rmb = deviceInfo.mouse.dwNumberOfButtons >= 2;
				// Assume MMB is scroll - seems like there's no obvious way to check for the presence of scroll wheel so this has to suffice
				if ( !mouseCaps->mmb ) mouseCaps->mmb = mouseCaps->wheelDown = mouseCaps->wheelUp = deviceInfo.mouse.dwNumberOfButtons >= 3;
				if ( !mouseCaps->bmx1 ) mouseCaps->bmx1 = deviceInfo.mouse.dwNumberOfButtons >= 4;
				if ( !mouseCaps->bmx2 ) mouseCaps->bmx2 = deviceInfo.mouse.dwNumberOfButtons >= 5;
			}
		}
		delete[] pDevices;
	}

	return mouseCaps;
}

void CPad::UpdateMouse()
{
	if ( IsForeground() )
	{
		OldMouseControllerState = NewMouseControllerState;
		NewMouseControllerState = PCTempMouseControllerState;

		// As TempMouseControllerState contains only raw data now, handle movement inversion here
		if ( !FrontEndMenuManager.m_bMenuActive )
		{
			if ( MousePointerStateHelper.m_bVerticalInvert )
				NewMouseControllerState.X = -NewMouseControllerState.X;
			if ( MousePointerStateHelper.m_bHorizontalInvert )
				NewMouseControllerState.Y = -NewMouseControllerState.Y;
		}
		
		// Clear mouse movement data and scroll data in temp buffer
		PCTempMouseControllerState.X = PCTempMouseControllerState.Y = PCTempMouseControllerState.Z = 0.0f;
		PCTempMouseControllerState.wheelDown = PCTempMouseControllerState.wheelUp = false;
		
		if ( NewMouseControllerState.CheckForInput() )
			LastTimeTouched = snTimeInMilliseconds;
	}
}

// RawInput mouse handler
void RegisterRawInputMouse()
{
    RAWINPUTDEVICE Rid[1];
    Rid[0].usUsagePage = 1; 
    Rid[0].usUsage = 2; 
    Rid[0].dwFlags = RIDEV_INPUTSINK;   
	Rid[0].hwndTarget = RsGlobal.ps->window;

    RegisterRawInputDevices(Rid, 1, sizeof(Rid[0]));
}

void RegisterMouseMovement(RAWINPUT* raw)
{
	// Interpret data
	CMouseControllerState&	StateBuf = CPad::GetMouseStateBuffer();
	
	// Movement
	StateBuf.X += static_cast<float>(raw->data.mouse.lLastX);
	StateBuf.Y += static_cast<float>(raw->data.mouse.lLastY);

	// LMB
	if ( !StateBuf.lmb )
		StateBuf.lmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_DOWN) != false;
	else
		StateBuf.lmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_LEFT_BUTTON_UP) == false;

	// RMB
	if ( !StateBuf.rmb )
		StateBuf.rmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_DOWN) != false;
	else
		StateBuf.rmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_RIGHT_BUTTON_UP) == false;

	// MMB
	if ( !StateBuf.mmb )
		StateBuf.mmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_DOWN) != false;
	else
		StateBuf.mmb = (raw->data.mouse.usButtonFlags & RI_MOUSE_MIDDLE_BUTTON_UP) == false;

	// 4th button
	if ( !StateBuf.bmx1 )
		StateBuf.bmx1 = (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_DOWN) != false;
	else
		StateBuf.bmx1 = (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_4_UP) == false;

	// 5th button
	if ( !StateBuf.bmx2 )
		StateBuf.bmx2 = (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_DOWN) != false;
	else
		StateBuf.bmx2 = (raw->data.mouse.usButtonFlags & RI_MOUSE_BUTTON_5_UP) == false;

	// Scroll
	if ( raw->data.mouse.usButtonFlags & RI_MOUSE_WHEEL )
	{
		StateBuf.Z += static_cast<signed short>(raw->data.mouse.usButtonData);
		if ( StateBuf.Z < 0.0f )
			StateBuf.wheelDown = true;
		else if ( StateBuf.Z > 0.0f )
			StateBuf.wheelUp = true;
	}
}

static LRESULT (CALLBACK **OldWndProc)(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK CustomWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch ( uMsg )
	{
	case WM_INPUT:
		{
			UINT dwSize = 0;
			static UINT dwSavedSize = 0;
			static BYTE* pRawBuf = nullptr;

			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,  nullptr, &dwSize, sizeof(RAWINPUTHEADER));
			if ( dwSize > dwSavedSize )
			{
				delete[] pRawBuf;
				pRawBuf = new BYTE[dwSize];
				dwSavedSize = dwSize;
			}
    
			RAWINPUT* raw = reinterpret_cast<RAWINPUT*>(pRawBuf);
			GetRawInputData(reinterpret_cast<HRAWINPUT>(lParam), RID_INPUT,  raw, &dwSize, sizeof(RAWINPUTHEADER));
			if ( raw->header.dwType == RIM_TYPEMOUSE )
			{
				RegisterMouseMovement(raw);
			}
			break;
		}
	}

	return (*OldWndProc)(hwnd, uMsg, wParam, lParam);
}
static auto* pCustomWndProc = CustomWndProc;

void Patch_SA_10()
{
	using namespace Memory;

	InjectHook(0x53F3C0, &CPad::UpdateMouse, PATCH_JUMP);

	InjectHook(0x74880B, RegisterRawInputMouse);
	Patch<BYTE>(0x7469A0, 0xC3);
	
	// Mouse caps
	InjectHook( 0x53F2D0, &CMousePointerStateHelper::GetMouseSetUp, PATCH_JUMP );

	OldWndProc = *(LRESULT (CALLBACK***)(HWND, UINT, WPARAM, LPARAM))AddressByRegion_10<DWORD>(0x748454);
	Patch(AddressByRegion_10<DWORD>(0x748454), &pCustomWndProc);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	UNREFERENCED_PARAMETER(lpvReserved);

	if ( fdwReason == DLL_PROCESS_ATTACH )
	{
		ScopedUnprotect::Section Protect( GetModuleHandle( nullptr ), ".text" );

		if (*(DWORD*)DynBaseAddress(0x82457C) == 0x94BF || *(DWORD*)DynBaseAddress(0x8245BC) == 0x94BF) Patch_SA_10();
		//else if (*(DWORD*)DynBaseAddress(0x8252FC) == 0x94BF || *(DWORD*)DynBaseAddress(0x82533C) == 0x94BF) Patch_SA_11();
		//else if (*(DWORD*)DynBaseAddress(0x85EC4A) == 0x94BF) Patch_SA_Steam();

		//else if ( *(DWORD*)DynBaseAddress(0x858D21) == 0x3539F633) Patch_SA_NewSteam_r1();
		//else if ( *(DWORD*)DynBaseAddress(0x858D51) == 0x3539F633) Patch_SA_NewSteam_r2();
		//else if ( *(DWORD*)DynBaseAddress(0x858C61) == 0x3539F633) Patch_SA_NewSteam_r2_lv();
		
		else return FALSE;
	}
	return TRUE;
}