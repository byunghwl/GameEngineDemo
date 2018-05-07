
Respawn_Point_1 = {}
Respawn_Point_2 = {}
Respawn_Point_3 = {}
Respawn_Point_4 = {}

-- 리스폰된 유령들이 통과하는 지점 의 종류--
PassPointArray_1 = {}
PassPointArray_2 = {}
PassPointArray_3 = {}
PassPointArray_4 = {}
-----------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------  여기서 부터 스폰포인트1 ---------------------------------------------------------
Pos1_x = 0  -- 리스폰 포인트 1  위치 X
Pos1_y = 5  -- 리스폰 포인트 1  위치 y
Pos1_z = 190  -- 리스폰 포인트 1  위치 z
Pos1_Radius = 35 -- 리스폰 범위 반지름

Pos1_LookVec_X = 0.0  -- 포인트1에서 나타나는 유령들의  바라보는 방향 x
Pos1_LookVec_Y = 0.0  -- 포인트1에서 나타나는 유령의 바라보는 방향 y
Pos1_LookVec_Z = 1.0  -- 포인트1에서 나타나는 유령의 바라보는 방향 z

-- start_time : 단위 초, interval : 생성시간의 간격, LV1 = xx%

Respawn_Point_1[1] = {start_time = 42,		end_time = 45 ,		interval = 3 ,		LV1 = 0,		LV2 = 0 ,	LV3 = 100 }
Respawn_Point_1[2] = {start_time = 48,		end_time = 51, 		interval = 1.5 ,		LV1 = 0,		LV2 = 0 ,	LV3 = 100 }
Respawn_Point_1[3] = {start_time = 87,		end_time = 90, 		interval = 1.5 ,		LV1 = 0,		LV2 = 0 ,	LV3 = 100 }
Respawn_Point_1[4] = {start_time = 117,		end_time = 120 , 		interval = 1 ,		LV1 = 0,		LV2 = 0 ,	LV3 = 100 }
Respawn_Point_1[5] = {start_time = 126,		end_time = 129 , 		interval = 1.5 ,		LV1 = 0,		LV2 = 0 ,	LV3 = 100 }
Respawn_Point_1[6] = {start_time = 153,		end_time = 156 , 		interval = 1 ,		LV1 = 0,		LV2 = 0 ,	LV3 = 100 }

Respawn_Point_1_Size = table.getn(Respawn_Point_1)


-----------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------  여기서 부터 스폰포인트2 ---------------------------------------------------------
Pos2_x = 250  -- 리스폰 포인트 2  위치 X
Pos2_y = 5  -- 리스폰 포인트 2  위치 y
Pos2_z = 190  -- 리스폰 포인트 2  위치 z
Pos2_Radius = 35 -- 리스폰 범위

Pos2_LookVec_X = 0.707  -- 포인트2에서 나타나는 유령들의  바라보는 방향 x
Pos2_LookVec_Y = 0.0  -- 포인트2에서 나타나는 유령의 바라보는 방향 y
Pos2_LookVec_Z = 0.707  -- 포인트2에서 나타나는 유령의 바라보는 방향 z

-- start_time : 단위 초, interval : 생성시간의 간격, LV1 = xx%

Respawn_Point_2[1] = {start_time = 0,		end_time = 3 ,			interval = 1 ,		LV1 = 100,		LV2 = 0 ,	LV3 = 0 }
Respawn_Point_2[2] = {start_time = 21,		end_time = 24 , 		interval = 1 ,		LV1 = 100,		LV2 = 0 ,	LV3 = 0 }
Respawn_Point_2[3] = {start_time = 63,		end_time = 66 , 		interval = 1.5 ,		LV1 = 100,		LV2 = 0 ,	LV3 = 0 }
Respawn_Point_2[4] = {start_time = 72,		end_time = 75 , 		interval = 1.5 ,		LV1 = 100,		LV2 = 0 ,	LV3 = 0 }
Respawn_Point_2[5] = {start_time = 93,		end_time = 96 , 		interval = 0.75 ,		LV1 = 90,		LV2 = 10 ,	LV3 = 0 }
Respawn_Point_2[6] = {start_time = 111,		end_time = 114 , 		interval = 1 ,		LV1 = 80,		LV2 = 20 ,	LV3 = 0 }
Respawn_Point_2[7] = {start_time = 129,		end_time = 132 , 		interval = 1 ,		LV1 = 70,		LV2 = 30 ,	LV3 = 0 }
Respawn_Point_2[8] = {start_time = 150,		end_time = 153 , 		interval = 1 ,		LV1 = 60,		LV2 = 40 ,	LV3 = 0 }

Respawn_Point_2_Size = table.getn(Respawn_Point_2)


-----------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------  여기서 부터 스폰포인트3 ---------------------------------------------------------
Pos3_x = 260.0  -- 리스폰 포인트 3  위치 X
Pos3_y = 5  -- 리스폰 포인트 3  위치 y
Pos3_z = 5  -- 리스폰 포인트 3  위치 z
Pos3_Radius = 35 -- 리스폰 범위

Pos3_LookVec_X = -1.0  -- 포인트1에서 나타나는 유령들의  바라보는 방향 x
Pos3_LookVec_Y = 0.0  -- 포인트1에서 나타나는 유령의 바라보는 방향 y
Pos3_LookVec_Z = 0.0  -- 포인트1에서 나타나는 유령의 바라보는 방향 z
-- start_time : 단위 초, interval : 생성시간의 간격, LV1 = xx%

Respawn_Point_3[1] = {start_time = 6,		end_time = 9 ,			interval = 1.5 ,		LV1 = 0,		LV2 = 100 ,	LV3 = 0 }
Respawn_Point_3[2] = {start_time = 24,		end_time = 27 ,		interval = 1.5 ,		LV1 = 0,		LV2 = 100 ,	LV3 = 0 }
Respawn_Point_3[3] = {start_time = 33,		end_time = 36 ,		interval = 1.5 ,		LV1 = 0,		LV2 = 100 ,	LV3 = 0 }
Respawn_Point_3[4] = {start_time = 60,		end_time = 63 ,		interval = 1.5 ,		LV1 = 0,		LV2 = 100 ,	LV3 = 0 }
Respawn_Point_3[5] = {start_time = 75,		end_time = 95 ,		interval = 1.5 ,		LV1 = 0,		LV2 = 100 ,	LV3 = 0 }
Respawn_Point_3[6] = {start_time = 96,		end_time = 99 ,		interval = 1 ,		LV1 = 0,		LV2 = 90 ,	LV3 = 10 }
Respawn_Point_3[7] = {start_time = 108,		end_time = 111 ,		interval = 1 ,		LV1 = 0,		LV2 = 80 ,	LV3 = 20 }
Respawn_Point_3[8] = {start_time = 132,		end_time = 135 ,		interval = 1 ,		LV1 = 0,		LV2 = 70 ,	LV3 = 30 }
Respawn_Point_3[9] = {start_time = 147,		end_time = 150 ,		interval = 1 ,		LV1 = 0,		LV2 = 60 ,	LV3 = 40 }

Respawn_Point_3_Size = table.getn(Respawn_Point_3)


-----------------------------------------------------------------------------------------------------------------------------------
------------------------------------------------ 여기서 부터 스폰포인트4 ---------------------------------------------------------
Pos4_x = 250  -- 리스폰 포인트 4  위치 X
Pos4_y = 20  -- 리스폰 포인트 4  위치 y
Pos4_z = -190  -- 리스폰 포인트 4 위치 z
Pos4_Radius = 35 -- 리스폰 범위 반지름

Pos4_LookVec_X = 0.0  -- 포인트2에서 나타나는 유령들의  바라보는 방향 x
Pos4_LookVec_Y = 0.0  -- 포인트2에서 나타나는 유령의 바라보는 방향 y
Pos4_LookVec_Z = -1.0  -- 포인트2에서 나타나는 유령의 바라보는 방향 z

-- start_time : 단위 초, interval : 생성시간의 간격, LV1 = xx%

Respawn_Point_4[1] = {start_time = 12,		end_time = 15 ,		interval = 1 ,		LV1 = 100,	LV2 = 0 ,	LV3 = 0 }
Respawn_Point_4[2] = {start_time = 30,		end_time = 33 ,		interval = 1 ,		LV1 = 100,	LV2 = 0 ,	LV3 = 0 }
Respawn_Point_4[3] = {start_time = 57,		end_time = 60 ,		interval = 1.5 ,		LV1 = 100,	LV2 = 0 ,	LV3 = 0 }
Respawn_Point_4[4] = {start_time = 78,		end_time = 81 ,		interval = 1 ,		LV1 = 90,	LV2 = 10 ,	LV3 = 0 }
Respawn_Point_4[5] = {start_time = 105,		end_time = 108 ,		interval = 1 ,		LV1 = 80,	LV2 = 20 ,	LV3 = 0 }
Respawn_Point_4[6] = {start_time = 135,		end_time = 143 ,		interval = 1 ,		LV1 = 70,	LV2 = 30 ,	LV3 = 0 }
Respawn_Point_4[7] = {start_time = 144,		end_time = 147 ,		interval = 1 ,		LV1 = 60,	LV2 = 40 ,	LV3 = 0 }

Respawn_Point_4_Size = table.getn(Respawn_Point_4)


-----------------------------------------------------------------------------------------------------------------------------------

BedPosition_X = -195.0
BedPosition_Y = 66.0
BedPosition_Z = 6.0


-----------------------------------------------------------------------------------------------------------------------------------
--								통과지점개수 최대 30개까지 !!
-----------------------------------------------------------------------------------------------------------------------------------
-- 1번 스폰위치에서 나타날  유령의 동과지점     -- 숫자로 써넣어도 됨!!
PassPointArray_1[1] = { X = 0,  	Y =0 , 	Z = 130  }   -- 첫번째 통과지점
PassPointArray_1[2] = { X = 0,  	Y = 0 , 	Z = 40  }
PassPointArray_1[3] = { X = 60,  	Y = 0  ,	 Z = 20  }
PassPointArray_1[4] = { X = 60,  Y = 0  , Z = 40  }
PassPointArray_1[5] = { X = 55,  Y = 10  , Z = 55  }
PassPointArray_1[6] = { X = 60,  Y = 5  , Z = 55  }
PassPointArray_1[7] = { X = 50,  Y = 20  , Z = 70  }
PassPointArray_1[8] = { X = 65,  Y = 5  , Z = 55  }
PassPointArray_1[9] = { X = 85,  Y = 0  , Z = 45  }
PassPointArray_1[10] = { X = 90,  Y = 0  , Z = 40  }
PassPointArray_1[11] = { X = 90,  Y = 0  , Z = 115  }
PassPointArray_1[12] = { X = -40,  Y = 0  , Z = 115  }
PassPointArray_1[13] = { X = -105,  Y = 0  , Z = 60  }
PassPointArray_1[14] = { X = BedPosition_X,  Y = BedPosition_Y , Z = BedPosition_Z }  -- 마지막 통과지점

-- 2번 스폰위치에서 에서 나타날  유령의 동과지점
PassPointArray_2[1] = { X = 155,  	 Y = 20  , 	Z = 130  }   -- 첫번째 통과지점
PassPointArray_2[2] = { X = 155,  	 Y = 5  , 	Z = 120  }   
PassPointArray_2[3] = { X = 155,  	 Y = 20  , 	Z = 110  } 
PassPointArray_2[4] = { X = 155,  	 Y = 5  , 	Z = 100  } 
PassPointArray_2[5] = { X = 155,  	 Y = 20 ,		Z = 90  } 
PassPointArray_2[6] = { X = 140,  	 Y = 0  , 	Z = 70  } 
PassPointArray_2[7] = { X = 100,  	 Y = 0  , 	Z = 40  } 
PassPointArray_2[8] = { X = 45,  	 Y = 0  , 	Z = -140  } 
PassPointArray_2[9] = { X = -90, 	  Y = 0  , 	Z = -140  }
PassPointArray_2[10] = { X = -105,   Y = 5  , 	Z = -170  }
PassPointArray_2[11] = { X = -125,   Y = 5  , 	Z = -140  }
PassPointArray_2[12] = { X = -190,   Y = 0  , 	Z = -50  }
PassPointArray_2[13] = { X = BedPosition_X,  Y = BedPosition_Y , Z = BedPosition_Z }	-- 마지막 통과지점

-- 3번 스폰위치에서 나타날  유령의 동과지점
PassPointArray_3[1] = { X = 135,		Y = 0 ,		Z = 30  }   -- 첫번째 통과지점
PassPointArray_3[2] = { X = 115,		Y = 0,		Z = -45 }
PassPointArray_3[3] = { X = 10,			Y=0 ,			Z = -10 }
PassPointArray_3[4] = { X = -10,		Y=15 ,			Z = -50 }
PassPointArray_3[5] = { X = 15,         Y=0 ,			Z = -75 }
PassPointArray_3[6] = { X = -45,      Y=0 ,             Z = -115 }
PassPointArray_3[7] = { X = -110,      Y=0.0 ,             Z = -40 }
PassPointArray_3[8] = { X = BedPosition_X,  Y = BedPosition_Y , Z = BedPosition_Z }   -- 마지막 통과지점

-- 4번 스폰위치에서 나타날  유령의 동과지점
PassPointArray_4[1] = { X = 200,		Y=40 ,		Z =  -115 }     	--첫번째 통과지점
PassPointArray_4[2] = { X = 90,			Y=0 ,		Z =  -115 }
PassPointArray_4[3] = { X = -90,		Y = 0 ,		Z =  150 }
PassPointArray_4[4] = { X = -100,		Y=10 ,		Z = 150 }
PassPointArray_4[5] = { X = -110,		Y = 0 , 		Z = 150  }
PassPointArray_4[6] = { X = -120,		Y=20 ,		Z = 140 }
PassPointArray_4[7] = { X = -120,		Y=0 ,			Z = 150 }
PassPointArray_4[8] = { X = -190,		Y=0 ,		Z = 60}
PassPointArray_4[9] = { X = BedPosition_X,  Y = BedPosition_Y , Z = BedPosition_Z }

--------------------------------------------------------------------------------------------------------------------------------------
--  PassPointArray_1 사이즈 변수
PassPointArray_1_Size = table.getn(PassPointArray_1)
--  PassPointArray_2 사이즈 변수
PassPointArray_2_Size = table.getn(PassPointArray_2)
--  PassPointArray_3 사이즈 변수
PassPointArray_3_Size = table.getn(PassPointArray_3)
--  PassPointArray_4 사이즈 변수
PassPointArray_4_Size = table.getn(PassPointArray_4)


