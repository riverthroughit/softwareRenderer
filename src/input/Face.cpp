#include "Face.h"

Face::Face(short ma, const array<Vertex, 3>& vert)
	:materialIndex(ma), vertexes(vert) {
	//vertexes[0].myObj = vertexes[1].myObj = vertexes[2].myObj = myObj;
	//是为了保证objData在拷贝构造和拷贝赋值时 顶点的指针不会被改变
	//vertexes[0].setObjPtr(myObj); vertexes[1].setObjPtr(myObj); vertexes[2].setObjPtr(myObj);
}

//array<Vertex, 3> Face::getVertexesFrom(const ObjData& obj)const {
//	//array<Vertex, 3> newVerts = vertexes;
//	//newVerts[0].myObj = newVerts[1].myObj = newVerts[2].myObj = &obj;
//	//return newVerts;
//
//	Vertex vert0(vertexes[0].getPosIndex(), vertexes[0].getNormalIndex(), vertexes[0].getTextureIndex(), &obj);
//	Vertex vert1(vertexes[1].getPosIndex(), vertexes[1].getNormalIndex(), vertexes[1].getTextureIndex(), &obj);
//	Vertex vert2(vertexes[2].getPosIndex(), vertexes[2].getNormalIndex(), vertexes[2].getTextureIndex(), &obj);
//
//	return array<Vertex, 3>{vert0, vert1, vert2};
//}

//const Material* Face::getMaterial()const {
//	if (materialIndex == -1)
//		return 0;
//	return &myObj->materials[materialIndex];
//}



