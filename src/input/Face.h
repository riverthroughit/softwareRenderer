#pragma once
#include"myMath/HoVector4.h"
#include"Vertex.h"
#include<array>

using myMath::HoVector4f;
using std::array;

class Face//�洢ĳ�����ϵ���������
{
	bool valid = 1;//�����Ƿ���Ч ���Ƿ񱻲ü���
	array<Vertex, 3> vertexes;//(������)�� �洢������ ÿ�����v vn vt ������
	short materialIndex;//��Ӧ���ʵ�����
	
public:
	Face(short ma, const array<Vertex, 3>& vert);

	//void setVertexesNormalIndex(const int& nIndex) {//ģ�Ͳ���������ʱ ���������Ҫ�ò���
	//	vertexes[0][1] = vertexes[1][1] = vertexes[2][1] = nIndex;
	//}

	short getMaterialIndex()const {
		return materialIndex;
	}

	array<Vertex, 3>& getVertexes() {
		return vertexes;
	}

	const array<Vertex, 3>& getVertexes()const {
		return vertexes;
	}
	//��ȡ����ģ���к͵�ǰ���ڵ��������ͬ�ĵ�
	//array<Vertex, 3> getVertexesFrom(const ObjData& obj)const;

	//array<HoVector4f*, 3> getWordPos();

	//array<HoVector4f*, 3> getNormals();

	//array<array<float, 2>*, 3> getTexture();

	//const array<const HoVector4f*, 3> getWordPos()const;

	//const array<const HoVector4f*, 3> getNormals()const;

	//const array<const array<float, 2>*, 3> getTexture()const;

	//const Material* getMaterial()const;

	//const ObjData* getObjPtr()const {
	//	return myObj;
	//}

	////getFrom���ڴ�ԭʼģ���л�ȡ����
	//array<HoVector4f*, 3> getWordPosFrom(ObjData& orignObj);

	//array<const HoVector4f*, 3> getWordPosFrom(const ObjData& orignObj)const;

	bool getValid()const {
		return valid;
	}

	void setValid(bool v) {
		valid = v;
	}
};




