/************************************************************************/
/*                         [2014.10.17 ]
						��Ƹ� ���� �������
*/
/************************************************************************/
#pragma  once

#pragma comment(lib, "./common/Lua/lua5.1.lib")
#pragma comment(lib, "./common/Lua/lua51.lib")

//////////////////////////////////////////////////////////////////////////
// ���� ��θ� ���ڷ� ��� ������Ʈ�� �ʱ�ȭ�ϰ� 



/************************************************************************/
/*						��Ƹ� ���� ����ϱ� ���� 
					    Ŭ����  [2014.10.18 ]
*/
/************************************************************************/
class SH_Lua
{
private: 
	lua_State*			m_pLuaState; // ��� ���º���

public:// ���� ,, �Ҹ���

	SH_Lua(const CHAR* _pPath);
	~SH_Lua();

public:

	void				Release();

public:// �Ϲ��Լ�
	// Arg    : <In> const char* _ValName  = ã�����  �����̸�
	//			<Out>int &_val = ã�� �������� �����Ұ� (�ܺκ���)
	// Desc   : ���ڿ� �̸�����  int �� ������ ã���ش�.	
	void				GetGlobal_int(const char* _ValName, int &_val);

	// Desc   : ���ڿ� �̸�����  float �� ������ ã���ش�.  // [2014.11.14 ]
	void				GetGlobal_float(const char* _ValName, float &_val);	

	// �̸��� �ش��ϴ� ���̺��� ��ƽ��� ������ �÷����´�.
	void				PushTable(const char* _Name);

	// ��ƽ��� ������ �ִ� ���̺��� pop���ش�.
	void				PopTable();

	// ���̺��� bool ������ ������ �´�.[2014.11.25 ]
	void				Getfield_bool(const char* _keyName, bool& _val);
	// ��ƽ��� ������ ���̺��� �ִٰ� �����ϰ� _keyName�� �ش��ϴ� ���� �����´�.
	// ������ ���� int �� �ƴҰ�� �����޼����� ���α׷� ����
	// <In> const char* _keyName, <Out> int &_val
	void				Getfield_int(const char* _keyName, int& _val);

	// ��ƽ��� ������ ���̺��� �ִٰ� �����ϰ� _keyName�� �ش��ϴ� ���� �����´�.
	// ������ ���� float�� �ƴҰ�� �����޼����� ���α׷� ����  //[2014.11.16 ]
	// <In> const char* _keyName, <Out> float &_val
	void				Getfield_float(const char* _keyName, float& _val);

	// ��ƽ��� ������ ���̺��� �ִٰ� �����ϰ� _keyName�� �ش��ϴ� ���� �����´�.
	// ������ ���� ���ڿ��� �ƴҰ�� �����޼����� ���α׷� ����
	void				Getfield_String(const char* _keyName, char* _String);


public:// get.. set

	lua_State*			GetLuaState(){return m_pLuaState;}

};