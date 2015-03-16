//barcodereader.h

#ifndef INCLUDED_BARCODESCAN_H                /* Has this file been included before?  */
#define INCLUDED_BARCODESCAN_H                /* No, remember it has been now         */

#include <windows.h>
//#include <winbase.h>

//see C:\Program Files\Intermec\Developer Library\Include
//and C:\Program Files\Intermec\Developer Library\Lib
#include "itcScan.h"
#pragma comment (lib, "ITCscan.lib")

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
 
DWORD WINAPI ScanThread( LPVOID lpParameter )
{
	//local copy
	HWND hWnd = g_Wnd;//(HWND)lpParameter;

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
    return 0;
  }
  return 1; // Normal return path.
}

#endif
