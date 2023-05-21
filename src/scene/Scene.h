#pragma once
#include"input/ObjData.h"
#include"Camera.h"
#include"Light.h"
#include"output/UserSceneData.h"

class Scene
{
	vector<ObjData> objs;//���ģ��
	Light light;//�����ͬ���͵Ĺ���
	Camera camera;//һ�����

	HoVector4f circleCentre;//��������λ�� �����ƴ˵���ת Ҳ�������ʼ����ĵ�

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

