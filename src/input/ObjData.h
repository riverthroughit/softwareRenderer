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
	//��Χ�е�������
	float topY = -1e10, bottomY = 1e10;
	float leftX = 1e10, rightX = -1e10;
	float frontZ = -1e10, backZ = 1e10;
	HoVector4f core;//��Χ������λ�� 
	void setCore() {//���������� �˵��ƽ��һ��
		//����ĳ�������ϳ���̫С �ֶ����ȼ�1
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


struct VertexData {//�洢��ʵ����
	HoVector4f pos;
	HoVector4f normal;
	HoVector3f texture;
	VertexData(const HoVector4f& p, const HoVector4f& n, const HoVector3f& t = { 0,0,0 }) :
		pos(p), normal(n), texture(t) {}

};

class ObjData
{
	//һ��obj�ļ����� ������Ϣ(����) �Ͷ��������Ϣ
	vector<HoVector4f> v;//������������
	vector<HoVector3f> vt;//�������� ��ʱ����������洢 (u,v,1)
	vector<HoVector4f> vn;//���㷨����
	vector<Face> faces;//(������)��
	shared_ptr<vector<Material>> materials;
	Boundry boundry;//�ֱ��¼����������ǰ��������
	
	void initialV(const string& line);
	void initialFaces(string line,int maIndex);

	void initialMtl(string mFile, unordered_map<string, int>& mymap);
	void initialK(const string& mLine, Material& curMa);

public:
	ObjData(string meshFile);
	
	//ObjData(const ObjData& obj);//��Ϊface��objPtr��� �����Զ��忽������
	//ObjData& operator=(const ObjData& obj);
	//ObjData(ObjData&& obj) = default;

	void scaleTo(float s);
	void rorate(const HoVector4f& hovec, float degree);
	void translateBy(HoVector4f hovec);
	void moveToBevelOf(Camera camera);
	void caculateNormals();//Ϊԭ��û�з�������ģ����ӷ�����

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

