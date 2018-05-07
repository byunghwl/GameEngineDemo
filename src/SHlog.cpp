

//*****************************************************************************
// 2014. 06. 20
//
// 
//
// log파일 남기기위한 함수
//
//*****************************************************************************

#include "DXUT.H"




static char LogPath[MAX_PATH];
//-----------------------------------------------------------------------------
// Desc   : 가변인자 적용   유니코드용  char*  [2014.10.30 ]
//-----------------------------------------------------------------------------

int LOG_WRITE_W(WCHAR* fmt, ...)
{
	WCHAR buf[1024] = {NULL,};
	va_list ap;


	FILE *LogFile;
	WCHAR l_str[512];
	SYSTEMTIME lpSystemTime;



	if ((LogFile = _fsopen( LogPath, "at",_SH_DENYNO )) == NULL)
	{
		return 1;
	}

	GetLocalTime(&lpSystemTime); // 현재 날짜, 시간 가져오는 함수

	swprintf_s(l_str, L"%02d.%02d.%04d - %02d:%02d:%02d:%03d : ",lpSystemTime.wDay,lpSystemTime.wMonth,lpSystemTime.wYear,lpSystemTime.wHour,lpSystemTime.wMinute,lpSystemTime.wSecond, lpSystemTime.wMilliseconds);

	wcscpy_s (buf, l_str);
	va_start(ap, fmt);
	int retval = StringCbVPrintfW(buf, sizeof(wchar_t) * 1024, fmt, ap); // retval == -2147024774
	va_end(ap);


	fputws( l_str, LogFile);
	fputws( buf, LogFile);
	fputws( L"\n", LogFile);
	fclose(LogFile);
	return(0);


}

//////////////////////////////////////////////////////////////////////////
// 가변인자 적용   멀티바이트용  char*
int LOG_WRITE_A(char* fmt, ...)
{
	char buf[1024] = {0,};
	va_list ap;


	FILE *LogFile;
	CHAR l_str[256];
	SYSTEMTIME lpSystemTime;

	

	if ((LogFile = _fsopen( LogPath, "at",_SH_DENYNO )) == NULL)
	{
		return 1;
	}

	GetLocalTime(&lpSystemTime); // 현재 날짜, 시간 가져오는 함수
	sprintf_s(l_str,"%02d.%02d.%04d - %02d:%02d:%02d:%03d : ",lpSystemTime.wDay,lpSystemTime.wMonth,lpSystemTime.wYear,lpSystemTime.wHour,lpSystemTime.wMinute,lpSystemTime.wSecond, lpSystemTime.wMilliseconds);

	strcpy_s (buf, l_str);
	va_start(ap, fmt);
	vsprintf_s(buf , fmt, ap);
	va_end(ap);


	fputs( l_str, LogFile);
	fputs( buf, LogFile);
	fputs( "\n", LogFile);
	fclose(LogFile);
	return(0);

	
}


//////////////////////////////////////////////////////////////////////////
int write_log_file(CHAR *wstring)
{
    FILE *LogFile;
    CHAR l_str[256];
    SYSTEMTIME lpSystemTime;



    if ((LogFile = _fsopen( LogPath,"at" ,_SH_DENYNO)) == NULL)
    {
            return(1);
    }

	GetLocalTime(&lpSystemTime); // 현재 날짜, 시간 가져오는 함수
	sprintf_s(l_str,"%02d.%02d.%04d - %02d:%02d:%02d:%03d : ",lpSystemTime.wDay,lpSystemTime.wMonth,lpSystemTime.wYear,lpSystemTime.wHour,lpSystemTime.wMinute,lpSystemTime.wSecond, lpSystemTime.wMilliseconds);


	fputs( l_str, LogFile);
	fputs( wstring, LogFile);
	fputs( "\n", LogFile);
	fclose(LogFile);
	return(0);
}

//////////////////////////////////////////////////////////////////////////

int create_log_file()
{
	 char l_str[256];
	 SYSTEMTIME lpSystemTime;
     FILE *LogFile = nullptr;

	_mkdir("./log");
// get current application path
	//_getcwd(LogPath, MAX_PATH );//Gets the current working directory.

// 
//     if(LogPath[strlen(LogPath) - 1] != '\\')
//        strcat(LogPath,"\\");

	strcat_s(LogPath,"./log/mylog(");//파일경로를 지정

	GetLocalTime(&lpSystemTime); // 현재 날짜, 시간 가져오는 함수
	//sprintf(l_str,"%02d.%02d.%04d - %02d:%02d:%02d:%03d).txt",lpSystemTime.wDay,lpSystemTime.wMonth,lpSystemTime.wYear,lpSystemTime.wHour,lpSystemTime.wMinute,lpSystemTime.wSecond, lpSystemTime.wMilliseconds);
	sprintf_s(l_str,"%04d년%02d월%02d일  %02d시%02d분%02d초).txt",lpSystemTime.wYear, lpSystemTime.wMonth,lpSystemTime.wDay , lpSystemTime.wHour,   lpSystemTime.wMinute,  lpSystemTime.wSecond);
	strcat_s(LogPath,l_str);
	//파일이름을 



	if ((LogFile = _fsopen( LogPath,"wt" ,_SH_DENYNO )) == nullptr)
	{
		 return(1);
	}

    fputs( "------------------------------ Log File Crate.... ", LogFile);
    fputs( " Log File ------------------------------\n", LogFile);
    fputs( "---------------------------------------------------------------------------------------------------\n", LogFile);

    fclose(LogFile);
    return(0);
}


//////////////////////////////////////////////////////////////////////////
CString PrintErrorMessage(DWORD err)
{
	LPTSTR lpMsgBuf;
	CString msg;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER   // 에러코드 해석 함수
		| FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	msg = lpMsgBuf;
	LocalFree(lpMsgBuf);
	return msg;
}


//************************************************* 끝 **************************************************//