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

	//bool valid;//是否有效 可能在early z中被剔除
	const int screenPos;//屏幕坐标 已转一维
	const float screenZ;//ndc中的深度值 -1到0
	const HoVector4f worldPos;//世界空间坐标	
	const HoVector4f normal;//法向量
	//const HoVector3f texturePos;//纹理坐标
	//const Material* material;//使用的材质
	pair<bool, HoVector3f> kd;//漫反射系数
	
	//Fragment(int index) :screenIndex(index) {}
	Fragment(const int& screenP, const float& z, const HoVector4f& wp, const HoVector4f& nPos,
		const pair<bool, HoVector3f>& kkd) :
		screenPos(screenP), screenZ(z), worldPos(wp), normal(nPos),kd(kkd) {}
	
	//HoVector4f getWorldPos()const {
	//	return worldPos;
	//}


};

