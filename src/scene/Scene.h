#pragma once
#include"input/ObjData.h"
#include"Camera.h"
#include"Light.h"
#include"output/UserSceneData.h"

class Scene
{
	vector<ObjData> objs;//多个模型
	Light light;//多个不同类型的光照
	Camera camera;//一个相机

	HoVector4f circleCentre;//场景中心位置 光照绕此点旋转 也是相机初始望向的点

	void openNewModel(string file);
	void initialScene();
	void changeCamera(const pair<short, pair<int, int>>& mouseData);
	void changeLight(const pair<short, pair<int, int>>& lightData);
public:
	Scene() = default;


	void changeScene(const userChange& change);
	


	void setObjs(const vector<ObjData>& o) {
		objs = o;
	}

	void addObj(string file) {
		objs.push_back(ObjData(file));
	}


	Camera& getCamera() {
		return camera;
	}

	vector<ObjData>& getObjs() {
		return objs;
	}

	void setLight(const Light& l) {
		light = l;
	}

	Light& getLight() {
		return light;
	}

};

