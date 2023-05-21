#include "Render.h"
#include"VertexShader.h"
#include"Rasterization.h"
#include"output/Frames.h"
#include"output/FramesInRenderer.h"
#include"output/FuncCodes.h"
#include"output/UserSceneData.h"
#include"FragmentShader.h"

void Render::moveAddedVerts(ObjData& orignObj) {//删掉因为裁剪而新增的顶点
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
		//初始化像素
		QRgb* frame = FramesInRenderer::generateFrames(s);
		//初始化zBuffer
		std::memset(zBuffer, 203, sizeof(float) * s);//按字节 随便赋一个很小的数字就行
		
		//用此信息改变相机或光照属性 打开文件
		scene.changeScene(UserSceneData::getChange());
		
		for (int i = 0; i != orignObjs.size();++i) {			
			unsigned short curCode = FuncCodes::getCode();//获取用户选择的功能
			//若没有选择新的选项 则延续当前模式
			funcCode = curCode == NODATA ? funcCode : curCode;

			//用此信息改变相机或光照属性 打开文件
			scene.changeScene(UserSceneData::getChange());

			//每个模型每次渲染前导入(经过model变换后的)原始模型数据
			ObjData obj = orignObjs[i];

			//顶点着色器 mvp变换以及裁剪 传入原始顶点的坐标是为了裁剪后生成的新顶点
			VertexShader(orignObjs[i].getVertexes(), obj, 
				scene.getCamera(), newlyAddedVertsSize).run(funcCode);//此处传入裁剪码

			//片元着色器
			FragmentShader fragShader(frags, frame, zBuffer, scene.getLight(),
				scene.getCamera().getPos());
			
			//接着是光栅化 完成后顶点坐标正式变为屏幕坐标 确定像素覆盖以及各种插值计算
			//传选择的功能码
			Rasterization(obj, orignObjs[i], zBuffer, frags, width, height,
				frame, fragShader, scene.getCamera(), funcCode).run();
			//phong频率时根据片元着色
			fragShader.run(funcCode);
			//移除可能因为裁剪新生成的点
			moveAddedVerts(orignObjs[i]);
		}
		if (Frames::getFramesSize() < 10)//前端画不过来 就别嗯入队了
			Frames::addToFrames(frame);//传入帧队列
		else
			delete[] frame;
	}
}

