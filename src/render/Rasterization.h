#pragma once
#include"input/ObjData.h"
#include"Fragment.h"
#include"FragmentShader.h"
#include<qrgb.h>


class CorrInterpolation {//͸�ӽ�����ֵ
	const float a, b, c;
public:
	
	CorrInterpolation( float a1,  float b1,  float c1,
		float za1,  float zb1,  float zc1,  float zt1) :
		a(a1 / za1 * zt1) , b(b1 / zb1 * zt1), c(c1 / zc1 * zt1) {}
	template<typename T>
	T interp(const T& ia, const T& ib, const T& ic)const {
		return ia * a + ib * b + ic * c;
	}
};


class Rasterization{
	short width, height;//��Ļ��С
	unsigned int s;
	ObjData& obj;
	const ObjData& orignObj;//ԭʼ��������Ϣ ��ȡ����ռ��µ�����
	float* zBuffer;
	vector<Fragment>& frags;
	//Fragment& frag;//ƬԪ
	FragmentShader& fragShader;
	QRgb* frame;	
	const Camera& camera;
	unsigned short curCode;

	void viewportTransform(vector<HoVector4f>& vertexs);
	void drawLine(const HoVector4f& p1, const HoVector4f& p2);
	void drawLineInFace(const vector<Face>& faces);
	//void drawFlatTriangle(array<HoVector4f, 3> newVertexes, const array<HoVector4f, 3>& vertexes, int faceIndex);
	void drawTrianglePhong(const vector<Face>& faces);
	void drawTriangleFlat(const vector<Face>& faces);
	void drawTriangleGouraud(const vector<Face>& faces);
	void PhongShading(const Face& face);
	void FlatShading(const Face& face);
	void GouraudShading(const Face& face);
	//void rasterize(const vector<Face>& faces);
	//��ͨ��Ȳ��� firstͨûͨ�� sec��ֵ������ֵ
	pair<bool, float> depthTest(const int& scIndex, const array<const float, 3>& scrZ, const HoVector4f& scBaryCoords);
	Fragment generateFrag(const int& index, const float& fragScreenZ,const Material* material, const array<Vertex, 3>& verts,const CorrInterpolation& corrInterp);
	
	//��ȡ�����ΰ�Χ��
	void getBoundingBox(int& left, int& right, int& up, int& bottom,const HoVector4f& scrVertPos1, const HoVector4f& scrVertPos2, const HoVector4f& scrVertPos3) {
		left = std::min({ scrVertPos1.x(),scrVertPos2.x() ,scrVertPos3.x() });
		right = std::max({ scrVertPos1.x(),scrVertPos2.x() ,scrVertPos3.x() }) + 1;
		up = std::max({ scrVertPos1.y(),scrVertPos2.y() ,scrVertPos3.y() }) + 1;
		bottom = std::min({ scrVertPos1.y(),scrVertPos2.y() ,scrVertPos3.y() });
	}

	//��ȡ͸�ӽ�����ֵ
	CorrInterpolation getCorrInterpo(const HoVector4f& baCoords, const array<Vertex, 3>& orignVerts);

	int getIndex(int x, int y, int width, int height) {//��άתһά ע����Ļ����y�᷽������
		return (height - y - 1) * width + x;
	}
	
	Matrix4f getViewportMatrix() {
		return{
			(float)width / 2,0,0,0,
			0,(float)height / 2,0,0,
			0,0,1,0,
			(float)width / 2,(float)height / 2,0,1
		};
	}


public:
	Rasterization(ObjData& o, const ObjData& ori,float* zB, vector<Fragment>& frs, short w, short h,
		QRgb* fra,FragmentShader& fragS,const Camera&c ,unsigned short funcd) :
		width(w),height(h),s(w*h), 
		obj(o), orignObj(ori),zBuffer(zB), frags(frs),
		fragShader(fragS), frame(fra),camera(c) , curCode(funcd) {}

	void run();
};

