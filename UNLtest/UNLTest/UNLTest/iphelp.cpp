//
// Link with IPHlpAPI.lib
//

#include "stdafx.h"
//#include "UNLTest.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <stdio.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))


bool ShowIpHelp() {
   
   FIXED_INFO * FixedInfo;
   ULONG    ulOutBufLen;
   DWORD    dwRetVal;
   IP_ADDR_STRING * pIPAddr;

   char res[4096] = "";

   FixedInfo = (FIXED_INFO *) GlobalAlloc( GPTR, sizeof( FIXED_INFO ) );
   ulOutBufLen = sizeof( FIXED_INFO );
   
   if( ERROR_BUFFER_OVERFLOW == GetNetworkParams( FixedInfo, &ulOutBufLen ) ) {
      GlobalFree( FixedInfo );
      FixedInfo = (FIXED_INFO *) GlobalAlloc( GPTR, ulOutBufLen );
   }

   if ( dwRetVal = GetNetworkParams( FixedInfo, &ulOutBufLen ) ) {
        sprintf_s(res, 4096, "Call to GetNetworkParams failed. Return Value: %08x\r\n", dwRetVal );
        exit(1);
   }
   else {
      sprintf_s(&res[strlen(res)], 4096- strlen(res), "Host Name: %s\r\n", FixedInfo -> HostName );
      sprintf_s(&res[strlen(res)], 4096- strlen(res), "Domain Name: %s\r\n", FixedInfo -> DomainName );
      
      sprintf_s(&res[strlen(res)], 4096- strlen(res), "DNS Servers:\r\n" );
      sprintf_s(&res[strlen(res)], 4096- strlen(res), "\t%s\r\n", FixedInfo -> DnsServerList.IpAddress.String );
      
      pIPAddr = FixedInfo -> DnsServerList.Next;
      while ( pIPAddr ) {
         sprintf_s(&res[strlen(res)], 4096- strlen(res), "\t%s\r\n", pIPAddr ->IpAddress.String );
         pIPAddr = pIPAddr ->Next;
      }
   }
   MessageBoxA(NULL, res, "IPHELP", MB_OK|MB_TASKMODAL);
   return true;
}

// Global variables
ULONG		ulOutBufLen;
DWORD		dwRetVal;

void ShowIpHelpMore() {
	char szfile[_MAX_PATH] = "";
	GetModuleFileNameA(NULL, szfile, _MAX_PATH);
	char * p = strrchr(szfile, '\\');
	strcpy(&p[1], "IPHELPER.TXT");
	FILE * pfile = NULL;
	fopen_s(&pfile, szfile, "w+b");
	
	int i;
    
    /* Variables used by GetIpAddrTable */
    PMIB_IPADDRTABLE pIPAddrTable;
    DWORD dwSize = 0;
    DWORD dwRetVal = 0;
    IN_ADDR IPAddr;

    /* Variables used to return error message */
    LPVOID lpMsgBuf;

    // Before calling AddIPAddress we use GetIpAddrTable to get
    // an adapter to which we can add the IP.
    pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(sizeof (MIB_IPADDRTABLE));

    if (pIPAddrTable) {
        // Make an initial call to GetIpAddrTable to get the
        // necessary size into the dwSize variable
        if (GetIpAddrTable(pIPAddrTable, &dwSize, 0) ==
            ERROR_INSUFFICIENT_BUFFER) {
            FREE(pIPAddrTable);
            pIPAddrTable = (MIB_IPADDRTABLE *) MALLOC(dwSize);

        }
        if (pIPAddrTable == NULL) {
            fprintf(pfile, "Memory allocation failed for GetIpAddrTable\r\n");
            exit(1);
        }
    }
    // Make a second call to GetIpAddrTable to get the
    // actual data we want
    if ( (dwRetVal = GetIpAddrTable( pIPAddrTable, &dwSize, 0 )) != NO_ERROR ) { 
        fprintf(pfile, "GetIpAddrTable failed with error %d\r\n", dwRetVal);
        if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, dwRetVal, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),       // Default language
                          (LPTSTR) & lpMsgBuf, 0, NULL)) {
            fprintf(pfile, "\tError: %s", lpMsgBuf);
            LocalFree(lpMsgBuf);
        }
        exit(1);
    }

    fprintf(pfile, "\tNum Entries: %ld\r\n", pIPAddrTable->dwNumEntries);
    for (i=0; i < (int) pIPAddrTable->dwNumEntries; i++) {
        fprintf(pfile, "\r\n\tInterface Index[%d]:\t%ld\r\n", i, pIPAddrTable->table[i].dwIndex);
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwAddr;
        fprintf(pfile, "\tIP Address[%d]:     \t%s\r\n", i, inet_ntoa(IPAddr) );
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwMask;
        fprintf(pfile, "\tSubnet Mask[%d]:    \t%s\r\n", i, inet_ntoa(IPAddr) );
        IPAddr.S_un.S_addr = (u_long) pIPAddrTable->table[i].dwBCastAddr;
        fprintf(pfile, "\tBroadCast[%d]:      \t%s (%ld%)\r\n", i, inet_ntoa(IPAddr), pIPAddrTable->table[i].dwBCastAddr);
        fprintf(pfile, "\tReassembly size[%d]:\t%ld\r\n", i, pIPAddrTable->table[i].dwReasmSize);
        fprintf(pfile, "\tType and State[%d]:", i);
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_PRIMARY)
            fprintf(pfile, "\tPrimary IP Address");
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_DYNAMIC)
            fprintf(pfile, "\tDynamic IP Address");
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_DISCONNECTED)
            fprintf(pfile, "\tAddress is on disconnected interface");
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_DELETED)
            fprintf(pfile, "\tAddress is being deleted");
        if (pIPAddrTable->table[i].wType & MIB_IPADDR_TRANSIENT)
            fprintf(pfile, "\tTransient address");
        fprintf(pfile, "\r\n");
    }

    if (pIPAddrTable) {
        FREE(pIPAddrTable);
        pIPAddrTable = NULL;
    }
	fclose(pfile);
	return;
}
