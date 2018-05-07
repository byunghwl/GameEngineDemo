/************************************************************************/
/*                         [2014.10.17 ]
						루아를 위한 헤더파일
*/
/************************************************************************/
#pragma  once

#pragma comment(lib, "./common/Lua/lua5.1.lib")
#pragma comment(lib, "./common/Lua/lua51.lib")

//////////////////////////////////////////////////////////////////////////
// 파일 경로를 인자로 루아 스테이트를 초기화하고 



/************************************************************************/
/*						루아를 쉽게 사용하기 위한 
					    클래스  [2014.10.18 ]
*/
/************************************************************************/
class SH_Lua
{
private: 
	lua_State*			m_pLuaState; // 루아 상태변수

public:// 생성 ,, 소멸자

	SH_Lua(const CHAR* _pPath);
	~SH_Lua();

public:

	void				Release();

public:// 일반함수
	// Arg    : <In> const char* _ValName  = 찾고싶은  변수이름
	//			<Out>int &_val = 찾은 변수값을 저장할곳 (외부변수)
	// Desc   : 문자열 이름으로  int 형 변수를 찾아준다.	
	void				GetGlobal_int(const char* _ValName, int &_val);

	// Desc   : 문자열 이름으로  float 형 변수를 찾아준다.  // [2014.11.14 ]
	void				GetGlobal_float(const char* _ValName, float &_val);	

	// 이름에 해당하는 테이블을 루아스택 맨위에 올려놓는다.
	void				PushTable(const char* _Name);

	// 루아스택 맨위에 있는 테이블을 pop해준다.
	void				PopTable();

	// 테이블에서 bool 변수를 가지고 온다.[2014.11.25 ]
	void				Getfield_bool(const char* _keyName, bool& _val);
	// 루아스텍 맨위에 테이블이 있다고 가정하고 _keyName에 해당하는 값을 가져온다.
	// 가져온 값이 int 가 아닐경우 에러메세지와 프로그램 종료
	// <In> const char* _keyName, <Out> int &_val
	void				Getfield_int(const char* _keyName, int& _val);

	// 루아스텍 맨위에 테이블이 있다고 가정하고 _keyName에 해당하는 값을 가져온다.
	// 가져온 값이 float이 아닐경우 에러메세지와 프로그램 종료  //[2014.11.16 ]
	// <In> const char* _keyName, <Out> float &_val
	void				Getfield_float(const char* _keyName, float& _val);

	// 루아스텍 맨위에 테이블이 있다고 가정하고 _keyName에 해당하는 값을 가져온다.
	// 가져온 값이 문자열이 아닐경우 에러메세지와 프로그램 종료
	void				Getfield_String(const char* _keyName, char* _String);


public:// get.. set

	lua_State*			GetLuaState(){return m_pLuaState;}

};