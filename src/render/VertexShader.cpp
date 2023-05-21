#include "VertexShader.h"
#include"output/FuncCodes.h"
void VertexShader::clip(Face& face) {
	//�����Χ��
	array<Vertex, 3> verts = face.getVertexes();
	//ע��w�Ǹ���
	//ע��z�� -1��0 ��-w��0
	for (int i = 0; i != 3; ++i) {
		if (!insideCone(obj.getVertPosAt(verts[i]))) {
			face.setValid(0);//ֱ���޳�������
			break;
		}
	}			
}

void VertexShader::SutherlandHodgman() {

	for (int i = 0; i != 3; ++i) {//xy    z��Ӧ��ƽ�浥������
		for (int j = 0; j != 2; ++j) {//��ƽ��
			HoVector4f posInPlane{ 0,0,0,0 }, normalOfPlane;//������ָ���Χ�����   ע������ά���� wҲҪ����
			if (i == 2) {//zƽ�� 0<z<-w �������ж�Զƽ�� ���ж�0<z����
				normalOfPlane = { 0,0,-1,0 };
			}
			else {
				if (j)//�һ���ƽ�� x=w��y=w
					normalOfPlane[i] = -1, normalOfPlane.w() = 1;
				else//�����ƽ��
					normalOfPlane[i] = normalOfPlane.w() = 1;				
			}
			//�Ե�ǰ�ü��߱�������ÿ���߽��вü�
			int lastVertexIndex = curVertsData.size() - 1, curVertexIndex = 0;
			vector<VertexData> newVertexes;//�²����Ķ���
			while (curVertexIndex != curVertsData.size()) {//�ж����ϵ�ÿ����
				const VertexData& curVertex = curVertsData[curVertexIndex], & lastVertex = curVertsData[lastVertexIndex];
				const HoVector4f& curVertexPos = curVertex.pos, & lastVertexPos = lastVertex.pos;

				HoVector4f pToCur = curVertexPos - posInPlane;
				HoVector4f pToLast = lastVertexPos - posInPlane;
				float crossPtoC = pToCur * normalOfPlane, crossPtoL = pToLast * normalOfPlane;
				if (crossPtoC < 0 && crossPtoL < 0)//���㶼����׶����
					newVertexes.emplace_back(curVertex);
				else if (crossPtoC < 0 && crossPtoL > 0) {//��ǰ������ ��һ������
					//���뽻���Լ���ǰ��
					newVertexes.emplace_back(initialNewVertsData(lastVertex, curVertex, i, j));
					newVertexes.emplace_back(curVertex);
				}
				else if (crossPtoC > 0 && crossPtoL < 0) {//��ǰ������ ��һ������
					//���뽻��
					newVertexes.emplace_back(initialNewVertsData(lastVertex, curVertex, i, j));
				}
				lastVertexIndex = curVertexIndex, ++curVertexIndex;				
			}
			curVertsData = newVertexes;
			if (i == 2)break;//zƽ���Զƽ�治����ȥ�ж�
		}
	}

	
}
                                               //��һ�� ���ڵĵ� �Ķ�ƽ�� �ö�ƽ���еľ����ĸ�
VertexData VertexShader::initialNewVertsData(const VertexData& beg, const VertexData& end, const int& planeIndex, const int&whichPlane) {
	//w=m w1+t*(w2-w1)=m1+t*(m2-m1) ��Ϊ  m��ȡx y z �ɴӴ�ʽ�ӽ��t
	//����w=-m
	float t, q;//���Բ�ֵϵ��
	const HoVector4f& begPos = beg.pos, endPos = end.pos;
	if (planeIndex == 2) {//��ƽ��
		t = (0 - begPos.z()) / (endPos.z() - begPos.z()), q = 1 - t;
	}
	else if (whichPlane) {//�� ��ƽ��
		float a = begPos.w() - begPos[planeIndex], b = endPos.w() - endPos[planeIndex];
		t = a / (a - b), q = 1 - t;
	}
	else {//�� ��ƽ��
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

	int curVertPosSize = obj.getVertexes().size();//ģ�͵�ǰ���ܶ�������
	int curNormalSize = obj.getVns().size();//ģ�͵�ǰ���ܷ���������
	int curTextureSize = obj.getTextures().size();//ģ�͵�ǰ������������
	for (VertexData & vertData : curVertsData) {//���µĶ��㷨�����������ģ����
		obj.getVertexes().emplace_back(vertData.pos);
		orignVertexes.emplace_back(vertData.pos * inverVP);//ԭʼģ������붥�� ���ڹ�դ���׶ε�͸�ӽ�����ֵ
		obj.getVns().emplace_back(vertData.normal);
		obj.getTextures().emplace_back(vertData.texture);
	}	

	int mateIndex = face.getMaterialIndex();
	for (int i = 1; i + 1 != curNewlyAddedVertSize; ++i) {
		Vertex vert1(curVertPosSize, curNormalSize, curTextureSize);
		Vertex vert2(curVertPosSize + i, curNormalSize + i, curTextureSize + i);
		Vertex vert3(curVertPosSize + i + 1, curNormalSize + i + 1, curTextureSize + i + 1);
		//ֻ���ڵ�ǰģ���������
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
		if (3 != vertOutside) {//�в��ֲ�����׶���� ������������
			//��ʼ������
			curVertsData.emplace_back(vertPos1, vertNorm1, vertTex1);
			curVertsData.emplace_back(vertPos2, vertNorm2, vertTex2);
			curVertsData.emplace_back(vertPos3, vertNorm3, vertTex3);

			SutherlandHodgman();//�õ��¶���
			//��װ���� obj������µĶ������ orignObj������µĶ���
			assemblingVertexes(face);
			curVertsData.clear();
		}
	}

}

void VertexShader::run(const unsigned short& curCode)
{	
	//�˴���Ӧ���и���ģ�͵�model matrix ������ʱ������


   //
	Matrix4f mvp = vpMatrix;//modelMatrix*vpMatrix
	auto& vertexs = obj.getVertexes();
	for (HoVector4f& vertex : vertexs) {//��ÿ���������mvp�任
		vertex *= mvp;
	}
	//mvp�任���ڲü��ռ� �˴�����Ϊ��λ�ü���������̨�е�������
	auto& faces = obj.getFaces();
	int curSize = faces.size();//����������ܻ�ı�
	bool hasCorec = FuncCodes::hasClIP(curCode);
	if (hasCorec) {
		for (int i = 0; i != curSize; ++i)
			correctionClip(faces[i]);
	}
	else {
		for (int i = 0; i != curSize; ++i)
			clip(faces[i]);
	}



	//����͸�ӳ��� ʹ����λ��NDC��
	for (HoVector4f& vertex : vertexs) {
		vertex /= vertex[3];
	}

	
}
