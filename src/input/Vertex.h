#pragma once
#include"myMAth/HoVector4.h"
using myMath::HoVector4f;
using myMath::HoVector3f;


class Vertex
{//存索引
	int posIndex;//世界坐标的索引
	int normalIndex;
	int textureIndex;


public:
	Vertex(const int& pos, const int& n, const int& t) :
		posIndex(pos), normalIndex(n), textureIndex(t) {}

	void setNormalIndex(const int& nIndex) {//模型不带法向量时 自行添加需要该操作
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

	////不可直接通过顶点修改obj数据 需从obj对象中直接修改
	//HoVector4f getWordPos();

	//HoVector4f getNormal();

	//HoVector3f getTexture();

	//HoVector4f getWordPos()const;

	//HoVector4f getNormal()const;

	//HoVector3f getTexture()const;

	////getFrom用于从别的模型（往往是原始模型）中获取数据
	//HoVector4f getWordPosFrom(ObjData& orignObj);

	//HoVector4f getWordPosFrom(const ObjData& orignObj)const;

	////VertexData initialVertexData();
};



