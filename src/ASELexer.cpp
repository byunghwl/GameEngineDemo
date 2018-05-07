#include "DXUT.H"
#include "ASELexer.h"


CLexer::CLexer(LPCWSTR p_FileName, DWORD p_SizeOfBuffer)
{
	// 1) Open�Ѵ�.
	if(p_FileName!=NULL)
	{
		if(Open(p_FileName, p_SizeOfBuffer) == FALSE)
		{LOG_WRITE_A("%s : ���Ͽ��� ���� ! ",p_FileName);     assert(FALSE);}
	}
}

CLexer::~CLexer()
{

}

//Lexical Analysis
int CLexer::GetToken(LPSTR p_TokenString)
{
	// ------------------------------------------------------------
	// 1. ���� �������� ����ã��
	// ------------------------------------------------------------
	// - ���ʿ� �����̳� Tab�̳� �����̳� ĳ�������� ���� ���ڿ��� ������ �����Ѵ�.
	CHAR	Char	 = GetNextChar();
	while(Char == ' ' || Char == '\t' || Char == '\r' || Char == '\n')
	{
		Char	 = GetNextChar();

		// Check) �������� Ȯ���Ѵ�.
		if(!Char)return TOKEND_END;
	}


	// ------------------------------------------------------------
	// 2. BLOCK Ȯ��.
	// ------------------------------------------------------------
	// 1) '{'�ΰ��� Ȯ���Ѵ�.
	if(Char == '{')
	{
		p_TokenString[0]	 = '{';
		p_TokenString[1]	 = NULL;

		return	TOKEND_BLOCK_START;
	}

	// 2) '}'�ΰ��� Ȯ���Ѵ�.
	else if(Char == '}')
	{
		p_TokenString[0]	 = '}';
		p_TokenString[1]	 = NULL;

		return	TOKEND_BLOCK_END;
	}


	// ------------------------------------------------------------
	// 3. Reserved Word
	// ------------------------------------------------------------
	// - '*'�ΰ��� Ȯ���Ѵ�.
	//   ���� '*'�� �����ϴ� ���� Reserved word�̹Ƿ� �˻��Ѵ�!
	else if(Char == '*')
	{
		int		p_StringPoint	 = 0;
		while(Char != ' ' && Char != '\t' && Char != '\r' && Char != '\n' && Char != '"' && Char != '{' && Char != '}')
		{
			p_TokenString[p_StringPoint]	 = Char;

			Char	 = GetNextChar();

			// Check) �������� Ȯ���Ѵ�.
			if(!Char)return TOKEND_END;

			++p_StringPoint;
		}
		p_TokenString[p_StringPoint]		 = NULL;

		for(int i=0; i<TOKERN_MAX; ++i)
		{
			if(!strcmp(p_TokenString, Token[i]))
			{
				// �´� Token�� ã�Ҵ�! �� ��ȣ�� return�����ش�.
				return	i;
			}
		}

		// �´� ���� ���� ã�� ���ߴ�. ���ǵ��� ���� TOKEN�̶�� �����ش�.
		return	TOKEND_NOTDEFINED;
	}


	// ------------------------------------------------------------
	// 4. ���������� Ȯ���Ѵ�.
	// ------------------------------------------------------------
	// - ���࿡ '0'���� '9'�����̰ų� '-'���ϰ� �پ� ������ ���ڶ�� �Ǵ��� �Ѵ�.
	else if(Char >= '0' && Char <= '9' || Char == '-')
	{
		int		p_StringPoint	 = 0;
		while((Char >= '0' && Char <= '9') || Char == '-' || Char == '.')
		{
			p_TokenString[p_StringPoint]	 = Char;

			Char	 = GetNextChar();

			// Check) �������� Ȯ���Ѵ�.
			if(!Char)return TOKEND_END;

			++p_StringPoint;
		}
		p_TokenString[p_StringPoint]	 = NULL;


		return	TOKEND_NUMBER;
	}


	// ------------------------------------------------------------
	// 5. String������ �˻��Ѵ�.
	// ------------------------------------------------------------
	// - '"' ����ǥ�� �����ϸ� �̰��� String�δ�!
	else if(Char == '"')
	{
		Char	 = GetNextChar();

		// Check) �������� Ȯ���Ѵ�.
		if(!Char)return TOKEND_END;

		int		p_StringPoint	 = 0;
		while(Char != '"')
		{
			p_TokenString[p_StringPoint]	 = Char;

			Char	 = GetNextChar();

			// Check) �������� Ȯ���Ѵ�.
			if(!Char)return TOKEND_END;

			++p_StringPoint;
		}
		p_TokenString[p_StringPoint]	 = NULL;
		
		return	TOKEND_STRING;
	}

	
	// Error) 
	return	TOKEND_NOTDEFINED;
}
