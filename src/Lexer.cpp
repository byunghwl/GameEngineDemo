#include "DXUT.H"
#include "Lexer.h"

ILexer::ILexer() : 
	m_File(INVALID_HANDLE_VALUE),
	m_dwFileSize(0),
	m_NowProgress(0),
	m_Pointer(0),
	m_NowLoaded(0),
	m_dwBufferSize(0),
	m_pBuffer(NULL)
{
}

ILexer::~ILexer()
{
	Close();
}

char ILexer::GetNextChar()
{
	// ------------------------------------------------------------
	// CASE) Buffer�� ���� �پ����� �ٽ� File�� ���� Buffer��ŭ�� �о� ���δ�.
	// ------------------------------------------------------------
	if(m_Pointer == m_NowLoaded)
	{
		// Declare) 
		BOOL	result;

		// 1) Read�Ѵ�.
		result	 = ReadFile(m_File, m_pBuffer, m_dwBufferSize, &m_NowLoaded, NULL);

		// Check) 
		if(result==FALSE) return 0;
		// Check) 
		if(m_NowLoaded==0) return 0;

		// 2) Reset�Ѵ�.
		m_Pointer	 = 0;
	}


	// ------------------------------------------------------------
	// Buffer�� ���� �ʾ��� ��� �׳� ���� ���ڸ� ����������.
	// ------------------------------------------------------------
	// ����...���...
	++m_NowProgress;


	// Return) ���� ���ڸ� �����ش�.
	return	m_pBuffer[m_Pointer++];
}

bool ILexer::Open(LPCWSTR p_FileName, DWORD p_dwBufferSize)
{
	// ------------------------------------------------------------
	// Check) 
	// ------------------------------------------------------------
	// 1) FileName String�� NULL�̸� �׳� ����������.
	if(p_FileName==NULL)return FALSE;

	// 2) Buffer�� Size�� 0�� ����������.
	if(p_dwBufferSize==0)return FALSE;


	// ------------------------------------------------------------
	// Declare) 
	// ------------------------------------------------------------
	// 1) Buffer
	HANDLE	hFile		 = INVALID_HANDLE_VALUE;
	void*	pBuffer		 = NULL;
	DWORD	dwFileSize;

	try
	{
		// ------------------------------------------------------------
		// 1. File Open�ϱ�.
		// ------------------------------------------------------------
		// 1) File�� Open�Ѵ�.
		hFile	= CreateFile(p_FileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		// Check) ������ ���� ���ߴ�.
		if(hFile == INVALID_HANDLE_VALUE)return FALSE;

		// 2) �⺻���� �������� �����Ѵ�.
		dwFileSize	 = GetFileSize(m_File, NULL);


		// ------------------------------------------------------------
		// 2. �ʱ�ȭ �� �غ�
		// ------------------------------------------------------------
		// 1) Buffer Size ����.
		pBuffer		 = (char*)malloc(p_dwBufferSize);


		// ------------------------------------------------------------
		// 2. �ʱ�ȭ �� �غ�
		// ------------------------------------------------------------
		// 1) File ������  �����Ѵ�.
		m_File			 = hFile;
		m_dwFileSize	 = dwFileSize;

		// 2) Buffer������ �����Ѵ�.
		m_pBuffer		 = (char*)pBuffer;
		m_dwBufferSize	 = p_dwBufferSize;

		// 3) �ʱ�ȭ
		Reset();
	}

	// ------------------------------------------------------------
	// Exception)
	// ------------------------------------------------------------
	catch(...)
	{
		// 1) File�ݱ�.
		if(hFile!=INVALID_HANDLE_VALUE){	 CloseHandle(hFile);	hFile=INVALID_HANDLE_VALUE;	}
		

		// 2) Buffer �����.
		if(pBuffer){ free(pBuffer); pBuffer=NULL;}

		// Return) ����....
		return	FALSE;
	}


	// Return) ����!!!
	return	TRUE;
}

void ILexer::Close()
{
	// ------------------------------------------------------------
	// Check) 
	// ------------------------------------------------------------
	// 1) ������ ���� ���������� �˻��Ͽ� �������°� �ƴ϶�� �׳� ����������.
	if(m_File == INVALID_HANDLE_VALUE) return;

	// ------------------------------------------------------------
	// 1. ��� �ݴ´�.
	// ------------------------------------------------------------
	// 1) Buffer�� ����������.
	
	if(m_pBuffer){ free(m_pBuffer); m_pBuffer=NULL;}
	m_dwBufferSize	 = 0;

	// 2) File�� �ݴ´�.
	if(m_File!=INVALID_HANDLE_VALUE){	 CloseHandle(m_File);	m_File=INVALID_HANDLE_VALUE;	}
}

bool ILexer::Reset()
{
	// ------------------------------------------------------------
	// Check) 
	// ------------------------------------------------------------
	// 1) ������ ���� ���������� �˻��Ѵ�.
	if(m_File == INVALID_HANDLE_VALUE)return FALSE;


	// ------------------------------------------------------------
	// 1. ������ �ʱ���·� ������.
	// ------------------------------------------------------------
	// 1) �⺻ ������ Reset�Ѵ�.
	m_NowProgress	 = 0;
	m_Pointer		 = 0;
	m_NowLoaded		 = 0;


	// ------------------------------------------------------------
	// 2. ������ �ʱ���·� ������.
	// ------------------------------------------------------------
	// Declare) 
	DWORD	dwResult;

	// 1) File�� Pointer�� ���� ó������ �ǵ�����.
	dwResult	 = SetFilePointer(m_File, 0, 0, FILE_BEGIN);

	// Check) �б⿡ ���������� FALSE�� Return�Ѵ�.
	if(dwResult==INVALID_SET_FILE_POINTER)return FALSE;


	// Return) ����!!
	return	TRUE;
}

/****************************************************** End *****************************************************/