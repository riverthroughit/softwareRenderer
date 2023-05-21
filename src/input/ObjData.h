#pragma once
#include<vector>
#include<array>
#include<unordered_map>
#include<exception>
#include<fstream>
#include<sstream>
#include <algorithm>
#include<memory>
#include"Material.h"
#include"myMath/HoVector4.h"
#include"scene/Camera.h"
#include"Face.h"
using myMath::HoVector4f;
using myMath::HoVector3f;
using myMath::Matrix4f;
using std::vector;
using std::unordered_map;
using std::ifstream;
using std::istringstream;
using std::array;
using std::shared_ptr;

struct Boundry {
	//包围盒的六个面
	float topY = -1e10, bottomY = 1e10;
	float leftX = 1e10, rightX = -1e10;
	float frontZ = -1e10, backZ = 1e10;
	HoVector4f core;//包围盒中心位置 
	void setCore() {//求物体中心 端点简单平均一下
		//可能某个方向上长度太小 手动长度加1
		if (topY - bottomY < 1e-5)
			topY += 1;
		if (rightX - leftX < 1e-5)
			rightX += 1;
		if (frontZ - backZ < 1e-5)
			frontZ += 1;
		core = (
			HoVector4f{ leftX,topY,frontZ } + HoVector4f{ rightX,topY,frontZ } +
			HoVector4f{ rightX,  topY,backZ } + HoVector4f{ leftX,  topY,backZ } +
			HoVector4f{ leftX,  bottomY,frontZ } + HoVector4f{ rightX,  bottomY,frontZ } +
			HoVector4f{ rightX,  bottomY,backZ } + HoVector4f{ leftX,  bottomY,backZ }
		) / 8;
	}

	void getScaledBy(float s) {
		topY *= s, bottomY *= s;
		leftX *= s, rightX *= s;
		frontZ *= s, backZ *= s;
		core*= { s, 0, 0, 0,
				0, s, 0, 0,
				0, 0, s, 0,
				0, 0, 0, 1,
							};
	}

	void getTranslatedBy(HoVector4f hovec) {
		topY += hovec[1], bottomY += hovec[1];
		leftX += hovec[0], rightX += hovec[0];
		frontZ += hovec[2], backZ += hovec[2];
		core *= { 1, 0, 0, 0,
				  0, 1, 0, 0,
				  0, 0, 1, 0,
			hovec[0], hovec[1], hovec[2], 1,
		};
	}


};


struct VertexData {//存储真实数据
	HoVector4f pos;
	HoVector4f normal;
	HoVector3f texture;
	VertexData(const HoVector4f& p, const HoVector4f& n, const HoVector3f& t = { 0,0,0 }) :
		pos(p), normal(n), texture(t) {}

};

class ObjData
{
	//一个obj文件包含 网格信息(点面) 和多个材质信息
	vector<HoVector4f> v;//顶点世界坐标
	vector<HoVector3f> vt;//纹理坐标 暂时考虑用数组存储 (u,v,1)
	vector<HoVector4f> vn;//顶点法向量
	vector<Face> faces;//(三角形)面
	shared_ptr<vector<Material>> materials;
	Boundry boundry;//分别记录最上下左右前后六个面
	
	void initialV(const string& line);
	void initialFaces(string line,int maIndex);

	void initialMtl(string mFile, unordered_map<string, int>& mymap);
	void initialK(const string& mLine, Material& curMa);

public:
	ObjData(string meshFile);
	
	//ObjData(const ObjData& obj);//因为face的objPtr会变 所以自定义拷贝构造
	//ObjData& operator=(const ObjData& obj);
	//ObjData(ObjData&& obj) = default;

	void scaleTo(float s);
	void rorate(const HoVector4f& hovec, float degree);
	void translateBy(HoVector4f hovec);
	void moveToBevelOf(Camera camera);
	void caculateNormals();//为原本没有法向量的模型添加法向量

	HoVector4f getVertPosAt(const Vertex& vertex)const {
		return v[vertex.getPosIndex()];
	}

	HoVector4f getVertNormAt(const Vertex& vertex)const {
		return vn[vertex.getNormalIndex()];
	}

	HoVector3f getVertTexAt(const Vertex& vertex)const {
		if (vertex.getTextureIndex() == -1)
			return {0,0,0};
		return vt[vertex.getTextureIndex()];
	}

	VertexData getVertDataAt(const Vertex& vertex)const {
		return VertexData(v[vertex.getPosIndex()], vn[vertex.getNormalIndex()], vt[vertex.getTextureIndex()]);
	}

	const Material* getMaterialAt(const Face& face)const {
		if (face.getMaterialIndex() == -1)
			return 0;
		return &(*materials)[face.getMaterialIndex()];
	}

	vector<HoVector4f>& getVertexes() {
		return v;
	}

	const vector<HoVector4f>& getVertexes()const {
		return v;
	}

	vector<HoVector4f>& getVns() {
		return vn;
	}

	const vector<HoVector4f>& getVns()const {
		return vn;
	}

	Boundry& getBoundry() {
		return boundry;
	}

	const Boundry& getBoundry()const {
		return boundry;
	}

	//const vector<Material>& getMaterials() {
	//	return materials;
	//}

	vector<Face>& ObjData::getFaces() {
		return faces;
	}

	const vector<Face>& ObjData::getFaces()const {
		return faces;
	}

	vector<HoVector3f>& ObjData::getTextures() {
		return vt;
	}

	const vector<HoVector3f>& ObjData::getTextures()const {
		return vt;
	}

};

