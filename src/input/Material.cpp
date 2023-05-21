
#include "Material.h"

void Material::setTextureKd(string file) {
	QImage image(QString::fromLocal8Bit(file.data()));
	int width = image.width(), height = image.height();
	textureKd.resize(height, vector<HoVector3f>(width));
	for (int row = 0; row < height; ++row) {
		QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(row));
		for (int col = 0; col < width; ++col) {
			QRgb color = line[col];
			textureKd[height - row - 1][col] = { (float)qRed(color), (float)qGreen(color), (float)qBlue(color) };
		}
	}
}

void Material::setTextureKs(string file) {
	QImage image(QString::fromLocal8Bit(file.data()));
	int width = image.width(), height = image.height();
	textureKs.resize(height, vector<HoVector3f>(width));
	for (int row = 0; row < height; ++row) {
		QRgb* line = reinterpret_cast<QRgb*>(image.scanLine(row));
		for (int col = 0; col < width; ++col) {
			QRgb color = line[col];
			textureKs[height - row - 1][col] = { (float)qRed(color), (float)qGreen(color), (float)qBlue(color) };
		}
	}
}

HoVector3f Material::getBilinearFor(const vector<vector<HoVector3f>>& texture, float u, float v)const {
	//注意理清像素中心与坐标的关系
	int length = texture.size();
	float xPos = u * (length - 1) + 0.5, yPos = v * (length - 1) + 0.5;
	int centrX = roundf(xPos), centrY = roundf(yPos);
	//该点恰好位于边缘某像素中心 不用插值
	if (centrX == length || centrY == length || centrX == 0 || centrY == 0)
		return getKdAt(u, v);
	//leftUp rightUp
	//leftBott rightBott
	HoVector3f leftUp, rightUp, leftBott, rightBott;
	leftUp = texture[centrY - 1][centrX], rightUp = texture[centrY][centrX];
	leftBott = texture[centrY - 1][centrX - 1], rightBott = texture[centrY - 1][centrX];

	float ty = (centrY + 0.5) - yPos, py = 1 - ty;
	HoVector3f yInterpL = leftBott * ty + leftUp * py;
	HoVector3f yInterpR = rightBott * ty + rightUp * py;

	float tx = (centrX + 0.5) - xPos;
	return (yInterpL * tx + yInterpR * (1 - tx)) / 255;
}

HoVector3f Material::getBiliKdAt(float u, float v)const {
	return getBilinearFor(textureKd, u, v);
}