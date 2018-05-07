
/************************************************************************/
/*							SHR_Bones.h
					�������� ������ �ִ� ���ҽ�
						[2014.9.15 �輺ȯ]
				
*/
/************************************************************************/
#pragma once



class SHR_Mesh;
/************************************************************************/
/*						class SHR_Bones
*/
/************************************************************************/
class SHR_Bones : public IResource, 
				  public IRefCounter

{
private:

	std::vector<SHR_Mesh*>				m_Vec_BoneList;

public:
	SHR_Bones();
	virtual ~SHR_Bones();
	int									Create(const WCHAR* _pName , const WCHAR* _pPath  );


};