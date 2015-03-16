// rdp_crypt.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"

TCHAR* g_szPassDecrypted=NULL;
TCHAR* g_szPassEncrypted=NULL;

int encryptPWD2Byte(TCHAR* szPass, BYTE* szReturn, DWORD* dSize);
int decryptPWDfromByte(BYTE* pPassBytes, DWORD nBytes, TCHAR* szReturn);

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch(ul_reason_for_call){
		case DLL_PROCESS_ATTACH:
			g_szPassDecrypted=(TCHAR*)malloc(2048);
			memset(g_szPassDecrypted,0, 2048);
			g_szPassEncrypted=(TCHAR*)malloc(2048);
			memset(g_szPassEncrypted,0, 2048);
			return TRUE;
			break;
		case DLL_THREAD_ATTACH:
			return TRUE;
		case DLL_PROCESS_DETACH:
			free(g_szPassDecrypted);
			free(g_szPassEncrypted);
			return TRUE;
			break;
		case DLL_THREAD_DETACH:
			return TRUE;
		default:
			return FALSE;
	}
}

//decrypt a given encrypted pass string in HEX format to clear text string
//dwSizeOut gives the length of the returned string
int decryptPassword(TCHAR* szPassEncryptedHEX, TCHAR* szPassDecrypted, DWORD* dwSizeOut){
	int iRet=0;
	DWORD nBytes=wcslen(szPassEncryptedHEX);
	BYTE* pByte = new BYTE[1024];
	TCHAR* szTemp=new TCHAR[1024];

	if(hexWToBin(szPassEncryptedHEX, pByte)==0){
		decryptPWDfromByte(pByte, nBytes, szTemp);
		wcsncpy(g_szPassDecrypted, szTemp, wcslen(szTemp));
		wcsncpy(szPassDecrypted, szTemp, wcslen(szTemp));
		*dwSizeOut=wcslen(szPassDecrypted);
	}
	else
		iRet=-1;

	delete(szTemp);
	delete(pByte);
	return iRet;
}

//encrypt a given password (szPass) and return result in szReturn
//dSize will get the lenghth of the returned string
int encryptPassword(TCHAR* szPass, TCHAR* szReturn, DWORD* dSize){
	int iRet=0;
	memset(szReturn,0,*dSize);
	DWORD dwSize=1024;
	BYTE* pByte=(BYTE*)malloc(1024);
	memset (pByte,0,1024);
	iRet=encryptPWD2Byte(szPass, pByte, &dwSize);
	
	TCHAR *szHex=NULL;
	szHex = new TCHAR[dwSize*2];
	binToHexW(pByte, dwSize, szHex);

	*dSize=wcslen(szHex);
	wcsncpy(g_szPassEncrypted, szHex, wcslen(szHex));
	wcsncpy(szReturn, szHex, wcslen(szHex));
	delete(szHex);
	free(pByte);
	return iRet;
}

// take a password string and get a binary array with the encrypted password
int encryptPWD2Byte(TCHAR* szPass, BYTE* szReturn, DWORD* dSize){
	DEBUGMSG(1, (L"###### Entering encryptPWD ######\n"));
	DWORD nLen = wcslen(szPass);

	DATA_BLOB blobIn, blobOut;
	blobIn.cbData = (nLen+1)*sizeof(WCHAR);
	blobIn.pbData = (PBYTE )szPass;
	blobOut.cbData = 0;
	blobOut.pbData = NULL;
	if (!CryptProtectData(&blobIn, L"rdp", NULL, NULL, NULL, CRYPTPROTECT_UI_FORBIDDEN, &blobOut))
	{
		DEBUGMSG(1, (L"Password encryption failed\n"));
		return FALSE;
	}

	ASSERT(blobOut.pbData);

	*dSize=blobOut.cbData;
	memcpy(szReturn, blobOut.pbData, blobOut.cbData);
	
#ifdef DEBUG
	for(UINT k=0; k<blobOut.cbData; k++){
		DEBUGMSG(1, (L"%02x", blobOut.pbData[k]));
	}
	DEBUGMSG(1, (L"\n"));
#endif
	
	LocalFree(blobOut.pbData);
	DEBUGMSG(1, (L"###### Leaving encryptPWD ######\n"));
	return 0;
}

// take a binary array with encrypted password and decrypt it
int decryptPWDfromByte(BYTE* pPassBytes, DWORD nBytes, TCHAR* szReturn){
	DEBUGMSG(1, (L"###### Entering decryptPWD ######\n"));
	
	DATA_BLOB blobIn, blobOut;

	blobIn.cbData = nBytes;
	blobIn.pbData = pPassBytes; 

	blobOut.cbData = 0;
	blobOut.pbData = NULL;
//	LPWSTR pszDescr = L"rdp";
	if (!CryptUnprotectData(&blobIn, NULL, NULL, NULL, NULL, CRYPTPROTECT_UI_FORBIDDEN, &blobOut))
	{
		DEBUGMSG(1, (L"Password decryption failed: %i\n", GetLastError()));
		//ERROR_INVALID_DATA               13L
		return FALSE;
	}

	ASSERT(blobOut.pbData);

	DEBUGMSG(1, ((LPWSTR)blobOut.pbData));
	DEBUGMSG(1, (L"\n"));

	wcscpy(szReturn, (LPWSTR)blobOut.pbData);
	LocalFree(blobOut.pbData);
	DEBUGMSG(1, (L"###### Leaving decryptPWD ######\n"));
	return 0;
}
