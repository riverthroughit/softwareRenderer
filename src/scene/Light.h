#pragma once
#include"myMath/HoVector4.h"
using myMath::HoVector4f;
using myMath::HoVector3f;
struct Light
{
	HoVector4f pos;//λ��
	//float intensity;//��ǿ
	//�Ȿ�����ɫ
	HoVector3f ka, kd, ks;//������ ������ �߹�ϵ�� 
	float ns;//�߹�ָ��

	Light() :ka{ 0.1,0.1,0.1 }, kd{ 0.3,0.3,0.3 }, ks{ 0.8,0.8,0.8 }, ns(120) {}
};

