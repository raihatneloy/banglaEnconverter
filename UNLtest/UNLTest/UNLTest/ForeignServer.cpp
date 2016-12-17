
#include "stdafx.h"
#include "UNLTest.h"

static char ru_conv[6] = "true";//false
static char ru_lang[3] = "en";//ru
char *ru_data = NULL;
static char ru_mode[5] = "html";//text
static char ru_encoding[15] = "utf-8";//windows-1251

static CAtlNavigateData ru_nav_data;

CAtlHttpClient* ru_client = NULL;

bool frm_dataConvertText(unsigned char * pszDst, int nDstSize, unsigned char * pszSrc)
{
	int nSrcSize = (int)strlen((char *)pszSrc);
	char szHex[16] = "";
	
	int i = 0, j = 0;
	for(; i < nSrcSize; i++)
	{
		if(j >= nDstSize)
			return false;
		_itoa(pszSrc[i], szHex, 16);
		_strupr(szHex);
		if(strlen(szHex) < 2)
		{
			szHex[1] = szHex[0];
			szHex[0] = '0';
			szHex[2] = '\0';
		}
		switch(pszSrc[i])
		{
			case 0x20:
				if(j < nDstSize)
					pszDst[j++] = '+';
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
			case 'A':
			case 'B':
			case 'C':
			case 'D':
			case 'E':
			case 'F':
			case 'G':
			case 'H':
			case 'I':
			case 'J':
			case 'K':
			case 'L':
			case 'M':
			case 'N':
			case 'O':
			case 'P':
			case 'Q':
			case 'R':
			case 'S':
			case 'T':
			case 'U':
			case 'V':
			case 'W':
			case 'X':
			case 'Y':
			case 'Z':
			case 'a':
			case 'b':
			case 'c':
			case 'd':
			case 'e':
			case 'f':
			case 'g':
			case 'h':
			case 'i':
			case 'j':
			case 'k':
			case 'l':
			case 'm':
			case 'n':
			case 'o':
			case 'p':
			case 'q':
			case 'r':
			case 's':
			case 't':
			case 'u':
			case 'v':
			case 'w':
			case 'x':
			case 'y':
			case 'z':
		//	case 0x0D:
		//	case 0x0A:
				if(j < nDstSize)
					pszDst[j++] = pszSrc[i];
				break;
			default:
			
				if(j < nDstSize)
					pszDst[j++] = '%';
				if(j < nDstSize)
					pszDst[j++] = szHex[0];
				if(j < nDstSize)
					pszDst[j++] = szHex[1];
				break;
		}
	}
	pszDst[j] = '\0';
	return true;
}

bool ru_set_data(char * pdata, char * pconv, char * plang, char * pmode, char * pencoding)
{
	char * c_data = new char[strlen(pdata)*2];
	frm_dataConvertText((unsigned char *)c_data, strlen(pdata)*2, (unsigned char *)pdata);
	int datBufSize = strlen(c_data)+176;
	char * ru_data1 = (char *)realloc(ru_data, datBufSize);
	if(ru_data1 == NULL)
		return false;
	ru_data = ru_data1;
	sprintf_s(ru_data, datBufSize, 
	"DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=%s&language=%s&data=%s&outputmode=%s&coding=%s",
	pconv, plang, c_data, pmode, pencoding);
//	MessageBox(NULL,ru_data,NULL,MB_OK|MB_TASKMODAL);
	delete[] c_data;
	ru_nav_data.SetPostData((BYTE *)ru_data, strlen(ru_data)+1, "application/x-www-form-urlencoded");
	ru_nav_data.SetMethod(ATL_HTTP_METHOD_POST);
	return true;
}

char * ru_convert()
{
	if (ru_client->Navigate("http://www.unl.ru//unl-cgi//deco.exe//html", &ru_nav_data))
	{
	//	DWORD timeEnd = timeGetTime();

		DWORD size;
		
		size = ru_client->GetBodyLength();
		BYTE* buf = new BYTE[size+1];
		
		memcpy( buf, ru_client->GetBody(), size );
		buf[size] = '\0';
	//	MessageBox(NULL,(char *)buf,NULL,MB_OK|MB_TASKMODAL);
		return (char *)buf;
	}
	else
	{
	return NULL;
//	SetDlgItemText(hDlg, IDC_EDTUNL, "request failed - status code ");
	//	_tcout << _T("request failed - status code ") << client.GetStatus() << endl;
	//	_tcout << _T("(See HttpPing.ini for advanced options)") << endl;
	}
}
  BOOL  bResults = FALSE;
  HINTERNET  hSession = NULL, 
             hConnect = NULL,
             hRequest = NULL;

bool ru_set_data1(char * pdata, char * pconv, char * plang, char * pmode, char * pencoding)
{
	char * c_data = new char[strlen(pdata)*2];
	frm_dataConvertText((unsigned char *)c_data, strlen(pdata)*2, (unsigned char *)pdata);
	int datBufSize = strlen(c_data)+176;
	char * ru_data1 = (char *)realloc(ru_data, datBufSize);
	if(ru_data1 == NULL)
		return false;
	ru_data = ru_data1;
	sprintf_s(ru_data, datBufSize, 
	"DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=%s&language=%s&data=%s&outputmode=%s&coding=%s",
	pconv, plang, c_data, pmode, pencoding);
//	MessageBox(NULL,ru_data,NULL,MB_OK|MB_TASKMODAL);
	delete[] c_data;












	hSession = WinHttpOpen( "WinHTTP Example/1.0",  
                          WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
                          WINHTTP_NO_PROXY_NAME, 
                          WINHTTP_NO_PROXY_BYPASS, 0 );
	if( hSession == NULL)
		return false;
	hConnect = WinHttpConnect( hSession, "http://www.unl.ru//unl-cgi//cgiunl.exe//html",
                               INTERNET_DEFAULT_HTTPS_PORT, 0 );
	if( hConnect == NULL)
	{
		WinHttpCloseHandle( hSession );
		return false;
	}
	hRequest = WinHttpOpenRequest( hConnect, L"POST", NULL,
                                   NULL, WINHTTP_NO_REFERER, 
                                   WINHTTP_DEFAULT_ACCEPT_TYPES, 
                                   0);
	if( hRequest == NULL)
	{
		WinHttpCloseHandle( hConnect );
		WinHttpCloseHandle( hSession );
		return false;
	}
	bResults = WinHttpSendRequest( hRequest,
                                   WINHTTP_NO_ADDITIONAL_HEADERS, 0,
                                   ru_data, datBufSize, 
                                   datBufSize, 0 );


	if(!bResults)
	{
		WinHttpCloseHandle( hRequest );
		WinHttpCloseHandle( hConnect );
		WinHttpCloseHandle( hSession );
		return false;
	}
	bResults = WinHttpReceiveResponse( hRequest, NULL );
	if(!bResults)
	{
		WinHttpCloseHandle( hRequest );
		WinHttpCloseHandle( hConnect );
		WinHttpCloseHandle( hSession );
		return false;
	}







	char * c_data = new char[strlen(pdata)*2];
	frm_dataConvertText((unsigned char *)c_data, strlen(pdata)*2, (unsigned char *)pdata);
	int datBufSize = strlen(c_data)+176;
	char * ru_data1 = (char *)realloc(ru_data, datBufSize);
	if(ru_data1 == NULL)
		return false;
	ru_data = ru_data1;
	sprintf_s(ru_data, datBufSize, 
	"DOMAIN=SPORT&password=guest&TAGERROR=NO&username=UNL_guest&conversion=%s&language=%s&data=%s&outputmode=%s&coding=%s",
	pconv, plang, c_data, pmode, pencoding);
//	MessageBox(NULL,ru_data,NULL,MB_OK|MB_TASKMODAL);
	delete[] c_data;
	ru_nav_data.SetPostData((BYTE *)ru_data, strlen(ru_data)+1, "application/x-www-form-urlencoded");
	ru_nav_data.SetMethod(ATL_HTTP_METHOD_POST);
	return true;
}