#include "Rasterization.h"
#include<map>
#include<numeric>
#include<algorithm>
#include"output/FuncCodes.h"
using std::map;

void Rasterization::viewportTransform(vector<HoVector4f>& vertexs) {
    Matrix4f viewportMatrix = getViewportMatrix();
    for (HoVector4f& vertex : vertexs) {//��ÿ����������ӿڱ任
        vertex *= viewportMatrix;
    }
}




void Rasterization::drawLine(const HoVector4f& p1, const HoVector4f& p2) {
	//Bresenham�㷨 round�����е���
	int x0 = p1[0], y0 = p1[1];//��ʼ����������Ļ�����е�λ�� ע�Ⲣ��Ϊ���ص����� �����е����
	int x1 = p2[0], y1 = p2[1];
	int dx = abs(x1 - x0), dy = abs(y1 - y0);
	int doubleDy = dy + dy, doubleDx = dx + dx;
	int plusX = x0 < x1 ? 1 : -1, plusY = y0 < y1 ? 1 : -1;
	int drawX = x0, drawY = y0;
	if (dx >= dy) {
		int t = -dx;
		while (x0 != x1) {
			//
			int index = getIndex(x0, drawY, width, height);
			if (index < s)
				frame[index] = qRgb(0, 0, 0);
			//
			if ((t += doubleDy) >= dx)
				y0 += plusY, t -= doubleDx;
			drawY = t > 0 ? y0 + plusY : y0;
			x0 += plusX;
		}
		//
		int index = getIndex(x0, drawY, width, height);
		if (index < s)
			frame[index] = qRgb(0, 0, 0);
		//
	}
	else {
		int t = -dy;
		while (y0 != y1) {
			//
			int index = getIndex(drawX, y0, width, height);
			if (index < s)
				frame[index] = qRgb(0, 0, 0);
			//
			if ((t += doubleDx) >= dy)
				x0 += plusX, t -= doubleDy;
			drawX = t > 0 ? x0 + plusX : x0;
			y0 += plusY;
		}

		//
		int index = getIndex(drawX, y0, width, height);
		if (index < s)
			frame[index] = qRgb(0, 0, 0);			
		//
	}

}
void Rasterization::drawLineInFace(const vector<Face>& faces) {
	//ʹ�û����㷨����
	for (const Face& face : faces) {
		if (face.getValid()) {
			//�õ�ͬһ�����������
			const array<Vertex, 3>& verts = face.getVertexes();
			const HoVector4f scrVertPos1 = obj.getVertPosAt(verts[0]), scrVertPos2 = obj.getVertPosAt(verts[1]), scrVertPos3 = obj.getVertPosAt(verts[2]);
			drawLine(scrVertPos1, scrVertPos2);
			drawLine(scrVertPos2, scrVertPos3);
			drawLine(scrVertPos3, scrVertPos1);
		}
	}
}


//void Rasterization::drawFlatTriangle(array<HoVector4f,3> newVertexes, const array<HoVector4f, 3>& vertexes, int faceIndex) {
//	//newVertexes��p1Ϊy��С�����ĵ� p2[1]==p3[1]
//	//vertexes ��Ҫ�÷ָ�ǰ�������������ֵ
//	HoVector4f& p1 = newVertexes[0], & p2 = newVertexes[1], & p3 = newVertexes[2];
//	BarycentricCoordinates ba(vertexes[0][0], vertexes[1][0], vertexes[2][0],
//		vertexes[0][1], vertexes[1][1], vertexes[2][1]);
//	//ɨ�����㷨
//	if (p2[0] > p3[0])//ʹ��p2�����
//		std::swap(p2, p3);
//	float recK1 = (p2[0] - p1[0]) / (p2[1] - p1[1]);//p1p2ֱ��б�ʵĵ��� y�仯plusʱx������
//	float recK2 = (p3[0] - p1[0]) / (p2[1] - p1[1]);//
//	
//	//top left �߽�ֻ����ߺ������ 	
//	float xBeg = p1[0], xEnd = p1[0] + 1;//��ʼ��x����
//	if (p1[1] > p2[1]) {
//		for (int y = p1[1]; y > (int)p2[1]; y -=1) {
//			int xb = xBeg, xe = xEnd;
//			for (; xb < xe; xb += 1) {
//				int index = getIndex(xb, y, width, height);
//				if (index < s)
//					frame[index] = qRgb(0, 0, 0);
//			}
//			xBeg -= recK1, xEnd -= recK2;
//		}
//	}
//	else {
//		for (int y = p1[1]; y <= (int)p2[1]; y += 1) {
//			int xb = xBeg, xe = xEnd;
//			for (; xb < xe; xb += 1) {
//				int index = getIndex(xb, y, width, height);
//				if (index < s)
//					frame[index] = qRgb(0, 0, 0);
//			}
//			////ѭ�������һ�� y��p2[1]�ӽ�ʱ��Ҫ�����ж� ��Ϊ��б�ʺܴ�ʱ ��ʹ����һ�ε�x�仯̫��
//			//if (y + 1 > p2[1])
//			//	xBeg += recK1 * (p2[1] - y), xEnd += recK2 * (p2[1] - y);
//			xBeg += recK1, xEnd += recK2;
//		}
//	}
//	////��ȡ�������Ϣ ��ʼ�������ƬԪ��ɫ��
//	//const FaceData& orignFaceData = orignObj.getFaces()[faceIndex].getFaceDataFrom(orignObj);
//	//const Material* material = orignFaceData.getMaterial();
//	//FragmentShader fragShader(material, lights);
//	////���ڸ������������������
//	//BarycentricCoordinates ba(vertexes[0][0], vertexes[1][0], vertexes[2][0], vertexes[0][1], vertexes[1][1], vertexes[2][1]);
//	
//	//auto update = [&](int x,int y) {
//	//	//�ȼ����������� 
//	//	array<float, 3> baryCoord = ba.getBaryCoords({ float(x + 0.5),float(y + 0.5) });
//	//	//����Ƿ�����������
//	//	if (baryCoord[0] < 0 || baryCoord[1] < 0 || baryCoord[2] < 0)
//	//		return;
//	//	array<HoVector4f, 3> orignVertexes = orignFaceData.getVerticesPos();
//	//		
//	//	float za = orignVertexes[0][2], zb = orignVertexes[1][2], zc = orignVertexes[2][2];
//	//	float nowZ = 1 / (baryCoord[0] / za + baryCoord[1] / zb + baryCoord[2] / zc);//͸�ӽ���
//	//	//float nowZ = p1[2] * baryCoord[0] + p2[2] * baryCoord[1] + p3[2] * baryCoord[2];
//	//	int index = getIndex(x, y, width, height);
//	//	if (index < s && nowZ > zBuffer[index]) {//zԽ��Խ��
//	//		zBuffer[index] = nowZ;
//	//		//����������(������ ����)���в�ֵ ����ƬԪ��Ϣ
//	//		CorrInterpolation corrinterp(baryCoord[0], baryCoord[1], baryCoord[2],
//	//			orignVertexes[0][2], orignVertexes[1][2], orignVertexes[2][2], nowZ);
//	//		array<HoVector4f, 3> orignNormals = orignFaceData.getNormals();
//	//		HoVector4f myNormal = corrinterp.interp(orignNormals[0], orignNormals[1], orignNormals[2]);
//	//		//array<array<float, 2>, 3> texturePos
//	//		
//	//		//ƬԪ��ɫ������õ���ɫ ���и���
//	//		frame[index] = fragShader.shading(Fragment({ float(x + 0.5),float(y + 0.5),nowZ }, myNormal));
//	//	}
//	//	//msaa ssaa�������в�ͬ
//	//};
//
//
//}

//void Rasterization::drawTriangle(const Face& face, int faceIndex) {
//	//����ͨ�����η�Ϊ����ƽ������
//	//��y��С���� �ҵ�λ���м��y
//	array<HoVector4f, 3> vertexes = face.getFaceDataFrom(obj).getVerticesPos();
//	std::sort(vertexes.begin(), vertexes.end(),
//		[](const auto& v1, const auto& v2) {return v1[1] < v2[1]; });
//	const HoVector4f& p1 = vertexes[0], & p2 = vertexes[1], & p3 = vertexes[2];
//	
//	if (abs(p2[1] - p1[1]) < 0.01)//�Ѿ���ƽ�׻�ƽ��������
//		drawFlatTriangle({ p3,p1,p2 }, vertexes, faceIndex);
//	else if (abs(p2[1] - p3[1]) < 0.01)
//		drawFlatTriangle({ p1,p2,p3 }, vertexes, faceIndex);
//	else {
//		//�ָ������������
//		//����ʽ ���µ�ĺ�����
//		float midX = (p2[1] - p1[1]) * (p3[0] - p1[0]) / (p3[1] - p1[1]) + p1[0];
//		HoVector4f newVertex(midX, p2[1], 0);
//		drawFlatTriangle({ p1,p2,newVertex }, vertexes, faceIndex);
//		drawFlatTriangle({ p3,p2,newVertex }, vertexes, faceIndex);
//	}
//
//}

CorrInterpolation Rasterization::getCorrInterpo(const HoVector4f& baCoords, const array<Vertex, 3>& verts) {
	//ע��˴���ԭʼ���Ӧ��������ռ��ڵ� ��������ռ��ڵ� Ҳ����Ļ����ϵ�µ�
	//����轫�ȼ����view�ռ��µ�z z=a+b/z
	const float viewZ1 = camera.getViewZfromProZ(obj.getVertPosAt(verts[0]).z()),
				viewZ2 = camera.getViewZfromProZ(obj.getVertPosAt(verts[1]).z()),
				viewZ3 = camera.getViewZfromProZ(obj.getVertPosAt(verts[2]).z());
	float z = //��ȷ��ֵ��view�ռ��µ����
		1 / (baCoords[0] / viewZ1 + baCoords[1] / viewZ2 + baCoords[2] / viewZ3);
	return CorrInterpolation(baCoords[0], baCoords[1], baCoords[2], viewZ1, viewZ2, viewZ3, z);
}

void Rasterization::PhongShading(const Face& face) {
	const array<Vertex, 3>& verts = face.getVertexes();
	const HoVector4f scrVertPos1 = obj.getVertPosAt(verts[0]),
		scrVertPos2 = obj.getVertPosAt(verts[1]), scrVertPos3 = obj.getVertPosAt(verts[2]);

	//��Χ��
	int left, right, up, bottom;
	getBoundingBox(left, right, up, bottom, scrVertPos1, scrVertPos2, scrVertPos3);
	
	//������������ ÿ�ν������ӷ�����

	float abpX = scrVertPos1.y() - scrVertPos2.y(), abpY = scrVertPos2.x() - scrVertPos1.x(), abpT = scrVertPos2.y() * scrVertPos1.x() - scrVertPos2.x() * scrVertPos1.y();
	float bcpX = scrVertPos2.y() - scrVertPos3.y(), bcpY = scrVertPos3.x() - scrVertPos2.x(), bcpT = scrVertPos3.y() * scrVertPos2.x() - scrVertPos3.x() * scrVertPos2.y();
	float capX = scrVertPos3.y() - scrVertPos1.y(), capY = scrVertPos1.x() - scrVertPos3.x(), capT = scrVertPos1.y() * scrVertPos3.x() - scrVertPos1.x() * scrVertPos3.y();

	//float centreX = (scrVertPos1.x() + scrVertPos2.x() + scrVertPos3.x()) / 3,
	//	centreY = (scrVertPos1.y() + scrVertPos2.y() + scrVertPos3.y()) / 3;
	//if (abpX * centreX + abpY * centreY + abpT < 0)
	//	return;//Ϊ���� ֱ���޳�

	float abp = abpX * (left + 0.5) + abpY * (bottom + 0.5) + abpT;//ab����ap��� ��Ϊ������abp���������
	float bcp = bcpX * (left + 0.5) + bcpY * (bottom + 0.5) + bcpT;
	float cap = capX * (left + 0.5) + capY * (bottom + 0.5) + capT;
	const float reciOfDoubldS = 1 / (abp + bcp + cap);//������������������ĵ��� ���Լ�����������

	for (int y = bottom; y != up; ++y) {
		bool flag = false;//�Ż�һ�� ������������ ������һ�����±���
		float abp1 = abp, bcp1 = bcp, cap1 = cap;
		for (int x = left; x != right; ++x) {
			if (abp1 >= 0 && bcp1 >= 0 && cap1 >= 0) {//���������� ͬʱҲ�Ǳ����޳�
				flag = true;
				int index = getIndex(x, y, width, height);
				//if (index < s) {
					//��Ļ�����µ���������
					HoVector4f scBaryCoords{ bcp1 * reciOfDoubldS ,cap1 * reciOfDoubldS ,abp1 * reciOfDoubldS };
					pair<bool, float> zTest = //�˴������ý��������z���бȽ� ��Ϊ��Ӧ��С˳�򲻱�
						depthTest(index, { scrVertPos1.z(),scrVertPos2.z() ,scrVertPos3.z() }, scBaryCoords);
					if (zTest.first) {//ͨ����Ȳ��� 
						//��ֵ����λ�� ���� ��ͼ���� ����ƬԪ
						frags.emplace_back(generateFrag(index, zTest.second, obj.getMaterialAt(face), verts, getCorrInterpo(scBaryCoords, verts)));
					}

				//}
			}
			else if (flag)
				break;
			abp1 += abpX; bcp1 += bcpX; cap1 += capX;//����
		}
		abp += abpY; bcp += bcpY; cap += capY;//����
	}
}

void Rasterization::FlatShading(const Face& face){
	const array<Vertex, 3>& verts = face.getVertexes();
	const HoVector4f scrVertPos1 = obj.getVertPosAt(verts[0]),
		scrVertPos2 = obj.getVertPosAt(verts[1]), scrVertPos3 = obj.getVertPosAt(verts[2]);

	//��Χ��
	int left, right, up, bottom;
	getBoundingBox(left, right, up, bottom, scrVertPos1, scrVertPos2, scrVertPos3);
	//������������ ÿ�ν������ӷ�����
	float abpX = scrVertPos1.y() - scrVertPos2.y(), abpY = scrVertPos2.x() - scrVertPos1.x(), abpT = scrVertPos2.y() * scrVertPos1.x() - scrVertPos2.x() * scrVertPos1.y();
	float bcpX = scrVertPos2.y() - scrVertPos3.y(), bcpY = scrVertPos3.x() - scrVertPos2.x(), bcpT = scrVertPos3.y() * scrVertPos2.x() - scrVertPos3.x() * scrVertPos2.y();
	float capX = scrVertPos3.y() - scrVertPos1.y(), capY = scrVertPos1.x() - scrVertPos3.x(), capT = scrVertPos1.y() * scrVertPos3.x() - scrVertPos1.x() * scrVertPos3.y(); 
	float abp = abpX * (left + 0.5) + abpY * (bottom + 0.5) + abpT;//ab�߲��
	float bcp = bcpX * (left + 0.5) + bcpY * (bottom + 0.5) + bcpT;
	float cap = capX * (left + 0.5) + capY * (bottom + 0.5) + capT;

	float centreX = (scrVertPos1.x() + scrVertPos2.x() + scrVertPos3.x()) / 3,
		centreY = (scrVertPos1.y() + scrVertPos2.y() + scrVertPos3.y()) / 3;
	if (abpX * centreX + abpY * centreY + abpT < 0)
		return;//Ϊ���� ֱ���޳�

	const float reciOfDoubldS = 1 / (abp + bcp + cap);//��������������ĵ��� ���Լ�����������

	//ÿ����ֻ������λ�õ���һ����ɫ������
	array<const float, 3> scrZ{ scrVertPos1.z(),scrVertPos2.z() ,scrVertPos3.z() };
	int barycentreIndex = getIndex(centreX, centreY, width, height);
	//if (barycentreIndex >= s)
	//	return;//������Ļ �޳�
	const HoVector4f centreBaryCoords = { (float)1 / 3,(float)1 / 3,(float)1 / 3 };
	pair<bool, float> zTest = depthTest(barycentreIndex, scrZ, centreBaryCoords);
	//�������Ĵ����ڵ�ҲҪ������ɫ ��Ϊ��������λ�ÿ���δ���ڵ� ��Ҫ�������Ǵ���ɫ
	QRgb centreColor = fragShader.shading(generateFrag(barycentreIndex, zTest.second, obj.getMaterialAt(face), verts, getCorrInterpo(centreBaryCoords, verts)));
	if (zTest.first)
		frame[barycentreIndex] = centreColor;

	for (int y = bottom; y != up; ++y) {
		bool flag = false;//�Ż�һ�� ������������ ������һ�����±���
		float abp1 = abp, bcp1 = bcp, cap1 = cap;
		for (int x = left; x != right; ++x) {
			if (abp1 >= 0 && bcp1 >= 0 && cap1 >= 0) {//����������
				if (y == 599) {
					int a = 1;
				}
				flag = true;
				int index = getIndex(x, y, width, height);
				//if (index < s) {

					//��Ļ�����µ���������
					HoVector4f scBaryCoords{ bcp1 * reciOfDoubldS ,cap1 * reciOfDoubldS ,abp1 * reciOfDoubldS };
					pair<bool, float> zTest = //
						depthTest(index, { scrVertPos1.z(),scrVertPos2.z() ,scrVertPos3.z() }, scBaryCoords);
					if (zTest.first) {//ͨ����Ȳ��� 						
						frame[index] = centreColor;
					}

				//}
			}
			else if (flag)
				break;
			abp1 += abpX; bcp1 += bcpX; cap1 += capX;//����
		}
		abp += abpY; bcp += bcpY; cap += capY;//����
	}
}

void Rasterization::GouraudShading(const Face& face){
	const array<Vertex, 3>& verts = face.getVertexes();
	const HoVector4f scrVertPos1 = obj.getVertPosAt(verts[0]),
		scrVertPos2 = obj.getVertPosAt(verts[1]), scrVertPos3 = obj.getVertPosAt(verts[2]);
	
	//��Χ��
	int left, right, up, bottom;
	getBoundingBox(left, right, up, bottom, scrVertPos1, scrVertPos2, scrVertPos3);
	//������������ ÿ�ν������ӷ�����
	float abpX = scrVertPos1.y() - scrVertPos2.y(), abpY = scrVertPos2.x() - scrVertPos1.x(), abpT = scrVertPos2.y() * scrVertPos1.x() - scrVertPos2.x() * scrVertPos1.y();
	float bcpX = scrVertPos2.y() - scrVertPos3.y(), bcpY = scrVertPos3.x() - scrVertPos2.x(), bcpT = scrVertPos3.y() * scrVertPos2.x() - scrVertPos3.x() * scrVertPos2.y();
	float capX = scrVertPos3.y() - scrVertPos1.y(), capY = scrVertPos1.x() - scrVertPos3.x(), capT = scrVertPos1.y() * scrVertPos3.x() - scrVertPos1.x() * scrVertPos3.y();	
	float abp = abpX * (left + 0.5) + abpY * (bottom + 0.5) + abpT;//ab�߲��
	float bcp = bcpX * (left + 0.5) + bcpY * (bottom + 0.5) + bcpT;
	float cap = capX * (left + 0.5) + capY * (bottom + 0.5) + capT;

	float centreX = (scrVertPos1.x() + scrVertPos2.x() + scrVertPos3.x()) / 3,
		centreY = (scrVertPos1.y() + scrVertPos2.y() + scrVertPos3.y()) / 3;
	if (abpX * centreX + abpY * centreY + abpT < 0)
		return;//Ϊ���� ֱ���޳�

	const float reciOfDoubldS = 1 / (abp + bcp + cap);//��������������ĵ��� ���Լ�����������

	//ÿ����Զ���ֱ����һ����ɫ��
	int vertexIndexes[3] = {
		getIndex(scrVertPos1.x(), scrVertPos1.y(), width, height) ,
		getIndex(scrVertPos2.x(), scrVertPos2.y(), width, height) ,
		getIndex(scrVertPos3.x(), scrVertPos3.y(), width, height) };
	QRgb vertexColors[3];
	//HoVector4f ambientLight = fragShader.getLight().ka * 255;
	//QRgb ambientColor = qRgb(ambientLight[0], ambientLight[1], ambientLight[2]);
	const HoVector4f baCoordses[3] = { {1,0,0},{0,1,0},{0,0,1} };
	for (int i = 0; i != 3; ++i) {
		vertexColors[i] = //��Ϊÿ���淨������һ����ͬһ������ܻᱻ����渴�� ���۵�ǰ����ͨ����Ȼ��嶼Ҫ����˴�
			fragShader.shading(generateFrag(vertexIndexes[i], obj.getVertPosAt(verts[i]).z(), obj.getMaterialAt(face), verts, getCorrInterpo(baCoordses[i], verts)));
	}


	for (int y = bottom; y != up; ++y) {
		bool flag = false;//�Ż�һ�� ������������ ������һ�����±���
		float abp1 = abp, bcp1 = bcp, cap1 = cap;
		for (int x = left; x != right; ++x) {
			if (abp1 >= 0 && bcp1 >= 0 && cap1 >= 0) {//����������
				flag = true;
				int index = getIndex(x, y, width, height);
				//if (index < s) {
					//��Ļ�����µ���������
					HoVector4f scBaryCoords{ bcp1 * reciOfDoubldS ,cap1 * reciOfDoubldS ,abp1 * reciOfDoubldS };
					pair<bool, float> zTest = 
						depthTest(index, { scrVertPos1.z(),scrVertPos2.z() ,scrVertPos3.z() }, scBaryCoords);
					if (zTest.first) {//ͨ����Ȳ��� 
						HoVector4f myColor = {
							qRed(vertexColors[0]) * scBaryCoords[0] + qRed(vertexColors[1]) * scBaryCoords[1] + qRed(vertexColors[2]) * scBaryCoords[2],
							qGreen(vertexColors[0]) * scBaryCoords[0] + qGreen(vertexColors[1]) * scBaryCoords[1] + qGreen(vertexColors[2]) * scBaryCoords[2],
							qBlue(vertexColors[0]) * scBaryCoords[0] + qBlue(vertexColors[1]) * scBaryCoords[1] + qBlue(vertexColors[2]) * scBaryCoords[2],
						};
						frame[index] = qRgb(myColor[0], myColor[1], myColor[2]);
					}

				//}
			}
			else if (flag)
				break;
			abp1 += abpX; bcp1 += bcpX; cap1 += capX;//����
		}
		abp += abpY; bcp += bcpY; cap += capY;//����
	}
}

void Rasterization::drawTriangleFlat(const vector<Face>& faces) {
	//ȷ��ÿ�������θ��ǵ�����
	for (int i = 0; i != faces.size(); ++i) {
		if (faces[i].getValid()) {//δ�ü�
			FlatShading(faces[i]);
		}
	}
}

void Rasterization::drawTriangleGouraud(const vector<Face>& faces) {
	//ȷ��ÿ�������θ��ǵ�����
	for (int i = 0; i != faces.size(); ++i) {
		if (faces[i].getValid()) {//δ�ü�
			GouraudShading(faces[i]);
		}
	}
	
}


void Rasterization::drawTrianglePhong(const vector<Face>& faces) {
	//ȷ��ÿ�������θ��ǵ�����
	for (int i = 0; i != faces.size(); ++i) {
		if (faces[i].getValid()) {//δ�ü�
			PhongShading(faces[i]);
		}
	}
}


Fragment Rasterization::generateFrag(const int& index, const float& fragScreenZ, const Material* material, const array<Vertex, 3>& verts, const CorrInterpolation& corrInterp) {
	const HoVector4f orignVertPos1 = orignObj.getVertPosAt(verts[0]), 
		orignVertPos2 = orignObj.getVertPosAt(verts[1]),
		orignVertPos3 = orignObj.getVertPosAt(verts[2]);
	float x = corrInterp.interp(orignVertPos1.x(), orignVertPos2.x(), orignVertPos3.x());
	float y = corrInterp.interp(orignVertPos1.y(), orignVertPos2.y(), orignVertPos3.y());
	float z = corrInterp.interp(orignVertPos1.z(), orignVertPos2.z(), orignVertPos3.z());
	HoVector4f worldPos{ x,y,z };
	HoVector4f normal = corrInterp.interp(obj.getVertNormAt(verts[0]), obj.getVertNormAt(verts[1]), obj.getVertNormAt(verts[2]));
	
	bool hasTxKd = false;
	HoVector3f texKd;
	if (material && material->hasTextureKd()) {
		hasTxKd = true;
		HoVector3f texKdPos = corrInterp.interp(obj.getVertTexAt(verts[0]), obj.getVertTexAt(verts[1]), obj.getVertTexAt(verts[2]));
		//ѡ��������˫���Բ�ֵ
		if (FuncCodes::hasBILINEAR(curCode)) {
			texKd = material->getBiliKdAt(texKdPos.x(), texKdPos.y());
		}
		else			
			texKd = material->getKdAt(texKdPos.x(), texKdPos.y());
	}
	
		
	return Fragment(index, fragScreenZ, worldPos, normal, { hasTxKd ,texKd });
}



pair<bool, float> Rasterization::depthTest(const int& scIndex, const array<const float, 3>& scrZ, const HoVector4f& scBaryCoords) {
	//δ͸�ӽ���
	float z = scrZ[0] * scBaryCoords[0] + scrZ[1] * scBaryCoords[1] + scrZ[2] * scBaryCoords[2];
	if (z > zBuffer[scIndex]) {
		zBuffer[scIndex] = z;
		return { true ,z };
	}
	else
		return { false,z };
}

void Rasterization::run() {
    //�ӿڱ任 �������Ļ �˺󶥵�����������Ļ�ϵ������� (height-y)
    viewportTransform(obj.getVertexes());
   

    //��դ�� ȷ�����ظ��� ����ƬԪ
	//rasterize(obj.getFaces());
	vector<Face>& faces = obj.getFaces();
	if (FuncCodes::hasMESH(curCode))
		drawLineInFace(faces);
	else if (FuncCodes::hasFLAT(curCode))
		drawTriangleFlat(faces);
	else if (FuncCodes::hasGOURAND(curCode))
		drawTriangleGouraud(faces);
	else if (FuncCodes::hasPHONG(curCode))
		drawTrianglePhong(faces);

}