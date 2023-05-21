#pragma once
#include"myMath/HoVector4.h"
using myMath::HoVector4f;
using myMath::HoVector3f;
struct Light
{
	HoVector4f pos;//位置
	//float intensity;//光强
	//光本身的颜色
	HoVector3f ka, kd, ks;//环境光 漫反射 高光系数 
	float ns;//高光指数

	Light() :ka{ 0.1,0.1,0.1 }, kd{ 0.3,0.3,0.3 }, ks{ 0.8,0.8,0.8 }, ns(120) {}
};

