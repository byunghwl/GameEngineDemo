/************************************************************************
            엔티티 Factory로 엔티티를 생성할때 각 엔티티마다 
		전달해줄 인자가 다를수 있기때문에 EntityFactory_Arg 를 상속받은
		클래스를 엔티티별로 각각 만들어준다.  [2014.11.15 ]
************************************************************************/

#pragma once

// 스폰될때 사용될 정보 [2014.11.16 ]
struct SPAWN_INFO
{
	float	StartTime , EndTime;
	float	Interval;
	int		SpawnProbability_Lv1;
	int		SpawnProbability_Lv2;
	int		SpawnProbability_Lv3;
};

/************************************************************************
                    엔티티 팩토리에 넘겨줄 함수 인자로 
					사용될 클래스 [2014.11.16 ]
							고스트 전용
************************************************************************/
class GhostFactory_Arg : public EntityFactory_Arg
{
public:// 생성시의 정보로만 사용될 것이므로 public [2014.11.16 ]


	float    			m_fSpawnRadius; // 스폰될 지역의 범위 반경 [2014.11.16 ]
	D3DXVECTOR3			m_GhostLookVec; //  각스폰지역마다 고스트가 생성됬을때 바라볼 방향 
	int					m_SpwnInfoCnt;
	SPAWN_INFO			m_SpawnInfo[50];// 

};



/*************************************************** end of this file **************************************************************/