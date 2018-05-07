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

	CRscPack_NormalObj					m_RscPack;// ���ҽ� ������ҵ�
	D3DXHANDLE							m_hTech;//���ũ���� ���ų� [2014.11.12 ]
	//////////////////////////////////////////////////////////////////////////
	// ����������� ����
	ROOM_DATA							m_RoomData;
	ROOM_DATA							m_RoomDataBck;// �����
	static CString						m_EntityType; // ��ƼƼ�� Ÿ�� �̸�

public:

	CRoom();
	virtual ~CRoom();

public:// �����Լ� ������ 
	// from IRenderable
	virtual void						Draw();
	// from IProgressable
	virtual void						Update(float _dTime);	
	// from ICollidable
	virtual HRESULT						InitCollider(); //[2014.11.11 ]
public:
	//�ʱ�ȭ�Ҷ� �ʿ��� �Լ�
	int									Init(LPDIRECT3DDEVICE9 _pDevice , D3DXVECTOR3 _SPos = D3DXVECTOR3(0,0,0),
											 ROOM_DATA* pRoomData= nullptr );



	static CString						GetCreatorName(){return m_EntityType;}
	CRscPack_NormalObj*					GetRscPack(){return &m_RscPack;} 

private: // �����Լ�

	void								_OnDrawStart();


};

///////////////////////////////////////////////////////////////////
// �ܺ��������� ����  
extern D3DXVECTOR3 G_RoomPoint_Max; //  2014.11.18 
extern D3DXVECTOR3 G_RoomPoint_Min; // ���� �ִ� �ּ� ��



/********************************************* end************************************************************************/

