#pragma once
#include<string>
#include<vector>
#include<qimage.h>
#include"myMath/HoVector4.h"
#include <qrgb.h>
using std::string;
using std::vector;
using std::pair;
using myMath::HoVector4f;
using myMath::HoVector3f;
struct Material
{
	//friend class Mtl;
	float ns = 0;//����� �ƺ�Ҳ�ò���
	HoVector3f ka, kd, ks, ke;
	float ni = 1;
	float d = 1;//͸����
	unsigned short illum = 0;//����ģ�� ���Ժ������
	vector<vector<HoVector3f>> textureKd;
	vector<vector<HoVector3f>> textureKs;

	
	void setTextureKd(string file);
	void setTextureKs(string file);

	bool hasTextureKd()const {
		return !textureKd.empty();
	}

	bool hasTextureKs()const {
		return !textureKs.empty();
	}

	HoVector3f getBiliKdAt(float u, float v)const;

	HoVector3f getKdAt(float u, float v)const {
		//ע��˳�� ע��y�ᷭת
		return textureKd[v * (textureKd.size() - 1)][u * (textureKd[0].size() - 1)] / 255;
	}

	HoVector3f getKsAt(float u, float v)const {
		//ע��˳��
		return textureKs[v * (textureKs.size() - 1)][u * (textureKs[0].size() - 1)] / 255;
	}

private:
	HoVector3f getBilinearFor(const vector<vector<HoVector3f>>& texture, float u, float v)const;
};


