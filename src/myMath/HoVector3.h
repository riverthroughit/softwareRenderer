#pragma once
#include<stdexcept>
namespace myMath {


	template<typename T>
	class HoVector3
	{

		T coordinate[3];


	public:

		
		HoVector3(T a = 0, T b = 0, T c = 0) :coordinate{ a,b,c } {}

		T& operator[](const int& index) {

			return coordinate[index];
		}

		const T& operator[](const int& index)const {

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


		const T& x()const {
			return coordinate[0];
		}

		const T& y()const {
			return coordinate[1];
		}

		const T& z()const {
			return coordinate[2];
		}

		//spot
		T operator*(const HoVector3& hoVector3)const {
			//checkIfVector(), hoVector4f.checkIfVector();
			return coordinate[0] * hoVector3[0] + coordinate[1] * hoVector3[1] +
				coordinate[2] * hoVector3[2];
		}

		HoVector3& operator*=(T num) {
			coordinate[0] *= num;
			coordinate[1] *= num;
			coordinate[2] *= num;
			return *this;
		}

		HoVector3 operator*(T num)const {
			HoVector3 v = *this;
			return v *= num;
		}



		HoVector3& operator/=(T num) {
			if (!num)
				throw std::range_error("divide by zero");
			return *this *= (1 / num);
		}

		HoVector3 operator/(T num)const {
			HoVector3 v = *this;
			return v /= num;
		}

		//cross right-handed
		//HoVector3 operator^(const HoVector3& hoVector3)const {
		//	//checkIfVector(), hoVector3.checkIfVector();
		//	return HoVector3(coordinate[1] * hoVector3[2] - coordinate[2] * hoVector3[1],
		//		coordinate[2] * hoVector3[0] - coordinate[0] * hoVector3[2],
		//		coordinate[0] * hoVector3[1] - coordinate[1] * hoVector3[0], 0);
		//}

		HoVector3& operator+=(const HoVector3& hoVector3) {
			//checkIfSameKind(hoVector3);
			coordinate[0] += hoVector3[0];
			coordinate[1] += hoVector3[1];
			coordinate[2] += hoVector3[2];
			return *this;
		}

		HoVector3 operator+(const HoVector3& hoVector3)const {
			HoVector3 v = *this;
			return v += hoVector3;
		}

		HoVector3 operator-()const {
			return HoVector3(-coordinate[0], -coordinate[1], -coordinate[2]);
		}

		HoVector3& operator-=(const HoVector3& hoVector3) {
			//checkIfSameKind(hoVector3);
			coordinate[0] -= hoVector3[0];
			coordinate[1] -= hoVector3[1];
			coordinate[2] -= hoVector3[2];
			return *this;
		}

		HoVector3 operator-(const HoVector3& vector4f)const {
			HoVector3 v = *this;
			return v -= vector4f;
		}

		T squareOfLength()const {
			//checkIfVector();
			return coordinate[0] * coordinate[0] + coordinate[1] * coordinate[1];
		}

		T length() const {
			return sqrt(squareOfLength());
		}

		HoVector3& normalize() {
			T len = length();
			coordinate[0] /= len;
			coordinate[1] /= len;
			return *this;
		}

	};

	using HoVector3f = HoVector3<float>;
	using HoVector3i = HoVector3<int>;
}