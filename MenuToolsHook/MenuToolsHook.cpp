// MenuToolsHook.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MenuTools.h"

#include "MenuCommon/Defines.h"
#include "MenuCommon/Log.h"
#define WM_GETSYSMENU						0x313

// Sent messages
LRESULT CALLBACK CallWndProc(
	_In_  int nCode,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	)
{
	switch(nCode)
	{
	case HC_ACTION:
		{
			CWPSTRUCT* sMsg = (CWPSTRUCT*) lParam;

			switch (sMsg->message)
			{
			case WM_CONTEXTMENU:
			case WM_INITMENU:
			case WM_INITMENUPOPUP:
			case WM_GETSYSMENU:
				{
					// Install custom menus
					if(MenuTools::Install(sMsg->hwnd))
					{
						// Update menu
						MenuTools::Status(sMsg->hwnd);
						return 0;
					}
				}
				break;
			case MT_HOOK_TRAY_MESSAGE:
				{
					// Process tray messages
					if(MenuTools::TrayProc(sMsg->hwnd, sMsg->wParam, sMsg->lParam))
					{
						return 0;
					}
				}
				break;
			case WM_COMMAND:
			case WM_SYSCOMMAND:
				{
					// Process windows messages
					if(MenuTools::WndProc(sMsg->hwnd, sMsg->wParam))
					{
						return 0;
					}
				}
				break;
			}
		}
		break;
	}

	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// Post messages
LRESULT CALLBACK GetMsgProc(
	_In_  int code,
	_In_  WPARAM wParam,
	_In_  LPARAM lParam
	)
{
	switch (code)
	{
	case HC_ACTION:
		{
			MSG* pMsg = (MSG*) lParam;

			switch(pMsg->message)
			{
			case WM_CONTEXTMENU:
			case WM_INITMENU:
			case WM_INITMENUPOPUP:
			case WM_GETSYSMENU:
				{
					// Install custom menus
					if(MenuTools::Install(pMsg->hwnd))
					{
						// Update menu
						MenuTools::Status(pMsg->hwnd);
						return 0;
					}
				}
				break;
			case MT_HOOK_TRAY_MESSAGE:
				{
					// Process tray messages
					if(MenuTools::TrayProc(pMsg->hwnd, pMsg->wParam, pMsg->lParam))
					{
						return 0;
					}
				}
				break;
			case WM_COMMAND:
			case WM_SYSCOMMAND:
				{
					if(wParam == PM_REMOVE)
					{
						// Process windows messages
						if(MenuTools::WndProc(pMsg->hwnd, pMsg->wParam))
						{
							return 0;
						}
					}
				}
				break;
			}
		}
		break;
	}

	return CallNextHookEx(NULL, code, wParam, lParam);
}