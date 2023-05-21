#pragma once
#include"myMath/HoVector4.h"
#include"input/Face.h"
#include"input/Material.h"
#include<array>
using myMath::HoVector4f;
using std::array;
class Fragment
{


public:

	//bool valid;//�Ƿ���Ч ������early z�б��޳�
	const int screenPos;//��Ļ���� ��תһά
	const float screenZ;//ndc�е����ֵ -1��0
	const HoVector4f worldPos;//����ռ�����	
	const HoVector4f normal;//������
	//const HoVector3f texturePos;//��������
	//const Material* material;//ʹ�õĲ���
	pair<bool, HoVector3f> kd;//������ϵ��
	
	//Fragment(int index) :screenIndex(index) {}
	Fragment(const int& screenP, const float& z, const HoVector4f& wp, const HoVector4f& nPos,
		const pair<bool, HoVector3f>& kkd) :
		screenPos(screenP), screenZ(z), worldPos(wp), normal(nPos),kd(kkd) {}
	
	//HoVector4f getWorldPos()const {
	//	return worldPos;
	//}


};

