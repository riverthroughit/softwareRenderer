#pragma once
#include"input/ObjData.h"
#include"scene/Light.h"
#include"Fragment.h"
#include<qrgb.h>

class FragmentShader
{
	vector<Fragment>& frags;
	QRgb* frames;
	const float* zBuffer;
	Light light;	
	//const ObjData& orignObj;
	const HoVector4f cameraPos;
	QRgb BlinnPhongShading(const Fragment& frag);
	
public:
	FragmentShader(vector<Fragment>& frs, QRgb* f, const float* zBu,
		const Light& l, const HoVector4f came) :
		frags(frs), frames(f),zBuffer(zBu), light(l), cameraPos(came) {}
	void run(const unsigned short& curCode);
	QRgb shading(const Fragment& frag);
	Light getLight() {
		return light;
	}
};

