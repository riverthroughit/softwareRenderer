#include "FragmentShader.h"
#include"output/FuncCodes.h"

QRgb FragmentShader::BlinnPhongShading(const Fragment& frag) {
	HoVector4f lightDirec = light.pos - frag.worldPos;
	//float reciDoubleDist = 1 / lightDirec.squareOfLength();
	lightDirec.normalize();
	HoVector3f ka = light.ka, kd, ks;
	float ns = light.ns;
	
	if (frag.kd.first)
		kd = frag.kd.second;
	else
		kd = light.kd;
	ks = light.ks;


	//HoVector4f dColor = kd * reciDoubleDist * std::max(lightDirec * frag.normal, float(0));
	HoVector3f dColor = kd * float(lightDirec * frag.normal * 0.5 + 0.5);//��������
	HoVector4f cameraDirec = (cameraPos - frag.worldPos).normalize();
	HoVector4f h = (lightDirec + cameraDirec) / 2;//�������
	//HoVector4f sColor = ks * reciDoubleDist * std::pow(std::max(h * frag.normal, float(0)), light.ns);
	HoVector3f sColor = ks * std::pow(std::max(h * frag.normal, float(0)), ns);
	HoVector3f aColor = ka;
	HoVector3f color = (dColor + sColor + aColor) * 255;
	return	qRgb(std::min((int)color[0], 255), std::min((int)color[1], 255), std::min((int)color[2], 255));
}




QRgb FragmentShader::shading(const Fragment& frag) {
	
	return BlinnPhongShading(frag);
	
}

void FragmentShader::run(const unsigned short& curCode) {
	if (FuncCodes::hasPHONG(curCode)) {//flat��gourand�Ѿ��ڹ�դ���׶����ƬԪ��ɫ
		for (Fragment& frag : frags) {
			if (std::abs(frag.screenZ - zBuffer[frag.screenPos]) < 1e-6)//ͨ��zbuffer
				frames[frag.screenPos] = shading(frag);
		}
		frags.clear();
	}
}