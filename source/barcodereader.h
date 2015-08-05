//barcodereader.h

#ifndef INCLUDED_BARCODESCAN_H                /* Has this file been included before?  */
#define INCLUDED_BARCODESCAN_H                /* No, remember it has been now         */

#include <windows.h>
//#include <winbase.h>

//see C:\Program Files\Intermec\Developer Library\Include
//and C:\Program Files\Intermec\Developer Library\Lib

//#include "itcScan.h"
//#pragma comment (lib, "ITCscan.lib")
//5. aug 2015: changing to dynamic loading
extern BOOL g_busescanner;
#define ITCSCANDLL L"itcScan.dll"
HMODULE hLib=NULL;
typedef struct
{
   BYTE  *rgbDataBuffer;
   DWORD dwDataBufferSize;
   DWORD dwBytesReturned;
   DWORD   dwTimeout;
   UINT32   iSymbology;
   UINT32   iDataType;
   
}READ_DATA_STRUCT;
typedef HRESULT (*PFN_ITCSCAN_Open)(INT32 *pHandle,LPCTSTR pszDeviceName);
typedef HRESULT (*PFN_ITCSCAN_Close)(INT32 pHandle);
typedef HRESULT (*PFN_ITCSCAN_SyncRead)(INT32 pHandle, READ_DATA_STRUCT * pReadDataBlock);
typedef HRESULT (*PFN_ITCSCAN_SetScannerEnable)(INT32 pHandle,INT32 OnOff);
PFN_ITCSCAN_Open ITCSCAN_Open=NULL;
PFN_ITCSCAN_Close ITCSCAN_Close=NULL;
PFN_ITCSCAN_SyncRead ITCSCAN_SyncRead=NULL;
PFN_ITCSCAN_SetScannerEnable ITCSCAN_SetScannerEnable=NULL;

static CRITICAL_SECTION cs;

#define WM_HAVE_SCAN WM_USER + 301

static BOOL bStillRunning = TRUE;          // Flag to stop scanning
static INT32 ghBarCodeReaderHandle = 0; // Handle to scanner instance
static READ_DATA_STRUCT gOneScan;          // Structure to receive scanned data
static BYTE gScanDataBuffer[1024];          // Buffer to hold scanned data
static BOOL bScannAhead = TRUE;            // Flag to turn on artificial
                                    // "no scan ahead" by disabling
                                    // the scanner after data is available
static HANDLE ghScannRead = NULL;          // Handle for an unnamed event
                                    // that allows synchronizing, not
                                    // scanning until data consumed
//static HWND ghWnd;                         // Copy of main Windows handle made
                                    // available to all functions
extern HWND g_Wnd;	//see win32.c

static HANDLE ghScannerThread = 0;      // Handle for the thread that
                                    // reads and manages the scanner
TCHAR sbuffer[128];

BOOL LoadFunctions(){
	if(!g_busescanner){
		DEBUGMSG(1,(L"LoadFunctions(): g_busescanner is FALSE\n"));
		return FALSE;
	}
	hLib=LoadLibrary(ITCSCANDLL);
	if(hLib==NULL){
		DEBUGMSG(1,(L"LoadFunctions(): LoadLibrary(itcscan.dll) FAILED\n"));
		return FALSE;
	}
	ITCSCAN_Open=(PFN_ITCSCAN_Open)GetProcAddress(hLib, L"ITCSCAN_Open");
	ITCSCAN_Close=(PFN_ITCSCAN_Close)GetProcAddress(hLib, L"ITCSCAN_Close");
	ITCSCAN_SyncRead=(PFN_ITCSCAN_SyncRead)GetProcAddress(hLib, L"ITCSCAN_SyncRead");
	ITCSCAN_SetScannerEnable=(PFN_ITCSCAN_SetScannerEnable)GetProcAddress(hLib, L"ITCSCAN_SetScannerEnable");
	if(ITCSCAN_Open!=NULL && ITCSCAN_Close!=NULL && ITCSCAN_SyncRead!=NULL && ITCSCAN_SetScannerEnable!=NULL)
		return TRUE;
	return FALSE;
}

void unLoadFunctions(){
	if(hLib!=NULL){
		ITCSCAN_Open=NULL;
		ITCSCAN_Close=NULL;
		ITCSCAN_SyncRead=NULL;
		ITCSCAN_SetScannerEnable=NULL;
		FreeLibrary(hLib);
	}
}

DWORD WINAPI ScanThread( LPVOID lpParameter )
{
	//local copy
	HWND hWnd = g_Wnd;//(HWND)lpParameter;
	if(!LoadFunctions())
		return -2;
	DEBUGMSG(1,(L"ScanThread()starting...\n"));
  // Initialize the buffer.
  gOneScan.rgbDataBuffer = gScanDataBuffer;
  gOneScan.dwDataBufferSize = sizeof(gScanDataBuffer);
  gOneScan.dwTimeout = INFINITE;
  // Open the scanner.
  if ( SUCCEEDED( ITCSCAN_Open( &ghBarCodeReaderHandle, TEXT("default") ) ) )
  {
    while ( bStillRunning )
    {
      if ( SUCCEEDED( ITCSCAN_SyncRead( ghBarCodeReaderHandle,&gOneScan) ) )
      {
        // If blocking scan ahead, disable the scanner until this one is processed.
        if (!bScannAhead)
          ITCSCAN_SetScannerEnable( ghBarCodeReaderHandle, FALSE);
        // Notify main thread that scan data is ready.
        PostMessage( hWnd, WM_HAVE_SCAN, 0, 0);
        // Wait until scan data is processed. Some delay is made in this example
        // to demonstrate scan ahead.
        WaitForSingleObject( ghScannRead, INFINITE );
        // Re-enable scanner in case scan ahead was disabled.
        ITCSCAN_SetScannerEnable( ghBarCodeReaderHandle, TRUE);
      }
    }
    ITCSCAN_Close( ghBarCodeReaderHandle);
  }
  else
  {
    // Put up message to user and exit the program.
    MessageBox(NULL, _T("ITCSCAN_Open Failed"),NULL, MB_OK);
	DEBUGMSG(1,(L"ITCSCAN_Open Failed\n"));
	unLoadFunctions();
    return 0;
  }
  unLoadFunctions();
	DEBUGMSG(1,(L"ScanThread()ended\n"));
  return 1; // Normal return path.
}

#endif
