#include "Scene.h"

void Scene::changeCamera(const pair<short, pair<int, int>>& mouseData) {
	if (abs(mouseData.second.first) == abs(mouseData.second.second))//����
		return;
	HoVector4f circleCentre = camera.getObserve();
	if (mouseData.first == 3) {//����۲���ƶ�
		const int& x = mouseData.second.first;
		const int& y = mouseData.second.second;
		//����۲췽�� ����ռ���
		HoVector4f observeDirec = circleCentre - camera.getPos();
		float t = 0.1;//����ƶ�1��������������ռ����ƶ�t������
		//��������ƶ� �������ϵ��
		HoVector4f cameraPos = { t * x,t * y,0 };
		// �ٽ���任���������¼��� ��view����������
		cameraPos *= camera.getInverseViewTransform();
		camera.setPosAndObserve(cameraPos, cameraPos + observeDirec);
	}
	else if (mouseData.first == 2) {//�������
		HoVector4f pos = camera.getPos();
		HoVector4f direc = (circleCentre - pos).normalize();
		int moveDistan = mouseData.second.second / 24;
		direc *= moveDistan;
		camera.setPos(pos + direc, camera.getUpDirec());
	}
	else {//���ת��
		const int& x = mouseData.second.first;
		const int& y = mouseData.second.second;
		//�������������ϵ�� ������ǡ��λ��z����
		//��ʱ���������Ƹõ��ƶ������߷���ǡ����xyƽ����
		//�ҷ�������(mouseData.x,mouseData.y)�ķ�����
		//���ƶ��Ƕ�������ƶ����뼴ΪԲ�ĽǺͻ��Ĺ�ϵ

		//��view�任������ת�����������ϵ��
		HoVector4f centreInCamera = circleCentre * camera.getViewTransform();
		//��ʱԲ�뾶��ΪԲ�ĵ�������� (�������ԭ�㴦 Բ����z��������)
		float r = -centreInCamera.z();

		//����ƶ�����ӳ��Ϊ����ƶ���Բ���Ƕ�
		//����굥λ�ƶ�����ӳ��Ϊ����ϵ��t���Ƕ� 
		float t = 0.01;
		float arcDegree = t * sqrt(x * x + y * y);
		//һ��Բ����Ӧ�ĽǶ�
		float angle = arcDegree / 2;
		//��ת�յ���������߳���(��ƽ��)
		float baseSquare = pow(r * sin(angle) * 2, 2);

		HoVector4f pos = camera.getPos(), upDirec = camera.getUpDirec();
		//cosU�۲췽���뵱ǰ���Ϸ���нǵ�����
		float cosU = upDirec * (circleCentre - pos).normalize();
		//���յ�Ϊ(x1,y1,z1) 
		//����ת�Ĺ켣(Բ)ͶӰ��xyƽ�� ǡΪһ����ԭ���ֱ�� y1/x1=y/x=k
		//�յ㵽Բ�����߳���Ϊr�ɵ�һ������
		//�յ���������߳��ȿɱ�ʾΪһ��ֱ��������б�� �ֵ�һ����
		//��������
		HoVector4f newPos;
		float z1 = -baseSquare / (2 * r);
		if (x && abs(cosU) < 0.95) {//abs(cosU)<0.9��ֹ�ڸ��ʱ������ת //175��(-0.9961) ��5��(0.9961)
			float k = y / x;
			float x1 =
				sqrtf((baseSquare - baseSquare * baseSquare / (4 * r * r)) / (k * k + 1));
			float y1 = x1 * k;
			//���ȷ�� x1 y1������(������ƶ������෴)
			if (x > 0)
				x1 = -x1, y1 = -y1;
			newPos = { x1,y1,z1 };
		}
		else {//xΪ0������ӽ���ֱ���ʱ�����ж�
			float y1 = sqrtf(baseSquare - baseSquare * baseSquare / (4 * r * r));
			if (y > 0)
				y1 = -y1;
			newPos = { 0,y1,z1 };
		}
		//��Ϊ��������֮ǰ���������ϵ�µ�
		// �ٽ���任���������¼��� ��view����������
		newPos *= camera.getInverseViewTransform();

		//ǰ�����ι۲췽�������Ϸ���Ĳ�˷����෴ ��˵������z=circlecentre.z()ƽ��
		//�����ƶ�ǰ������ ����������¶��� �н��� Ϊ�������¶���ı�Ҫ����

		HoVector4f obserDirc = circleCentre - pos, newObserDirc = circleCentre - newPos;

		//���ǰ��λ����Բ���γɵ�ƽ�� ��ƽ��Ĵ���
		//������y����תʱ �ô��߿���y�� ������x����תʱ �ô��߿���x��
		HoVector4f vert = (obserDirc ^ newObserDirc).normalize();

		//��������y����תʱ ���Ϸ���Ӧ��ȡ��

		//if (abs(vert * upDirec) < 0.1 && (upDirec ^ obserDirc)[0] * (upDirec ^ newObserDirc)[0] < 0)
		//	camera.setPos(newPos, -upDirec);
		//else
		//	camera.setPos(newPos, upDirec);

		//23 3 8
		//(upDirec ^ obserDirc)��(upDirec ^ newObserDirc)֮��нǹ���ɽ��ƿ�����Խ���˻��ߵ׶�(��ʱ����ǡ�÷���)
		if (abs(vert * upDirec) < 0.1 && (upDirec ^ obserDirc) * (upDirec ^ newObserDirc) < 0)
			camera.setPos(newPos, -upDirec);
		else
			camera.setPos(newPos, upDirec);
	}
}

void Scene::changeLight(const pair<short, pair<int, int>>& lightData) {
	float degree = my_pi / 36;//һ���н�5��
	const float r = circleCentre.z();//�뾶
	float yRatioR = light.pos.y() / r;//y������뾶�н�����
	float yRdegree = std::asinf(yRatioR);//y������뾶�н�

	//������ת tanXZ����
	//������ת y���� ʼ������ƽ����xz���СԲ����ת

	if (lightData.second.second == 1 || lightData.second.second == -1) {//�ϻ���
		if (lightData.second.second == -1) {//����
			degree = -degree;
			if (std::abs(yRdegree + degree * 4 + my_pi / 2) < 1e-6)//��Դ��ﵽģ�͵׶�
				return;
		}
		else if (std::abs(yRdegree + degree * 4 - my_pi / 2) < 1e-6)//��Դ��ﵽģ�Ͷ���
			return;

		yRdegree += degree;//�ƶ���ļн�
		float y = std::sinf(yRdegree) * r;//ת�����y
		if (std::abs(light.pos.x()) < 1e-6) {//xΪ0�����ж�
			float z;
			if (light.pos.z() < circleCentre.z())
				z = -std::sqrtf(r * r - y * y) + circleCentre.z();
			else
				z = std::sqrtf(r * r - y * y) + circleCentre.z();
			light.pos = { 0,y , z };
			return;
		}
		//�ⷽ���� x^2+(z-o.z)^2=r^2-y^2   z-o.z=x*tanZX
		//x^2=(r^2-y^2)/(1+tanZX^2)    (1+tanZX^2)=cosZX^2
		float tanZX = (light.pos.z() - circleCentre.z()) / light.pos.x();//����z���򳤶���x�����ֵ
		float x = std::sqrtf((r * r - y * y) / (1 + tanZX * tanZX));
		if (light.pos.x() < 0)//���·���
			x = -x;
		float z = x * tanZX + circleCentre.z();
		light.pos = { x,y ,z };
	}
	else if (lightData.second.first == 1 || lightData.second.first == -1) {
		if (lightData.second.first == 1)//˳ʱ��ʱ
			degree = -degree;
		float rr = r * std::cosf(yRdegree);//СԲ��(xzƽ��)�İ뾶
		float curDegree = acosf(light.pos.x() / rr);//��ǰ��Դλ����СԲ��Բ������ͬx������н�
		if (light.pos.z() < circleCentre.z())//���·���
			curDegree = -curDegree;
		curDegree += degree;
		light.pos.x() = rr * std::cosf(curDegree);
		light.pos.z() = rr * std::sinf(curDegree) + circleCentre.z();
	}
}

void Scene::initialScene() {
	//�����Ūһ��ģ�ͱ任 �������ƶ��������ǰ
	for (ObjData& obj : objs)
		obj.moveToBevelOf(camera);

	//�������ĺ������ʼ�Ĺ۲������Ϊ��һ��ģ�͵�����
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
	//��ʼ����� ������Ϣ
	const pair<short, pair<int, int>>& sceneData = change.data;//�������ձ仯��Ϣ
	if (!sceneData.first)//�ޱ仯
		return;
	if (sceneData.first == 5)//�����ļ�
		openNewModel(change.file);
	else if (sceneData.first == 4)//���ձ仯
		changeLight(sceneData);
	else//����仯
		changeCamera(sceneData);
}

