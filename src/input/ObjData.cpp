#include "ObjData.h"


//ObjData::ObjData(const ObjData& obj) :
//	v(obj.v), vt(obj.vt), vn(obj.vn), materials(obj.materials), boundry(obj.boundry) {
//	faces.reserve(obj.faces.size());
//	for (const Face& f : obj.faces) {
//		//注意要更改顶点所指的ObjData
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
		//记录六个面的位置
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
	//注意减一 文件内的索引从1开始
	//注意可能不存在法向量,但纹理存在 x/x/
	//也可能只有顶点 没有纹理和法向量 此时 f a b c 不存在'/'
	vector<Vertex> vertices;//存储所有的点 注意考虑点数大于三的情况
	while (iss >> nums) {
		if (nums.back() != '/')//若结尾无/ 加上/
			nums.push_back('/');
		int verIndex = -1, texIndex = -1, vnIndex = -1;//物理坐标 纹理坐标 法向量
		string indexStr;
		for (int i = 0, flag = 0, beg = 0; i != nums.size(); ++i) {
			if (nums[i] == '/') {
				int index = -1;
				if (!indexStr.empty())// a//c 此种无纹理情况indexStr为空
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
	//多边形面的情况 将其划分为多个三角形即可
	//vertices.size() - 2即为划分出的三角形面的数量
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
		//去除最前面的空白先
		mLine.erase(0, mLine.find_first_not_of(" "));
		//mLine.erase(mLine.find_first_not_of(" ") + 1);
		if (mLine.empty() || mLine[0] == '#')//跳过注释和空白
			continue;
		if (mLine[0] == 'n') {//newmtl xxxxxx
			materials->emplace_back();//创建material对象
			mymap[mLine.substr(7)] = materials->size() - 1;//关联其名字与索引 为了后续f的构造
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
				//将向量的三个坐标都读入nums中 用于初始化对应属性
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
				string textureKind, file;//纹理类型 和纹理文件
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
	unordered_map<string, int> mymap;//关联材质名称和其位于vector<Material>中的索引
	short maIndex = -1;//初始时未使用usemtl 故令材质索引为-1
	while (getline(fin, line)) {
		//除去最前面的空白先
		line.erase(0, line.find_first_not_of(" "));
		//line.erase(line.find_first_not_of(" ") + 1);
		if (line.empty() || line[0]=='#')//空行和注释就没必要读了
			continue;	
		switch (line[0])
		{
		case 'm'://mtllib xxxx.mtl
			//去读取材质文件内的信息
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
	if (vn.empty())//没有法向量 为每个面构造一个
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
	//先根据包围盒将自身缩放到合适大小 scaleTo
	//再将此时包围盒移动到四棱台 与其底面贴合 求出其中心平移的向量
	//对其余顶点均用此向量平移即可
	float boundXlength = boundry.rightX - boundry.leftX,
		boundYlength = boundry.topY - boundry.bottomY,
		boundZlength = boundry.frontZ - boundry.backZ;
	array<float, 2> xyLength =
		camera.getInscribedRectlength(boundXlength, boundYlength, boundZlength);
	float scaleByX = xyLength[0] / boundXlength,
		scaleByY = xyLength[1] / boundYlength;
	float scaleByZ = camera.getBevelZlength() / boundZlength;

	float s = std::min({ scaleByX,scaleByY,scaleByZ });
	//选取缩放程度最大的那条边进行缩放 因为要保证包围盒所有边都位于四棱台内
	scaleTo(s);
	//因为相机是固定了往z轴正方向看的 所以我们不用对物体进行旋转 包围盒能与四棱台贴合
	
	//新的中心 四棱台内
	HoVector4f newCore = camera.getCoreOfBoundry(boundry.frontZ - boundry.backZ);
	HoVector4f translateVec = newCore - boundry.core;
	// 接下来平移即可
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

