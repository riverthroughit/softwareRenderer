#pragma once
#include"scene/Scene.h"
class VertexShader
{

	ObjData& obj;//��ǰģ������
	vector<HoVector4f>& orignVertexes;//ԭʼģ�͵Ķ����������� ���ܻ�������
	vector<VertexData> curVertsData;//��һ����ü������ɵ��µĶ��� λ�� ������ ��ͼ
	int& newlyAddedVertsSize;
	Matrix4f vpMatrix;//
	Matrix4f inverVP;
	void clip(Face& face);
	void correctionClip(Face& face);//�Ż��ü� ������������
	void SutherlandHodgman();
	
	void assemblingVertexes(const Face& face);//��װ�¶����Ϊ���������� �������ݼ���ģ�Ͷ�����
	//�ü������¶���
	VertexData initialNewVertsData(const VertexData&beg, const VertexData& end, const int& planeIndex, const int& whichPlane);

	//�жϵ��Ƿ�����׶���� Զƽ�治����
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
		curVertsData.reserve(6);//���������������
	}
	void run(const unsigned short& curCode);
};

