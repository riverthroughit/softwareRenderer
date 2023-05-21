#pragma once
#include"myMath/HoVector4.h"
#include"Vertex.h"
#include<array>

using myMath::HoVector4f;
using std::array;

class Face//存储某个面上的数据索引
{
	bool valid = 1;//该面是否有效 （是否被裁剪）
	array<Vertex, 3> vertexes;//(三角形)面 存储三个点 每个点存v vn vt 的索引
	short materialIndex;//对应材质的索引
	
public:
	Face(short ma, const array<Vertex, 3>& vert);

	//void setVertexesNormalIndex(const int& nIndex) {//模型不带法向量时 自行添加需要该操作
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
	//获取给定模型中和当前面内点的索引相同的点
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

	////getFrom用于从原始模型中获取数据
	//array<HoVector4f*, 3> getWordPosFrom(ObjData& orignObj);

	//array<const HoVector4f*, 3> getWordPosFrom(const ObjData& orignObj)const;

	bool getValid()const {
		return valid;
	}

	void setValid(bool v) {
		valid = v;
	}
};




