#pragma once
#include"myMath/HoVector4.h"
#include"myMath/mathValue.h"
#include<array>
using myMath::HoVector4f;
using myMath::Matrix4f;
class Camera
{
	HoVector4f pos;//相机位置 即原点
	HoVector4f observe;//观察点
	HoVector4f upDirection;//向上方向
	HoVector4f w;//z
	HoVector4f u;//相机空间的x轴标准基向量
	HoVector4f v;//y
	float aspectRatio;//宽高比
	float vFOV;//垂直可视角
	float nearPlane, farPlane;//近平面和远平面 注意右手系 为负数
			
	void updateSystem() {//更新坐标系
		w = -(observe - pos).normalize();
		u = (upDirection ^ w).normalize();
		v = w ^ u;
	}
public:
	Camera(float ratio = (float)1920 / 1080,
		const HoVector4f& p = HoVector4f(0, 0, 0, 1),
		const HoVector4f& observ= HoVector4f(0, 0, 1, 1), 
		const HoVector4f& upDirec = HoVector4f(0, 1, 0, 0),
		 float fov = my_pi / 4, float n = -10, float f = -100) ://-0.1 -50
		pos(p), observe(observ), upDirection(upDirec),
		w(-(observ - pos).normalize()), u((upDirec^ w).normalize()), v(w^ u),
		aspectRatio(ratio), vFOV(fov), nearPlane(n), farPlane(f)
	{
	//由位置 正在观测的坐标点 向上方向(感觉这个一般都是沿世界空间y轴)
	//构建出相机的坐标轴以及原点 为后续变换做好准备
    //龙书 p157 注意右手系
	}

	void setRatio(float ratio) {
		aspectRatio = ratio;
	}

	//注意可能需要改变向上方向 因为当观察方向与向上方向夹角pi时 坐标系可能翻转
	void setPos(const HoVector4f& p,const HoVector4f& up) {
		pos = p, upDirection = up;
		updateSystem();
	}

	void setObserve(const HoVector4f& observ) {
		observe = observ;
		updateSystem();
	}

	void setPosAndObserve(const HoVector4f& p, const HoVector4f& observ) {
		pos = p, observe = observ;
		updateSystem();
	}

	inline Matrix4f getViewTransform()const;
	inline Matrix4f getProjcTransform()const;
	inline Matrix4f getInverseViewTransform()const;
	inline Matrix4f getInverseProjcTransform()const;
	inline float getViewZfromProZ(float z)const;
	
	Matrix4f getVPmatrix()const {
		return getViewTransform() * getProjcTransform();
	}


	HoVector4f getObserve() {
		return observe;
	}

	HoVector4f getPos() const{
		return pos;
	}

	HoVector4f getWdirec() {
		return w;
	}

	HoVector4f getUpDirec() {
		return upDirection;
	}

	float getBevelZlength() {
		return nearPlane - farPlane - 1;
	}
	
	//获取内接长方体x轴和y轴长度的临界值 
	std::array<float, 2> getInscribedRectlength(float boundXlength, float boundYlength, float boundZlength) {		
		float cotHalfVfov = 1 / tanf((vFOV-0.2) / 2);//vFOV-0.2 防止与边缘太近
		float cotHalfHfov = 1 / aspectRatio * cotHalfVfov;
		float k1 = boundZlength / boundYlength;
		float k2 = boundZlength / boundXlength;
		float posToFar = -farPlane;
		return { posToFar / (k2 + cotHalfHfov / 2),posToFar / (k1 + cotHalfVfov / 2) };

		//23 3 7下面是错误的
		////23 3 7 似乎直接求出视锥体远平面的长宽即可
		//float maxYlength = -farPlane * tanf((vFOV - 0.2) / 2) * 2;
		//float maxXlength = -farPlane * aspectRatio * tanf((vFOV - 0.2) / 2) * 2;
		//return { maxXlength,maxYlength };
	}

	//根据内接长方体z轴长度获取其中心应该移动到的位置
	HoVector4f getCoreOfBoundry(float zLength) {
		//注意相机刚开始时固定在世界坐标(0,0,0) 往(0,0,1)看
		return { 0,0,-farPlane - zLength / 2 };
	}
};

Matrix4f Camera::getViewTransform()const {
	//龙书p156
	return Matrix4f{
		u[0],v[0],w[0],0,
		u[1],v[1],w[1],0,
		u[2],v[2],w[2],0,
		-pos * u,-pos * v,-pos * w,1,
	};
}

Matrix4f Camera::getInverseViewTransform()const {
	//龙书p156
	return Matrix4f{
		u[0],u[1],u[2],0,
		v[0],v[1],v[2],0,
		w[0],w[1],w[2],0,
		pos[0],pos[1],pos[2],1,
	};
}

Matrix4f Camera::getProjcTransform()const {
	//龙书p163
	return Matrix4f{
		 -1 / (aspectRatio * tanf(vFOV / 2)),0,0,0,
		0,-1 / tanf(vFOV / 2),0,0,
		0,0,farPlane / (nearPlane - farPlane),1,
		0,0,nearPlane * farPlane / (farPlane - nearPlane),0
	};
}

float Camera::getViewZfromProZ(float z)const {
	return nearPlane * farPlane / (z * (farPlane - nearPlane) + farPlane);
}

Matrix4f Camera::getInverseProjcTransform()const {
	//待定系数求一下即可
	return Matrix4f{
	 -(aspectRatio * tanf(vFOV / 2)),0,0,0,
	0,-tanf(vFOV / 2),0,0,
	0,0,0,(farPlane - nearPlane) /(nearPlane * farPlane),
	0,0,1,1 / nearPlane
	};
}

