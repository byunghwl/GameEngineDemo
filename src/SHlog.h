
//*****************************************************************************
// 2014. 06. 20
//
// 
//
// log���� ��������� �Լ�
//
//*****************************************************************************


#pragma once
#include <iostream>
#include <atlstr.h>

//�α����Ͽ� ���ڿ��� ���� �Լ�
int write_log_file(CHAR *wstring);

//�����۾� ���丮�� �α� �ؽ�Ʈ ���� ������ִ� �Լ�
int create_log_file();

//////////////////////////////////////////////////////////////////////////
// �����۾� ���丮�� �α� �ؽ�Ʈ ���� ������ִ� �Լ�
// �������� ����
int LOG_WRITE_A(char* fmt, ...);


//-----------------------------------------------------------------------------
// Desc   : �������� ����   �����ڵ��  char*  [2014.10.30 ]
//-----------------------------------------------------------------------------
int LOG_WRITE_W(WCHAR* fmt, ...);


CString PrintErrorMessage(DWORD err);

//************************************ �� ***************************************