#pragma once

#include "resource.h"


extern char *ru_data;
//extern CAtlHttpClient* ru_client;
bool frm_dataConvertText(unsigned char * pszDst, int nDstSize, unsigned char * pszSrc);
bool ru_set_data(char * pdata, char * pconv, char * plang, char * pmode, char * pencoding);
char * ru_convert();
bool ShowIpHelp();
void ShowIpHelpMore();