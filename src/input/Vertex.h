#pragma once
#include"myMAth/HoVector4.h"
using myMath::HoVector4f;
using myMath::HoVector3f;


class Vertex
{//������
	int posIndex;//�������������
	int normalIndex;
	int textureIndex;


public:
	Vertex(const int& pos, const int& n, const int& t) :
		posIndex(pos), normalIndex(n), textureIndex(t) {}

	void setNormalIndex(const int& nIndex) {//ģ�Ͳ���������ʱ ���������Ҫ�ò���
		normalIndex = nIndex;
	}

	//void setObjPtr(const ObjData* obj) {//
	//	myObj = obj;
	//}

	//const ObjData* getObjPtr()const {//
	//	return myObj;
	//}

	int getPosIndex()const {
		return posIndex;
	}

	int getNormalIndex()const {
		return normalIndex;
	}

	int getTextureIndex()const {
		return textureIndex;
	}

	////����ֱ��ͨ�������޸�obj���� ���obj������ֱ���޸�
	//HoVector4f getWordPos();

	//HoVector4f getNormal();

	//HoVector3f getTexture();

	//HoVector4f getWordPos()const;

	//HoVector4f getNormal()const;

	//HoVector3f getTexture()const;

	////getFrom���ڴӱ��ģ�ͣ�������ԭʼģ�ͣ��л�ȡ����
	//HoVector4f getWordPosFrom(ObjData& orignObj);

	//HoVector4f getWordPosFrom(const ObjData& orignObj)const;

	////VertexData initialVertexData();
};



