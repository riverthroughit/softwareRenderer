#pragma once
#include"myMath/HoVector4.h"
#include"myMath/mathValue.h"
#include<array>
using myMath::HoVector4f;
using myMath::Matrix4f;
class Camera
{
	HoVector4f pos;//���λ�� ��ԭ��
	HoVector4f observe;//�۲��
	HoVector4f upDirection;//���Ϸ���
	HoVector4f w;//z
	HoVector4f u;//����ռ��x���׼������
	HoVector4f v;//y
	float aspectRatio;//��߱�
	float vFOV;//��ֱ���ӽ�
	float nearPlane, farPlane;//��ƽ���Զƽ�� ע������ϵ Ϊ����
			
	void updateSystem() {//��������ϵ
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
	//��λ�� ���ڹ۲������� ���Ϸ���(�о����һ�㶼��������ռ�y��)
	//������������������Լ�ԭ�� Ϊ�����任����׼��
    //���� p157 ע������ϵ
	}

	void setRatio(float ratio) {
		aspectRatio = ratio;
	}

	//ע�������Ҫ�ı����Ϸ��� ��Ϊ���۲췽�������Ϸ���н�piʱ ����ϵ���ܷ�ת
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
	
	//��ȡ�ڽӳ�����x���y�᳤�ȵ��ٽ�ֵ 
	std::array<float, 2> getInscribedRectlength(float boundXlength, float boundYlength, float boundZlength) {		
		float cotHalfVfov = 1 / tanf((vFOV-0.2) / 2);//vFOV-0.2 ��ֹ���Ե̫��
		float cotHalfHfov = 1 / aspectRatio * cotHalfVfov;
		float k1 = boundZlength / boundYlength;
		float k2 = boundZlength / boundXlength;
		float posToFar = -farPlane;
		return { posToFar / (k2 + cotHalfHfov / 2),posToFar / (k1 + cotHalfVfov / 2) };

		//23 3 7�����Ǵ����
		////23 3 7 �ƺ�ֱ�������׶��Զƽ��ĳ�����
		//float maxYlength = -farPlane * tanf((vFOV - 0.2) / 2) * 2;
		//float maxXlength = -farPlane * aspectRatio * tanf((vFOV - 0.2) / 2) * 2;
		//return { maxXlength,maxYlength };
	}

	//�����ڽӳ�����z�᳤�Ȼ�ȡ������Ӧ���ƶ�����λ��
	HoVector4f getCoreOfBoundry(float zLength) {
		//ע������տ�ʼʱ�̶�����������(0,0,0) ��(0,0,1)��
		return { 0,0,-farPlane - zLength / 2 };
	}
};

Matrix4f Camera::getViewTransform()const {
	//����p156
	return Matrix4f{
		u[0],v[0],w[0],0,
		u[1],v[1],w[1],0,
		u[2],v[2],w[2],0,
		-pos * u,-pos * v,-pos * w,1,
	};
}

Matrix4f Camera::getInverseViewTransform()const {
	//����p156
	return Matrix4f{
		u[0],u[1],u[2],0,
		v[0],v[1],v[2],0,
		w[0],w[1],w[2],0,
		pos[0],pos[1],pos[2],1,
	};
}

Matrix4f Camera::getProjcTransform()const {
	//����p163
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
	//����ϵ����һ�¼���
	return Matrix4f{
	 -(aspectRatio * tanf(vFOV / 2)),0,0,0,
	0,-tanf(vFOV / 2),0,0,
	0,0,0,(farPlane - nearPlane) /(nearPlane * farPlane),
	0,0,1,1 / nearPlane
	};
}

