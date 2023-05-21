#include "VertexShader.h"
#include"output/FuncCodes.h"
void VertexShader::clip(Face& face) {
	//先求包围盒
	array<Vertex, 3> verts = face.getVertexes();
	//注意w是负数
	//注意z轴 -1到0 即-w到0
	for (int i = 0; i != 3; ++i) {
		if (!insideCone(obj.getVertPosAt(verts[i]))) {
			face.setValid(0);//直接剔除整个面
			break;
		}
	}			
}

void VertexShader::SutherlandHodgman() {

	for (int i = 0; i != 3; ++i) {//xy    z对应的平面单独处理
		for (int j = 0; j != 2; ++j) {//两平面
			HoVector4f posInPlane{ 0,0,0,0 }, normalOfPlane;//法向量指向包围盒外侧   注意是四维坐标 w也要考虑
			if (i == 2) {//z平面 0<z<-w 但不用判断远平面 仅判断0<z即可
				normalOfPlane = { 0,0,-1,0 };
			}
			else {
				if (j)//右或上平面 x=w或y=w
					normalOfPlane[i] = -1, normalOfPlane.w() = 1;
				else//左或下平面
					normalOfPlane[i] = normalOfPlane.w() = 1;				
			}
			//对当前裁剪边遍历面上每条边进行裁剪
			int lastVertexIndex = curVertsData.size() - 1, curVertexIndex = 0;
			vector<VertexData> newVertexes;//新产生的顶点
			while (curVertexIndex != curVertsData.size()) {//判断面上的每条边
				const VertexData& curVertex = curVertsData[curVertexIndex], & lastVertex = curVertsData[lastVertexIndex];
				const HoVector4f& curVertexPos = curVertex.pos, & lastVertexPos = lastVertex.pos;

				HoVector4f pToCur = curVertexPos - posInPlane;
				HoVector4f pToLast = lastVertexPos - posInPlane;
				float crossPtoC = pToCur * normalOfPlane, crossPtoL = pToLast * normalOfPlane;
				if (crossPtoC < 0 && crossPtoL < 0)//两点都在视锥体内
					newVertexes.emplace_back(curVertex);
				else if (crossPtoC < 0 && crossPtoL > 0) {//当前点在内 上一点在外
					//放入交点以及当前点
					newVertexes.emplace_back(initialNewVertsData(lastVertex, curVertex, i, j));
					newVertexes.emplace_back(curVertex);
				}
				else if (crossPtoC > 0 && crossPtoL < 0) {//当前点在外 上一点在内
					//放入交点
					newVertexes.emplace_back(initialNewVertsData(lastVertex, curVertex, i, j));
				}
				lastVertexIndex = curVertexIndex, ++curVertexIndex;				
			}
			curVertsData = newVertexes;
			if (i == 2)break;//z平面的远平面不用再去判断
		}
	}

	
}
                                               //上一点 现在的点 哪对平面 该对平面中的具体哪个
VertexData VertexShader::initialNewVertsData(const VertexData& beg, const VertexData& end, const int& planeIndex, const int&whichPlane) {
	//w=m w1+t*(w2-w1)=m1+t*(m2-m1) 此为  m可取x y z 可从此式子解出t
	//或是w=-m
	float t, q;//线性插值系数
	const HoVector4f& begPos = beg.pos, endPos = end.pos;
	if (planeIndex == 2) {//近平面
		t = (0 - begPos.z()) / (endPos.z() - begPos.z()), q = 1 - t;
	}
	else if (whichPlane) {//左 下平面
		float a = begPos.w() - begPos[planeIndex], b = endPos.w() - endPos[planeIndex];
		t = a / (a - b), q = 1 - t;
	}
	else {//右 上平面
		float a = begPos.w() + begPos[planeIndex], b = endPos.w() + endPos[planeIndex];
		t = a / (a - b), q = 1 - t;
	}

	HoVector4f newVertPos = begPos * q + endPos * t;
	HoVector4f newVertNorm = beg.normal * q + end.normal * t;
	HoVector3f newVertTex = beg.texture * q + end.texture * t;
	return VertexData(newVertPos, newVertNorm, newVertTex);
}

void VertexShader::assemblingVertexes(const Face& face) {	
	int curNewlyAddedVertSize= curVertsData.size();
	newlyAddedVertsSize += curNewlyAddedVertSize;

	int curVertPosSize = obj.getVertexes().size();//模型当前的总顶点数量
	int curNormalSize = obj.getVns().size();//模型当前的总法向量数量
	int curTextureSize = obj.getTextures().size();//模型当前的总纹理数量
	for (VertexData & vertData : curVertsData) {//将新的顶点法向量纹理加入模型中
		obj.getVertexes().emplace_back(vertData.pos);
		orignVertexes.emplace_back(vertData.pos * inverVP);//原始模型需加入顶点 用于光栅化阶段的透视矫正插值
		obj.getVns().emplace_back(vertData.normal);
		obj.getTextures().emplace_back(vertData.texture);
	}	

	int mateIndex = face.getMaterialIndex();
	for (int i = 1; i + 1 != curNewlyAddedVertSize; ++i) {
		Vertex vert1(curVertPosSize, curNormalSize, curTextureSize);
		Vertex vert2(curVertPosSize + i, curNormalSize + i, curTextureSize + i);
		Vertex vert3(curVertPosSize + i + 1, curNormalSize + i + 1, curTextureSize + i + 1);
		//只需在当前模型中添加面
		obj.getFaces().emplace_back(mateIndex, array<Vertex, 3>{ vert1,vert2,vert3 });
	}
}

void VertexShader::correctionClip(Face& face) {
	const array<Vertex, 3>& verts = face.getVertexes();
	const HoVector4f vertPos1 = obj.getVertPosAt(verts[0]), vertPos2 = obj.getVertPosAt(verts[1]), vertPos3 = obj.getVertPosAt(verts[2]);
	const HoVector4f vertNorm1 = obj.getVertNormAt(verts[0]), vertNorm2 = obj.getVertNormAt(verts[1]), vertNorm3 = obj.getVertNormAt(verts[2]);
	const HoVector3f vertTex1 = obj.getVertTexAt(verts[0]), vertTex2 = obj.getVertTexAt(verts[1]), vertTex3 = obj.getVertTexAt(verts[2]);
	short vertOutside = 0;
	if (!insideCone(vertPos1))++vertOutside;
	if (!insideCone(vertPos2))++vertOutside;
	if (!insideCone(vertPos3))++vertOutside;

	if (vertOutside != 0) {
		face.setValid(false);
		if (3 != vertOutside) {//有部分不在视锥体内 生成新三角形
			//初始化输入
			curVertsData.emplace_back(vertPos1, vertNorm1, vertTex1);
			curVertsData.emplace_back(vertPos2, vertNorm2, vertTex2);
			curVertsData.emplace_back(vertPos3, vertNorm3, vertTex3);

			SutherlandHodgman();//得到新顶点
			//组装顶点 obj中添加新的顶点和面 orignObj中添加新的顶点
			assemblingVertexes(face);
			curVertsData.clear();
		}
	}

}

void VertexShader::run(const unsigned short& curCode)
{	
	//此处还应该有各个模型的model matrix 不过暂时不考虑


   //
	Matrix4f mvp = vpMatrix;//modelMatrix*vpMatrix
	auto& vertexs = obj.getVertexes();
	for (HoVector4f& vertex : vertexs) {//对每个顶点进行mvp变换
		vertex *= mvp;
	}
	//mvp变换后处于裁剪空间 此处以面为单位裁剪不在四棱台中的三角形
	auto& faces = obj.getFaces();
	int curSize = faces.size();//面的数量可能会改变
	bool hasCorec = FuncCodes::hasClIP(curCode);
	if (hasCorec) {
		for (int i = 0; i != curSize; ++i)
			correctionClip(faces[i]);
	}
	else {
		for (int i = 0; i != curSize; ++i)
			clip(faces[i]);
	}



	//进行透视除法 使坐标位于NDC中
	for (HoVector4f& vertex : vertexs) {
		vertex /= vertex[3];
	}

	
}
