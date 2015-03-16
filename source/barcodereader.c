//barcodereader.cpp
#include <windows.h>

#include "barcodereader.h"

/*
	// Create a synchronization event for scanner thread synchronization.
	ghScannRead = CreateEvent(NULL, FALSE, FALSE, NULL);

	// Now create the read thread to monitor the scanner.
	ghScannerThread = CreateThread(NULL, NULL, ScanThread, HWND hWnd, 0, NULL);

case WM_HAVE_SCAN:
  // Display the data from the scan.
  hdc = GetDC(hWnd);
  GetClientRect(hWnd, &rt);
  swprintf(sbuffer,TEXT("\n\rPress 0 to exit\n\rPress 1 to toggle \n\rscan ahead \n\rData
    Received: \n\r%S"), gOneScan.rgbDataBuffer);
  DrawText(hdc, sbuffer, _tcslen(sbuffer), &rt, DT_LEFT);
  EndPaint(hWnd, &ps);
  Sleep(2000);
  // Note: Artificial delay added so we can see scan ahead work.
  SetEvent( ghScannRead);
  // Let scanning thread know we have consumed data.
  break;
case WM_CHAR:
  {
    char MvChar = (char)wParam;
    DWORD iTmp;
    if ( '0' == MyChar)
    {
      // Kill the thread first.
      bStillRunning = false;
      SetEvent( ghScannRead);
      // Bypass the wait for data. If it happens to hit, stop the scanner read.
      ITCSCAN_CancelRead( gh BarCodeReaderHandle, TRUE, &iTmp, &iTmp );
      Sleep(500):
      // Allow the thread an opportunity to run and shut down.
      PostMessage(ghWnd, WM_CLOSE, 0,0);
    }
    if ( '1' == MyChar)
    // Toggle scan ahead.
    {
      bScannAhead = !bScannAhead;
      if (bScannAhead)
        ITCSCAN_SetDataLED( ghBarCodeReaderHandle, FALSE);
      else
        ITCSCAN_SetDataLED( ghBarCodeReaderHandle, TRUE);
    }
}
break;

*/



