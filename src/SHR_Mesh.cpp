/***********************************************************************
                                                       
************************************************************************/

#include "DXUT.h"
#include "SHR_Mesh.h"


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//
// 여기서부터 Struct Mesh_Info
//
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
//-------------------------------------------------------------------------
// =연산자 오버로딩
//-------------------------------------------------------------------------
Mesh_Info&  Mesh_Info::operator=(const Mesh_Info& rhs)
{
	
	MeshType = rhs.MeshType; //[2014.11.25 ]
	strcpy_s(Name, rhs.Name);	
	strcpy_s(ParentName, rhs.ParentName);	

	NodeTM = rhs.NodeTM;
	NodeTM_Pos = rhs.NodeTM_Pos ;
	NodeTM_Scale = rhs.NodeTM_Scale; 
	NodeTM_Rot = NodeTM_Rot;
	RotAxis = rhs.RotAxis;
	RotAngle = RotAngle;
	Pos = rhs.Pos;
	Scale = rhs.Scale; 
	ScaleAxis = rhs.ScaleAxis;
	ScaleAngle = rhs.ScaleAngle;
	HaveMesh = rhs.HaveMesh;
	isSkinned = rhs.isSkinned;

	BoneIniMat = rhs.BoneIniMat;
	LTM_scale = rhs.LTM_scale;
	LTM_Rot = rhs.LTM_Rot;
	LTM_Pos = rhs.LTM_Pos;

	BoundingBox_Min = rhs.BoundingBox_Min; // [2014.11.25 ]
	BoundingBox_Max = rhs.BoundingBox_Max; // [2014.11.25 ]
	BoundShpere_Radius = rhs.BoundShpere_Radius; // [2014.11.25 ]
	return *this;
}



//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 class FaceSet 함수정의
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------
// 생성자
//-------------------------------------------------------------------------
FaceSet::FaceSet()
{
	
	m_pTexDiff = nullptr;
	m_pTexSpecularMap = nullptr;
	m_pTexBumpMap = nullptr;
	m_pTexEmissiveMap = nullptr;
	m_pVtxBuff= nullptr;
	m_pIdxBuf = nullptr; // [2014.10.30 ] 

	m_pVtxBuff= nullptr;
	m_pIdxBuf = nullptr;
}


//-------------------------------------------------------------------------
//  같은 텍스쳐를 공유하는 부분 그려주기
//-------------------------------------------------------------------------

void	FaceSet::Draw(LPDIRECT3DDEVICE9 _device)
{
	
	if( m_pTexDiff != nullptr) // [2014.10.29 ]
	{ G_FXShader.GetPointer()->SetTexture(G_ConstantTable.h_TexDiffMap, m_pTexDiff->GetD3DTex());	}

	if( m_pTexSpecularMap != nullptr) // [2014.10.29 ]
	{ G_FXShader.GetPointer()->SetTexture(G_ConstantTable.h_TexSpecularMap, m_pTexSpecularMap->GetD3DTex());	}

	G_FXShader.GetPointer()->SetFloat(G_ConstantTable.h_Mtrl_SpecularPower , m_Material.Power );// [2014.10.29 ]
	
	
	//_device->SetMaterial(&m_Material);	

	G_FXShader.GetPointer()->CommitChanges(); // [2014.10.30 ] 

	_device->SetStreamSource( 0, m_pVtxBuff->GetD3DVtxBuf(), 0, m_pVtxBuff->GetStrideSize() );
	_device->SetIndices( m_pIdxBuf->GetD3DVtxBuf());
	_device->SetFVF( m_pVtxBuff->GetFVF() );

	_device->DrawIndexedPrimitive( D3DPT_TRIANGLELIST, 0, 0, m_pVtxBuff->GetNumVtx(), 0 , m_pIdxBuf->GetNumFace() );
}







//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 class SHR_Mesh 함수정의
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//-------------------------------------------------------------------------
// 생성자1
//-------------------------------------------------------------------------
SHR_Mesh::SHR_Mesh()
{
	m_pFaceSet = nullptr;
	m_pParent = nullptr;
	m_CurAniTime = 0.0f;
}


//-------------------------------------------------------------------------
// 생성자2
//-------------------------------------------------------------------------
SHR_Mesh::SHR_Mesh(const WCHAR* _Name , const WCHAR* _Path /*= nullptr*/)
{	
	assert(_Name != nullptr);
		
	eResourceType = eRT_MESH_NORMAL;

	m_NameSize = sizeof(WCHAR)*wcslen(_Name);
	wcscpy_s(m_Name,_Name);

	if(_Path != nullptr)
	{
		m_PathSize = sizeof(WCHAR)*wcslen(_Path);
		wcscpy_s(m_Path,_Path);
	}
	else
	{			
		wcscpy_s(m_Path,L"./object/");
		wcscat_s(m_Path,_Name);
		wcscat_s(m_Path,L".shrnm");//shr_Normal_Mesh
		m_PathSize = sizeof(WCHAR)*wcslen(m_Path);
	}

	m_pFaceSet = nullptr;
	m_pParent = nullptr;
	m_CurAniTime = 0.0f;
}



//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
SHR_Mesh::~SHR_Mesh()
{
	SAFE_DELETE_ARR(m_pFaceSet)

}



//-------------------------------------------------------------------------
// 업데이트 함수
// arg : _dTime (delay time) , pMoveTM(캐릭터 이동 행렬)
//-------------------------------------------------------------------------
void	SHR_Mesh::Update(CAnimator& m_Animator ,  D3DXMATRIX* pMoveTM /*=nullptr*/)
{

	D3DXMATRIX TempMoveTM;
	if(pMoveTM == nullptr)
	{
		D3DXMatrixIdentity(&TempMoveTM);
	}
	else
	{
		TempMoveTM = *pMoveTM;
	}

	m_AniTm_Pos = m_MeshInfo.LTM_Pos;
	m_AniTm_Rot = m_MeshInfo.LTM_Rot;
	m_AniTM_Scale = m_MeshInfo.LTM_scale;

	m_Animator.AniTimeUpdate();// [2014.10.13 ]

	 if(m_Animator.GetEndTime() < m_Animator.GetAniTime())//[2014.10.13 ]
	 {	m_Animator.SetAniTimeFirst() ; }

 	_Animation(m_Animator);

	if(m_pParent !=NULL) //자식일떄
	{//m_MWorldTM = m_MLocalTM *  m_pParent->m_MWorldTM ;
		m_WorldTM = m_AniTM_Scale * m_AniTm_Rot  *m_AniTm_Pos *  m_pParent->m_WorldTM;
	}

	if(pMoveTM == nullptr)
	{
		D3DXMatrixIdentity(pMoveTM);
	}

	if(m_pParent ==NULL) //자기가부모일때
	{	//m_MWorldTM = m_MLocalTM * MoveTM;
		m_WorldTM =  (m_AniTM_Scale  * m_AniTm_Rot *m_AniTm_Pos) * (*TempMoveTM);
	}

}


//-------------------------------------------------------------------------
// 메쉬가 본속성을 가지고 있을 때에만(BoneList 안에서만) 사용되는
//-------------------------------------------------------------------------
void	SHR_Mesh::UpdateBone(CAnimator &_Animator , D3DXMATRIX* pMoveTM/* = nullptr*/)
{
	m_AniTm_Pos = m_MeshInfo.LTM_Pos;
	m_AniTm_Rot = m_MeshInfo.LTM_Rot;
	m_AniTM_Scale = m_MeshInfo.LTM_scale;

	_Animation(_Animator);

	if(m_pParent !=NULL) //자식일떄
	{//m_MWorldTM = m_MLocalTM *  m_pParent->m_MWorldTM ;
		m_WorldTM = m_AniTM_Scale * m_AniTm_Rot  *m_AniTm_Pos *  m_pParent->m_WorldTM;
	}

	if(pMoveTM == nullptr)
	{
		D3DXMatrixIdentity(pMoveTM);
	}

	if(m_pParent ==NULL) //자기가부모일때
	{	//m_MWorldTM = m_MLocalTM * MoveTM;
		m_WorldTM =  (m_AniTM_Scale  * m_AniTm_Rot *m_AniTm_Pos) * (*pMoveTM);
	}
}


//-------------------------------------------------------------------------
// 그리기
//-------------------------------------------------------------------------
void	SHR_Mesh::Draw( LPDIRECT3DDEVICE9 _device)
{
		if(m_MeshInfo.HaveMesh == FALSE)
			return;

// 		if(m_hTech == nullptr )  // 테크 핸들 확인해보기 [2014.10.30 ]
// 		{		assert(false);	} // [2014.10.30 ]

		G_FXShader.GetPointer()->SetTechnique(m_hTech);// [2014.10.30 ]

		UINT MaxPass = 0;

		G_FXShader.m_pShader->Begin(&MaxPass, NULL);//테크닉 시작 [2014.10.29 ]

			for(UINT PassNum = 0; PassNum < MaxPass ; PassNum++ ) // 패스별로 그리기 [2014.11.21 ]
			{
				G_FXShader.GetPointer()->BeginPass(PassNum);// [2014.10.29 ]

				// 렌더타깃 위에 그린다.

				if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0/*PassNum 일단 0으로..[2014.11.19 ]*/]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
				{//gpSceneRenderTarget은 서페이스를 만들어줄때 필요한 녀석 [2014.11.7 ]
					_device->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // 만든 서페이스를 렌더타겟으로 지정(복사) [2014.11.7 ]
				}

				G_FXShader.GetPointer()->SetMatrix(G_ConstantTable.h_WorldTM , &m_WorldTM); // [2014.11.14 ]
			
				if(m_FaceSetCnt >0)
				{
					for(int i=0; i<m_FaceSetCnt ; i++)
					{	m_pFaceSet[i].Draw(_device);		}
				}	

				G_FXShader.GetPointer()->EndPass();// 패스별로 그리기 끝 [2014.10.29 ]
			}

		G_FXShader.GetPointer()->End();//테크닉 끝 [2014.10.29 ]


}

//-------------------------------------------------------------------------
// Arg	  : float _dTime
// Desc   : 애니메이션 해주는 함수
//-------------------------------------------------------------------------
void	SHR_Mesh::_Animation(CAnimator &_Animator)
{
	if(m_AniInfo.Animateable == false)//애니메이션 정보가 없다면 애니메이션을 하지 않는다;;
		return;

	//_AniTimer.Update();[2014.10.9 ]

// 	if(m_AniInfo.LastTime < _AniTimer.GetAniTime())[2014.10.9 ]
// 		_AniTimer = 0.0f;

	m_AniTm_Pos = _GetCurr_PosTM(_Animator); // CurrTick에 해당하는 PositionTM을 계산해서 반환한다.
	m_AniTm_Rot = _GetCurr_RotTM(_Animator);
	m_AniTM_Scale = _GetCurr_ScaleTM(_Animator );
}


//-------------------------------------------------------------------------
//  현재 _CurTick에 대한 PosTM을 구한다.
//-------------------------------------------------------------------------
D3DXMATRIX	SHR_Mesh::_GetCurr_PosTM(CAnimator &_Animator)
{
	float _CurTick = _Animator.GetAniTime();


	D3DXVECTOR3 V;
	float ProgRate = 0.0f ;//보간 진행률 
	D3DXMATRIX retM ,Inv; //  최종 반환 매트릭스 

	retM = m_MeshInfo.LTM_Pos;

	if(m_AniInfo.pPos != NULL)
	{
		int len = m_AniInfo.PosSize;

		//int startIdx = ((m_AniInfo.FrameCnt/FRAME_PER_SAMPLE * _CurTick)/m_AniInfo.LastTime) +1;
		int startIdx = (_CurTick * (float)m_AniInfo.PosSize / m_AniInfo.LastTime)+10;  //  현재 틱이 배열의 어느정도 위치에 있는지 계산하여 그것보다 10프레임정도 뒷쪽에서 검색한다.

		if(startIdx>=len) startIdx = len-1; // 위의 계산결과가 배열인덱스를 넘어가지 않게 해주기


		for(int i=startIdx; i>=1 ; i--)
		{
			if(m_AniInfo.pPos[i-1].Curtick <= _CurTick)
			{
				ProgRate = (_CurTick - m_AniInfo.pPos[i-1].Curtick) / (m_AniInfo.pPos[i].Curtick - m_AniInfo.pPos[i-1].Curtick);
				D3DXVec3Lerp(&V, &m_AniInfo.pPos[i-1].pos ,  &m_AniInfo.pPos[i].pos,  ProgRate);

				if(_Animator.is_BlendStart() == false ) // 애니메이션 블랜딩 중이 아니라면 
				{  m_Blend_PosKey = V;	  }   // 키값을 복사해준다.
				else									// 애니메이션 블랜딩 중이라면
				{
				   D3DXVec3Lerp(&V,  &m_Blend_PosKey ,   &V  ,  _Animator.GetBlendingTime()/_Animator.GetBlendMaxTime()    );
				}

				D3DXMatrixTranslation(&retM, V.x, V.y, V.z);

				break;
			}
			else if(m_AniInfo.pPos[i-1].Curtick == _CurTick)
			{
				V = m_AniInfo.pPos[i-1].pos;

				if(_Animator.is_BlendStart() == false ) // 애니메이션 블랜딩 중이 아니라면 
				{		m_Blend_PosKey =  V;	}   // 키값을 복사해준다.
				else									// 애니메이션 블랜딩 중이라면
				{
					D3DXVec3Lerp(&V,  &m_Blend_PosKey ,   &m_AniInfo.pPos[i-1].pos  ,  _Animator.GetBlendingTime()/_Animator.GetBlendMaxTime()    );
				}

				D3DXMatrixTranslation(&retM, V.x, V.y , V.z);
				break;
			}
		}
	}
	return retM;

}


//-------------------------------------------------------------------------
//  현재 _CurTick에 대한 RotTM을 구한다.
//-------------------------------------------------------------------------
D3DXMATRIX	SHR_Mesh::_GetCurr_RotTM(CAnimator &_Animator)
{

	float _CurTick = _Animator.GetAniTime();

	D3DXQUATERNION Q;
	float ProgRate = 0.0f ;//보간 진행률 
	D3DXMATRIX retM ,Inv; //  최종 반환 매트릭스 

	retM = m_MeshInfo.LTM_Rot;
	if(m_AniInfo.pRot != NULL)
	{
		int len = m_AniInfo.RotSize;

		//int startIdx = ((m_AniInfo.FrameCnt/FRAME_PER_SAMPLE * _CurTick)/m_AniInfo.LastTick) +1;
		int startIdx = (_CurTick* (float)m_AniInfo.RotSize /m_AniInfo.LastTime)+10;
		if(startIdx>=len) startIdx = len-1;

		for(int i=startIdx; i>=1 ; i--)
		{
			if(m_AniInfo.pRot[i-1].Curtick < _CurTick)
			{
				ProgRate = (_CurTick - m_AniInfo.pRot[i-1].Curtick) / (m_AniInfo.pRot[i].Curtick - m_AniInfo.pRot[i-1].Curtick);
				D3DXQuaternionSlerp(&Q,   &m_AniInfo.pRot[i-1].quat, &m_AniInfo.pRot[i].quat,  ProgRate);
				

				if(_Animator.is_BlendStart() == false ) // 애니메이션 블랜딩 중이 아니라면 
				{   m_Blend_RotKey = Q;	  }   // 키값을 복사해준다.
				else									// 애니메이션 블랜딩 중이라면
				{
					D3DXQuaternionSlerp(&Q,  &m_Blend_RotKey ,   &Q  ,  _Animator.GetBlendingTime()/_Animator.GetBlendMaxTime()    );
				}

				D3DXMatrixRotationQuaternion(&retM, &Q);

				break;
			}
			else if(m_AniInfo.pRot[i-1].Curtick == _CurTick)
			{
				Q = m_AniInfo.pRot[i-1].quat;

				if(_Animator.is_BlendStart() == false ) // 애니메이션 블랜딩 중이 아니라면 
				{   m_Blend_RotKey = m_AniInfo.pRot[i-1].quat;	  }   // 키값을 복사해준다.
				else									// 애니메이션 블랜딩 중이라면
				{
					D3DXQuaternionSlerp(&Q,  &m_Blend_RotKey ,   &Q  ,  _Animator.GetBlendingTime()/_Animator.GetBlendMaxTime()    );
				}

				D3DXMatrixRotationQuaternion(&retM, &Q);
				break;
			}
		}
	}
	return retM;
}

//-----------------------------------------------------------------------------
//  현재 _CurTick에 대한 RotTM을 구한다.
//-----------------------------------------------------------------------------
D3DXMATRIX		SHR_Mesh::_GetCurr_ScaleTM(CAnimator &_Animator)
{
	float _CurTick = _Animator.GetAniTime();

	D3DXVECTOR3 V;
	D3DXQUATERNION Q;
	float ProgRate = 0.0f ;//보간 진행률 
	D3DXMATRIX retM ,SM, SAxisM, Inv, SAxisInv; //  최종 반환 매트릭스 

	retM = m_MeshInfo.LTM_scale;
	if(m_AniInfo.pScale != NULL)
	{
		int len = m_AniInfo.ScaleSize;
		int startIdx = (_CurTick* (float)m_AniInfo.ScaleSize /m_AniInfo.LastTime)+10;
		if(startIdx>=len) startIdx = len-1;

		for(int i=startIdx; i>=1 ; i--)
		{
			if(m_AniInfo.pScale[i-1].Curtick < _CurTick)
			{
				ProgRate = (_CurTick - m_AniInfo.pScale[i-1].Curtick) / (m_AniInfo.pScale[i].Curtick - m_AniInfo.pScale[i-1].Curtick);

				D3DXVec3Lerp(&V, &m_AniInfo.pScale[i-1].scale ,  &m_AniInfo.pScale[i].scale,  ProgRate);
				D3DXQuaternionSlerp(&Q,   &m_AniInfo.pScale[i-1].quat, &m_AniInfo.pScale[i].quat,  ProgRate);
				
				if(_Animator.is_BlendStart() == false ) // 애니메이션 블랜딩 중이 아니라면 
				{   
					m_Blend_ScalePosKey = V;
					m_Blend_ScaleRotKey = Q;
				}   // 키값을 복사해준다.
				else									// 애니메이션 블랜딩 중이라면
				{
					D3DXVec3Lerp(&V,  &m_Blend_ScalePosKey ,   &V  ,  _Animator.GetBlendingTime()/_Animator.GetBlendMaxTime()    );
					D3DXQuaternionSlerp(&Q,  &m_Blend_ScaleRotKey ,   &Q  ,  _Animator.GetBlendingTime()/_Animator.GetBlendMaxTime()    );
				}

				D3DXMatrixScaling(&SM, V.x, V.y, V.z);
				D3DXMatrixRotationQuaternion(&SAxisM, &Q);

				break;


			}
			else if(m_AniInfo.pScale[i-1].Curtick == _CurTick)
			{	

				V = m_AniInfo.pScale[i-1].scale;
				Q = m_AniInfo.pScale[i-1].quat;

				if(_Animator.is_BlendStart() == false ) // 애니메이션 블랜딩 중이 아니라면 
				{   
					m_Blend_ScalePosKey = V;
					m_Blend_ScaleRotKey = Q;
				}   // 키값을 복사해준다.
				else									// 애니메이션 블랜딩 중이라면
				{
					D3DXVec3Lerp(&V,  &m_Blend_ScalePosKey ,   &V  ,  _Animator.GetBlendingTime()/_Animator.GetBlendMaxTime()    );
					D3DXQuaternionSlerp(&Q,  &m_Blend_ScaleRotKey ,   &Q  ,  _Animator.GetBlendingTime()/_Animator.GetBlendMaxTime()    );
				}

				D3DXMatrixScaling(&SM, V.x,  V.y,  V.z);
				D3DXMatrixRotationQuaternion(&SAxisM, &Q);

				break;
			}
		}
		D3DXMatrixInverse(&SAxisInv, NULL, &SAxisM);

		retM = SAxisInv * SM * SAxisM;
	}
	return retM;
}

//-----------------------------------------------------------------------------
// Arg    : IResource* _pRsc
// Return : Succ(S_OK), Fail(S_FALSE)
// Desc   : 다이나믹 캐스팅하여 검사한후 셰이더 포인터를 셋팅해준다.
//-----------------------------------------------------------------------------
int		SHR_Mesh::SetVtxShader(IResource* _pRsc)
{
	if(_pRsc ==	nullptr)
	{
		SHUtil::MsgBox("SHR_Mesh::SetVtxShader(), _pRsc 가 Nullptr입니다.");
		assert(false);
	}

	SHR_VtxShader* pVS = dynamic_cast<SHR_VtxShader*>(_pRsc);
	if(pVS !=nullptr)
	{
		m_pVtxShader = pVS;
		return S_OK;
	}

	return S_FALSE;
}


//-----------------------------------------------------------------------------
// Arg    : IResource* _pRsc
// Return : Succ(S_OK), Fail(S_FALSE)
// Desc   : 다이나믹 캐스팅하여 검사한후 셰이더 포인터를 셋팅해준다.
//-----------------------------------------------------------------------------
int		SHR_Mesh::SetPixShader(IResource* _pRsc)
{
	if(_pRsc ==	nullptr)
	{
		SHUtil::MsgBox("SHR_Mesh::SetPixShader(), _pRsc 가 Nullptr입니다.");
		assert(false);
	}

	SHR_PixShader* pPS = dynamic_cast<SHR_PixShader*>(_pRsc);
	if(pPS !=nullptr)
	{
		m_pPixShader = pPS;
		return S_OK;
	}
	
	return S_FALSE;
}



//-------------------------------------------------------------------------
// 이름을 설정하는 함수
//-------------------------------------------------------------------------
void	SHR_Mesh::SetName(const WCHAR* _pName)
{
	if(_pName == nullptr)
	{	SHUtil::MsgBox("SHR_Mesh::SetName(), _pName 이 nullptr 이다.!!");
		assert(false);
	}

	wcscpy_s(m_Name, _pName);
	m_NameSize = sizeof(WCHAR) * wcslen(m_Name);

}

//-----------------------------------------------------------------------------
// Arg    : 
// Return : 
// Desc   :
//-----------------------------------------------------------------------------
void	SHR_Mesh::Set_m_hTech(const char* _str)
{
	m_hTech = G_FXShader.GetPointer()->GetTechniqueByName(_str);
}


///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
// 여기서부터 SHR_SkinMesh  class  구현
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////




//-------------------------------------------------------------------------
// 생성자
//-------------------------------------------------------------------------
SHR_SkinMesh::SHR_SkinMesh()
{
	m_pMatPalette = nullptr;
	eResourceType = eRT_SKIN_MESH; 
	memset(m_pBoneName, NULL, sizeof(char)*Max_Bone*Max_Name_Len);
}
//-----------------------------------------------------------------------------
// 생성자2
// Arg : const WCHAR* _Name, const WCHAR* _Path = nullptr
//-----------------------------------------------------------------------------
SHR_SkinMesh::SHR_SkinMesh(const WCHAR* _Name , const WCHAR* _Path /*= nullptr*/ )
{	
	
	assert(_Name != nullptr);

	m_pMatPalette = nullptr;
	eResourceType = eRT_SKIN_MESH;
	
	m_NameSize = sizeof(WCHAR)*wcslen(_Name);
	wcscpy_s(m_Name,_Name);

	if(_Path != nullptr)
	{
		m_PathSize = sizeof(WCHAR)*wcslen(_Path);
		wcscpy_s(m_Path,_Path);
	}
	else
	{			
		wcscpy_s(m_Path,L"./object/");
		wcscat_s(m_Path,_Name);
		wcscat_s(m_Path,L".shrsm");
		m_PathSize = sizeof(WCHAR)*wcslen(m_Path);
	}

	memset(m_pBoneName, NULL, sizeof(char)*Max_Bone*Max_Name_Len);

}

//-------------------------------------------------------------------------
// 소멸자
//-------------------------------------------------------------------------
SHR_SkinMesh::~SHR_SkinMesh()	
{
	SAFE_DELETE_ARR(m_pMatPalette)
}


//-------------------------------------------------------------------------
// 스킨드 메쉬 업데이트 함수
//-------------------------------------------------------------------------
void	SHR_SkinMesh::Update(CAnimator& m_Animator ,   D3DXMATRIX* pMoveTM/* = nullptr*/)
{

	int i = 0;
	if(m_BoneCnt > 0)
	{
		//메트릭스 팔레트 만들어주기
		std::vector<SHR_Mesh*>::iterator Iter;

		for(Iter = m_Vec_BoneList.begin()  ; Iter != m_Vec_BoneList.end() ; ++Iter)
		{
			SHR_Mesh* temp = (*Iter);
			m_pMatPalette[i] = (*Iter)->m_MeshInfo.BoneIniMat * (*Iter)->m_WorldTM;
			i++;
		}
	}

}



//-------------------------------------------------------------------------
//  스킨드 메쉬 그리기
//-------------------------------------------------------------------------
void	SHR_SkinMesh::Draw( LPDIRECT3DDEVICE9 _device)
{

	G_FXShader.GetPointer()->SetTechnique(m_hTech);// [2014.10.30 ]

	UINT MaxPass = 0;

	G_FXShader.m_pShader->Begin(&MaxPass, NULL);//테크닉 시작 [2014.10.29 ]

	for(UINT PassNum = 0; PassNum < MaxPass ; PassNum++ ) // 패스별로 그리기 [2014.11.21 ]
	{
		G_FXShader.GetPointer()->BeginPass(PassNum);// [2014.10.29 ]

		// 렌더타깃 위에 그린다.

		if( SUCCEEDED( G_ScreenQuad.g_pRenderTargetTexPass[0/*PassNum 일단 0으로..[2014.11.19 ]*/]->GetSurfaceLevel( 0, &(G_ScreenQuad.g_pSceneSurface[0]) ) ) )
		{//gpSceneRenderTarget은 서페이스를 만들어줄때 필요한 녀석 [2014.11.7 ]
			_device->SetRenderTarget( 0, G_ScreenQuad.g_pSceneSurface[0] ); // 만든 서페이스를 렌더타겟으로 지정(복사) [2014.11.7 ]
		}

	
			// 메트릭스 팔레트를 셰이더 상수로  전달 [2014.10.30 ]
			G_FXShader.GetPointer()->SetMatrixArray(G_ConstantTable.h_MatPallete , m_pMatPalette, m_BoneCnt );
	
			if(m_FaceSetCnt >0)
			{
				for(int i=0; i<m_FaceSetCnt ; i++)
				{
					m_pFaceSet[i].Draw(_device); //direct9.0c에서 제공해주는 매트릭스 팔레트를 이용한 그리기
				}
			}

		G_FXShader.GetPointer()->EndPass();// 패스별로 그리기 끝 [2014.10.29 ]
	}

	G_FXShader.GetPointer()->End();//테크닉 끝 [2014.10.29 ]


}





//-----------------------------------------------------------------------------
// 바이너리로 저장된 파일을 읽어 정보를 저장한다.
// Desc : _Path == nullptr 이면 이미 m_Path가 저장되어있다고 가정하고 사용하는 경우이다.!!
//-----------------------------------------------------------------------------
int	SHR_SkinMesh::LoadResource(const WCHAR* _Path/* = nullptr*/)
{
	int hr = S_OK;

	FILE* fp = NULL;
	errno_t err;
	
	if(_Path != nullptr)
	{
		wcscpy_s(m_Path, _Path);
	}
	err	= _wfopen_s(&fp, m_Path, L"rb");

	if(err != 0)
	{
		// 파일 읽기 실패
		SHUtil::MsgBox("SHR_SkinMesh::LoadResource(),  파일읽기 실패");assert(false);
		fclose(fp);
		return S_FALSE;
	}

	bool isVS, isPS  ;
	
	fread(&isVS, sizeof(isVS),1,fp);

	if(isVS == true)
	{	
		WCHAR TempPath[Max_Path_Len] = {NULL,}; WCHAR TempName[Max_Name_Len] = {NULL,};
		int size;
		fread(&size, sizeof(int),1 , fp);   
		fread(TempName, size, 1, fp);		
		fread(&size, sizeof(int),1 , fp);
		//m_pVtxShader->SetPathSize(size);
		fread(TempPath, size, 1, fp);
		//m_pVtxShader->SetPath(TempPath);
		IResource* pRsc = CResourceMgr::getInstance()->_LoadResource(TempPath,TempName, eRT_VTXSHADER);
		SetVtxShader(pRsc);
	}

	fread(&isPS, sizeof(isPS),1,fp);

	if(isPS == true)
	{	
		WCHAR TempPath[Max_Path_Len] = {NULL,}; WCHAR TempName[Max_Name_Len] = {NULL,};
		int size;
		fread(&size, sizeof(int),1 , fp);   
		fread(TempName, size, 1, fp);	
		fread(&size, sizeof(int),1 , fp);    
		fread(TempPath, size, 1, fp);		
		IResource* pRsc = CResourceMgr::getInstance()->_LoadResource(TempPath, TempName, eRT_PIXSHADER);
		SetPixShader(pRsc);
	}




	fread(&m_MeshInfo, sizeof(m_MeshInfo), 1, fp);
	fread(&m_FaceSetCnt, sizeof(m_FaceSetCnt) , 1, fp);
	if(m_FaceSetCnt > 0)
	{
		m_pFaceSet = new FaceSet[m_FaceSetCnt];

		for(int i = 0 ; i <  m_FaceSetCnt ; i++)
		{
			//메테리얼 읽기
			fread(&(m_pFaceSet[i].m_Material), sizeof(m_pFaceSet[i].m_Material),1,fp);

			//--------------------------------------------------------------------------------------------------------------------------------------------------
			{			
				// 텍스쳐 디퓨즈맵 // [2014.10.30 ]
				int Texpathsize=0; WCHAR Texpath[Max_Path_Len]={NULL,}; WCHAR Texname[Max_Name_Len]={NULL,};
				fread(&Texpathsize, sizeof(int), 1, fp);

				if(Texpathsize > 0)
				{
					fread(Texpath, Texpathsize,1,fp);
					// [2014.10.30 ] cutSlash(Texpath, Texname);
					IResource* tempTex = CResourceMgr::getInstance()->_LoadResource(Texpath, Texpath, eRT_TEXTURE);// [2014.10.30 ] 그냥 이름을 패쓰로해서 유일한 아이디로 사용
					m_pFaceSet[i].m_pTexDiff = dynamic_cast<SHR_D3D9Texture*>(tempTex);

					if(m_pFaceSet[i].m_pTexDiff == nullptr)
					{	SHUtil::MsgBox("dynamic_cast<SHR_D3D9Texture*>(m_pTexDiff); 실패");
						hr=S_FALSE;
					}
				}
			}
			//--------------------------------------------------------------------------------------------------------------------------------------------------
		
			//--------------------------------------------------------------------------------------------------------------------------------------------------
			// 텍스쳐 스페큘러맵 // [2014.10.30 ]
			{
				int Texpathsize=0; WCHAR Texpath[Max_Path_Len]={NULL,}; WCHAR Texname[Max_Name_Len]={NULL,};
				fread(&Texpathsize, sizeof(int), 1, fp);

				if(Texpathsize > 0)
				{
					fread(Texpath, Texpathsize,1,fp);
					// [2014.10.30 ] cutSlash(Texpath, Texname);
					IResource* tempTex = CResourceMgr::getInstance()->_LoadResource(Texpath, Texpath, eRT_TEXTURE);// [2014.10.30 ] 그냥 이름을 패쓰로해서 유일한 아이디로 사용
					m_pFaceSet[i].m_pTexSpecularMap = dynamic_cast<SHR_D3D9Texture*>(tempTex);

					if(m_pFaceSet[i].m_pTexSpecularMap == nullptr)
					{	SHUtil::MsgBox("dynamic_cast<SHR_D3D9Texture*>(m_pTexSpecularMap); 실패");
						hr=S_FALSE;
					}
				}
			}

			//--------------------------------------------------------------------------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------------------------------------------------------------------------
			// 텍스쳐  범프맵 // [2014.10.30 ]
			{
				int Texpathsize=0; WCHAR Texpath[Max_Path_Len]={NULL,}; WCHAR Texname[Max_Name_Len]={NULL,};
				fread(&Texpathsize, sizeof(int), 1, fp);

				if(Texpathsize > 0)
				{
					fread(Texpath, Texpathsize,1,fp);
					// [2014.10.30 ] cutSlash(Texpath, Texname);
					IResource* tempTex = CResourceMgr::getInstance()->_LoadResource(Texpath, Texpath, eRT_TEXTURE);// [2014.10.30 ] 그냥 이름을 패쓰로해서 유일한 아이디로 사용
					m_pFaceSet[i].m_pTexBumpMap = dynamic_cast<SHR_D3D9Texture*>(tempTex);

					if(m_pFaceSet[i].m_pTexBumpMap == nullptr)
					{	SHUtil::MsgBox("dynamic_cast<SHR_D3D9Texture*>(m_pTexBumpMap); 실패");
					    hr=S_FALSE;
					}
				}
			}
			//--------------------------------------------------------------------------------------------------------------------------------------------------
			// 텍스쳐  Emissive맵 // [2014.10.30 ]
			{
				int Texpathsize=0; WCHAR Texpath[Max_Path_Len]={NULL,}; WCHAR Texname[Max_Name_Len]={NULL,};
				fread(&Texpathsize, sizeof(int), 1, fp);

				if(Texpathsize > 0)
				{
					fread(Texpath, Texpathsize,1,fp);
					// [2014.10.30 ] cutSlash(Texpath, Texname);
					IResource* tempTex = CResourceMgr::getInstance()->_LoadResource(Texpath, Texpath, eRT_TEXTURE);// [2014.10.30 ] 그냥 이름을 패쓰로해서 유일한 아이디로 사용
					m_pFaceSet[i].m_pTexEmissiveMap = dynamic_cast<SHR_D3D9Texture*>(tempTex);

					if(m_pFaceSet[i].m_pTexEmissiveMap == nullptr)
					{	SHUtil::MsgBox("dynamic_cast<SHR_D3D9Texture*>(m_pTexEmissiveMap); 실패");
						hr=S_FALSE;
					}
				}
			}

			//--------------------------------------------------------------------------------------------------------------------------------------------------


			// 버텍스버퍼			
			if(m_MeshInfo.HaveMesh == true)
			{	
				void* pVtxBuf;

				m_pFaceSet[i].m_pVtxBuff = new SHR_VtxBuff();
				m_pFaceSet[i].m_pIdxBuf = new SHR_IdxBuff();
				int tempVal = 0; int NameSize; WCHAR tempVName[Max_Name_Len] = {NULL,};WCHAR tempIName[Max_Name_Len] = {NULL,};

				fread(&tempVal , sizeof(int) , 1, fp);      m_pFaceSet[i].m_pVtxBuff->SetNumVtx(tempVal);
				fread(&tempVal, sizeof(int) , 1, fp);		 m_pFaceSet[i].m_pVtxBuff->SetStrideSize(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);		 m_pFaceSet[i].m_pVtxBuff->SetBuffSize(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);		 m_pFaceSet[i].m_pVtxBuff->SetFvF(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);		 m_pFaceSet[i].m_pVtxBuff->SetPoolType(tempVal);
				pVtxBuf = (void*)malloc(m_pFaceSet[i].m_pVtxBuff->GetBuffSize());
				fread(pVtxBuf, m_pFaceSet[i].m_pVtxBuff->GetStrideSize(),  m_pFaceSet[i].m_pVtxBuff->GetNumVtx(), fp);
				//MY_VERTEX_SKINED* v1 = (MY_VERTEX_SKINED*)pVtxBuf;
				fread(&NameSize, sizeof(int), 1, fp);		 m_pFaceSet[i].m_pVtxBuff->SetNameSize(NameSize);// [2014.10.9 ]
				fread(tempVName, NameSize, 1, fp);			 m_pFaceSet[i].m_pVtxBuff->SetName(tempVName); //[2014.10.9 ]
				m_pFaceSet[i].m_pVtxBuff->Create();
				m_pFaceSet[i].m_pVtxBuff->WriteBuf(pVtxBuf);
				CResourceMgr::getInstance()->RegisterRsc(m_pFaceSet[i].m_pVtxBuff);//[2014.10.9 ]
				SAFE_DELETE_ARR(pVtxBuf)
				//인덱스버퍼
				void* pIdxBuf;

				fread(&tempVal , sizeof(int) , 1, fp);		m_pFaceSet[i].m_pIdxBuf->SetNumIdx(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);		m_pFaceSet[i].m_pIdxBuf->SetNumFace(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);		m_pFaceSet[i].m_pIdxBuf->SetBuffSize(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);		m_pFaceSet[i].m_pIdxBuf->SetPoolType(tempVal);
				pIdxBuf = (void*)malloc(m_pFaceSet[i].m_pIdxBuf->GetBuffSize());
				fread(pIdxBuf, sizeof(WORD),m_pFaceSet[i].m_pIdxBuf->GetNumIdx() , fp);
				m_pFaceSet[i].m_pIdxBuf->Create();
				m_pFaceSet[i].m_pIdxBuf->WriteBuf(pIdxBuf);
				fread(&NameSize, sizeof(int), 1, fp);		 m_pFaceSet[i].m_pIdxBuf->SetNameSize(NameSize);// [2014.10.9 ]
				fread(tempIName, NameSize, 1, fp);			 m_pFaceSet[i].m_pIdxBuf->SetName(tempIName); //[2014.10.9 ]
				CResourceMgr::getInstance()->RegisterRsc(m_pFaceSet[i].m_pIdxBuf);//[2014.10.9 ]
				SAFE_DELETE_ARR(pIdxBuf)
			}
		
		}
	}	

	// Ani_Info 읽기
// 	fread(&m_AniInfo, sizeof(m_AniInfo), 1, fp);
// 	if(m_AniInfo.PosSize > 0)
// 		fread(m_AniInfo.pPos,sizeof(CONTROL_Pos), m_AniInfo.PosSize, fp);
// 	if(m_AniInfo.RotSize > 0)
// 		fread(m_AniInfo.pRot,sizeof(CONTROL_Rot), m_AniInfo.RotSize, fp);
// 	if(m_AniInfo.ScaleSize > 0)
// 		fread(m_AniInfo.pScale,sizeof(CONTROL_Scale), m_AniInfo.ScaleSize, fp);


	fread(&m_BoneCnt, sizeof(m_BoneCnt), 1, fp);
	for(int i = 0 ; i< m_BoneCnt; i++)
	{
		int len;
		fread(&len, sizeof(len), 1, fp);
		fread(m_pBoneName[i],sizeof(CHAR) * len , 1, fp);
	}


	fclose(fp);

	return hr;

}




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 SHR_BoneList 함수정의
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// 기본 생성자
//-----------------------------------------------------------------------------
SHR_BoneList::SHR_BoneList()
{
	eResourceType = eRT_BONE_LIST;
	m_bShow = true;
};

//-----------------------------------------------------------------------------
// Arg    : const WCHAR* _Name, const WCHAR* _Path = nullptr
// Desc   : 이름과 패쓰로 생성
//-----------------------------------------------------------------------------
SHR_BoneList::SHR_BoneList(const WCHAR* _Name, const WCHAR* _Path /*= nullptr*/)
{
	eResourceType = eRT_BONE_LIST;

	assert(_Name != nullptr);
	
	m_NameSize = sizeof(WCHAR)*wcslen(_Name);
	wcscpy_s(m_Name,_Name);

	if(_Path != nullptr)
	{
		m_PathSize = sizeof(WCHAR)*wcslen(_Path);
		wcscpy_s(m_Path,_Path);
	}
	else
	{			
		wcscpy_s(m_Path,L"./object/");
		wcscat_s(m_Path,_Name);
		wcscat_s(m_Path,L".shrml");
		m_PathSize =sizeof(WCHAR)* wcslen(m_Path);
	}

}
//-----------------------------------------------------------------------------
// 소멸자
//-----------------------------------------------------------------------------
SHR_BoneList::~SHR_BoneList()
{

};


//-------------------------------------------------------------------------
// = 연산자 오버로딩
//-------------------------------------------------------------------------
// SHR_BoneList&	SHR_BoneList::operator=(SHR_BoneList &_rhs)
// {
// 	m_Vec_MeshList.resize((int)(_rhs.m_Vec_MeshList.size())); 
// 
// 	std::copy( sourceVector.begin(), sourceVector.end(), destVector.begin() ); 
// 
// 	std::copy( sourceVector.begin() + 2, sourceVector.begin() + 5, destVector.begin() ); 
// 
// 
// 	std::vector<SHR_Mesh*>				m_Vec_MeshList; // 가지고 있는 업데이트 할 모든 메쉬 리스트
// 	bool								m_bShow;       
// 	SHR_VtxShader*						m_pVtxShader;   // 버텍스 셰이더
// 	SHR_PixShader*						m_pPixShader;   // 픽셀 셰이더
// 	ANI_INFO							m_BL_AniInfo;   // BoneList_Ani_Info
// 	return *this;
// }



//-----------------------------------------------------------------------------
// 업데이트
// Arg :  CAni_Timer* _AniTimer 는 각 뼈마다 적용될 애니 타이머이다.
//-----------------------------------------------------------------------------
void	SHR_BoneList::Update(CAnimator& _Animator ,D3DXMATRIX* pMoveTM/* = nullptr*/)
{
	//메쉬 전부 업데이트
	int idx = 0;
	
	//_Animator.AniTimeUpdate(); // BoneList 전체에 적용될 타이머 업데이트 // [2014.10.9 ]

	if(_Animator.GetEndTime() < _Animator.GetAniTime())   // [2014.10.9 ]
	{	
		if(_Animator.GetCurClipInfo() != nullptr )
		{
			if(_Animator.GetCurClipInfo()->isLoop == true)
			{_Animator.SetAniTimeFirst();	}
			else
			{	_Animator.GetCurClipInfo()->eAniState = eAniState_Stop;
				//_Animator.Change_Ani_Clip("IDLE"); ★★★ 원인확인하기 ★★★ //[2014.11.27 ]
			}

		}
		
	}			

	std::vector<SHR_Mesh*>::iterator IterBone;
	for( IterBone = m_Vec_MeshList.begin() ; IterBone !=m_Vec_MeshList.end() ; ++IterBone)
	{

		(*IterBone)->UpdateBone( _Animator ,  pMoveTM);
// 		if(strcmp((*IterBone)->m_MeshInfo.Name ,"Hat") == 0)
// 		{
// 			LOG_WRITE_A("Hat > 41 : %f , 42 : %f, 43 : %f",(*IterBone)->m_WorldTM._41, (*IterBone)->m_WorldTM._42, (*IterBone)->m_WorldTM._43);
// 		}

// 		if(strcmp((*IterBone)->m_MeshInfo.ParentName ,"Hat") == 0)
// 		{
// 			LOG_WRITE_A("BoundSphere > 41 : %f , 42 : %f, 43 : %f",(*IterBone)->m_WorldTM._41, (*IterBone)->m_WorldTM._42, (*IterBone)->m_WorldTM._43);
// 		}
		int a = 0;
	}

}



//-----------------------------------------------------------------------------
// 그리기
//-----------------------------------------------------------------------------
void	SHR_BoneList::Draw( LPDIRECT3DDEVICE9 _device)
{
	if(m_bShow == false)
		return;

	// 뼈대 전부 그리기
	std::vector<SHR_Mesh*>::iterator Iter, IterEnd;
	IterEnd = m_Vec_MeshList.end();


	for( Iter = m_Vec_MeshList.begin() ; Iter != IterEnd ; ++Iter) // [2014.11.12 ] 
	{
		(*Iter)->Draw(_device);
	}

}


//-------------------------------------------------------------------------
// Arg	  : SHR_Mesh* _pNewMesh, const WCHAR* pParentName = nullptr
// Return : return S_FALSE or S_OK 
// Desc   : _pNewMesh 를 m_Vec_MeshList에 추가시켜주고 pParentName 이름으로 부모를 찾아 연결시켜준다. 
//			만약  pParentName가 nullptr 이면 그냥 추가만한다. // [2014.11.20 ]
//-------------------------------------------------------------------------
int		SHR_BoneList::AddMesh(SHR_Mesh* _pNewMesh, const WCHAR* pParentName/* = nullptr*/)	
{
	// 에러처리 [2014.11.20 ]
	if(_pNewMesh == nullptr) return S_FALSE;

	m_Vec_MeshList.push_back(_pNewMesh);

	if(pParentName !=nullptr)
	{
		SetParent(_pNewMesh); // 부모포인터 찾아주기   [2014.11.20 ]
	}

	return S_OK;
}


//-----------------------------------------------------------------------------
// Arg    : SHR_MeshList
// Desc   : SHR_MeshList의 안에 있는 메쉬들을 돌면서 부모가 있다면 본의 부모를  추가한다. // [2014.11.27 ]
//-----------------------------------------------------------------------------
void	SHR_BoneList::ConnectParent_From_MeshList(SHR_MeshList* _pMeshList)
{
	if(_pMeshList == nullptr) 
	{
		SHUtil::MsgBox("ConnectParent_From_MeshList(SHR_MeshList* _pMeshList), Fail ,_pMeshList 가 nullptr "); return;
	}
	int size = m_Vec_MeshList.size();

	if(size > 0)
	{	
		//계층구조 만들어주기
		for( std::vector<SHR_Mesh*>::iterator IterBone = m_Vec_MeshList.begin();IterBone != m_Vec_MeshList.end(); ++IterBone )
		{
			for(std::vector<SHR_Mesh*>::iterator IterMesh = _pMeshList->m_Vec_MeshList.begin() ; IterMesh !=_pMeshList->m_Vec_MeshList.end() ; ++IterMesh )
			{
				//char* TempName = (*Iter)->m_MeshInfo.Name;
				if(strcmp((*IterBone)->m_MeshInfo.ParentName, (*IterMesh)->m_MeshInfo.Name) == 0)
				{
					(*IterBone)->m_pParent = (*IterMesh);
					break;
				}
			}
		}

	}

}


//-----------------------------------------------------------------------------
// Arg    : SHR_Mesh*
// Desc   : SHR_Mesh의 부모의 포인터를 m_Vec_MeshList 안에서 찾아 부모로 만든다. 
//-----------------------------------------------------------------------------
void	SHR_BoneList::SetParent(SHR_Mesh* _pMesh)
{
	assert(_pMesh != nullptr);
	int size = m_Vec_MeshList.size();

	if(size > 0)
	{	
		//계층구조 만들어주기
		for( std::vector<SHR_Mesh*>::iterator Iter = m_Vec_MeshList.begin();Iter != m_Vec_MeshList.end(); ++Iter )
		{
			char* TempName = (*Iter)->m_MeshInfo.Name;
			if(strcmp((*Iter)->m_MeshInfo.Name, _pMesh->m_MeshInfo.ParentName) == 0)
			{
				_pMesh->m_pParent = (*Iter);
				break;
			}

		}

	}
}

//-------------------------------------------------------------------------
// Arg	  : const CHAR* pszName 찾을메쉬이름
// Return : const SHR_Mesh*     콘스트포인터  , 못찾으면 nullptr 을 반환  // [2014.11.11 ]	
// Desc   : 이름으로 본메쉬를 찾아준다. 
//-------------------------------------------------------------------------
SHR_Mesh*	SHR_BoneList::FindMeshByName(const CHAR* pszName) 	
{
	//에러처리
	if(pszName == nullptr )
	{assert(false);  SHUtil::MsgBox("SHR_BoneList::FindMeshByName() , 에러!!!!!!!!!!!!!!!!!!! 이름이 널값임");	}

	std::vector<SHR_Mesh*>::iterator Iter, endIter;
	endIter = m_Vec_MeshList.end();

	for(Iter = m_Vec_MeshList.begin() ; Iter != endIter ; ++Iter )
	{
		if(strcmp( (*Iter)->m_MeshInfo.Name , pszName) == 0)
		{
			return (*Iter);
		}
	}

	return nullptr;


}

//-----------------------------------------------------------------------------
// Arg    : D3DXHANDLE _hTech , 셰이더 테크닉핸들
// Return : 
// Desc   : m_Vec_MeshList 안에있는 모든 메쉬들에게 _hTech 를 장착한다. [2014.11.21 ]
//-----------------------------------------------------------------------------
void	SHR_BoneList::Set_hTech_AllMesh(const char* _str)
{
	std::vector<SHR_Mesh*>::iterator IterCurr, IterEnd;
	IterEnd = m_Vec_MeshList.end();

	for(IterCurr = m_Vec_MeshList.begin() ; IterCurr != IterEnd ;  ++IterCurr  )
	{
		(*IterCurr)->Set_m_hTech(_str); 
	}
}


//-----------------------------------------------------------------------------
// Arg    : D3DXHANDLE _hTech , 셰이더 테크닉핸들
// Return : S_OK(같은이름을 찾았다) , S_FALSE(같은이름을 가진 메쉬가 리스트에 없다.)
// Desc   : 메쉬 이름으로 m_Vec_MeshList 안에있는 메쉬를 찾아 _hTech를 설정해준다.  [2014.11.21 ]
//-----------------------------------------------------------------------------
HRESULT 	SHR_BoneList::Set_hTech_byMeshName(const char* _str, const CHAR* pszName)
{

	// 에러처리
	if(pszName == nullptr)
	{SHUtil::MsgBox("SHR_BoneList::Set_hTech_byMeshName() ,pszName가  nullptr "); return S_FALSE; }

	std::vector<SHR_Mesh*>::iterator IterCurr, IterEnd;
	IterEnd = m_Vec_MeshList.end();

	for(IterCurr = m_Vec_MeshList.begin() ; IterCurr != IterEnd ;  ++IterCurr  )
	{
		if(strcmp((*IterCurr)->m_MeshInfo.Name, pszName) == 0 ) // 같은 이름의 메쉬를 찾았다면 [2014.11.21 ]
		{
			(*IterCurr)->Set_m_hTech(_str); 
			return S_OK;
		}
	}

	return S_FALSE;
}

//-----------------------------------------------------------------------------
// 자체포멧에서 데이터를 읽어온다.
//-----------------------------------------------------------------------------
int	SHR_BoneList::LoadResource(const WCHAR* _Path/* = nullptr*/)
{
	int hr = S_OK;
	bool BLAni_once = true;

	FILE* fp = NULL;
	errno_t err;

	if(_Path != nullptr)
	{
		wcscpy_s(m_Path, _Path);
	}
	
	err	= _wfopen_s(&fp, m_Path, L"rb");
	
	if(err != 0)
	{
		// 로드 파일 생성 실패..
		LOG_WRITE_A("[SHR_BoneList::LoadResource()] 에러!!!!!!!!!!!!!, 로드실패");
		SHUtil::MsgBox("[SHR_BoneList::LoadResource()] 에러!, 로드실패");
		fclose(fp);
		return S_FALSE;
	}
	int VecSize ;
	fread(&VecSize, sizeof(int), 1,fp);
	
	for(int VecNum = 0 ;VecNum < VecSize ; VecNum++)
	{	
		
		SHR_Mesh* tempMesh = new SHR_Mesh();

		//////////////////////////////////////////////////////////////////////////
		// 메쉬인포 저장
		fread(&(tempMesh->m_MeshInfo), sizeof(tempMesh->m_MeshInfo), 1, fp);
		WCHAR MeshName[Max_Name_Len];
		MultiByteToWideChar(CP_ACP, NULL, tempMesh->m_MeshInfo.Name, -1, MeshName,sizeof(WCHAR)*Max_Name_Len);
		tempMesh->SetName(MeshName);// 메쉬의 이름 설정

		//////////////////////////////////////////////////////////////////////////
		// faceset 저장
		fread(&(tempMesh->m_FaceSetCnt), sizeof(tempMesh->m_FaceSetCnt),1,fp);
		if(tempMesh->m_FaceSetCnt > 0)
		{		tempMesh->m_pFaceSet = new FaceSet[tempMesh->m_FaceSetCnt];	}

		for(int faceNum = 0; faceNum< tempMesh->m_FaceSetCnt ; faceNum++)
		{	// 메테리얼
			fread(&(tempMesh->m_pFaceSet[faceNum].m_Material), sizeof(tempMesh->m_pFaceSet[faceNum].m_Material),1,fp);

			//--------------------------------------------------------------------------------------------------------------------------------------------------
			{			
				// 텍스쳐 디퓨즈맵 // [2014.10.30 ]
				int Texpathsize=0; WCHAR Texpath[Max_Path_Len]={NULL,}; WCHAR Texname[Max_Name_Len]={NULL,};
				fread(&Texpathsize, sizeof(int), 1, fp);

				if(Texpathsize > 0)
				{
					fread(Texpath, Texpathsize,1,fp);
					// [2014.10.30 ] cutSlash(Texpath, Texname);
					IResource* tempTex = CResourceMgr::getInstance()->_LoadResource(Texpath, Texpath, eRT_TEXTURE);// [2014.10.30 ] 그냥 이름을 패쓰로해서 유일한 아이디로 사용
					tempMesh->m_pFaceSet[faceNum].m_pTexDiff = dynamic_cast<SHR_D3D9Texture*>(tempTex);

					if(tempMesh->m_pFaceSet[faceNum].m_pTexDiff == nullptr)
					{	SHUtil::MsgBox("dynamic_cast<SHR_D3D9Texture*>(m_pTexDiff); 실패");
					hr=S_FALSE;
					}
				}
			}
			//--------------------------------------------------------------------------------------------------------------------------------------------------

			//--------------------------------------------------------------------------------------------------------------------------------------------------
			// 텍스쳐 스페큘러맵 // [2014.10.30 ]
			{
				int Texpathsize=0; WCHAR Texpath[Max_Path_Len]={NULL,}; WCHAR Texname[Max_Name_Len]={NULL,};
				fread(&Texpathsize, sizeof(int), 1, fp);

				if(Texpathsize > 0)
				{
					fread(Texpath, Texpathsize,1,fp);
					// [2014.10.30 ] cutSlash(Texpath, Texname);
					IResource* tempTex = CResourceMgr::getInstance()->_LoadResource(Texpath, Texpath, eRT_TEXTURE);// [2014.10.30 ] 그냥 이름을 패쓰로해서 유일한 아이디로 사용
					tempMesh->m_pFaceSet[faceNum].m_pTexSpecularMap = dynamic_cast<SHR_D3D9Texture*>(tempTex);

					if(tempMesh->m_pFaceSet[faceNum].m_pTexSpecularMap == nullptr)
					{	SHUtil::MsgBox("dynamic_cast<SHR_D3D9Texture*>(m_pTexSpecularMap); 실패");
					hr=S_FALSE;
					}
				}
			}

			//--------------------------------------------------------------------------------------------------------------------------------------------------
			//--------------------------------------------------------------------------------------------------------------------------------------------------
			// 텍스쳐  범프맵 // [2014.10.30 ]
			{
				int Texpathsize=0; WCHAR Texpath[Max_Path_Len]={NULL,}; WCHAR Texname[Max_Name_Len]={NULL,};
				fread(&Texpathsize, sizeof(int), 1, fp);

				if(Texpathsize > 0)
				{
					fread(Texpath, Texpathsize,1,fp);
					// [2014.10.30 ] cutSlash(Texpath, Texname);
					IResource* tempTex = CResourceMgr::getInstance()->_LoadResource(Texpath, Texpath, eRT_TEXTURE);// [2014.10.30 ] 그냥 이름을 패쓰로해서 유일한 아이디로 사용
					tempMesh->m_pFaceSet[faceNum].m_pTexBumpMap = dynamic_cast<SHR_D3D9Texture*>(tempTex);

					if(tempMesh->m_pFaceSet[faceNum].m_pTexBumpMap == nullptr)
					{	SHUtil::MsgBox("dynamic_cast<SHR_D3D9Texture*>(m_pTexBumpMap); 실패");
					hr=S_FALSE;
					}
				}
			}
			//--------------------------------------------------------------------------------------------------------------------------------------------------
			// 텍스쳐  Emissive맵 // [2014.10.30 ]
			{
				int Texpathsize=0; WCHAR Texpath[Max_Path_Len]={NULL,}; WCHAR Texname[Max_Name_Len]={NULL,};
				fread(&Texpathsize, sizeof(int), 1, fp);

				if(Texpathsize > 0)
				{
					fread(Texpath, Texpathsize,1,fp);
					// [2014.10.30 ] cutSlash(Texpath, Texname);
					IResource* tempTex = CResourceMgr::getInstance()->_LoadResource(Texpath, Texpath, eRT_TEXTURE);// [2014.10.30 ] 그냥 이름을 패쓰로해서 유일한 아이디로 사용
					tempMesh->m_pFaceSet[faceNum].m_pTexEmissiveMap = dynamic_cast<SHR_D3D9Texture*>(tempTex);

					if(tempMesh->m_pFaceSet[faceNum].m_pTexEmissiveMap == nullptr)
					{	SHUtil::MsgBox("dynamic_cast<SHR_D3D9Texture*>(m_pTexEmissiveMap); 실패");
					hr=S_FALSE;
					}
				}
			}

			//--------------------------------------------------------------------------------------------------------------------------------------------------

			//////////////////////////////////////////////////////////////////////////
			// 버텍스버퍼
			if(tempMesh->m_MeshInfo.HaveMesh == true)
			{	
				void* pVtxBuf; WCHAR tempVName[Max_Name_Len] = {NULL,};

				tempMesh->m_pFaceSet[faceNum].m_pVtxBuff = new SHR_VtxBuff();
				tempMesh->m_pFaceSet[faceNum].m_pIdxBuf = new SHR_IdxBuff();
				int tempVal = 0;
				fread(&tempVal , sizeof(DWORD) , 1, fp);  tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetNameSize(tempVal);// [2014.10.12 ]
				fread(tempVName , tempVal , 1, fp); tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetName(tempVName);// [2014.10.12 ]
				fread(&tempVal , sizeof(int) , 1, fp);    tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetNumVtx(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);  tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetStrideSize(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);  tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetBuffSize(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);  tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetFvF(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);  tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetPoolType(tempVal);
				pVtxBuf = (void*)malloc(tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->GetBuffSize());
				fread(pVtxBuf,tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->GetBuffSize(), 1, fp);
				tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->Create();
				tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->WriteBuf(pVtxBuf);
				CResourceMgr::getInstance()->RegisterRsc(tempMesh->m_pFaceSet[faceNum].m_pVtxBuff);
				SAFE_DELETE_ARR(pVtxBuf)

				//MY_VERTEX_SKINED* v1 = (MY_VERTEX_SKINED*)m_pFaceSet[i].m_pVtxBuff->m_pVtxBuffPoint;

				//인덱스버퍼
				void* pIdxBuf; WCHAR tempIName[Max_Name_Len] = {NULL,};
				fread(&tempVal , sizeof(DWORD) , 1, fp);  tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetNameSize(tempVal);// [2014.10.12 ]
				fread(tempIName , tempVal , 1, fp); tempMesh->m_pFaceSet[faceNum].m_pVtxBuff->SetName(tempIName);// [2014.10.12 ]
				fread(&tempVal , sizeof(int) , 1, fp);      tempMesh->m_pFaceSet[faceNum].m_pIdxBuf->SetNumIdx(tempVal);
				fread(&tempVal, sizeof(int) , 1, fp);		tempMesh->m_pFaceSet[faceNum].m_pIdxBuf->SetNumFace(tempVal);
				fread(&tempVal , sizeof(int) , 1, fp);		tempMesh->m_pFaceSet[faceNum].m_pIdxBuf->SetBuffSize(tempVal);
				fread(&tempVal, sizeof(int) , 1, fp);		tempMesh->m_pFaceSet[faceNum].m_pIdxBuf->SetPoolType(tempVal);

				pIdxBuf = (void*)malloc(tempMesh->m_pFaceSet[faceNum].m_pIdxBuf->GetBuffSize());
				fread(pIdxBuf, tempMesh->m_pFaceSet[faceNum].m_pIdxBuf->GetBuffSize(), 1,fp);
				tempMesh->m_pFaceSet[faceNum].m_pIdxBuf->Create();
				tempMesh->m_pFaceSet[faceNum].m_pIdxBuf->WriteBuf(pIdxBuf);
				CResourceMgr::getInstance()->RegisterRsc(tempMesh->m_pFaceSet[faceNum].m_pIdxBuf);
				
				SAFE_DELETE_ARR(pIdxBuf)
			}
		}
		// Ani_Info저장
		fread(&tempMesh->m_AniInfo,sizeof(tempMesh->m_AniInfo),1,fp);
		
		

		if((tempMesh->m_AniInfo.Animateable == true) && (BLAni_once))
		{
			m_BL_AniInfo = tempMesh->m_AniInfo;  BLAni_once = false;  // 본리스트의 Ani정보를 저장한다. 
		}

		if(tempMesh->m_AniInfo.PosSize > 0)
		{
			tempMesh->m_AniInfo.pPos = new CONTROL_Pos[tempMesh->m_AniInfo.PosSize+120];
			fread(tempMesh->m_AniInfo.pPos,sizeof(CONTROL_Pos), tempMesh->m_AniInfo.PosSize, fp);
		}

		if(tempMesh->m_AniInfo.RotSize > 0)
		{	
			tempMesh->m_AniInfo.pRot = new CONTROL_Rot[tempMesh->m_AniInfo.RotSize+120];
			fread(tempMesh->m_AniInfo.pRot,sizeof(CONTROL_Rot), tempMesh->m_AniInfo.RotSize, fp);
 		}

		if(tempMesh->m_AniInfo.ScaleSize > 0)
		{
			tempMesh->m_AniInfo.pScale = new CONTROL_Scale[tempMesh->m_AniInfo.ScaleSize+120];
			fread(tempMesh->m_AniInfo.pScale,sizeof(CONTROL_Scale), tempMesh->m_AniInfo.ScaleSize, fp);
		}

		SetParent(tempMesh);
		m_Vec_MeshList.push_back(tempMesh);
		CResourceMgr::getInstance()->RegisterRsc(tempMesh);//[2014.10.12 ]
	}

	fclose(fp);
	LOG_WRITE_A("[SHR_BoneList::LoadResource()] , 성공!");
	return S_OK;
}




//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//
// 여기서부터 SHR_MeshList 함수정의  [2014.11.12 ]
//
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////


//-----------------------------------------------------------------------------
// 기본 생성자
//-----------------------------------------------------------------------------
SHR_MeshList::SHR_MeshList()
{
	eResourceType = eRT_BONE_LIST;
	m_bShow = true;
};

//-----------------------------------------------------------------------------
// Arg    : const WCHAR* _Name, const WCHAR* _Path = nullptr
// Desc   : 이름과 패쓰로 생성
//-----------------------------------------------------------------------------
SHR_MeshList::SHR_MeshList(const WCHAR* _Name, const WCHAR* _Path /*= nullptr*/)
{
	eResourceType = eRT_MESH_LIST;

	assert(_Name != nullptr);

	m_NameSize = sizeof(WCHAR)*wcslen(_Name);
	wcscpy_s(m_Name,_Name);

	if(_Path != nullptr)
	{
		m_PathSize = sizeof(WCHAR)*wcslen(_Path);
		wcscpy_s(m_Path,_Path);
	}
	else
	{			
		wcscpy_s(m_Path,L"./object/");
		wcscat_s(m_Path,_Name);
		wcscat_s(m_Path,L".shrml");
		m_PathSize =sizeof(WCHAR)* wcslen(m_Path);
	}

}



//-----------------------------------------------------------------------------
// 소멸자
//-----------------------------------------------------------------------------
SHR_MeshList::~SHR_MeshList()
{
	// 어차피 리소스메니저에서 해제할때 리소스들은 다 해제됨!!
// 	std::vector<SHR_Mesh*>::const_iterator iterCur, iterEnd;
// 	iterEnd = m_Vec_MeshList.end();
// 
// 
// 	for(iterCur = m_Vec_MeshList.begin() ; iterCur != iterEnd ; ++iterCur)
// 	{
// 		SHR_Mesh* pM = (*iterCur);
// 
// 		SAFE_DELETE_(pM)
// 		
// 	}
};


//-----------------------------------------------------------------------------
// Arg    : SHR_Mesh* //[2014.11.20 ]
// Desc   : _pBoneList로 부터 부모 찾아서 연결,, 
//-----------------------------------------------------------------------------
void	SHR_MeshList::ConnectParent_with_BoneList(SHR_BoneList* _pBoneList)
{
	assert(_pMesh != nullptr);


	//////////////////////////////////////////////////////////////////////////
	// 외부의 _pBoneList 에있는 본메쉬에서  부모찾아주기 [2014.11.20 ]
	int size = _pBoneList->GetMeshList()->size();
	std::vector<SHR_Mesh*>* pBL = _pBoneList->GetMeshList();
	if(size > 0)
	{	
		//이터레이터
		std::vector<SHR_Mesh*>::iterator IterMyMesh, IterEnd;
		IterEnd = m_Vec_MeshList.end();

		for(IterMyMesh = m_Vec_MeshList.begin() ; IterMyMesh != IterEnd ; ++IterMyMesh)
		{
			//계층구조 만들어주기
			for( std::vector<SHR_Mesh*>::iterator Iter = pBL->begin();Iter != pBL->end(); ++Iter )
			{
				//char* TempName = (*Iter)->m_MeshInfo.Name;
				if(strcmp((*Iter)->m_MeshInfo.Name, (*IterMyMesh)->m_MeshInfo.ParentName) == 0)
				{
					(*IterMyMesh)->m_pParent = (*Iter);
					break;
				}
			}
		}
		
	}

}


//-----------------------------------------------------------------------------
// Arg    : const SHR_MeshList& _rhs
// Return :
// Desc   :  operator= 
//----------------------------------------------------------------------------- //[2014.11.24 ]
SHR_MeshList&	SHR_MeshList::operator=(const SHR_MeshList& _rhs)
{
	if(_rhs.m_Vec_MeshList.max_size() > 0)
	{
		std::vector<SHR_Mesh*>::const_iterator iterCur, iterEnd;
		iterEnd = _rhs.m_Vec_MeshList.end();


		for(iterCur = _rhs.m_Vec_MeshList.begin() ; iterCur != iterEnd ; ++iterCur)
		{
			// 여기서 어쩔수 없이 얕은복사(리소스에서 worldTM 까지 가지고있는 문제떄문에 씨발!! //[2014.11.24 ])
			SHR_Mesh* pTempMesh = new SHR_Mesh();
			*pTempMesh = *(*iterCur);
			m_Vec_MeshList.push_back(pTempMesh);
		}
				
	}

	m_bShow = _rhs.m_bShow;       
	
	m_BL_AniInfo = _rhs.m_BL_AniInfo;   // BoneList_Ani_Info
	m_Animator = _rhs.m_Animator;// 뼈대에만 적용될 타이머

	return *this;
	
}
/****************************************************** End *****************************************************/