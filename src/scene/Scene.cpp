#include "Scene.h"

void Scene::changeCamera(const pair<short, pair<int, int>>& mouseData) {
	if (abs(mouseData.second.first) == abs(mouseData.second.second))//忘了
		return;
	HoVector4f circleCentre = camera.getObserve();
	if (mouseData.first == 3) {//相机观测点移动
		const int& x = mouseData.second.first;
		const int& y = mouseData.second.second;
		//相机观察方向 世界空间下
		HoVector4f observeDirec = circleCentre - camera.getPos();
		float t = 0.1;//鼠标移动1个距离则在相机空间中移动t个距离
		//相机本身移动 相机坐标系下
		HoVector4f cameraPos = { t * x,t * y,0 };
		// 再将其变换世界坐标下即可 乘view矩阵的逆矩阵
		cameraPos *= camera.getInverseViewTransform();
		camera.setPosAndObserve(cameraPos, cameraPos + observeDirec);
	}
	else if (mouseData.first == 2) {//相机距离
		HoVector4f pos = camera.getPos();
		HoVector4f direc = (circleCentre - pos).normalize();
		int moveDistan = mouseData.second.second / 24;
		direc *= moveDistan;
		camera.setPos(pos + direc, camera.getUpDirec());
	}
	else {//相机转动
		const int& x = mouseData.second.first;
		const int& y = mouseData.second.second;
		//试想在相机坐标系内 此中心恰好位于z轴上
		//此时可想出相机绕该点移动的切线方向恰好在xy平面上
		//且方向向量(mouseData.x,mouseData.y)的反方向
		//且移动角度与鼠标移动距离即为圆心角和弧的关系

		//先view变换将中心转换到相机坐标系下
		HoVector4f centreInCamera = circleCentre * camera.getViewTransform();
		//此时圆半径即为圆心到相机距离 (且相机在原点处 圆心在z负半轴上)
		float r = -centreInCamera.z();

		//鼠标移动距离映射为相机移动的圆弧角度
		//将鼠标单位移动距离映射为坐标系中t个角度 
		float t = 0.01;
		float arcDegree = t * sqrt(x * x + y * y);
		//一半圆弧对应的角度
		float angle = arcDegree / 2;
		//旋转终点与起点连线长度(的平方)
		float baseSquare = pow(r * sin(angle) * 2, 2);

		HoVector4f pos = camera.getPos(), upDirec = camera.getUpDirec();
		//cosU观察方向与当前向上方向夹角的余弦
		float cosU = upDirec * (circleCentre - pos).normalize();
		//设终点为(x1,y1,z1) 
		//将旋转的轨迹(圆)投影到xy平面 恰为一条过原点的直线 y1/x1=y/x=k
		//终点到圆心连线长度为r可得一个方程
		//终点与起点连线长度可表示为一个直角三角形斜边 又得一方程
		//联立即可
		HoVector4f newPos;
		float z1 = -baseSquare / (2 * r);
		if (x && abs(cosU) < 0.95) {//abs(cosU)<0.9防止在俯瞰时快速旋转 //175度(-0.9961) 或5度(0.9961)
			float k = y / x;
			float x1 =
				sqrtf((baseSquare - baseSquare * baseSquare / (4 * r * r)) / (k * k + 1));
			float y1 = x1 * k;
			//最后确定 x1 y1正负号(与鼠标移动方向相反)
			if (x > 0)
				x1 = -x1, y1 = -y1;
			newPos = { x1,y1,z1 };
		}
		else {//x为0或相机接近垂直俯瞰时特殊判断
			float y1 = sqrtf(baseSquare - baseSquare * baseSquare / (4 * r * r));
			if (y > 0)
				y1 = -y1;
			newPos = { 0,y1,z1 };
		}
		//因为该坐标是之前的相机坐标系下的
		// 再将其变换世界坐标下即可 乘view矩阵的逆矩阵
		newPos *= camera.getInverseViewTransform();

		//前后两次观察方向与向上方向的叉乘方向相反 则说明经过z=circlecentre.z()平面
		//连接移动前后两点 连接球的上下顶点 有交点 为经过上下顶点的必要条件

		HoVector4f obserDirc = circleCentre - pos, newObserDirc = circleCentre - newPos;

		//相机前后位置与圆心形成的平面 该平面的垂线
		//当沿着y轴旋转时 该垂线靠近y轴 当沿着x轴旋转时 该垂线靠近x轴
		HoVector4f vert = (obserDirc ^ newObserDirc).normalize();

		//而当沿着y轴旋转时 向上方向不应该取反

		//if (abs(vert * upDirec) < 0.1 && (upDirec ^ obserDirc)[0] * (upDirec ^ newObserDirc)[0] < 0)
		//	camera.setPos(newPos, -upDirec);
		//else
		//	camera.setPos(newPos, upDirec);

		//23 3 8
		//(upDirec ^ obserDirc)和(upDirec ^ newObserDirc)之间夹角过大可近似看作翻越顶端或者底端(此时两者恰好反向)
		if (abs(vert * upDirec) < 0.1 && (upDirec ^ obserDirc) * (upDirec ^ newObserDirc) < 0)
			camera.setPos(newPos, -upDirec);
		else
			camera.setPos(newPos, upDirec);
	}
}

void Scene::changeLight(const pair<short, pair<int, int>>& lightData) {
	float degree = my_pi / 36;//一次行进5度
	const float r = circleCentre.z();//半径
	float yRatioR = light.pos.y() / r;//y方向与半径夹角正弦
	float yRdegree = std::asinf(yRatioR);//y方向与半径夹角

	//上下旋转 tanXZ不变
	//左右旋转 y不变 始终是在平行于xz面的小圆面旋转

	if (lightData.second.second == 1 || lightData.second.second == -1) {//上或下
		if (lightData.second.second == -1) {//向下
			degree = -degree;
			if (std::abs(yRdegree + degree * 4 + my_pi / 2) < 1e-6)//光源快达到模型底端
				return;
		}
		else if (std::abs(yRdegree + degree * 4 - my_pi / 2) < 1e-6)//光源快达到模型顶端
			return;

		yRdegree += degree;//移动后的夹角
		float y = std::sinf(yRdegree) * r;//转动后的y
		if (std::abs(light.pos.x()) < 1e-6) {//x为0特殊判断
			float z;
			if (light.pos.z() < circleCentre.z())
				z = -std::sqrtf(r * r - y * y) + circleCentre.z();
			else
				z = std::sqrtf(r * r - y * y) + circleCentre.z();
			light.pos = { 0,y , z };
			return;
		}
		//解方程组 x^2+(z-o.z)^2=r^2-y^2   z-o.z=x*tanZX
		//x^2=(r^2-y^2)/(1+tanZX^2)    (1+tanZX^2)=cosZX^2
		float tanZX = (light.pos.z() - circleCentre.z()) / light.pos.x();//球上z方向长度与x方向比值
		float x = std::sqrtf((r * r - y * y) / (1 + tanZX * tanZX));
		if (light.pos.x() < 0)//变下符号
			x = -x;
		float z = x * tanZX + circleCentre.z();
		light.pos = { x,y ,z };
	}
	else if (lightData.second.first == 1 || lightData.second.first == -1) {
		if (lightData.second.first == 1)//顺时针时
			degree = -degree;
		float rr = r * std::cosf(yRdegree);//小圆面(xz平面)的半径
		float curDegree = acosf(light.pos.x() / rr);//当前光源位置与小圆面圆心连线同x正方向夹角
		if (light.pos.z() < circleCentre.z())//变下符号
			curDegree = -curDegree;
		curDegree += degree;
		light.pos.x() = rr * std::cosf(curDegree);
		light.pos.z() = rr * std::sinf(curDegree) + circleCentre.z();
	}
}

void Scene::initialScene() {
	//最好先弄一个模型变换 将物体移动到相机面前
	for (ObjData& obj : objs)
		obj.moveToBevelOf(camera);

	//场景中心和相机初始的观察点设置为第一个模型的中心
	if (!objs.empty()) {
		circleCentre = objs[0].getBoundry().core;
		camera.setObserve(circleCentre);
	}
}

void Scene::openNewModel(string file) {
	objs.clear();
	objs.emplace_back(file);
	initialScene();
}

void Scene::changeScene(const userChange& change) {
	//初始化相机 光照信息
	const pair<short, pair<int, int>>& sceneData = change.data;//相机或光照变化信息
	if (!sceneData.first)//无变化
		return;
	if (sceneData.first == 5)//打开新文件
		openNewModel(change.file);
	else if (sceneData.first == 4)//光照变化
		changeLight(sceneData);
	else//相机变化
		changeCamera(sceneData);
}

