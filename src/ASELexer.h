
#pragma once
//-----------------------------------------------------------------------------
//  ASE Lexer
//  
//  1) ASE Lexer란...
//     - 정해진 File을 차례대로 읽어서 ASE에 정의된 Token단위로 잘라주는 역할을
//       하는 Class이다.
//     - GetToken()함수를 호출하면 Text File의 첫 Token부터 차례대로 나온다.
//     - GetToken(LPSTR p_TokenString)의 Return Value는 Token의 종류를 돌려주고
//       넘져진 p_TokenString에는 그엥 해당하는 String을 돌려준다.
//  
//-----------------------------------------------------------------------------
#include "Lexer.h"
#include "TokenDefinition.h"


class CLexer : 
// ****************************************************************************
// Inherited Classes)
// ----------------------------------------------------------------------------
	public						ILexer
{
// ****************************************************************************
// Constructor/Destructor)
// ----------------------------------------------------------------------------
public:
			CLexer(LPCWSTR p_strFileName=NULL, DWORD p_dwSizeOfBuffer=MAX_SIZE_OF_BUFFER);
	virtual ~CLexer();


// ****************************************************************************
// Public)
// ----------------------------------------------------------------------------
	// 1) Token을 얻는 함수.
	//    - Return값	Token의 종류를 의미한다.(Definition에 정의된 숫자.)
	//    - String값 	(In/Out)Token에 해당하는 문자열 값을 돌려준다.
	virtual	int				GetToken(LPSTR p_TokenString);
};

