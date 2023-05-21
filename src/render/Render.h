#pragma once
#include"scene/Scene.h"
#include"Fragment.h"
#include<qrgb.h>
#include <chrono> 
#include <iomanip>
#define NODATA 0
class Render
{
	short width, height;
	int s;
	//�û�ѡ���ģʽ ��ʼΪ2 ������gourand��ɫ
	unsigned short funcCode;
	//������Ϣ
	Scene scene;
	//��Ȼ���
	float* zBuffer;
	//ƬԪ
	vector<Fragment> frags;
	//��Ϊ�ü��������Ķ�������
	int newlyAddedVertsSize;
	//ɾ����Ϊ�ü��������Ķ���
	void moveAddedVerts(ObjData& orignObj);
public:
	Render(short w, short h, Scene sc) :
		width(w), height(h), s(w* h), funcCode(2),
		newlyAddedVertsSize(0),
		scene(sc), zBuffer(new float[s]) {

		scene.getCamera().setRatio((float)w / h);
		frags.reserve(s);

	}
	void run();

};