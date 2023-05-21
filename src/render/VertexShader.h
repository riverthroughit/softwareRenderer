#pragma once
#include"scene/Scene.h"
class VertexShader
{

	ObjData& obj;//当前模型数据
	vector<HoVector4f>& orignVertexes;//原始模型的顶点世界坐标 可能会对其添加
	vector<VertexData> curVertsData;//对一个面裁剪后生成的新的顶点 位置 法向量 贴图
	int& newlyAddedVertsSize;
	Matrix4f vpMatrix;//
	Matrix4f inverVP;
	void clip(Face& face);
	void correctionClip(Face& face);//优化裁剪 生成新三角形
	void SutherlandHodgman();
	
	void assemblingVertexes(const Face& face);//组装新顶点成为若干三角形 并将数据加入模型对象中
	//裁剪生成新顶点
	VertexData initialNewVertsData(const VertexData&beg, const VertexData& end, const int& planeIndex, const int& whichPlane);

	//判断点是否在视锥体内 远平面不管了
	bool insideCone(const HoVector4f& vertex) {
		const float& w = vertex.w();
		if (vertex.z() > 0)
			if (vertex.x() > w && vertex.x() < -w)
				if (vertex.y() > w && vertex.y() < -w)
					return true;
		return false;
	}


public:
	VertexShader(vector<HoVector4f>& orignV, ObjData& ob, const Camera& camera, int& newlyAd) :
		orignVertexes(orignV), obj(ob), newlyAddedVertsSize(newlyAd),
		vpMatrix(camera.getViewTransform()* camera.getProjcTransform()),
		inverVP(camera.getInverseProjcTransform()* camera.getInverseViewTransform()) {
		curVertsData.reserve(6);//最多生成六个顶点
	}
	void run(const unsigned short& curCode);
};

