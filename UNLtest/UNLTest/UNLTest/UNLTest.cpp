// UNLTest.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "UNLTest.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

static char szdic[MAX_PATH] = "";
static char szBanglaFont[64] = "";
static char szer[MAX_PATH] = "";
static char szein[MAX_PATH] = "";
static char szeout[MAX_PATH] = "";
static char szdr[MAX_PATH] = "";
static char szdin[MAX_PATH] = "";
static char szdout[MAX_PATH] = "";
static char szenco[MAX_PATH] = "";
static char szdeco[MAX_PATH] = "";
static FILE *pfileUNL = NULL;
static FILE *pfileBangla = NULL;

CAtlHttpClient* ru_client;

INT_PTR CALLBACK StartupDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK ConvDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
void SetControlFont(HWND hDlg, int cid, char * lpszName, int height, int weight, int orientation);
BOOL mySetDlgItemText(HWND hDlg, LPCTSTR lpString, DWORD lang);

void InitApp(HINSTANCE hInstance)
{
	ru_client = new CAtlHttpClient;
	hInst = hInstance;
	char szFileName[MAX_PATH] = "";
	GetModuleFileName((HMODULE)hInstance, szFileName, MAX_PATH);
	char * pc = strrchr(szFileName, '\\');
	strcpy(&pc[1], "test.ini");
	GetPrivateProfileString("COMMON","DICFILE","",szdic,MAX_PATH, szFileName);
	GetPrivateProfileString("COMMON","BANGLAFONT","",szBanglaFont,MAX_PATH,szFileName);
	GetPrivateProfileString("ENCO","EXEFILE","",szenco,MAX_PATH,szFileName);
	GetPrivateProfileString("ENCO","RULEFILE","",szer,MAX_PATH,szFileName);
	GetPrivateProfileString("ENCO","INPUTFILE","",szein,MAX_PATH,szFileName);
	GetPrivateProfileString("ENCO","OUTPUTFILE","",szeout,MAX_PATH,szFileName);
	GetPrivateProfileString("DECO","EXEFILE","",szdeco,MAX_PATH,szFileName);
	GetPrivateProfileString("DECO","RULEFILE","",szdr,MAX_PATH,szFileName);
	GetPrivateProfileString("DECO","INPUTFILE","",szdin,MAX_PATH,szFileName);
	GetPrivateProfileString("DECO","OUTPUTFILE","",szdout,MAX_PATH,szFileName);
}

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
/*
char szBuf1[256] = "";
char szBuf[64] = "I eat rice & what about you[]123";
unsigned short xtype;
unsigned short xcount;
char sz_vcode[32] = "";
unsigned short xcls;
unsigned char xvcode[5];

int r = -1;

frm_dataConvertText((BYTE*)szBuf1,256, (BYTE*)szBuf);

MessageBox(NULL, szBuf1, "Test", MB_OK|MB_TASKMODAL);
return 0;


*/
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	InitApp(hInstance);
	DialogBox(hInst, MAKEINTRESOURCE(IDD_STARTUP), NULL, StartupDialog);
	DialogBox(hInst, MAKEINTRESOURCE(IDD_CONVDIALOG), NULL, ConvDialog);
	return 0;
	
 	// TODO: Place code here.
/*	MSG msg;
	HACCEL hAccelTable;
	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_UNLTEST, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_UNLTEST));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;*/
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_UNLTEST));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_UNLTEST);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK ConvDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	bool fdeco = false;
	char *pszunl = NULL;
	char *pszbangla = NULL;
	int l;
	FILE * pFile;
	switch (message)
	{
	case WM_INITDIALOG:
		CheckDlgButton(hDlg, IDC_RADENCO, BST_CHECKED);
		CheckDlgButton(hDlg, IDC_RADLANG_BN, BST_CHECKED);
		CheckDlgButton(hDlg, IDC_CHK_FOREIGN, BST_CHECKED);
		SetControlFont (hDlg, IDC_STATIC, NULL, 12, 700, 0) ;
		SetControlFont (hDlg, IDC_STATIC1, NULL, 12, 700, 0) ;
		SetControlFont (hDlg, IDC_STATIC2, NULL, 12, 700, 0) ;
		SetControlFont (hDlg, IDC_EDTUNL, "Courier New", 12, 400, 0) ;
		SetControlFont (hDlg, IDC_EDTBANGLA, szBanglaFont, 18, 400, 0) ;
		SetFocus(GetDlgItem(hDlg, IDC_EDTBANGLA));
	//	return (INT_PTR)TRUE;
		break;
		
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
			case IDC_CHK_FOREIGN:
				if(IsDlgButtonChecked(hDlg, IDC_CHK_FOREIGN) != BST_CHECKED)
				{
					EnableWindow(GetDlgItem(hDlg,IDC_RADENCO), TRUE);
					EnableWindow(GetDlgItem(hDlg,IDC_RADDECO), TRUE);
					CheckDlgButton(hDlg, IDC_RADLANG_BN, BST_CHECKED);
					CheckDlgButton(hDlg, IDC_RADLANG_EN, BST_UNCHECKED);
					SetControlFont (hDlg, IDC_EDTBANGLA, szBanglaFont, 18, 400, 0) ;
					EnableWindow(GetDlgItem(hDlg,IDC_RADLANG_BN), FALSE);
					EnableWindow(GetDlgItem(hDlg,IDC_RADLANG_EN), FALSE);
				}
				else
				{
					EnableWindow(GetDlgItem(hDlg,IDC_RADENCO), FALSE);
					EnableWindow(GetDlgItem(hDlg,IDC_RADDECO), FALSE);
					EnableWindow(GetDlgItem(hDlg,IDC_RADLANG_BN), TRUE);
					EnableWindow(GetDlgItem(hDlg,IDC_RADLANG_EN), TRUE);
					if(IsDlgButtonChecked(hDlg, IDC_RADLANG_BN) == BST_CHECKED)
						SetControlFont (hDlg, IDC_EDTBANGLA, szBanglaFont, 18, 400, 0) ;
					if(IsDlgButtonChecked(hDlg, IDC_RADLANG_EN) == BST_CHECKED)
						SetControlFont (hDlg, IDC_EDTBANGLA, "Times New Roman", 18, 400, 0) ;
						
				}
				SetFocus(GetDlgItem(hDlg, IDC_EDTBANGLA));
				break;
			case IDC_RADLANG_BN:
			{
				SetControlFont (hDlg, IDC_EDTBANGLA, szBanglaFont, 18, 400, 0) ;
				HWND hw = GetDlgItem(hDlg,IDC_EDTBANGLA);
				RECT rc;
				GetClientRect(hw, &rc);
				InvalidateRect(hw, &rc, false);
				UpdateWindow(hw);
				SetFocus(GetDlgItem(hDlg, IDC_EDTBANGLA));
				break;
			}
			case IDC_RADLANG_EN:
			{
				SetControlFont (hDlg, IDC_EDTBANGLA, "Times New Roman", 18, 400, 0) ;
				HWND hw = GetDlgItem(hDlg,IDC_EDTBANGLA);
				RECT rc;
				GetClientRect(hw, &rc);
				InvalidateRect(hw, &rc, true);
				UpdateWindow(hw);
				SetFocus(GetDlgItem(hDlg, IDC_EDTBANGLA));
				break;
			}
			case IDOK:
				ShowIpHelp();
				ShowIpHelpMore();
				break;
				if(IsDlgButtonChecked(hDlg, IDC_CHK_FOREIGN) == BST_CHECKED)
				{
					if(IsDlgButtonChecked(hDlg, IDC_RADLANG_BN) == BST_CHECKED)
					{
						l = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDTBANGLA));
						if(l > 0)
						{
							pszbangla = (char *)realloc(pszbangla,(size_t)l+1);
							GetDlgItemText(hDlg, IDC_EDTBANGLA, pszbangla, l+1);
							fopen_s(&pFile, szein, "w+b");
							fwrite(pszbangla, 1, l, pFile);
							fclose(pFile);
							//D:\UNLTEST\DeCoL27.exe "WD.dic" "DeRules.txt" "DeInput.txt" "DeOutput.txt" -go -w -l4
							_spawnl(_P_WAIT, szenco, szenco, szdic, szer, szein, szeout, "-go", "-l0", NULL);
							fopen_s(&pFile, szeout, "rb");
							l = _filelength(_fileno(pFile));
							pszunl = (char *)realloc(pszunl,(size_t)l+1);
							fread(pszunl, 1, l, pFile);
							fclose(pFile);
							pszunl[l] = '\0';
							char * psstart = strstr(pszunl, "{unl}");
							if(psstart != NULL)
							{
								char * pend = strstr(pszunl, "{/unl}");
								if(pend != NULL)
									pend[6] = '\0';
								
								SetDlgItemText(hDlg, IDC_EDTUNL, psstart);
								ru_set_data(psstart,"false", "en", "text", "windows-1251");
								
								char * res = ru_convert();
								if(res != NULL)
								{
									res[strlen(res)-2] = '\0';
									mySetDlgItemText(hDlg, &res[2], 'en');
									delete[] res;
								}
								fdeco = false;
							}
						}
						break;
					}
					if(IsDlgButtonChecked(hDlg, IDC_RADLANG_EN) == BST_CHECKED)
					{
						l = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDTBANGLA));
						if(l > 0)
						{
							pszbangla = (char *)realloc(pszbangla,(size_t)l+1);
							GetDlgItemText(hDlg, IDC_EDTBANGLA, pszbangla, l+1);
							ru_set_data(pszbangla,"true", "en", "text", "windows-1251");
							char * res = ru_convert();
							if(res != NULL)
							{
								char * psstart = strstr(res, "{unl}");
								if(psstart != NULL)
								{
									char * pend = strstr(res, "{/unl}");
									if(pend != NULL)
										pend[6] = '\0';
									SetDlgItemText(hDlg, IDC_EDTUNL, psstart);
									pFile = fopen(szdin, "w+b");
									fwrite(psstart, 1, strlen(psstart), pFile);
									fclose(pFile);
									delete[] res;
									_spawnl(_P_WAIT, szdeco, szdeco, szdic, szdr, szdin, szdout, "-go", "-l0", NULL);
									pFile = fopen(szdout, "rb");
									l = _filelength(fileno(pFile));
									pszbangla = (char *)realloc(pszbangla,(size_t)l+1);
									fread(pszbangla, 1, l, pFile);
									pszbangla[l] = '\0';
									fclose(pFile);
									char * pc = strstr(pszbangla, "\r\n;;");
									if(pc != NULL)
										pc[0] = '\0';
									mySetDlgItemText(hDlg, pszbangla,'bn');
								}
							}
						}
						break;
					}
				}
				else
				{
					if(IsDlgButtonChecked(hDlg, IDC_RADDECO) == BST_CHECKED)
					{
						l = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDTUNL));
						if(l > 0)
						{
							pszunl = (char *)realloc(pszunl,(size_t)l+1);
							GetDlgItemText(hDlg, IDC_EDTUNL, pszunl, l+1);
							pFile = fopen(szdin, "w+b");
							fwrite(pszunl, 1, l, pFile);
							fclose(pFile);
							//D:\UNLTEST\DeCoL27.exe "WD.dic" "DeRules.txt" "DeInput.txt" "DeOutput.txt" -go -w -l4
							_spawnl(_P_WAIT, szdeco, szdeco, szdic, szdr, szdin, szdout, "-go", "-l0", NULL);
							pFile = fopen(szdout, "rb");
							l = _filelength(fileno(pFile));
							pszbangla = (char *)realloc(pszbangla,(size_t)l+1);
							fread(pszbangla, 1, l, pFile);
							pszbangla[l] = '\0';
							fclose(pFile);
							char * pc = strstr(pszbangla, "\r\n;;");
							if(pc != NULL)
								pc[0] = '\0';
							SetDlgItemText(hDlg, IDC_EDTBANGLA, pszbangla);
							fdeco = true;
						}
					}
					else
					{
						l = GetWindowTextLength(GetDlgItem(hDlg, IDC_EDTBANGLA));
						if(l > 0)
						{
							pszbangla = (char *)realloc(pszbangla,(size_t)l+1);
							GetDlgItemText(hDlg, IDC_EDTBANGLA, pszbangla, l+1);
							fopen_s(&pFile, szein, "w+b");
							fwrite(pszbangla, 1, l, pFile);
							fclose(pFile);
							//D:\UNLTEST\DeCoL27.exe "WD.dic" "DeRules.txt" "DeInput.txt" "DeOutput.txt" -go -w -l4
							_spawnl(_P_WAIT, szenco, szenco, szdic, szer, szein, szeout, "-go", "-l0", NULL);
							fopen_s(&pFile, szeout, "rb");
							l = _filelength(_fileno(pFile));
							pszunl = (char *)realloc(pszunl,(size_t)l+1);
							fread(pszunl, 1, l, pFile);
							fclose(pFile);
							pszunl[l] = '\0';
							char * psstart = strstr(pszunl, "{unl}");
							if(psstart != NULL)
							{
								char * pend = strstr(pszunl, "{/unl}");
								if(pend != NULL)
									pend[6] = '\0';
								
								SetDlgItemText(hDlg, IDC_EDTUNL, psstart);
						/*		ru_set_data(psstart,"false", "en", "text", "utf-8");
								
								char * res = ru_convert();
								if(res != NULL)
								{
									mySetDlgItemText(hDlg, res, 'en');
									delete[] res;
								}*/
								fdeco = false;
							}
						}
					}
				}
				break;
			case IDCANCEL:
				if(ru_data != NULL)
					free(ru_data);
				if(ru_client != NULL)
					delete ru_client;
				if(pszunl != NULL)
					free(pszunl);
				if(pszbangla != NULL)
					free(pszbangla);
				EndDialog(hDlg, LOWORD(wParam));
				return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}

void SetControlFont(HWND hDlg, int cid, char * lpszName, int height, int weight, int orientation)
{
	HFONT hfont;
	char szFace[32] = "MS Sans Serif";
	HDC hdc;
	if(lpszName)
		strcpy_s(szFace, 32, lpszName);
	
	hdc = GetDC(hDlg);
	hfont = CreateFontA( (int)-MulDiv(height, GetDeviceCaps(hdc, LOGPIXELSY), 72), // logical height of font 
					(int) 0, // logical average character width 
					(int) orientation, // angle of escapement 
					(int) orientation, // base-line orientation angle 
					weight, // font weight 
					(DWORD) FALSE, // italic attribute flag 
					(DWORD) FALSE, // underline attribute flag 
					(DWORD) FALSE, // strikeout attribute flag 
					(DWORD) DEFAULT_CHARSET, // character set identifier 
					(DWORD) OUT_DEFAULT_PRECIS , // output precision 
					(DWORD) CLIP_DEFAULT_PRECIS , // clipping precision 
					(DWORD) DEFAULT_QUALITY , // output quality
					(DWORD) 0, // pitch and family 
					szFace // pointer to typeface name string 
					);

	SendDlgItemMessage(hDlg, cid, WM_SETFONT,
						(WPARAM)hfont,
						MAKELPARAM(FALSE, 0));
	ReleaseDC(hDlg, hdc);
	return;
}


BOOL mySetDlgItemText(	HWND hDlg,
					//	int nIDDlgItem,
						LPCTSTR lpString,
						DWORD lang
					)
{
	switch(lang)
	{
		case 'en':
			SetControlFont (hDlg, IDC_EDTBANGLA, "Times New Roman", 18, 400, 0) ;
			CheckDlgButton(hDlg, IDC_RADLANG_EN, BST_CHECKED);
			CheckDlgButton(hDlg, IDC_RADLANG_BN, BST_UNCHECKED);
			break;
		case 'bn':
			SetControlFont (hDlg, IDC_EDTBANGLA, szBanglaFont, 18, 400, 0) ;
			CheckDlgButton(hDlg, IDC_RADLANG_BN, BST_CHECKED);
			CheckDlgButton(hDlg, IDC_RADLANG_EN, BST_UNCHECKED);
			break;
	}
	return SetDlgItemText(hDlg, IDC_EDTBANGLA, lpString);
}

INT_PTR CALLBACK StartupDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		SetControlFont (hDlg, IDC_STATIC1, NULL, 14, 700, 0) ;
		SetControlFont (hDlg, IDC_STATIC2, NULL, 14, 700, 0) ;
		SetControlFont (hDlg, IDC_STATIC3, NULL, 14, 700, 0) ;
		SetTimer(hDlg, 1000, 10000, NULL);
		return (INT_PTR)FALSE;
	case WM_LBUTTONUP:
		KillTimer(hDlg, 1000);
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;
	case WM_TIMER:
		if(wParam == 1000)
		{
			KillTimer(hDlg, 1000);
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
	}
	return (INT_PTR)FALSE;
}