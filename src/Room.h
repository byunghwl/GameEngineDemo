/***********************************************************************
                      Room.h
					[2014.11.11]

************************************************************************/

#pragma  once

struct ROOM_DATA
{

	ROOM_DATA();
};
/***********************************************************************
                       class CRoom   

						[2014.11.11]
************************************************************************/
class CRoom : public CEntity, //Base Class
			  virtual public IRenderable,
			  virtual public ICollidable,
			  virtual public IProgressable
{
private:

	CRscPack_NormalObj					m_RscPack;// 리소스 구성요소들
	D3DXHANDLE							m_hTech;//어떤테크닉을 쓸거냐 [2014.11.12 ]
	//////////////////////////////////////////////////////////////////////////
	// 데이터저장용 변수
	ROOM_DATA							m_RoomData;
	ROOM_DATA							m_RoomDataBck;// 백업용
	static CString						m_EntityType; // 엔티티별 타입 이름

public:

	CRoom();
	virtual ~CRoom();

public:// 가상함수 재정의 
	// from IRenderable
	virtual void						Draw();
	// from IProgressable
	virtual void						Update(float _dTime);	
	// from ICollidable
	virtual HRESULT						InitCollider(); //[2014.11.11 ]
public:
	//초기화할때 필요한 함수
	int									Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,0),
											 ROOM_DATA* pRoomData= nullptr );



	static CString						GetCreatorName(){return m_EntityType;}
	CRscPack_NormalObj*					GetRscPack(){return &m_RscPack;} 

private: // 내부함수

	void								_OnDrawStart();


};

///////////////////////////////////////////////////////////////////
// 외부전역변수 선언  
extern D3DXVECTOR3 G_RoomPoint_Max; //  2014.11.18 
extern D3DXVECTOR3 G_RoomPoint_Min; // 방의 최대 최소 점



/********************************************* end************************************************************************/

