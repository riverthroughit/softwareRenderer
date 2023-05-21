#pragma once
#include"HoVector3.h"
#include"Matrix4f.h"
namespace myMath {

	
	template<typename T>
	class HoVector4
	{
		//array<T, 4> coordinate;
		T coordinate[4];
		//T* coordinate;//便于移动构造

		//void checkIfVector() const{
		//	if (coordinate[3])
		//		throw std::runtime_error("only vectors can do this");
		//}

		//void checkIfSameKind(const HoVector4f& hoVector4f) const {
		//	if (coordinate[3] != hoVector4f[3])
		//		throw std::runtime_error("only vectors or coordinate of the same kind can do this");
		//}

	public:
		//默认为点 而非向量
		HoVector4(T a = 0, T b = 0, T c = 0, T d = 1) :
			coordinate{ a, b, c, d } {}//(new T[4] { a, b, c, d })

		////拷贝控制
		//HoVector4f(const HoVector4f& hoVector4f):
		//	coordinate(new T[4] { hoVector4f[0], hoVector4f[1], hoVector4f[2], hoVector4f[3] }) {}
		//HoVector4f(HoVector4f&& hoVector4f)noexcept :
		//	coordinate(hoVector4f.coordinate) {
		//	hoVector4f.coordinate = 0;
		//}
		////注意自赋值
		//HoVector4f& operator=(const HoVector4f& hoVector4f) {
		//	T* c = new T[4] {hoVector4f[0], hoVector4f[1], hoVector4f[2], hoVector4f[3]};
		//	delete[] coordinate;
		//	coordinate = c;
		//	return *this;
		//}

		//HoVector4f& operator=(HoVector4f&& hoVector4f)noexcept {
		//	if (this != &hoVector4f) {
		//		delete[] coordinate;
		//		coordinate = hoVector4f.coordinate;
		//		hoVector4f.coordinate = 0;
		//	}			
		//	return *this;
		//}

		//~HoVector4f() {
		//	delete[]coordinate;
		//}


		T& operator[](const int& index) {
			//if (index < 0 || index>3)
			//	throw std::out_of_range("the subscript out of range");
			//else
				return coordinate[index];
		}

		const T& operator[](const int& index)const {
			//if (index < 0 || index>3)
			//	throw std::out_of_range("the subscript out of range");
			//else
				return coordinate[index];
		}

		T& x() {
			return coordinate[0];
		}

		T& y() {
			return coordinate[1];
		}

		T& z() {
			return coordinate[2];
		}

		T& w(){
			return coordinate[3];
		}

		const T& x()const {
			return coordinate[0];
		}

		const T& y()const {
			return coordinate[1];
		}

		const T& z()const {
			return coordinate[2];
		}

		const T& w()const {
			return coordinate[3];
		}
		//spot
		T operator*(const HoVector4& hoVector4f)const {
			//checkIfVector(), hoVector4f.checkIfVector();
			return coordinate[0] * hoVector4f[0] + coordinate[1] * hoVector4f[1] +
				coordinate[2] * hoVector4f[2] + coordinate[3] * hoVector4f[3];
		}

		HoVector4& operator*=(T num) {
			coordinate[0] *= num;
			coordinate[1] *= num;
			coordinate[2] *= num;
			coordinate[3] *= num;
			return *this;
		}

		HoVector4 operator*(T num)const {
			HoVector4 v = *this;
			return v *= num;
		}

		HoVector4& operator*=(const Matrix4f& matrix) {
			T vec[4]{coordinate[0],coordinate[1], coordinate[2], coordinate[3]};
			for (int col = 0; col != 4; ++col)
				coordinate[col] = vec[0] * matrix[0][col] + vec[1] * matrix[1][col] +
				vec[2] * matrix[2][col] + vec[3] * matrix[3][col];
			return *this;
		}

		HoVector4 operator*(const Matrix4f& matrix)const {
			HoVector4 v = *this;
			return v *= matrix;
		}

		HoVector4& operator/=(T num) {
			if (!num)
				throw std::range_error("divide by zero");
			return *this *= (1 / num);
		}

		HoVector4 operator/(T num)const {
			HoVector4 v = *this;
			return v /= num;
		}

		//cross right-handed
		HoVector4 operator^(const HoVector4& hoVector4f)const {
			//checkIfVector(), hoVector4f.checkIfVector();
			return HoVector4(coordinate[1] * hoVector4f[2] - coordinate[2] * hoVector4f[1],
				coordinate[2] * hoVector4f[0] - coordinate[0] * hoVector4f[2],
				coordinate[0] * hoVector4f[1] - coordinate[1] * hoVector4f[0], 0);
		}

		HoVector4& operator+=(const HoVector4& hoVector4f) {
			//checkIfSameKind(hoVector4f);
			coordinate[0] += hoVector4f[0];
			coordinate[1] += hoVector4f[1];
			coordinate[2] += hoVector4f[2];
			coordinate[3] += hoVector4f[3];
			return *this;
		}

		HoVector4 operator+(const HoVector4& hoVector4f)const {
			HoVector4 v = *this;
			return v += hoVector4f;
		}

		HoVector4 operator-()const {
			return HoVector4(-coordinate[0], -coordinate[1], -coordinate[2], -coordinate[3]);
		}

		HoVector4& operator-=(const HoVector4& hoVector4f) {
			//checkIfSameKind(hoVector4f);
			coordinate[0] -= hoVector4f[0];
			coordinate[1] -= hoVector4f[1];
			coordinate[2] -= hoVector4f[2];
			coordinate[3] -= hoVector4f[3];
			return *this;
		}

		HoVector4 operator-(const HoVector4& vector4f)const {
			HoVector4 v = *this;
			return v -= vector4f;
		}

		T squareOfLength()const {
			//checkIfVector();
			return coordinate[0] * coordinate[0] +
				coordinate[1] * coordinate[1] + coordinate[2] * coordinate[2];
		}

		T length() const {
			return sqrt(squareOfLength());
		}

		HoVector4& normalize() {
			T len = length();
			coordinate[0] /= len;
			coordinate[1] /= len;
			coordinate[2] /= len;
			return *this;
		}
			
	};

	using HoVector4f = HoVector4<float>;
	using HoVector4i = HoVector4<int>;
}

