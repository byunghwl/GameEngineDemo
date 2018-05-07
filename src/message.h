/************************************************************************/
/*						messge.h [2014.11.6 ]
			다양한 메세지 클래스를 정의해 놓는다.
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
			다양한 메세지들의 최상위 클래스   [2014.11.6 ]  
*/
/************************************************************************/
class SH_MSG
{
protected:

	int								m_MsgSize;
	int								m_MsgType; 

public:

// 	///////////////  여기서부터는 윈도우 기본 메세지용 //////////////////////////////
// 	//  윈도우용이지만 굳이 윈도우용으로만 쓸 필요는 없다. [2014.11.6 ]
	HWND							m_hWnd;
	UINT							m_WinMsg;	// 윈도우메세지프로시져의 msg
	WPARAM							m_wParam;
	LPARAM							m_lParam;

public:

	SH_MSG(){ m_MsgSize = sizeof(*this);  m_MsgType = eMsgType_NORMAL; }   // 항상 초기화 할때 사이즈를 지정해줘야한다.
	virtual ~SH_MSG(){};

public:

	int								GetMsgSize() const {return m_MsgSize;}
	int								GetMsgType() const {return m_MsgType;}

	void							SetMsgSize(const int& _val){m_MsgSize = _val;} 
	void							SetMsgType(const int& _val){m_MsgType = _val;} 


};


/***********************************************************************
                      class SH_MSG_TO_RenderObj
		UI에서 IRenderable 속성을 가진 모든 개체에게 보낼때 사용하는 메세지 [2014.11.6 ]
************************************************************************/
class SH_MSG_from_UI : public SH_MSG
{
	// UI ID용 ///////////////////////////////////////////////////////////////
public:
	DWORD							m_UI_id;	// 선언된 UI들의 id	 [2014.11.6 ]
	
	SH_MSG_from_UI(){ m_MsgSize = sizeof(*this);  m_MsgType = eMsgType_UI; }

};