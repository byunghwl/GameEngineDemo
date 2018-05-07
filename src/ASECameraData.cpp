/************************************************************************/
/*   2014. 07. 09														*/
/*	 ASECameraData = AseData�� ��ӹ���						            */
/*	 Camera Data�� Ase���Ͽ��� �о�� �����س��� Ŭ����		            */
/************************************************************************/

#include "DXUT.H"
#include "ASEData.h"
#include "ASECameraData.h"

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
//////////////////////////////////////////////////////////////////////////
//
// ���⼭���� ASECameraData class ����Լ� ����
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
// ������ ,, �Ҹ��� 
//-----------------------------------------------------------------------------
ASECameraData::ASECameraData() : m_FOV(0.0f), m_CurAniTime(0.0f)
{
	D3DXMatrixIdentity(&m_MLocalTM);
	D3DXMatrixIdentity(&m_MWorldTM);
	D3DXMatrixIdentity(&m_LTM_scale);
	D3DXMatrixIdentity(&m_LTM_Rot);
	D3DXMatrixIdentity(&m_LTM_Pos);
	D3DXMatrixIdentity(&m_AniTM_Scale);
	D3DXMatrixIdentity(&m_AniTm_Rot);
	D3DXMatrixIdentity(&m_AniTm_Pos);

}

ASECameraData::~ASECameraData()
{

}


//-----------------------------------------------------------------------------
// ī�޶� �ִϸ��̼� ����Լ�
//-----------------------------------------------------------------------------
void  ASECameraData::Animation(float _dTime)
{

	if(m_TMAni.m_pScene ==NULL)//TMAni�� ���ٸ� �ִϸ��̼��� ���� �ʴ´�;;
		return;
		
	m_CurAniTime +=  (_dTime*1000.0f); 

	if(m_TMAni.LastTime < m_CurAniTime)
		m_CurAniTime = 0.0f;
	// 		 	
	m_AniTm_Pos = GetCurr_PosTM(m_CurAniTime); //CurrTick�� �ش��ϴ� PositionTM�� ����ؼ� ��ȯ�Ѵ�.
	m_AniTm_Rot = GetCurr_RotTM(m_CurAniTime);

	D3DXMATRIX TempRotTM;
	D3DXMatrixIdentity(&TempRotTM);
	D3DXMatrixRotationX(&TempRotTM, 1.5707f);
	NodeTM.NodeTM = TempRotTM * m_AniTm_Rot * m_AniTm_Pos;
	
	
	int a=0;
}

//-----------------------------------------------------------------------------
// GetCurr_PosTM : ���� _CurTick�� �ش��ϴ� Position TM�� ���ؿ´�.
//-----------------------------------------------------------------------------

D3DXMATRIX ASECameraData::GetCurr_PosTM(float _CurTick)
{
	D3DXVECTOR3 V;
	float ProgRate = 0.0f ;//���� ����� 
	D3DXMATRIX retM ,Inv; //  ���� ��ȯ ��Ʈ���� 

	retM = m_LTM_Pos;
	if(m_TMAni.pPos != NULL)
	{
		int len = m_TMAni.PosSize;

		//int startIdx = ((m_TMAni.FrameCnt/FRAME_PER_SAMPLE * _CurTick)/m_TMAni.LastTime) +1;
		int startIdx = (_CurTick* (float)m_TMAni.PosSize /m_TMAni.LastTime)+10;

		if(startIdx>=len) startIdx = len-1;


		for(int i=startIdx; i>=1 ; i--)
		{
			if(m_TMAni.pPos[i-1].Curtick < _CurTick)
			{
				ProgRate = (_CurTick - m_TMAni.pPos[i-1].Curtick) / (m_TMAni.pPos[i].Curtick - m_TMAni.pPos[i-1].Curtick);
				D3DXVec3Lerp(&V, &m_TMAni.pPos[i-1].pos ,  &m_TMAni.pPos[i].pos,  ProgRate);

				D3DXMatrixTranslation(&retM, V.x, V.y, V.z);

				break;
			}
			else if(m_TMAni.pPos[i-1].Curtick == _CurTick)
			{
				D3DXMatrixTranslation(&retM, m_TMAni.pPos[i-1].pos.x, m_TMAni.pPos[i-1].pos.y , m_TMAni.pPos[i-1].pos.z);

				break;
			}
		}
	}


	return retM;

}

//-----------------------------------------------------------------------------
// GetCurr_PosTM : ���� _CurTick�� �ش��ϴ� Rotation TM�� ���ؿ´�.
//-----------------------------------------------------------------------------
D3DXMATRIX ASECameraData::GetCurr_RotTM(float _CurTick)
{
	D3DXQUATERNION Q;
	float ProgRate = 0.0f ;//���� ����� 
	D3DXMATRIX retM ,Inv; //  ���� ��ȯ ��Ʈ���� 

	retM = m_LTM_Rot;
	if(m_TMAni.pRot != NULL)
	{
		int len = m_TMAni.RotSize;

		//int startIdx = ((m_TMAni.FrameCnt/FRAME_PER_SAMPLE * _CurTick)/m_TMAni.LastTick) +1;
		int startIdx = (_CurTick* (float)m_TMAni.RotSize /m_TMAni.LastTime)+10;
		if(startIdx>=len) startIdx = len-1;

		for(int i=startIdx; i>=1 ; i--)
		{
			if(m_TMAni.pRot[i-1].Curtick < _CurTick)
			{
				ProgRate = (_CurTick - m_TMAni.pRot[i-1].Curtick) / (m_TMAni.pRot[i].Curtick - m_TMAni.pRot[i-1].Curtick);
				D3DXQuaternionSlerp(&Q,   &m_TMAni.pRot[i-1].quat, &m_TMAni.pRot[i].quat,  ProgRate);
				D3DXMatrixRotationQuaternion(&retM, &Q);

				break;
			}
			else if(m_TMAni.pRot[i-1].Curtick == _CurTick)
			{
				D3DXMatrixRotationQuaternion(&retM, &m_TMAni.pRot[i-1].quat);

				break;
			}
		}


	}
	return retM;
}

//-----------------------------------------------------------------------------
// GetCurr_PosTM : ���� _CurTick�� �ش��ϴ� Scale TM�� ���ؿ´�.
//-----------------------------------------------------------------------------
D3DXMATRIX ASECameraData::GetCurr_ScaleTM(float _CurTick)
{
	D3DXMATRIX retM;

	return retM;

}

//-----------------------------------------------------------------------------
// ����� �����
// _pViewTM (_Out)
//-----------------------------------------------------------------------------
void ASECameraData::MakeViewMatrix(D3DXMATRIX* _pViewTM)
{
	D3DXVECTOR3  tempX;
	D3DXVECTOR3  tempY;
	D3DXVECTOR3  tempZ;
	D3DXVec3Normalize(&tempX, (LPD3DXVECTOR3)&NodeTM.NodeTM._11);
	D3DXVec3Normalize(&tempY, (LPD3DXVECTOR3)&NodeTM.NodeTM._21);
	D3DXVec3Normalize(&tempZ, (LPD3DXVECTOR3)&NodeTM.NodeTM._31);

	_pViewTM->_11 = tempX.x;
	_pViewTM->_12 = tempY.x;
	_pViewTM->_13 = tempZ.x;
	_pViewTM->_14 = 0.0f;

	_pViewTM->_21 = tempX.y;
	_pViewTM->_22 = tempY.y;
	_pViewTM->_23 = tempZ.y;
	_pViewTM->_24 = 0.0f;

	_pViewTM->_31 = tempX.z;
	_pViewTM->_32 = tempY.z;
	_pViewTM->_33 = tempZ.z;
	_pViewTM->_34 = 0.0f;

	D3DXVECTOR3 NodePos = D3DXVECTOR3(NodeTM.NodeTM._41, NodeTM.NodeTM._42, NodeTM.NodeTM._43);

	_pViewTM->_41 = - D3DXVec3Dot(&tempX, &NodePos);
	_pViewTM->_42 = - D3DXVec3Dot(&tempY, &NodePos);
	_pViewTM->_43 = - D3DXVec3Dot(&tempZ, &NodePos);
	_pViewTM->_44 = 1.0f;
}

/****************************************************** End *****************************************************/