

//*****************************************************************************
// 2014. 06. 20
//
// 
//
// log���� ��������� �Լ�
//
//*****************************************************************************

#include "DXUT.H"




static char LogPath[MAX_PATH];
//-----------------------------------------------------------------------------
// Desc   : �������� ����   �����ڵ��  char*  [2014.10.30 ]
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

	GetLocalTime(&lpSystemTime); // ���� ��¥, �ð� �������� �Լ�

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
// �������� ����   ��Ƽ����Ʈ��  char*
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

	GetLocalTime(&lpSystemTime); // ���� ��¥, �ð� �������� �Լ�
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

	GetLocalTime(&lpSystemTime); // ���� ��¥, �ð� �������� �Լ�
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

	strcat_s(LogPath,"./log/mylog(");//���ϰ�θ� ����

	GetLocalTime(&lpSystemTime); // ���� ��¥, �ð� �������� �Լ�
	//sprintf(l_str,"%02d.%02d.%04d - %02d:%02d:%02d:%03d).txt",lpSystemTime.wDay,lpSystemTime.wMonth,lpSystemTime.wYear,lpSystemTime.wHour,lpSystemTime.wMinute,lpSystemTime.wSecond, lpSystemTime.wMilliseconds);
	sprintf_s(l_str,"%04d��%02d��%02d��  %02d��%02d��%02d��).txt",lpSystemTime.wYear, lpSystemTime.wMonth,lpSystemTime.wDay , lpSystemTime.wHour,   lpSystemTime.wMinute,  lpSystemTime.wSecond);
	strcat_s(LogPath,l_str);
	//�����̸��� 



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
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER   // �����ڵ� �ؼ� �Լ�
		| FORMAT_MESSAGE_FROM_SYSTEM
		| FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	msg = lpMsgBuf;
	LocalFree(lpMsgBuf);
	return msg;
}


//************************************************* �� **************************************************//