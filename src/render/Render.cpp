#include "Render.h"
#include"VertexShader.h"
#include"Rasterization.h"
#include"output/Frames.h"
#include"output/FramesInRenderer.h"
#include"output/FuncCodes.h"
#include"output/UserSceneData.h"
#include"FragmentShader.h"

void Render::moveAddedVerts(ObjData& orignObj) {//ɾ����Ϊ�ü��������Ķ���
	if (newlyAddedVertsSize) {
		auto& vertsPos = orignObj.getVertexes();
		vertsPos.erase(vertsPos.end() - newlyAddedVertsSize, vertsPos.end());
		//auto& vns = orignObj.getVns();
		//vns.erase(vns.end() - newlyAddedVertsSize, vns.end());
		//auto& texs = orignObj.getTextures();
		//texs.erase(texs.end() - newlyAddedVertsSize, texs.end());
		newlyAddedVertsSize = 0;
	}
}



void Render::run()
{
	vector<ObjData>& orignObjs = scene.getObjs();
	while (1) {				
		//��ʼ������
		QRgb* frame = FramesInRenderer::generateFrames(s);
		//��ʼ��zBuffer
		std::memset(zBuffer, 203, sizeof(float) * s);//���ֽ� ��㸳һ����С�����־���
		
		//�ô���Ϣ�ı������������� ���ļ�
		scene.changeScene(UserSceneData::getChange());
		
		for (int i = 0; i != orignObjs.size();++i) {			
			unsigned short curCode = FuncCodes::getCode();//��ȡ�û�ѡ��Ĺ���
			//��û��ѡ���µ�ѡ�� ��������ǰģʽ
			funcCode = curCode == NODATA ? funcCode : curCode;

			//�ô���Ϣ�ı������������� ���ļ�
			scene.changeScene(UserSceneData::getChange());

			//ÿ��ģ��ÿ����Ⱦǰ����(����model�任���)ԭʼģ������
			ObjData obj = orignObjs[i];

			//������ɫ�� mvp�任�Լ��ü� ����ԭʼ�����������Ϊ�˲ü������ɵ��¶���
			VertexShader(orignObjs[i].getVertexes(), obj, 
				scene.getCamera(), newlyAddedVertsSize).run(funcCode);//�˴�����ü���

			//ƬԪ��ɫ��
			FragmentShader fragShader(frags, frame, zBuffer, scene.getLight(),
				scene.getCamera().getPos());
			
			//�����ǹ�դ�� ��ɺ󶥵�������ʽ��Ϊ��Ļ���� ȷ�����ظ����Լ����ֲ�ֵ����
			//��ѡ��Ĺ�����
			Rasterization(obj, orignObjs[i], zBuffer, frags, width, height,
				frame, fragShader, scene.getCamera(), funcCode).run();
			//phongƵ��ʱ����ƬԪ��ɫ
			fragShader.run(funcCode);
			//�Ƴ�������Ϊ�ü������ɵĵ�
			moveAddedVerts(orignObjs[i]);
		}
		if (Frames::getFramesSize() < 10)//ǰ�˻������� �ͱ��������
			Frames::addToFrames(frame);//����֡����
		else
			delete[] frame;
	}
}

