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
	//用户选择的模式 初始为2 仅开启gourand着色
	unsigned short funcCode;
	//场景信息
	Scene scene;
	//深度缓冲
	float* zBuffer;
	//片元
	vector<Fragment> frags;
	//因为裁剪而新增的定点数量
	int newlyAddedVertsSize;
	//删掉因为裁剪而新增的顶点
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