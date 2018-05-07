/************************************************************************/
/*						messge.h [2014.11.6 ]
			�پ��� �޼��� Ŭ������ ������ ���´�.
*/
/************************************************************************/

#pragma  once


//////////////////////////////////////////////////////////////////////////
enum SHMsgType
{
	eMsgType_NORMAL = 0,
	eMsgType_UI,
// 	eMsgType_TO_Renderable,
// 	eMsgType_TO_Messagable,
// 	eMsgType_TO_Progressable,
// 	eMsgType_TO_Messagable,
// 	eMsgType_TO_IPickable,
// 	eMsgType_NetWorkPacket,
	eMsgType_Max
};
/************************************************************************/
/*				class SH_MSG
			�پ��� �޼������� �ֻ��� Ŭ����   [2014.11.6 ]  
*/
/************************************************************************/
class SH_MSG
{
protected:

	int								m_MsgSize;
	int								m_MsgType; 

public:

// 	///////////////  ���⼭���ʹ� ������ �⺻ �޼����� //////////////////////////////
// 	//  ������������� ���� ����������θ� �� �ʿ�� ����. [2014.11.6 ]
	HWND							m_hWnd;
	UINT							m_WinMsg;	// ������޼������ν����� msg
	WPARAM							m_wParam;
	LPARAM							m_lParam;

public:

	SH_MSG(){ m_MsgSize = sizeof(*this);  m_MsgType = eMsgType_NORMAL; }   // �׻� �ʱ�ȭ �Ҷ� ����� ����������Ѵ�.
	virtual ~SH_MSG(){};

public:

	int								GetMsgSize() const {return m_MsgSize;}
	int								GetMsgType() const {return m_MsgType;}

	void							SetMsgSize(const int& _val){m_MsgSize = _val;} 
	void							SetMsgType(const int& _val){m_MsgType = _val;} 


};


/***********************************************************************
                      class SH_MSG_TO_RenderObj
		UI���� IRenderable �Ӽ��� ���� ��� ��ü���� ������ ����ϴ� �޼��� [2014.11.6 ]
************************************************************************/
class SH_MSG_from_UI : public SH_MSG
{
	// UI ID�� ///////////////////////////////////////////////////////////////
public:
	DWORD							m_UI_id;	// ����� UI���� id	 [2014.11.6 ]
	
	SH_MSG_from_UI(){ m_MsgSize = sizeof(*this);  m_MsgType = eMsgType_UI; }

};