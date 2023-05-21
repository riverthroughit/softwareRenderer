#include "ObjData.h"


//ObjData::ObjData(const ObjData& obj) :
//	v(obj.v), vt(obj.vt), vn(obj.vn), materials(obj.materials), boundry(obj.boundry) {
//	faces.reserve(obj.faces.size());
//	for (const Face& f : obj.faces) {
//		//ע��Ҫ���Ķ�����ָ��ObjData
//		array<Vertex, 3> newVerts = f.getVertexes();
//		newVerts[0].setObjPtr(this); newVerts[1].setObjPtr(this); newVerts[2].setObjPtr(this);
//		faces.emplace_back(f.getMaterialIndex(), std::move(newVerts));
//	}		
//}


//ObjData& ObjData::operator=(const ObjData& obj) {
//	v = obj.v; vt = obj.vt; vn = obj.vn;
//	materials = obj.materials; boundry = obj.boundry;
//	faces.reserve(obj.faces.size());
//	for (const Face& f : obj.faces) {
//		array<Vertex, 3> newVerts = f.getVertexes();
//		newVerts[0].setObjPtr(this); newVerts[1].setObjPtr(this); newVerts[2].setObjPtr(this);
//		faces.emplace_back(f.getMaterialIndex(), std::move(newVerts));
//	}
//	return *this;
//}

void ObjData::initialV(const string& line)
{
	istringstream iss(line);
	string s;
	float num[3];
	iss >> s;
	for (int i = 0;i != 3;++i)
		iss >> s, num[i] = stof(s);
	switch (line[1])
	{
	case' '://v -1.000000 -1.000000 1.000000
		v.emplace_back(num[0], num[1], num[2]);
		//��¼�������λ��
		if (num[0] > boundry.rightX)
			boundry.rightX = num[0];
		else if(num[0] < boundry.leftX)
			boundry.leftX = num[0];

		if (num[1] > boundry.topY)
			boundry.topY = num[1];
		else if (num[1] < boundry.bottomY)
			boundry.bottomY = num[1];

		if (num[2] > boundry.frontZ)
			boundry.frontZ = num[2];
		else if (num[2] < boundry.backZ)
			boundry.backZ = num[2];
		break;

	case't'://vt 0.625000 0.750000
		vt.emplace_back(num[0], num[1], 1);
		break;

	case'n'://vn 0.0000 0.0000 1.0000
		vn.emplace_back(num[0], num[1], num[2], 0);
		break;
	}
}

void ObjData::initialFaces(string line, int maIndex)
{
	istringstream iss(line);
	string nums;
	//ע���һ �ļ��ڵ�������1��ʼ
	//ע����ܲ����ڷ�����,��������� x/x/
	//Ҳ����ֻ�ж��� û������ͷ����� ��ʱ f a b c ������'/'
	vector<Vertex> vertices;//�洢���еĵ� ע�⿼�ǵ��������������
	while (iss >> nums) {
		if (nums.back() != '/')//����β��/ ����/
			nums.push_back('/');
		int verIndex = -1, texIndex = -1, vnIndex = -1;//�������� �������� ������
		string indexStr;
		for (int i = 0, flag = 0, beg = 0; i != nums.size(); ++i) {
			if (nums[i] == '/') {
				int index = -1;
				if (!indexStr.empty())// a//c �������������indexStrΪ��
					index = stoi(indexStr) - 1;
				switch (flag) {
				case 0:
					verIndex = index;
					break;
				case 1:
					texIndex = index;
					break;
				case 2:
					vnIndex = index;
					break;
				}
				beg = i + 1, ++flag;
				indexStr.clear();
			}
			else
				indexStr += nums[i];
		}
		vertices.emplace_back(verIndex, vnIndex, texIndex);
	}
	//����������� ���仮��Ϊ��������μ���
	//vertices.size() - 2��Ϊ���ֳ����������������
	for (int i = 0;i != vertices.size() - 2;++i) {
		array<Vertex, 3> vertexInFace{
			vertices[0],vertices[1 + i],vertices[2 + i]
		};
		faces.emplace_back(maIndex, vertexInFace);
	}
	
}

void ObjData::initialMtl(string mFile, unordered_map<string, int>& mymap) {
	ifstream mfin(mFile);
	if (!mfin) {
		throw std::runtime_error("material file did not exist");
	}
	string mLine;
	while (getline(mfin, mLine)) {
		//ȥ����ǰ��Ŀհ���
		mLine.erase(0, mLine.find_first_not_of(" "));
		//mLine.erase(mLine.find_first_not_of(" ") + 1);
		if (mLine.empty() || mLine[0] == '#')//����ע�ͺͿհ�
			continue;
		if (mLine[0] == 'n') {//newmtl xxxxxx
			materials->emplace_back();//����material����
			mymap[mLine.substr(7)] = materials->size() - 1;//���������������� Ϊ�˺���f�Ĺ���
		}
		else {
			Material& curMa = materials->back();
			switch (mLine[0]) {
			case'N':
				if (mLine[1] == 's')//ns
					curMa.ns = stof(mLine.substr(3));
				else//ni
					curMa.ni = stof(mLine.substr(3));
				break;
			case'K'://ka kd ks ke
				//���������������궼����nums�� ���ڳ�ʼ����Ӧ����
				initialK(mLine.substr(0), curMa);
				break;
			case'd'://d
				curMa.d = stof(mLine.substr(2));
				break;
			case 'i'://illum
				curMa.illum = stoi(mLine.substr(6));
				break;
			case'm':
				istringstream ism(mLine);
				string textureKind, file;//�������� �������ļ�
				ism >> textureKind >> file;
				switch (textureKind.back())
				{
				case 'd'://map_Kd
					curMa.setTextureKd(mFile.substr(0, mFile.rfind('/') + 1) + file);
					break;
				case 's'://map_Ks
					curMa.setTextureKs(mFile.substr(0, mFile.rfind('/') + 1) + file);
					break;
				default:
					break;
				}								
				break;
			}
		}
	}
}

void ObjData::initialK(const string& mLine, Material& curMa) {
	istringstream ism(mLine.substr(3));
	float nums[3];
	string num;
	for (int i = 0; i != 3; ++i)
		ism >> num, nums[i] = stof(num);
	auto getNums = [&](HoVector3f& k) {
		for (int i = 0; i != 3; ++i)
			k[i] = nums[i];
	};
	switch (mLine[1]) {
	case'a':
		getNums(curMa.ka);
		break;
	case'd':
		getNums(curMa.kd);
		break;
	case's':
		getNums(curMa.ks);
		break;
	case'e':
		getNums(curMa.ke);
		break;
	}
}

ObjData::ObjData(string meshFile):materials(std::make_shared<vector<Material>>())
{
	ifstream fin(meshFile);
	if (!fin)
		throw std::runtime_error("obj file did not exist");
	string line;
	unordered_map<string, int> mymap;//�����������ƺ���λ��vector<Material>�е�����
	short maIndex = -1;//��ʼʱδʹ��usemtl �����������Ϊ-1
	while (getline(fin, line)) {
		//��ȥ��ǰ��Ŀհ���
		line.erase(0, line.find_first_not_of(" "));
		//line.erase(line.find_first_not_of(" ") + 1);
		if (line.empty() || line[0]=='#')//���к�ע�;�û��Ҫ����
			continue;	
		switch (line[0])
		{
		case 'm'://mtllib xxxx.mtl
			//ȥ��ȡ�����ļ��ڵ���Ϣ
			initialMtl(meshFile.substr(0,meshFile.rfind('/')+1) + line.substr(7),mymap);
			break;
		case 'v':
			initialV(line.substr(0));
			break;
		case 'u'://usemtl Material
			maIndex = mymap[line.substr(7)];
			break;
			
		case 'f'://f 1/1/1 5/2/1 7/3/1
			initialFaces(line.substr(2), maIndex);
			break;
		}
	}
	boundry.setCore();
	if (vn.empty())//û�з����� Ϊÿ���湹��һ��
		caculateNormals();
}




void ObjData::scaleTo(float s) {
	for (HoVector4f& vertex : v)
		vertex *= { s, 0, 0, 0,
					0, s, 0, 0,
					0, 0, s, 0,
					0, 0, 0, 1,
								};
	boundry.getScaledBy(s);
}


void ObjData::rorate(const HoVector4f& hovec, float degree) {

}


void ObjData::translateBy(HoVector4f hovec) {
	for (HoVector4f& vertex : v)
		vertex *= { 1, 0, 0, 0,
					0, 1, 0, 0,
					0, 0, 1, 0,
		hovec[0], hovec[1], hovec[2], 1,
								};
	boundry.getTranslatedBy(hovec);
}


void ObjData::moveToBevelOf(Camera camera) {
	//�ȸ��ݰ�Χ�н��������ŵ����ʴ�С scaleTo
	//�ٽ���ʱ��Χ���ƶ�������̨ ����������� ���������ƽ�Ƶ�����
	//�����ඥ����ô�����ƽ�Ƽ���
	float boundXlength = boundry.rightX - boundry.leftX,
		boundYlength = boundry.topY - boundry.bottomY,
		boundZlength = boundry.frontZ - boundry.backZ;
	array<float, 2> xyLength =
		camera.getInscribedRectlength(boundXlength, boundYlength, boundZlength);
	float scaleByX = xyLength[0] / boundXlength,
		scaleByY = xyLength[1] / boundYlength;
	float scaleByZ = camera.getBevelZlength() / boundZlength;

	float s = std::min({ scaleByX,scaleByY,scaleByZ });
	//ѡȡ���ų̶����������߽������� ��ΪҪ��֤��Χ�����б߶�λ������̨��
	scaleTo(s);
	//��Ϊ����ǹ̶�����z�������򿴵� �������ǲ��ö����������ת ��Χ����������̨����
	
	//�µ����� ����̨��
	HoVector4f newCore = camera.getCoreOfBoundry(boundry.frontZ - boundry.backZ);
	HoVector4f translateVec = newCore - boundry.core;
	// ������ƽ�Ƽ���
	translateBy(translateVec);
}

void ObjData::caculateNormals() {
	for (Face& face : faces) {
		array<Vertex, 3>& vertexes = face.getVertexes();
		HoVector4f ab = getVertPosAt(vertexes[1]) - getVertPosAt(vertexes[0]),
			bc = getVertPosAt(vertexes[2]) - getVertPosAt(vertexes[1]);

		vertexes[0].setNormalIndex(vn.size()); 
		vertexes[1].setNormalIndex(vn.size()); 
		vertexes[2].setNormalIndex(vn.size());
		vn.emplace_back((ab ^ bc).normalize());
	}
}

