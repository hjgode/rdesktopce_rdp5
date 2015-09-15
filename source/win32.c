/* -*- c-basic-offset: 8 -*-
   rdesktop: A Remote Desktop Protocol client.
   win32 calls
   Copyright (C) Jay Sorg 2006

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifdef UNDER_CE
#define MYWINCE
#endif

//use barcode scanner code? NOW handled via g_busescanner!
///#define USE_SCANNER
//#undef USE_SCANNER

int MENU_HEIGHT=26; //was a #define

#include <winsock2.h> /* winsock2.h first */
#pragma comment (lib, "ws2.lib")
extern int WSAAsyncSelect(
		IN SOCKET s,
		IN HWND hWnd,
		IN u_int wMsg,
		IN long lEvent
		);

//show a menubar?
#define USE_MENUBAR

#include <windows.h>
	#ifdef MYWINCE
	#include <aygshell.h> /* aygshell.lib */
	#pragma comment (lib, "aygshell.lib")
#endif /* MYWINCE */

#include <winuser.h>
#include <stdio.h>
#include "resource.h" //HGO
#include "uimain.h"
#include "rdesktop.h"

#include "winscancodes.h"

	#include "barcodereader.h"

#include "keymap.h"

//MapVirtualKey
#ifndef MAPVK_VK_TO_VSC
	#define MAPVK_VK_TO_VSC      0
	#define	MAPVK_VSC_TO_VK      1
	#define MAPVK_VK_TO_CHAR     2
	#define MAPVK_VSC_TO_VK_EX   3
	#define MAPVK_VK_TO_VSC_EX   4
#endif

BOOL g_rdpclip = FALSE;

//wait cursor on startup
HCURSOR hCurs1;

extern BOOL g_bnosip;
extern BOOL g_busescanner;
extern char *g_username;
extern char g_hostname[16];
extern char g_servername[];
extern char g_password[];
extern char g_shell[];
extern char g_directory[];
extern char g_domain[];
extern int g_width;
extern int g_height;
extern int g_tcp_sck;
extern int g_server_depth;
extern int g_tcp_port_rdp; /* in tcp.c */
extern int pal_entries[];

//	#define KEYEVENTF_KEYUP 0x2
	#define KEYEVENTF_KEYDOWN 0x0

//removed static to be able to access from barcodereader.h
HWND g_Wnd = 0;
static HWND g_hMenuBar = 0 ; //HGO
static HINSTANCE g_Instance = 0;
static HCURSOR g_cursor = 0;
static int g_block = 0;
static int g_xoff = 0; /* offset from window to client coords */
static int g_yoff = 0;
static int g_xscroll = 0; /* current scroll position */
static int g_yscroll = 0;
static int g_screen_width = 0;
static int g_screen_height = 0;
static int g_wnd_cwidth = 0; /* set from WM_SIZE */
static int g_wnd_cheight = 0;
static int g_fullscreen = 0;
static int g_workarea = 0;
static int g_mousex = 0; /* in client coords */
static int g_mousey = 0;
static int g_width_height_set = 0;

static int g_clip_left = 0;
static int g_clip_top = 0;
static int g_clip_right = 800;
static int g_clip_bottom = 600;
static RECT g_wnd_clip; /* this client area of whats actually visable */
                        /* set from WM_SIZE */
#ifdef MYWINCE
static int g_sip_up = 0;
#endif


//
// SendKeys (szTxt)
//
// sends the keystrokes for szTxt
//
// invoked by ReadCommThread
// added const as faster access
 void SendKeys(const char * szTxt)
{
	/* VK_0 thru VK_9 are the same as ASCII '0' thru '9' (0x30 - 0x39) */
	/* VK_A thru VK_Z are the same as ASCII 'A' thru 'Z' (0x41 - 0x5A) */
	/* small ASCII letters are 'a' thru 'z', 0x61 - 0x7A */
	byte bCode;
	byte vCode;
	BOOL bShift;
	uint i;
#ifdef DEBUG
	TCHAR szwTxt[1000];
	mbstowcs(szwTxt, szTxt, 1000);
	DEBUGMSG(DBK_KEY, (L"\r\nSendKeys: Sending Key...\r\n"));
#endif
	for (i=0; i < strlen(szTxt); i++)
	{
		//chars ' ' thru z
		//WARNING only 0-9, a-z is equal to VK_0-VK_9 and VK_A-VK_Z
		//the REST must be translated! ie 
		//ASCII ';' 0x3B to VK_SEMICOLON 0xBA !!!!!!!!!
		//ASCII '$' to VK_SHIFT + VK_4 !!!!!!!!
		//lookup for the VK code to send
		bCode=(char)szTxt[i];
		vCode=vkTable[bCode].kVKval;
		bShift=vkTable[bCode].kShift;
#ifdef DEBUG
		DEBUGMSG(1, (L"%s\r\n", szwTxt));
#endif
		if ((char)szTxt[i] < KTAB_SIZE)  // ( ((char)szTxt[i] >= 0x30) && ((char)szTxt[i] <= 0x7F) )
		{
#ifdef DEBUG
			DEBUGMSG(DBK_KEY, (L"Char=(0x%2x)\tbCode=%2x\tvCode=%2x\tShift=%2x\r\n", (char)szTxt[i], bCode, vCode, bShift));
#endif
			if (bShift) //has to be shifted?
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYDOWN | KEYEVENTF_SILENT, 0); 

			//send the code with key down and then key up
			keybd_event(vCode, 0, KEYEVENTF_KEYDOWN | KEYEVENTF_SILENT, 0); 
			keybd_event(vCode, 0, KEYEVENTF_KEYUP | KEYEVENTF_SILENT, 0); 

			if (bShift) //has to be unshifted?
				keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP | KEYEVENTF_SILENT, 0); 
			//Sleep(10);
		}
	}
}

/*****************************************************************************/
TCHAR* getAppDir(){
	//get the app dir
	static TCHAR* strPath;
	TCHAR* p;
	strPath = (TCHAR*) malloc (MAX_PATH * sizeof(TCHAR));
	GetModuleFileName (NULL, strPath, MAX_PATH);
	//find the last backslash
	if ( p = wcsrchr ( strPath, '\\')) 
		*++p = _T('\0'); // zero-terminate at the last backslash
	return strPath;
}

/*****************************************************************************/
int ExistFile(TCHAR* filename)
{
	HANDLE hFile;

	hFile = CreateFile (filename,   // Open MYFILE.TXT
					  GENERIC_READ,           // Open for reading
					  FILE_SHARE_READ,        // Share for reading
					  NULL,                   // No security
					  OPEN_EXISTING,          // Existing file only
					  FILE_ATTRIBUTE_NORMAL,  // Normal file
					  NULL);                  // No template file

	if (hFile == INVALID_HANDLE_VALUE)
	{
		// Your error-handling code goes here.
		return FALSE;
	}
	else
	{
		CloseHandle(hFile);
		return TRUE;
	}
}

/*****************************************************************************/
// PFCSipButtonShow
// simple code to show/hide the SIP button
void SipButtonShow( BOOL bShow )
{
	HWND hWnd = NULL;
	if(g_bnosip)
		bShow=FALSE;
	hWnd = FindWindow( _T( "MS_SIPBUTTON" ), NULL );
	if (hWnd == NULL)
		return;

	if (bShow)
		ShowWindow( hWnd, SW_SHOW );
	else
		ShowWindow( hWnd, SW_HIDE );

} // PFCSipButtonShow

/*****************************************************************************/
// menubar is only created if not fullscreen
void doCreateMenu(HWND _hwndMain, HINSTANCE _hInstMain){
	SHMENUBARINFO mb;
  if(g_hMenuBar==0)
  {
	memset(&mb,0,sizeof(SHMENUBARINFO));// ZeroMemory(&mb, sizeof(SHMENUBARINFO));
	mb.cbSize = sizeof(SHMENUBARINFO);
	mb.hwndParent = _hwndMain;
	mb.dwFlags = SHCMBF_HMENU; //SHCMBF_EMPTYBAR; //SHCMBF_HMENU; //SHCMBF_EMPTYBAR;
	/*
	The only difference is we specify the popup menu identifier for nToolBarId member instead 
	of the menu bar identifier and we also set the SHCMBF_HMENU flag. When SHCMBF_HMENU flag 
	is not specified then the nToolBarId value is treated as a toolbar identifer which creates 
	our usual softkey bar with left and right softkeys. If the flag is set then nToolBarId is 
	treated as a menu identifier. Just a regular menu bar with menus and submenus. 
	[http://geekswithblogs.net/TechTwaddle/archive/2009/07/09/fun-with-menus.aspx]
	*/
	mb.nToolBarId = IDR_MENU1;	// HGO 0;                     // ID of toolbar resource
	//mb.nToolBarId = IM_KEYBOARD_LAYOUT; //will show blank menu with system 'New' item and keyboard
	mb.hInstRes = _hInstMain;			   // Inst handle of app
	mb.nBmpId = 0;                         // ID of bitmap resource
	mb.cBmpImages = 0;                     // Num of images in bitmap 
	mb.hwndMB = 0;                         // Handle of bar returned

	DEBUGMSG(1, (L"Main: 0x%08x, Inst: 0x%08x\n", _hwndMain, _hInstMain));
	if (!SHCreateMenuBar(&mb))
		DEBUGMSG(DBG_W32, (L"Could not create menubar! ERROR=%i\n", GetLastError()));	//120L = not supported
	else
		DEBUGMSG(DBG_W32, (L"Menubar created! mb.hwndMB=0x%08x\n", mb.hwndMB));	//120L = not supported
	
	SipButtonShow(TRUE);

	//SHFullScreen (hwndMain, SHFS_SHOWSIPBUTTON);
	g_hMenuBar = mb.hwndMB; //HGO
  }
}

static LRESULT handle_WM_COMMAND(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam){
	switch (wParam){
		case ID_FILE_EXIT:
			//SetCursor(LoadCursor(NULL, IDC_WAIT));
			//ShowCursor(TRUE);
			//UpdateWindow(hWnd);
			//MessageBox(hWnd, L"Currently disabled", L"Exit rdesktop?", MB_SETFOREGROUND|MB_TOPMOST|MB_ICONSTOP);
			//return 0;
			PostQuitMessage(-33);
			return 0;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
	}
	DEBUGMSG(DBG_W32, (L"Hello\n"));
  /* need default behavoir here */
  return 0; //DefWindowProc(hWnd, message, wParam, lParam);
}

void do_hide_taskbar(BOOL bHide){
	HWND hTaskBar=0;
	RECT rc;
	int h, w;
	hTaskBar=FindWindow(L"HHTaskBar", NULL);
	if(hTaskBar!=0){
		GetWindowRect(hTaskBar, &rc);
		h = rc.bottom - rc.top;
		w = rc.right - rc.left;
		if(bHide){
			//MoveWindow(hTaskBar, 0, -h, 240, h, TRUE); 
			//hide the taskbar
			//ShowWindow(hTaskBar, SW_HIDE);
			//UpdateWindow(hTaskBar);
			//SetWindowPos(hTaskBar, HWND_BOTTOM, 0,0, 0,0, SWP_HIDEWINDOW | SWP_NOREPOSITION | SWP_NOSIZE);
			SHFullScreen(g_Wnd, SHFS_HIDETASKBAR | SHFS_HIDESTARTICON);
			//resize the app window
			SetWindowPos(g_Wnd, HWND_TOPMOST, 0,0, g_screen_width, g_screen_height-MENU_HEIGHT, SWP_SHOWWINDOW);
			AllKeys(TRUE);
		}
		else{
			//MoveWindow(hTaskBar, 0, 0, 240, h, TRUE); 
			//show the taskbar
			//ShowWindow(hTaskBar, SW_SHOW);
			//UpdateWindow(hTaskBar);
			//SetWindowPos(hTaskBar, HWND_TOPMOST, 0,0, 240, 26, SWP_SHOWWINDOW);
			SHFullScreen(g_Wnd, SHFS_SHOWTASKBAR | SHFS_SHOWSTARTICON);
			//resize the app window
			SetWindowPos(g_Wnd, HWND_TOPMOST, 0,MENU_HEIGHT, g_screen_width,g_screen_height-MENU_HEIGHT-MENU_HEIGHT, SWP_SHOWWINDOW);
			AllKeys(FALSE);
		}
		UpdateWindow(g_Wnd);
		//UpdateWindow(hTaskBar);
	}
}
/*****************************************************************************/
static void
str_to_uni(TCHAR * sizex, char * size1)
{
  int len;
  int i;

  len = strlen(size1);
  for (i = 0; i < len; i++)
  {
    sizex[i] = size1[i];
  }
  sizex[len] = 0;
}

/*****************************************************************************/
/* returns non zero if it processed something */
static int
check_sck(void)
{
  fd_set rfds;
  struct timeval tm;
  int count;
  int rv;

  rv = 0;
  if (g_block == 0)
  {
    g_block = 1;
    /* see if there really is data */
    FD_ZERO(&rfds);
    FD_SET((unsigned int)g_tcp_sck, &rfds);
    ZeroMemory(&tm, sizeof(tm));
    count = select(g_tcp_sck + 1, &rfds, 0, 0, &tm);
    if (count > 0)
    {
      if (ui_read_wire())
      {
        rv = 1;
      }
      else
      {
        PostQuitMessage(0);
      }
    }
    g_block = 0;
  }
  return rv;
}

/*****************************************************************************/
void
mi_error(char * msg)
{
#ifdef WITH_DEBUG
  printf(msg);
#else /* WITH_DEBUG */
  TCHAR lmsg[512];
  TCHAR ltitle[512];

  str_to_uni(lmsg, msg);
  str_to_uni(ltitle, "Error");
  MessageBox(g_Wnd, lmsg, ltitle, MB_OK);
#endif /* WITH_DEBUG */
}

static int get_ScanCode_from_VKey(int code){
	return myScancodeTable[code].scancode;
}

static int get_ScanCode_from_Char(DWORD c){
	return myCharTable[c].scancode;
	//#############################
/*	BYTE code=0;
	int i=0;
	BYTE found= 0;
	do{
		if ( myCharTable[i].charByte == c )
			found=myCharTable[i].scancode;
		i++;
	}while (!found && myCharTable[i].scancode!=0x00);
	if(found>0)
		return myCharTable[i--].scancode;
	else
		return 0;
*/
}

static BOOL get_isChar_from_VKey(int code)
{
	return myScancodeTable[code].isChar;
}
/*****************************************************************************/
static int
get_scan_code_from_ascii(int code)
{
  int rv;

  rv = 0;
  switch (code & 0xff)
  {
    case 0x30: rv = 0x0b; break; // 0
    case 0x31: rv = 0x02; break; // 1
    case 0x32: rv = 0x03; break; // 2
    case 0x33: rv = 0x04; break; // 3
    case 0x34: rv = 0x05; break; // 4
    case 0x35: rv = 0x06; break; // 5
    case 0x36: rv = 0x07; break; // 6
    case 0x37: rv = 0x08; break; // 7
    case 0x38: rv = 0x09; break; // 8
    case 0x39: rv = 0x0a; break; // 9

    case 0xbd: rv = 0x0c; break; // -
    case 0xbb: rv = 0x0d; break; // =
    case 0x08: rv = 0x0e; break; // backspace
    case 0x09: rv = 0x0f; break; // tab
    case 0xdb: rv = 0x1b; break; // ]
    case 0xdd: rv = 0x1a; break; // [
    case 0x14: rv = 0x3a; break; // capslock
    case 0xba: rv = 0x27; break; // ; //ö
    case 0xde: rv = 0x28; break; // '
    case 0x10: rv = 0x2a; break; // shift
    case 0xbc: rv = 0x33; break; // ,
    case 0xbe: rv = 0x34; break; // .
    case 0xbf: rv = 0x35; break; // /
    case 0x0d: rv = 0x1c; break; // enter
    case 0x27: rv = 0x4d; break; // arrow right
    case 0x25: rv = 0x4b; break; // arrow left
    case 0x26: rv = 0x48; break; // arrow up
    case 0x28: rv = 0x50; break; // arrow down
    case 0x20: rv = 0x39; break; // space
    case 0xdc: rv = 0x2b; break; // backslash
    case 0xc0: rv = 0x29; break; // `
    case 0x11: rv = 0x1d; break; // ctl

    case 0x41: rv = 0x1e; break; // a
    case 0x42: rv = 0x30; break; // b
    case 0x43: rv = 0x2e; break; // c
    case 0x44: rv = 0x20; break; // d
    case 0x45: rv = 0x12; break; // e
    case 0x46: rv = 0x21; break; // f
    case 0x47: rv = 0x22; break; // g
    case 0x48: rv = 0x23; break; // h
    case 0x49: rv = 0x17; break; // i
    case 0x4a: rv = 0x24; break; // j
    case 0x4b: rv = 0x25; break; // k
    case 0x4c: rv = 0x26; break; // l
    case 0x4d: rv = 0x32; break; // m
    case 0x4e: rv = 0x31; break; // n
    case 0x4f: rv = 0x18; break; // o
    case 0x50: rv = 0x19; break; // p
    case 0x51: rv = 0x10; break; // q
    case 0x52: rv = 0x13; break; // r
    case 0x53: rv = 0x1f; break; // s
    case 0x54: rv = 0x14; break; // t
    case 0x55: rv = 0x16; break; // u
    case 0x56: rv = 0x2f; break; // v
    case 0x57: rv = 0x11; break; // w
    case 0x58: rv = 0x2d; break; // x
    case 0x59: rv = 0x15; break; // y
    case 0x5a: rv = 0x2c; break; // z

	case 0xf6: rv = 0x27; break; // ouml
	default: rv=code;
  }
  return rv;
}

/*****************************************************************************/
static void
mi_scroll(int dx, int dy)
{
  HRGN rgn;

  rgn = CreateRectRgn(0, 0, 0, 0);
  ScrollWindowEx(g_Wnd, dx, dy, 0, 0, rgn, 0, SW_ERASE);
  InvalidateRgn(g_Wnd, rgn, 0);
  DeleteObject(rgn);
}

/*****************************************************************************/
int
mi_read_keyboard_state(void)
{
  short keydata;
  int code;

  code = 0;
  keydata = GetKeyState(VK_SCROLL);
  if (keydata & 0x0001)
  {
    code |= 1;
  }
  keydata = GetKeyState(VK_NUMLOCK);
  if (keydata & 0x0001)
  {
    code |= 2;
  }
  keydata = GetKeyState(VK_CAPITAL);
  if (keydata & 0x0001)
  {
    code |= 4;
  }
  return code;
}

/*****************************************************************************/
static void
mi_check_modifier(void)
{
  int code;

  code = mi_read_keyboard_state();
  ui_set_modifier_state(code);
	DEBUGMSG(DBK_KEY, (L"mi_check_modifier: keyboardstate=0x%08x: \n", code));
}

/*****************************************************************************/
static LRESULT
handle_WM_SETCURSOR(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  if (g_mousex >= g_wnd_clip.left &&
      g_mousey >= g_wnd_clip.top &&
      g_mousex < g_wnd_clip.right &&
      g_mousey < g_wnd_clip.bottom)
  {
    SetCursor(g_cursor);
  }
  /* need default behavoir here */
  return DefWindowProc(hWnd, message, wParam, lParam);
}

/*****************************************************************************/
static LRESULT
handle_WM_NCHITTEST(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  POINT pt;

  pt.x = LOWORD(lParam);
  pt.y = HIWORD(lParam);
  if (ScreenToClient(g_Wnd, &pt))
  {
    g_mousex = pt.x;
    g_mousey = pt.y;
  }
  return DefWindowProc(hWnd, message, wParam, lParam);
}

/*****************************************************************************/
static LRESULT
handle_WM_MOUSEMOVE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  g_mousex = LOWORD(lParam);
  g_mousey = HIWORD(lParam);
  ui_mouse_move(g_mousex + g_xscroll, g_mousey + g_yscroll);
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_LBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  g_mousex = LOWORD(lParam);
  g_mousey = HIWORD(lParam);
  ui_mouse_button(1, g_mousex + g_xscroll, g_mousey + g_yscroll, 1);
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_LBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  g_mousex = LOWORD(lParam);
  g_mousey = HIWORD(lParam);
  ui_mouse_button(1, g_mousex + g_xscroll, g_mousey + g_yscroll, 0);
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_LBUTTONDBLCLK(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	WORD xPos = LOWORD(lParam); 
	WORD yPos = HIWORD(lParam);
    g_mousex = LOWORD(lParam);
    g_mousey = HIWORD(lParam);
	if((xPos < g_screen_width/2 + 10) && (xPos > g_screen_width/2 -10)){
		if(yPos < 20){
			if (MessageBox(hWnd, L"Exit?", L"rdesktop", MB_YESNO | MB_SETFOREGROUND | MB_TOPMOST)==IDYES)
				PostQuitMessage(-99);
		}
	}

	/*
	wParam
    Indicates whether various virtual keys are down. This parameter can be one or more of the following values.
    Value				Meaning
    MK_CONTROL	0x0008	The CTRL key is down.
    MK_LBUTTON	0x0001	The left mouse button is down.
    MK_MBUTTON	0x0010	The middle mouse button is down.
    MK_RBUTTON	0x0002	The right mouse button is down.
    MK_SHIFT	0x0004	The SHIFT key is down.
    MK_XBUTTON1	0x0020	The first X button is down.
    MK_XBUTTON2	0x0040	The second X button is down.

	lParam
		The low-order word specifies the x-coordinate of the cursor. The coordinate is relative to the upper-left corner of the client area.
		The high-order word specifies the y-coordinate of the cursor. The coordinate is relative to the upper-left corner of the client area.
	xPos = GET_X_LPARAM(lParam); 
	yPos = GET_Y_LPARAM(lParam);
	*/

	//simulate a WM_LBUTTONDBLCLK by fast sending down/up+Down/up
	//mouse down
	ui_mouse_button(1, g_mousex + g_xscroll, g_mousey + g_yscroll, 1);
	Sleep(0);
	//mouse up
	ui_mouse_button(1, g_mousex + g_xscroll, g_mousey + g_yscroll, 0);
	Sleep(0);
	//mouse down
	ui_mouse_button(1, g_mousex + g_xscroll, g_mousey + g_yscroll, 1);
	Sleep(0);
	//mouse up
	ui_mouse_button(1, g_mousex + g_xscroll, g_mousey + g_yscroll, 0);

  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_RBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  g_mousex = LOWORD(lParam);
  g_mousey = HIWORD(lParam);
  ui_mouse_button(2, g_mousex + g_xscroll, g_mousey + g_yscroll, 1);
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_RBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  g_mousex = LOWORD(lParam);
  g_mousey = HIWORD(lParam);
  ui_mouse_button(2, g_mousex + g_xscroll, g_mousey + g_yscroll, 0);
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_MBUTTONDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  g_mousex = LOWORD(lParam);
  g_mousey = HIWORD(lParam);
  ui_mouse_button(3, g_mousex + g_xscroll, g_mousey + g_yscroll, 1);
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_MBUTTONUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  g_mousex = LOWORD(lParam);
  g_mousey = HIWORD(lParam);
  ui_mouse_button(3, g_mousex + g_xscroll, g_mousey + g_yscroll, 0);
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_MOUSEWHEEL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int delta;

  delta = ((signed short)HIWORD(wParam)); /* GET_WHEEL_DELTA_WPARAM(wParam); */
  if (delta > 0)
  {
    ui_mouse_button(4, 0, 0, 1);
    ui_mouse_button(4, 0, 0, 0);
  }
  else
  {
    ui_mouse_button(5, 0, 0, 1);
    ui_mouse_button(5, 0, 0, 0);
  }
  return 0;
}

/*****************************************************************************/
static LRESULT handle_WM_CHAR(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	/*
	wParam has CHAR code ie 0x61 for 'a' or 0x41 for 'A'
	'a'
	Message		Meaning		wParam		LParam
	KEY_DOWN				0x0041		0x001c0001
	WM_CHAR		'a'			0x0061		0x001c0001
	KEY_UP					0x0041		0xc01c0001

	'A'
	KEY_DOWN	VK_SHIFT	0x0010		0x00000001
	KEY_DOWN				0x0041		0x00000001
	WM_CHAR		'A'			0x0041		0x00000001
	KEY_UP					0x0041		0x80000001
	KEY_UP		VK_SHIFT	0x0010		0xc0000001

	lParam bits:
	30 Specifies the previous key state. The value is 1 if the key is down before the message is sent, or it is 0 if the key is up.
	31 Specifies the transition state. The value is 1 if the key is being released, or it is 0 if the key is being pressed.

	// trying to send Shift+a on SIP:
	__ WM_KEYDOWN: wParam=0x10, lParam=0x1
	__ WM_KEYUP: wParam=0x10, lParam=0x1

	__ WM_KEYDOWN: wParam=0x41, lParam=0x1
	__ WM_KEYUP: wParam=0x41, lParam=0x1
		WM_/WM_SYS_CHAR-DEADCHAR: wParam=0x41, lParam=0x1
		WM_CHAR: wParam(vKey)=0x41, lParam=0x01, scancode=0x00
	->WM_CHAR: get_ScanCode_from_Char=0x41
		WM_KEY-ui_key_down: scancode=0x41, ext=0x0
	__ WM_KEYUP: wParam=0x41, lParam=0x80000001
	__ WM_KEYUP: wParam=0x10, lParam=0xc0000001

	*/
	int scancode;
	int ext;
	int down;

	ext = HIWORD(lParam);
	scancode = LOBYTE(ext); //scancode is only the lower byte of the upper word

	down = !(ext & 0x8000);

	DEBUGMSG(DBK_KEY, (L"-->WM_CHAR: wParam(vKey)=0x%02x, lParam=0x%02x, scancode=0x%02x\n", wParam, lParam, scancode));

	//Translate WM_CHAR to scancode and send scancode
	scancode = get_ScanCode_from_Char(wParam);
	//scancode = MapVirtualKey(wParam, 0); //translate from VKEY to SCANCODE

	DEBUGMSG(DBK_KEY, (L"--->WM_CHAR: get_ScanCode_from_Char=0x%02x\n", scancode));

	ext &= 0x0100;

	/*
	if we start to process WM_CHAR messages we have to watch for side effects
	*/
		//ui_key_down(scancode, ext);
		//ui_key_up(scancode, ext);

	return TRUE;
}

/*****************************************************************************/
static LRESULT handle_WM_KEYDOWN(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int scancode;
  int ext;
  int down;
  int mScancode=0;
  short CapsLock=0;
  //if WM_CHAR, lParam and wParam are 0x00
  //but we will get double chars for normal keypresses!
    //if((lParam!=0) && (message!=WM_CHAR))
  	 // return TRUE;

  /*
	we need to manage the modifier keys here: VK_SHIFT, VK_CONTROL, VK_CAPITAL
	{ 0xA0, L"VK_LSHIFT" },
	{ 0xA1, L"VK_RSHIFT" },
	{ 0xA2, L"VK_LCONTROL" },
	{ 0xA3, L"VK_RCONTROL" },
	
	wParam is the KeyCode (VKEY_)

	lParam meanings:

	0–15 Specifies the repeat count. The value is the number of times the keystroke is repeated as a result of the user holding down the key. 
	16-23 Specifies the scan code. This value depends on the original equipment manufacturer.
	29 Specifies the context code. The value is 1 if the ALT key is held down while the key is pressed; otherwise, the value is 0.
	30 Specifies the previous key state. The value is 1 if the key is down before the message is sent, or it is 0 if the key is up.
	31 Specifies the transition state. The value is 1 if the key is being released, or it is 0 if the key is being pressed.
  */
  ext = HIWORD(lParam);
  scancode = LOBYTE(ext); //scancode is only the lower byte of the upper word
  down = !(ext & 0x8000);

  
	//get CAPS lock state
	CapsLock=GetKeyState(VK_CAPITAL);
	CapsLock &= 0x01; //is lowest bit toggled?
	DEBUGMSG(DBK_KEY, (L"WM_KEYDOWN: CapsLock Status: 0x%02x\n", CapsLock));
	//capslock on SIP delivers up/down for each tap!
	//do not send CapsLock key strokes, these will be handled elsewhere!
	if(wParam==VK_CAPITAL){
		mi_check_modifier();
		DEBUGMSG(DBK_KEY, (L"WM_KEYDOWN: ==== VK_CAPITAL detected, return immediately\n"));
		return TRUE;
	}

  DEBUGMSG(DBK_KEY, (L"WM_KEYDOWN: wParam(vKey)=0x%02x, lParam=0x%02x, scancode=0x%02x\n", wParam, lParam, scancode));

  //although keyboard buttons produce a scancode, this is not a standard PS/2 one on WinCE
  //so we trust wParam
  //the soft Input Panel always sends scancode=0x00
  if (TRUE) //(scancode == 0)
  {
	scancode = get_ScanCode_from_VKey(wParam);
    //scancode = get_scan_code_from_ascii(wParam);//wParam is vk_ value
    DEBUGMSG(DBK_KEY, (L"WM_KEYDOWN: MODIFIED scancode=0x%0x\n", scancode));
  }
	//TEST scancode &= 0xff;
	mScancode = MapVirtualKey(wParam, 0); //uCode is a virtual-key code and is translated into a scan code.
	DEBUGMSG(DBK_KEY, (L"WM_KEYDOWN: MapVirtualKey wParm=0x%0x => 0x%02x\n", wParam, mScancode));
	//  scancode=mScancode; 
	// end TEST

  ui_key_down(scancode, ext);
  return 0;
}

/*****************************************************************************/
static LRESULT handle_WM_KEYUP(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int scancode;
  int ext;
  int down;
  int mScancode=0;
  short CapsLock=0;
  //if WM_CHAR, lParam and wParam are 0x00
  //but we will get double chars for normal keypresses!
    //if((lParam!=0) && (message!=WM_CHAR))
  	 // return TRUE;

  /*
	we need to manage the modifier keys here: VK_SHIFT, VK_CONTROL, VK_CAPITAL
	{ 0xA0, L"VK_LSHIFT" },
	{ 0xA1, L"VK_RSHIFT" },
	{ 0xA2, L"VK_LCONTROL" },
	{ 0xA3, L"VK_RCONTROL" },
	
	wParam is the KeyCode (VKEY_)

	lParam meanings:

	0–15 Specifies the repeat count. The value is the number of times the keystroke is repeated as a result of the user holding down the key. 
	16-23 Specifies the scan code. This value depends on the original equipment manufacturer.
	29 Specifies the context code. The value is 1 if the ALT key is held down while the key is pressed; otherwise, the value is 0.
	30 Specifies the previous key state. The value is 1 if the key is down before the message is sent, or it is 0 if the key is up.
	31 Specifies the transition state. The value is 1 if the key is being released, or it is 0 if the key is being pressed.
  */
  ext = HIWORD(lParam);
  scancode = LOBYTE(ext); //scancode is only the lower byte of the upper word
  down = !(ext & 0x8000);

  
	//get CAPS lock state
	CapsLock=GetKeyState(VK_CAPITAL);
	CapsLock &= 0x01; //is lowest bit toggled?
	DEBUGMSG(DBK_KEY, (L"WM_KEYUP: CapsLock Status: 0x%02x\n", CapsLock));
	//capslock on SIP delivers up/down for each tap!
	//do not send CapsLock key strokes, these will be handled elsewhere!
	if(wParam==VK_CAPITAL){
		mi_check_modifier();
		DEBUGMSG(DBK_KEY, (L"WM_KEYUP: ==== VK_CAPITAL detected, return immediately\n"));
		return TRUE;
	}

  DEBUGMSG(DBK_KEY, (L"WM_KEYUP: wParam(vKey)=0x%02x, lParam=0x%02x, scancode=0x%02x\n", wParam, lParam, scancode));

  //although keyboard buttons produce a scancode, this is not a standard PS/2 one on WinCE
  //so we trust wParam
  //the soft Input Panel always sends scancode=0x00
  if (TRUE) //(scancode == 0)
  {
	scancode = get_ScanCode_from_VKey(wParam);
    //scancode = get_scan_code_from_ascii(wParam);//wParam is vk_ value
    DEBUGMSG(DBK_KEY, (L"WM_KEYUP: MODIFIED scancode=0x%0x\n", scancode));
  }
	//TEST scancode &= 0xff;
	mScancode = MapVirtualKey(wParam, MAPVK_VK_TO_VSC); //uCode is a virtual-key code and is translated into a scan code.
	DEBUGMSG(DBK_KEY, (L"WM_KEYUP: MapVirtualKey wParm=0x%0x => 0x%02x\n", wParam, mScancode));
	//  scancode=mScancode; 
	// end TEST

  ui_key_up(scancode, ext);
  DEBUGMSG(DBK_KEY, (L"WM_KEYUP: END\n\n"));
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_PAINT(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  RECT rect;
  HBRUSH brush;

  BeginPaint(hWnd, &ps);
  /* paint the area outside the rdp screen with one colour */
  rect = ps.rcPaint;
  rect.left = UI_MAX(rect.left, g_width);
  if (!IsRectEmpty(&rect))
  {
    brush = CreateSolidBrush(RGB(0, 0, 255));
    FillRect(ps.hdc, &rect, brush);
    DeleteObject(brush);
  }
  rect = ps.rcPaint;
  rect.top = UI_MAX(rect.top, g_height);
  if (!IsRectEmpty(&rect))
  {
    brush = CreateSolidBrush(RGB(0, 0, 255));
    FillRect(ps.hdc, &rect, brush);
    DeleteObject(brush);
  }
  rect = ps.rcPaint;
  EndPaint(hWnd, &ps);
  ui_invalidate(rect.left + g_xscroll,
                rect.top + g_yscroll,
                (rect.right - rect.left) + 1,
                (rect.bottom - rect.top) + 1);
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_SIZE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int oldxscroll;
  int oldyscroll;

  if (wParam == SIZE_MINIMIZED)
  {
	  PostQuitMessage(-66); //HGO
    return DefWindowProc(hWnd, message, wParam, lParam);
  }
  g_wnd_cwidth = LOWORD(lParam); /* client width / height */
  g_wnd_cheight = HIWORD(lParam);
  g_wnd_clip.left = 0;
  g_wnd_clip.top = 0;
  g_wnd_clip.right = g_wnd_clip.left + g_wnd_cwidth;
  g_wnd_clip.bottom = g_wnd_clip.top + g_wnd_cheight;
  if (g_wnd_cwidth < g_width || g_wnd_cheight < g_height)
  {
    SetScrollRange(g_Wnd, SB_HORZ, 0, g_width - g_wnd_cwidth, 1);
    SetScrollRange(g_Wnd, SB_VERT, 0, g_height - g_wnd_cheight, 1);
  }
  oldxscroll = g_xscroll;
  oldyscroll = g_yscroll;
  if (g_wnd_cwidth >= g_width)
  {
    g_xscroll = 0;
  }
  else
  {
    g_xscroll = UI_MIN(g_xscroll, g_width - g_wnd_cwidth);
  }
  if (g_wnd_cheight >= g_height)
  {
    g_yscroll = 0;
  }
  else
  {
    g_yscroll = UI_MIN(g_yscroll, g_height - g_wnd_cheight);
  }
  mi_scroll(oldxscroll - g_xscroll, oldyscroll - g_yscroll);
  if (wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED)
  {
    /* check the caps, num, and scroll lock here */
    mi_check_modifier();
  }
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_SIZING(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  LPRECT prect;
  int width;
  int height;
  int style;

  prect = (LPRECT) lParam; /* total window rect */
  width = (prect->right - prect->left) - (g_xoff * 2);
  height = (prect->bottom - prect->top) - (g_yoff + g_xoff);
  if (height < g_height || width < g_width)
  {
    style = GetWindowLong(g_Wnd, GWL_STYLE);
    if (!(style & WS_HSCROLL))
    {
      style |= WS_HSCROLL | WS_VSCROLL;
      SetWindowLong(g_Wnd, GWL_STYLE, style);
      g_xscroll = 0;
      g_yscroll = 0;
      SetScrollPos(g_Wnd, SB_HORZ, g_xscroll, 1);
      SetScrollPos(g_Wnd, SB_VERT, g_yscroll, 1);
    }
  }
  else if (height >= g_height && width >= g_width)
  {
    style = GetWindowLong(g_Wnd, GWL_STYLE);
    if (style & WS_HSCROLL)
    {
      style &= ~WS_HSCROLL;
      style &= ~WS_VSCROLL;
      SetWindowLong(g_Wnd, GWL_STYLE, style);
      g_xscroll = 0;
      g_yscroll = 0;
    }
  }
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_HSCROLL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int code;
  int oldxscroll;

  code = (int) LOWORD(wParam); /* scroll bar value */
  if (code == SB_LINELEFT)
  {
    oldxscroll = g_xscroll;
    g_xscroll--;
    g_xscroll = UI_MAX(g_xscroll, 0);
    SetScrollPos(g_Wnd, SB_HORZ, g_xscroll, 1);
    mi_scroll(oldxscroll - g_xscroll, 0);
  }
  else if (code == SB_LINERIGHT)
  {
    oldxscroll = g_xscroll;
    g_xscroll++;
    g_xscroll = UI_MIN(g_xscroll, g_width - g_wnd_cwidth);
    SetScrollPos(g_Wnd, SB_HORZ, g_xscroll, 1);
    mi_scroll(oldxscroll - g_xscroll, 0);
  }
  else if (code == SB_PAGELEFT)
  {
    oldxscroll = g_xscroll;
    g_xscroll -= g_wnd_cwidth / 2;
    g_xscroll = UI_MAX(g_xscroll, 0);
    SetScrollPos(g_Wnd, SB_HORZ, g_xscroll, 1);
    mi_scroll(oldxscroll - g_xscroll, 0);
  }
  else if (code == SB_PAGERIGHT)
  {
    oldxscroll = g_xscroll;
    g_xscroll += g_wnd_cwidth / 2;
    g_xscroll = UI_MIN(g_xscroll, g_width - g_wnd_cwidth);
    SetScrollPos(g_Wnd, SB_HORZ, g_xscroll, 1);
    mi_scroll(oldxscroll - g_xscroll, 0);
  }
  else if (code == SB_BOTTOM)
  {
    oldxscroll = g_xscroll;
    g_xscroll = g_width - g_wnd_cwidth;
    SetScrollPos(g_Wnd, SB_HORZ, g_xscroll, 1);
    mi_scroll(oldxscroll - g_xscroll, 0);
  }
  else if (code == SB_TOP)
  {
    oldxscroll = g_xscroll;
    g_xscroll = 0;
    SetScrollPos(g_Wnd, SB_HORZ, g_xscroll, 1);
    mi_scroll(oldxscroll - g_xscroll, 0);
  }
  else if (code == SB_THUMBPOSITION)
  {
    oldxscroll = g_xscroll;
    g_xscroll = (signed short) HIWORD(wParam);
    SetScrollPos(g_Wnd, SB_HORZ, g_xscroll, 1);
    mi_scroll(oldxscroll - g_xscroll, 0);
  }
  return 0;
}

/*****************************************************************************/
static LRESULT
handle_WM_VSCROLL(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  int code;
  int oldyscroll;

  code = (int) LOWORD(wParam); /* scroll bar value */
  if (code == SB_LINELEFT)
  {
    oldyscroll = g_yscroll;
    g_yscroll--;
    g_yscroll = UI_MAX(g_yscroll, 0);
    SetScrollPos(g_Wnd, SB_VERT, g_yscroll, 1);
    mi_scroll(0, oldyscroll - g_yscroll);
  }
  else if (code == SB_LINERIGHT)
  {
    oldyscroll = g_yscroll;
    g_yscroll++;
    g_yscroll = UI_MIN(g_yscroll, g_height - g_wnd_cheight);
    SetScrollPos(g_Wnd, SB_VERT, g_yscroll, 1);
    mi_scroll(0, oldyscroll - g_yscroll);
  }
  else if (code == SB_PAGELEFT)
  {
    oldyscroll = g_yscroll;
    g_yscroll -= g_wnd_cheight / 2;
    g_yscroll = UI_MAX(g_yscroll, 0);
    SetScrollPos(g_Wnd, SB_VERT, g_yscroll, 1);
    mi_scroll(0, oldyscroll - g_yscroll);
  }
  else if (code == SB_PAGERIGHT)
  {
    oldyscroll = g_yscroll;
    g_yscroll += g_wnd_cheight / 2;
    g_yscroll = UI_MIN(g_yscroll, g_height - g_wnd_cheight);
    SetScrollPos(g_Wnd, SB_VERT, g_yscroll, 1);
    mi_scroll(0, oldyscroll - g_yscroll);
  }
  else if (code == SB_BOTTOM)
  {
    oldyscroll = g_yscroll;
    g_yscroll = g_height - g_wnd_cheight;
    SetScrollPos(g_Wnd, SB_VERT, g_yscroll, 1);
    mi_scroll(0, oldyscroll - g_yscroll);
  }
  else if (code == SB_TOP)
  {
    oldyscroll = g_yscroll;
    g_yscroll = 0;
    SetScrollPos(g_Wnd, SB_VERT, g_yscroll, 1);
    mi_scroll(0, oldyscroll - g_yscroll);
  }
  else if (code == SB_THUMBPOSITION)
  {
    oldyscroll = g_yscroll;
    g_yscroll = (signed short) HIWORD(wParam);
    SetScrollPos(g_Wnd, SB_VERT, g_yscroll, 1);
    mi_scroll(0, oldyscroll - g_yscroll);
  }
  return 0;
}

#ifdef MYWINCE
/*****************************************************************************/
// process SIP up/down
static LRESULT
handle_WM_SETTINGCHANGE(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
  SIPINFO si;
  int x;
  int y;
  int w;
  int h;
  int style;

  DEBUGMSG(1, (L"handle_WM_SETTINGCHANGE...\n"));

  SipButtonShow(TRUE);

	return 0; //HGO we dont resize the window

  ZeroMemory(&si, sizeof(SIPINFO));
  si.cbSize = sizeof(SIPINFO);
  SHSipInfo(SPI_GETSIPINFO, lParam, &si, 0);
  //we need to calculate our own dimensions, rcVisibleDesktop does not support hidden HHTaskbar
  x = si.rcVisibleDesktop.left;
  y = si.rcVisibleDesktop.top;
  w = si.rcVisibleDesktop.right - x;
  h = si.rcVisibleDesktop.bottom - y - MENU_HEIGHT;
  /* get rid of menu */
  //HGO DestroyWindow(SHFindMenuBar(g_Wnd));
  if (si.fdwFlags & SIPF_ON)
  {
	DEBUGMSG(1, (L"\thandle_WM_SETTINGCHANGE g_sip_up = 1\n"));
    g_sip_up = 1; /* used for WM_SETFOCUS */

#ifdef USE_MENUBAR
	// ######## NOT used, see doCreateMenu! ########
	////XXX
	//SHMENUBARINFO mb;
	//ZeroMemory(&mb, sizeof(SHMENUBARINFO));
	//mb.cbSize = sizeof(SHMENUBARINFO);
	//mb.hwndParent = g_Wnd;
	//mb.hInstRes = g_Instance;			   // Inst handle of app
	//mb.dwFlags = SHCMBF_HMENU;  // SHCMBF_EMPTYBAR;
	//mb.nToolBarId = IDR_MENU1;	// HGO 0;                     // ID of toolbar resource
	//mb.nBmpId = 0;                         // ID of bitmap resource
	//mb.cBmpImages = 0;                     // Num of images in bitmap 
	//mb.hwndMB = 0;                         // Handle of bar returned

	//if (!SHCreateMenuBar(&mb))
	//	DEBUGMSG(DBG_W32, (L"Could not create menubar!\n"));
	//g_hMenuBar = mb.hwndMB; //HGO
	////XXX
 //   MoveWindow(g_Wnd, x, y, w, h, FALSE);
	//SetWindowPos(g_Wnd, HWND_TOPMOST, 0,0, 240,294, SWP_SHOWWINDOW);

 //   SHFullScreen(g_Wnd, SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON);
	//SHFullScreen(g_Wnd, SHFS_HIDETASKBAR | SHFS_SHOWSIPBUTTON | SHFS_HIDESTARTICON);
#endif
  }
  else
  {
    g_sip_up = 0;
	DEBUGMSG(1, (L"\thandle_WM_SETTINGCHANGE g_sip_up = 1\n"));
    if (g_fullscreen)
    {
      MoveWindow(g_Wnd, 0, 0, g_screen_width, g_screen_height, FALSE);
    }
    else
    {
      MoveWindow(g_Wnd, x, y, w, h, FALSE);
    }
    if ((g_fullscreen && g_width <= g_screen_width &&
         g_height <= g_screen_height) ||
        (!g_fullscreen && g_width <= w && g_height <= h))
    {
		//Add scrollbars
      style = GetWindowLong(g_Wnd, GWL_STYLE);
      if (style & WS_HSCROLL)
      {
        style &= ~WS_HSCROLL;
        style &= ~WS_VSCROLL;
        SetWindowLong(g_Wnd, GWL_STYLE, style);
        g_xscroll = 0;
        g_yscroll = 0;
      }
    }
   // if (g_fullscreen)
   // {
   //   SHFullScreen(g_Wnd, SHFS_HIDETASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON);
   // }
   // else
   // {
	  //SHFullScreen(g_Wnd, SHFS_HIDETASKBAR | SHFS_SHOWSIPBUTTON | SHFS_HIDESTARTICON);
   //   //SHFullScreen(g_Wnd, SHFS_SHOWTASKBAR | SHFS_SHOWSIPBUTTON | SHFS_SHOWSTARTICON);
   // }
  }
  return 0;
}
#endif /* MYWINCE */

/*****************************************************************************/
LRESULT CALLBACK
WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	char* cStr;
	TCHAR* wStr;
  switch (message)
  {
	case WM_CREATE:
		if(!g_fullscreen)
			doCreateMenu(hWnd, g_Instance); //HGO
		if(g_busescanner){	//do not start scanner thread if g_busescanner is FALSE
			DEBUGMSG(1, (L"starting with barcode scanner support\n"));
	  		// Create a synchronization event for scanner thread synchronization.
			ghScannRead = CreateEvent(NULL, FALSE, FALSE, NULL);
			// Now create the read thread to monitor the scanner.
			ghScannerThread = CreateThread(0, 0, ScanThread, (LPVOID) hWnd, 0, 0);
		}
		else
			DEBUGMSG(1, (L"+++ starting without barcode scanner support +++\n"));

		break;
	case WM_HAVE_SCAN:
		// here is the scanned data
		cStr=(char*)malloc(gOneScan.dwBytesReturned+1);
	#ifdef DEBUG
		wStr=(TCHAR*)malloc(gOneScan.dwBytesReturned*2+2);
		memset(wStr, 0, gOneScan.dwBytesReturned*2+2);
	#endif
		memset(cStr,0,gOneScan.dwBytesReturned+1);
		memcpy(cStr, gOneScan.rgbDataBuffer, gOneScan.dwBytesReturned);
		//strcat(cStr, "\0");
		//sprintf(cStr, "%s", gOneScan.rgbDataBuffer);
	#ifdef DEBUG
		str_to_uni(wStr, cStr);
		DEBUGMSG(DBG_SCAN, (L"++++ Scanned data: %s\n",wStr));
		free(wStr);
	#endif
		SendKeys(cStr);
//		SendKeys("\r");
		free(cStr); 
		// Note: Artificial delay added so we can see scan ahead work.
		//Sleep(10); //was 2000 HGO
		// Let scanning thread know we have consumed data.
		SetEvent( ghScannRead);
		break;
    case WM_SETCURSOR:
      return handle_WM_SETCURSOR(hWnd, message, wParam, lParam);
    case 0x0084: /* WinCE don't have this WM_NCHITTEST: */
      return handle_WM_NCHITTEST(hWnd, message, wParam, lParam);
    case WM_MOUSEMOVE:
      return handle_WM_MOUSEMOVE(hWnd, message, wParam, lParam);
    case WM_LBUTTONDOWN:
		//use a timer to detect tap-and-hold?
	  DEBUGMSG(DBK_KEY, (L"WM_LBUTTONDOWN: wParam=0x%0x, lParam=0x%0x\n", wParam, lParam));
      return handle_WM_LBUTTONDOWN(hWnd, message, wParam, lParam);
    case WM_LBUTTONUP:
	  DEBUGMSG(DBK_KEY, (L"WM_LBUTTONUP: wParam=0x%0x, lParam=0x%0x\n", wParam, lParam));
		return handle_WM_LBUTTONUP(hWnd, message, wParam, lParam);
/*
	case WM_CONTEXTMENU:
	  DEBUGMSG(DBK_KEY, (L"WM_CONTEXTMENU: wParam=0x%0x, lParam=0x%0x\n", wParam, lParam));
	case WM_NOTIFY:
	  DEBUGMSG(DBK_KEY, (L"WM_NOTIFY: wParam=0x%0x, lParam=0x%0x\n", wParam, lParam));
*/
    case WM_RBUTTONDOWN:
      return handle_WM_RBUTTONDOWN(hWnd, message, wParam, lParam);
    case WM_RBUTTONUP:
      return handle_WM_RBUTTONUP(hWnd, message, wParam, lParam);
    case WM_MBUTTONDOWN:
      return handle_WM_MBUTTONDOWN(hWnd, message, wParam, lParam);
    case WM_MBUTTONUP:
      return handle_WM_MBUTTONUP(hWnd, message, wParam, lParam);
	case WM_LBUTTONDBLCLK: //HGO
		//test for location
		return handle_WM_LBUTTONDBLCLK(hWnd, message, wParam, lParam);
    /* some windows compilers don't have these defined like vc6 */
    case 0x020a: /* WM_MOUSEWHEEL: */
      return handle_WM_MOUSEWHEEL(hWnd, message, wParam, lParam);
	  // dont handle WM_KEY.. for printable chars
	  // let windows do the work: return DefWindowProc(hWnd, message, wParam, lParam);
	  // process ONLY non-chars key within WM_KEY...
    case WM_KEYDOWN:
		DEBUGMSG(DBK_KEY, (L"__ WM_KEYDOWN: wParam=0x%0x, lParam=0x%0x\n", wParam, lParam));
    case WM_SYSKEYDOWN:
		return handle_WM_KEYDOWN(hWnd, message, wParam, lParam);
    case WM_KEYUP:
		DEBUGMSG(DBK_KEY, (L"__ WM_KEYUP: wParam=0x%0x, lParam=0x%0x\n", wParam, lParam));
    case WM_SYSKEYUP:
		//handle these keys using vk_ (wParam) value!
		//if(get_isChar_from_VKey(wParam))
		//	return DefWindowProc(hWnd, message, wParam, lParam); //let windows translate this to WM_CHAR
		//else
		return handle_WM_KEYUP(hWnd, message, wParam, lParam);
    case WM_CHAR: //########### HGO ##############
    case WM_DEADCHAR:
    case WM_SYSCHAR:
    case WM_SYSDEADCHAR:
		DEBUGMSG(DBK_KEY, (L"WM_/WM_SYS_CHAR-DEADCHAR: wParam=0x%0x, lParam=0x%0x\n", wParam, lParam));
		return handle_WM_CHAR(hWnd, message, wParam, lParam);
		//return DefWindowProc(hWnd, message, wParam, lParam);
      break;
    case WM_PAINT:
	  SipButtonShow(TRUE);
      return handle_WM_PAINT(hWnd, message, wParam, lParam);
    case WM_DESTROY:
		do_hide_taskbar(FALSE);
      PostQuitMessage(0);
      break;
    case WM_APP + 1:
    case WM_TIMER:
      check_sck();
      break;
    case WM_SIZE:
      return handle_WM_SIZE(hWnd, message, wParam, lParam);
    case 532: /* not defined in wince WM_SIZING: */
      return handle_WM_SIZING(hWnd, message, wParam, lParam);
    case WM_HSCROLL:
      return handle_WM_HSCROLL(hWnd, message, wParam, lParam);
    case WM_VSCROLL:
      return handle_WM_VSCROLL(hWnd, message, wParam, lParam);
	case WM_COMMAND:
		return handle_WM_COMMAND(hWnd, message, wParam, lParam);
    case WM_SETTINGCHANGE:
	case WM_ACTIVATE:
		do_hide_taskbar(TRUE);
      return handle_WM_SETTINGCHANGE(hWnd, message, wParam, lParam);
    case WM_SETFOCUS:
      mi_check_modifier();
      if (g_sip_up)
      {
        SHSipPreference(hWnd, SIP_UP);
      }
      return DefWindowProc(hWnd, message, wParam, lParam);
    default:
      return DefWindowProc(hWnd, message, wParam, lParam);
  }
  return 0;
}

/*****************************************************************************/
static HRGN
mi_clip(HDC dc)
{
  HRGN rgn;

  rgn = CreateRectRgn(g_clip_left + g_xoff - g_xscroll,
                      g_clip_top + g_yoff - g_yscroll,
                      g_clip_right + g_xoff - g_xscroll,
                      g_clip_bottom + g_yoff - g_yscroll);
  SelectClipRgn(dc, rgn);
  IntersectClipRect(dc, g_wnd_clip.left + g_xoff, g_wnd_clip.top + g_yoff,
                    g_wnd_clip.right + g_xoff, g_wnd_clip.bottom + g_yoff);
  return rgn;
}

/*****************************************************************************/
/* returns non zero if ok */
int
mi_create_window(void)
{
  RECT rc;
  WNDCLASS wc;
  TCHAR classname[512];
  TCHAR caption[512];
  DWORD style;
  DWORD exStyle=0;
  TCHAR str[512];
  int x;
  int y;
  int w;
  int h;
  int winBorder;
  BOOL iRet;
	HWND tHwnd=0;
	INITCOMMONCONTROLSEX icex;	//for menubar and SIP!

  if (g_Wnd != 0 || g_Instance != 0)
  {
    return 0;
  }
  if(FindWindow(L"rdesktop", NULL))
	  return 0; //EXIT!, no second instance allowed

  g_Instance = GetModuleHandle(NULL);
  ZeroMemory(&wc, sizeof(wc));
  wc.lpfnWndProc = WndProc; /* points to window procedure */
  
  /* name of window class */
  str_to_uni(classname, "rdesktop");
  wc.lpszClassName = classname;
  str_to_uni(caption, "WinRDesktop");
	

  //HGO for better redraw!
  wc.style = CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS;       // Window styles plus double click option!
  wc.hbrBackground = (HBRUSH) GetStockObject (GRAY_BRUSH); 

  /* Register the window class. */
  if (!RegisterClass(&wc))
  {
	  DEBUGMSG(DBG_W32, (L"Failed to register window class"));
	  return 0; /* Failed to register window class */
  }

    // Load the command bar common control class. For menubar etc.
    icex.dwSize = sizeof (INITCOMMONCONTROLSEX);
    icex.dwICC = ICC_BAR_CLASSES;
    InitCommonControlsEx (&icex);

  //SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);	//not available in WINCE

  //get local screen size
  rc.left = 0; rc.right = 0; rc.top = 0; rc.bottom = 0;
  rc.right = GetSystemMetrics(SM_CXSCREEN);		//check SM_CXVSCROLL, SM_CYVSCROLL too?
  rc.bottom = GetSystemMetrics(SM_CYSCREEN);	// SM_CYMENU=menu hight?, SM_CYCAPTION=titlebar height?

  style = WS_POPUP | WS_NONAVDONEBUTTON;
  //style = WS_POPUP | WS_BORDER | WS_CAPTION | WS_SYSMENU;
  exStyle = 0; //WS_EX_TOPMOST; // WS_EX_NODRAG | WS_EX_CAPTIONOKBTN | WS_EX_WINDOWEDGE; //HGO 
  //HGO SetRect (&rc, 0, 0, CX_WINDOW, 194);
  iRet = AdjustWindowRectEx(&rc, style, FALSE, exStyle); //get the desired rect size
  x = 0; //CW_USEDEFAULT;
  y = 0; //CW_USEDEFAULT;
  w = rc.right - rc.left;
  MENU_HEIGHT=GetSystemMetrics(SM_CYMENU); //says 23, should be 26
  winBorder = GetSystemMetrics(SM_CYBORDER); //says 1
  DEBUGMSG(1, (L"CreateWindows(): menuH=%i, winBorder=%i\r\n", MENU_HEIGHT, winBorder));	
  if(g_fullscreen)
	{
		MENU_HEIGHT=0;
		winBorder=0;
	}

  h = rc.bottom - rc.top - MENU_HEIGHT - 3*winBorder;
	ASSERT(w>0);
	ASSERT(h>0);
  //if(w<240 || h<320){
	 // w=240;
	 // h=320;
  //}
  wsprintf(str, L"\n#### Window size is (%i/%i)-(%i/%i)\n", x,y, w,h);
  DEBUGMSG(DBG_W32, (str));

  g_Wnd = CreateWindow(wc.lpszClassName, caption,
                       style, x, y, w, h,
                       (HWND) NULL, (HMENU) NULL, g_Instance,
                       (LPVOID) NULL);
  wsprintf(str, L"\n#### Window handle is 0x%0x\n", g_Wnd);
  DEBUGMSG(DBG_W32, (str));
  
  // SHDoneButton(g_Wnd, SHDB_HIDE); //HGO see WS_NONAVDONEBUTTON, SHFullScreen should be used in WM_ACTIVATE

  do_hide_taskbar(TRUE);

  g_clip_left = 0;
  g_clip_top = 0;
  g_clip_right = g_clip_left + g_width;
  g_clip_bottom = g_clip_top + g_height;
  ShowWindow(g_Wnd, SW_SHOWNORMAL); //SW_HIDE);
  UpdateWindow(g_Wnd);

  OutputDebugString(L"Window Created!!");

  WSAAsyncSelect(g_tcp_sck, g_Wnd, WM_APP + 1, FD_READ);
  SetTimer(g_Wnd, 1, 333, 0);

  return 1;
}

/*****************************************************************************/
int
mi_main_loop(void)
{
  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  return msg.wParam;
}

/*****************************************************************************/
void
mi_warning(char * msg)
{
}

/*****************************************************************************/
static void
mi_show_error(char * caption)
{
  LPVOID lpMsgBuf;
  TCHAR lcaption[512];

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
                NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                (LPTSTR) &lpMsgBuf, 0, NULL);
#ifdef WITH_DEBUG
  printf(lpMsgBuf);
#else /* WITH_DEBUG */
  str_to_uni(lcaption, caption);
  MessageBox(g_Wnd, (LPTSTR) lpMsgBuf, lcaption,
             MB_OK | MB_ICONINFORMATION);
#endif /* WITH_DEBUG */
  LocalFree(lpMsgBuf);
}

/*****************************************************************************/
void
mi_paint_rect(char * data, int width, int height, int x, int y, int cx, int cy)
{
  HBITMAP bitmap;
  BITMAPINFO bi;
  HDC dc;
  HDC maindc;
  HGDIOBJ save;
  HRGN rgn;
  void * bits;
  int i;
  int j;
  int colour;
  int red;
  int green;
  int blue;

  ZeroMemory(&bi, sizeof(bi));
  bi.bmiHeader.biSize = sizeof(bi.bmiHeader);
  bi.bmiHeader.biWidth = width;
  bi.bmiHeader.biHeight = -height;
  bi.bmiHeader.biPlanes = 1;
  bi.bmiHeader.biBitCount = 32;
  bi.bmiHeader.biCompression = BI_RGB;
  maindc = GetWindowDC(g_Wnd);
  bitmap = CreateDIBSection(maindc, &bi, DIB_RGB_COLORS, (void **) &bits, 0, 0);
  if (bitmap == 0)
  {
    mi_show_error("CreateDIBSection failed");
  }

  if (g_server_depth == 8)
  {
    for (i = cy - 1; i >= 0; i--)
    {
      for (j = cx - 1; j >= 0; j--)
      {
        colour = ((unsigned char*)data)[i * cx + j];
        red = (pal_entries[colour & 0xff] & 0xff0000) >> 16;
        green = (pal_entries[colour & 0xff] & 0xff00) >> 8;
        blue = pal_entries[colour & 0xff] & 0xff;
        MAKE_COLOUR32(colour, red, green, blue);
        ((unsigned int*)bits)[i * cx + j] = colour;
      }
    }
  }
  else if (g_server_depth == 15)
  {
    for (i = cy - 1; i >= 0; i--)
    {
      for (j = cx - 1; j >= 0; j--)
      {
        colour = ((unsigned short*)data)[i * cx + j];
        SPLIT_COLOUR15(colour, red, green, blue);
        MAKE_COLOUR32(colour, red, green, blue);
        ((unsigned int*)bits)[i * cx + j] = colour;
      }
    }
  }
  else if (g_server_depth == 16)
  {
    for (i = cy - 1; i >= 0; i--)
    {
      for (j = cx - 1; j >= 0; j--)
      {
        colour = ((unsigned short*)data)[i * cx + j];
        SPLIT_COLOUR16(colour, red, green, blue);
        MAKE_COLOUR32(colour, red, green, blue);
        ((unsigned int*)bits)[i * cx + j] = colour;
      }
    }
  }
  dc = CreateCompatibleDC(maindc);
  if (dc == 0)
  {
    mi_show_error("CreateCompatibleDC failed");
  }
  save = SelectObject(dc, bitmap);
  rgn = mi_clip(maindc);
  BitBlt(maindc, x + g_xoff - g_xscroll, y + g_yoff - g_yscroll, cx, cy, dc,
         0, 0, SRCCOPY);
  SelectObject(dc, save);
  DeleteObject(bitmap);
  DeleteDC(dc);
  ReleaseDC(g_Wnd, maindc);
  DeleteObject(rgn);

}

/*****************************************************************************/
static int
mi_process_a_param(char * param1, int state)
{
  char * p;

  if (state == 0)
  {
    if (strcmp(param1, "-g") == 0 || strcmp(param1, "geometry") == 0)
    {
      state = 1;
    }
    if (strcmp(param1, "-t") == 0 || strcmp(param1, "port") == 0)
    {
      state = 2;
    }
    if (strcmp(param1, "-a") == 0 || strcmp(param1, "bpp") == 0)
    {
      state = 3;
    }
    if (strcmp(param1, "-f") == 0 || strcmp(param1, "fullscreen") == 0)
    {
      g_fullscreen = 1;
    }
    if (strcmp(param1, "-u") == 0 || strcmp(param1, "username") == 0)
    {
      state = 5;
    }
    if (strcmp(param1, "-p") == 0 || strcmp(param1, "password") == 0)
    {
      state = 6;
    }
    if (strcmp(param1, "-d") == 0 || strcmp(param1, "domain") == 0)
    {
      state = 7;
    }
    if (strcmp(param1, "-s") == 0 || strcmp(param1, "shell") == 0)
    {
      state = 8;
    }
    if (strcmp(param1, "-c") == 0 || strcmp(param1, "directory") == 0)
    {
      state = 9;
    }
    if (strcmp(param1, "-n") == 0 || strcmp(param1, "hostname") == 0)
    {
      state = 10;
    }
    if (strcmp(param1, "-x") == 0 || strcmp(param1, "clipboard") == 0)
    {
      state = 11;
    }
    if (strcmp(param1, "-b") == 0 || strcmp(param1, "barcodescanner") == 0)
    {
      state = 12;
    }
    if (strcmp(param1, "-sip") == 0 || strcmp(param1, "nosip") == 0)
    {
      state = 13;
    }
  }
  else
  {
    if (state == 1) /* -g */
    {
      state = 0;
      if (strcmp(param1, "workarea") == 0)
      {
        g_workarea = 1;
        return state;
      }
      g_width = strtol(param1, &p, 10);
      if (g_width <= 0)
      {
        mi_error("invalid geometry\r\n");
      }
      if (*p == 'x')
      {
        g_height = strtol(p + 1, &p, 10);
      }
      if (g_height <= 0)
      {
        mi_error("invalid geometry\r\n");
      }
      g_width_height_set = 1;
    }
    if (state == 2) /* -t */
    {
      state = 0;
      g_tcp_port_rdp = atol(param1);
    }
    if (state == 3) /* -a */
    {
      state = 0;
      g_server_depth = atol(param1);
      if (g_server_depth != 8 && g_server_depth != 15 && g_server_depth != 16)
      {
        mi_error("invalid server bpp\r\n");
      }
    }
    if (state == 5) /* -u */
    {
      state = 0;
      //strcpy(g_username, param1);
	  g_username = (char *) malloc(strlen(param1) + 1);
	  STRNCPY(g_username, param1, strlen(param1) + 1);
    }
    if (state == 6) /* -p */
    {
      state = 0;
      strcpy(g_password, param1);
    }
    if (state == 7) /* -d */
    {
      state = 0;
      strcpy(g_domain, param1);
    }
    if (state == 8) /* -s */
    {
      state = 0;
      strcpy(g_shell, param1);
    }
    if (state == 9) /* -c */
    {
      state = 0;
      strcpy(g_directory, param1);
    }
    if (state == 10) /* -n */
    {
      state = 0;
      strcpy(g_hostname, param1);
    }
    if (state == 11) /* -x clipboard */
    {
      state = 0;
	  //g_rdpclip = TRUE;
   //   strcpy(g_hostname, param1);
	  //cliprdr_set_mode("PRIMARYCLIPBOARD");
    }
    if (state == 12) /* -b enable scanner */
    {
		g_busescanner=TRUE;
      state = 0;
	}
    if (state == 13) /* -sip / nosip */
    {
		g_bnosip=TRUE;
      state = 0;
	}
  }
  return state;
}

/*****************************************************************************/
static int
mi_post_param(void)
{
  /* after parameters */
  if (g_fullscreen)
  {
    g_xoff = 0;
    g_yoff = 0;
    if (!g_width_height_set)
    {
      g_width = g_screen_width;
      g_height = g_screen_height;
    }
  }
  else if (g_workarea)
  {
#ifdef MYWINCE
    g_xoff = 0;
    g_yoff = 0;
    g_width = g_screen_width;
    g_height = g_screen_height - MENU_HEIGHT; /* start menu and menubar size is 26 */
#else /* MYWINCE */
    g_xoff = GetSystemMetrics(SM_CXEDGE) * 2;
    g_yoff = GetSystemMetrics(SM_CYCAPTION) +
             GetSystemMetrics(SM_CYEDGE) * 2;
    g_width = g_screen_width;
    g_height = g_screen_height;
    g_height = (g_height - g_yoff) - g_xoff - 20; /* todo */
#endif /* MYWINCE */
    g_width_height_set = 1;
  }
  else
  {
#ifdef MYWINCE
    g_xoff = 0;
    g_yoff = 0;
#else /* MYWINCE */
    g_xoff = GetSystemMetrics(SM_CXEDGE) * 2;
    g_yoff = GetSystemMetrics(SM_CYCAPTION) +
             GetSystemMetrics(SM_CYEDGE) * 2;
#endif /* MYWINCE */
  }
  g_width = g_width & (~3);
  return 1;
}

/*****************************************************************************/
static int
mi_check_config_file(void)
{
  HANDLE fd;
  DWORD count;
  TCHAR filename[256];
  char buffer[256];
  char vname[256];
  char value[256];
  int index;
  int mode;
  int vnameindex;
  int valueindex;
  int state;
  int rv;

  TCHAR* appPath;
  TCHAR iniFileName[MAX_PATH];

  //first try to find ini file in applications dir
  appPath=getAppDir();
  wsprintf(iniFileName, L"%swinrdesktop.ini", appPath);
  if(!ExistFile(iniFileName))//if not found try root dir
	  wsprintf(iniFileName, L"\\winrdesktop.ini");

  rv = 0;
  mode = 0;
  vnameindex = 0;
  valueindex = 0;
  vname[vnameindex] = 0;
  value[valueindex] = 0;

  //str_to_uni(filename, "\\winrdesktop.ini");
  wsprintf(filename, iniFileName);

  fd = CreateFile(filename, GENERIC_READ,
                  FILE_SHARE_READ | FILE_SHARE_WRITE,
                  NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
  count = 255;
  while (ReadFile(fd, buffer, count, &count, NULL))
  {
    if (count == 0)
    {
      break;
    }
    buffer[count] = 0;
    index = 0;
    while (index < (int) count)
    {
      if (buffer[index] == '=')
      {
        mode = 1;
      }
      else if (buffer[index] == 10 || buffer[index] == 13)
      {
        mode = 0;
        vname[vnameindex] = 0;
        value[valueindex] = 0;
        if (strlen(vname) > 0 || strlen(value) > 0)
        {
          if (strcmp(vname, "server") == 0)
          {
            strcpy(g_servername, value);
            rv = 1;
          }
          else
          {
            state = mi_process_a_param(vname, 0);
            mi_process_a_param(value, state);
          }
        }
        vnameindex = 0;
        valueindex = 0;
      }
      else if (mode == 0)
      {
        vname[vnameindex] = buffer[index];
        vnameindex++;
      }
      else
      {
        value[valueindex] = buffer[index];
        valueindex++;
      }
      index++;
    }
    count = 255;
  }
  CloseHandle(fd);
  if (rv)
  {
    mi_post_param();
  }
  return rv;
}

/*****************************************************************************/
/* process the command line parameters */
/* returns boolean, non zero is ok */
static int
mi_process_cl(LPTSTR lpCmdLine)
{
  char param[256];
  char param1[256];
#ifndef MYWINCE
  TCHAR l_username[256];
#endif
  DWORD size;
  int len;
  int i;
  int i1;
  int state;

//  strcpy(g_hostname, "test");
//  strcpy(g_username, "pda");
  /* get username and convert it from unicode */
  size = 255;
#ifndef MYWINCE
  /* WinCE doesn't have GetUserName */
  if (GetUserName(l_username, &size))
  {
    for (i = size; i >= 0; i--)
    {
      g_username[i] = (char) l_username[i];
    }
    g_username[size] = 0;
  }
  else
  {
    mi_show_error("GetUserName");
  }
#endif /* MYWINCE */
  /* get computer name */
  if (gethostname(g_hostname, 255) != 0)
  {
    mi_show_error("gethostname");
  }
  /* defaults */
  strcpy(g_servername, "127.0.0.1");
  g_server_depth = 8;
  g_screen_width = GetSystemMetrics(SM_CXSCREEN);
  g_screen_height = GetSystemMetrics(SM_CYSCREEN);
  
  DEBUGMSG(DBG_TEST, (L"Local screen %ix%i\r\n", g_screen_width, g_screen_height));
  /* process parameters */
  i1 = 0;
  state = 0;
  len = lstrlen(lpCmdLine);
  if (len == 0)
  {
    //try to process an ini file if any
    return mi_check_config_file();
  }
  for (i = 0; i < len; i++)
  {
    if (lpCmdLine[i] != 32 && lpCmdLine[i] != 9) /* space or tab */
    {
      param[i1] = (char) lpCmdLine[i];
      i1++;
    }
    else
    {
      param[i1] = 0;
      i1 = 0;
      strcpy(param1, param);
      state = mi_process_a_param(param1, state);
      strcpy(g_servername, param1);
    }
  }
  if (i1 > 0)
  {
    param[i1] = 0;
    strcpy(param1, param);
    state = mi_process_a_param(param1, state);
    strcpy(g_servername, param1); //the last arg is the server to connect to
  }

  if (state == 0)
  {
    mi_post_param();
  }
  return (state == 0);
}

/*****************************************************************************/
/* display the command line options available */
static void
mi_show_params(void)
{
  char text1[512 * 4];
  TCHAR textx[512 * 4];
  TCHAR lcaption[64];

  strcpy(text1, "");
  strcat(text1, "WinRDesktop - an RDP client based on rdesktop\r\n");
  strcat(text1, "You can't run this application without " );
  strcat(text1, "correct parameters\r\n");
  strcat(text1, "\r\n");
  strcat(text1, "command line options\r\n");
  strcat(text1, "\r\n");
  strcat(text1, "WinRDesktop [-g widthxheight] [-t port] [-a bpp]\r\n");
  strcat(text1, "    [-f] [-u username] [-p password] [-d domain]\r\n");
  strcat(text1, "    [-s shell] [-c working directory] [-n host name]\r\n");
  strcat(text1, "    [-sip]\r\n");
  strcat(text1, "    server-name-or-ip\r\n");
  strcat(text1, "\r\n");
  strcat(text1, "You can use a config file in the current directory\r\n");
  strcat(text1, "called WinRDesktop.ini\r\n");
  strcat(text1, "The file should look like this...\r\n");
  strcat(text1, "[main]\r\n");
  strcat(text1, "server=192.168.1.1\r\n");
  strcat(text1, "port=3389\r\n");
  strcat(text1, "username=user1\r\n");
  strcat(text1, "password=password1\r\n");
  strcat(text1, "bpp=16\r\n");
  strcat(text1, "geometry=800x600\r\n");
  strcat(text1, "fullscreen\r\n");
  strcat(text1, "clipboard\r\n");
  strcat(text1, "barcodescanner\r\n");
  strcat(text1, "nosip\r\n");
#ifdef WITH_DEBUG
  printf(text1);
#else /* WITH_DEBUG */
  str_to_uni(lcaption, "WinRDesktop");
  str_to_uni(textx, text1);
  MessageBox(g_Wnd, textx, lcaption, MB_OK);
#endif /* WITH_DEBUG */
}

#ifdef WITH_DEBUG
/*****************************************************************************/
int
main(int argc, char ** argv)
{
  WSADATA d;

  WSAStartup(MAKEWORD(2, 0), &d);
  if (!mi_process_cl(argv[0]))
  {
    mi_show_params();
    WSACleanup();
    return 0;
  }
  return ui_main();
}
#else /* WITH_DEBUG */
/*****************************************************************************/
int WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPTSTR lpCmdLine, int nCmdShow)
{
  WSADATA d;

  WSAStartup(MAKEWORD(2, 0), &d);
  if (!mi_process_cl(lpCmdLine))
  {
    mi_show_params();
    WSACleanup();
    return 0;
  }
  DEBUGMSG(1, (L"global vars:\r\nbpp=%i, usescanner=%i, workdir='%S', domain='%S', fullscreen=%i\
				\r\nheight=%i, width=%i, screenH=%i, screenW=%i, \
				\r\nhostname='%S', user='%S', pass='%S', shell='%S', port=%i\r\n",
	  g_server_depth,
	  g_busescanner,
	  g_directory,
	  g_domain,
	  g_fullscreen,
	  g_height,
	  g_width,
	  g_screen_height,
	  g_screen_width,
	  g_hostname,
	  g_username,
	  g_password,
	  g_shell,
	  g_tcp_port_rdp));
  hCurs1 = LoadCursor(NULL, IDC_WAIT);
  if(hCurs1!=INVALID_HANDLE_VALUE)
	SetCursor(hCurs1);

  ImmDisableIME(0);
       

  return ui_main();
}
#endif /* WITH_DEBUG */

/*****************************************************************************/
void
mi_begin_update(void)
{
	
}

/*****************************************************************************/
void
mi_end_update(void)
{
}

/*****************************************************************************/
void
mi_fill_rect(int x, int y, int cx, int cy, int colour)
{
  HBRUSH brush;
  RECT rect;
  HDC maindc;
  HRGN rgn;
  int red;
  int green;
  int blue;

  if (g_server_depth == 8)
  {
    red = (pal_entries[colour & 0xff] & 0xff0000) >> 16;
    green = (pal_entries[colour & 0xff] & 0xff00) >> 8;
    blue = pal_entries[colour & 0xff] & 0xff;
  }
  else if (g_server_depth == 15)
  {
    SPLIT_COLOUR15(colour, red, green, blue);
  }
  else if (g_server_depth == 16)
  {
    SPLIT_COLOUR16(colour, red, green, blue);
  }
  else
  {
    red = 0;
    green = 0;
    blue = 0;
  }
  maindc = GetWindowDC(g_Wnd);
  rgn = mi_clip(maindc);
  brush = CreateSolidBrush(RGB(red, green, blue));
  rect.left = x + g_xoff - g_xscroll;
  rect.top = y + g_yoff - g_yscroll;
  rect.right = rect.left + cx;
  rect.bottom = rect.top + cy;
  FillRect(maindc, &rect, brush);
  DeleteObject(brush);
  ReleaseDC(g_Wnd, maindc);
  DeleteObject(rgn);
}

/*****************************************************************************/
void
mi_line(int x1, int y1, int x2, int y2, int colour)
{
  HPEN pen;
  HDC maindc;
  HGDIOBJ save;
  HRGN rgn;
  int red;
  int green;
  int blue;

  if (g_server_depth == 8)
  {
    red = (pal_entries[colour & 0xff] & 0xff0000) >> 16;
    green = (pal_entries[colour & 0xff] & 0xff00) >> 8;
    blue = pal_entries[colour & 0xff] & 0xff;
  }
  else if (g_server_depth == 15)
  {
    SPLIT_COLOUR15(colour, red, green, blue);
  }
  else if (g_server_depth == 16)
  {
    SPLIT_COLOUR16(colour, red, green, blue);
  }
  else
  {
    red = 0;
    green = 0;
    blue = 0;
  }
  maindc = GetWindowDC(g_Wnd);
  rgn = mi_clip(maindc);
  pen = CreatePen(PS_SOLID, 0, RGB(red, green, blue));
  save = SelectObject(maindc, pen);
  MoveToEx(maindc, x1 + g_xoff - g_xscroll, y1 + g_yoff - g_yscroll, 0);
  LineTo(maindc, x2 + g_xoff - g_xscroll, y2 + g_yoff - g_yscroll);
  SelectObject(maindc, save);
  DeleteObject(pen);
  ReleaseDC(g_Wnd, maindc);
  DeleteObject(rgn);
}

/*****************************************************************************/
void
mi_screen_copy(int x, int y, int cx, int cy, int srcx, int srcy)
{
  RECT rect;
  RECT clip_rect;
  RECT draw_rect;
  HRGN rgn;
  int ok_to_ScrollWindowEx;

  /* WinCE can't scroll in 2 directions at once */
#ifdef MYWINCE
  ok_to_ScrollWindowEx = cx == 0 || cy == 0;
#else /* MYWINCE */
  ok_to_ScrollWindowEx = 1;
#endif /* MYWINCE */
  if (!ok_to_ScrollWindowEx)
  {
    rgn = CreateRectRgn(x - g_xscroll, y - g_yscroll,
                        (x - g_xscroll) + cx,
                        (y - g_yscroll) + cy);
    InvalidateRgn(g_Wnd, rgn, 0);
    DeleteObject(rgn);
  }
  else
  {
    /* this is all in client coords */
    rect.left = srcx - g_xscroll;
    rect.top = srcy - g_yscroll;
    rect.right = rect.left + cx;
    rect.bottom = rect.top + cy;
    clip_rect.left = g_clip_left - g_xscroll;
    clip_rect.top = g_clip_top - g_yscroll;
    clip_rect.right = g_clip_right - g_xscroll;
    clip_rect.bottom = g_clip_bottom - g_yscroll;
    if (IntersectRect(&draw_rect, &clip_rect, &g_wnd_clip))
    {
      rgn = CreateRectRgn(0, 0, 0, 0);
      ScrollWindowEx(g_Wnd, x - srcx, y - srcy, &rect, &draw_rect,
                     rgn, 0, SW_ERASE);
      InvalidateRgn(g_Wnd, rgn, 0);
      DeleteObject(rgn);
    }
  }
}

/*****************************************************************************/
void
mi_set_clip(int x, int y, int cx, int cy)
{
  g_clip_left = x;
  g_clip_top = y;
  g_clip_right = g_clip_left + cx;
  g_clip_bottom = g_clip_top + cy;
}

/*****************************************************************************/
void
mi_reset_clip(void)
{
  g_clip_left = 0;
  g_clip_top = 0;
  g_clip_right = g_clip_left + g_width;
  g_clip_bottom = g_clip_top + g_height;
}

/*****************************************************************************/
void *
mi_create_cursor(unsigned int x, unsigned int y,
                 int width, int height,
                 unsigned char * andmask, unsigned char * xormask)
{
#ifdef MYWINCE
  return (void *) 1;
#else /* MYWINCE */
  HCURSOR hCur;

  hCur = CreateCursor(g_Instance, x, y, width, height, andmask, xormask);
  if (hCur == 0)
  {
    hCur = LoadCursor(NULL, IDC_ARROW);
  }
  return hCur;
#endif /* MYWINCE */
}

/*****************************************************************************/
void
mi_destroy_cursor(void * cursor)
{
#ifdef MYWINCE
#else /* MYWINCE */
  if (g_cursor == cursor)
  {
    g_cursor = 0;
  }
  DestroyCursor(cursor);
#endif /* MYWINCE */
}

/*****************************************************************************/
void
mi_set_cursor(void * cursor)
{
#ifdef MYWINCE
#else /* MYWINCE */
  g_cursor = cursor;
  SetCursor(g_cursor);
#endif /* MYWINCE */
}

/*****************************************************************************/
void
mi_set_null_cursor(void)
{
}

