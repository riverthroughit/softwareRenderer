#pragma once
#include<stdexcept>
#include<initializer_list>
//#include<array>
//using std::array;
namespace myMath {

	template<typename T>
	class Matrix4
	{
		//T** element;//动态数组 方便移动构造
		//array<array<T, 4>, 4> element;
		T element[4][4];
		//void destroy() {
		//	for (int i = 0; i != 4; ++i)
		//		delete[] element[i];
		//	delete[] element;
		//}
	public:
		//Matrix4f() :element(new T* [4]) {
		//	for (int i = 0; i != 4; ++i)
		//		element[i] = new T[4]();
		//}

		//Matrix4f(std::initializer_list<T> init) :element(new T* [4]) {
		//	if (init.size() != 16)
		//		throw std::runtime_error("too few formal parameters");
		//	for (int row = 0; row != 4; ++row) {
		//		element[row] = new T[4];
		//		for (int col = 0; col != 4; ++col)
		//			element[row][col] = *(init.begin() + (4 * row + col));
		//	}
		//}

		//Matrix4f() :element{ {0}, {0}, {0}, {0}} {}
		Matrix4(std::initializer_list<T> init) {
			if (init.size() != 16)
				throw std::runtime_error("too few formal parameters");
			for (int row = 0; row != 4; ++row) {
				for (int col = 0; col != 4; ++col)
					element[row][col] = *(init.begin() + (4 * row + col));
			}
			//element = ele;
		}

		////拷贝控制
		//Matrix4f(const Matrix4f& matrix4f) :element(new T* [4]) {
		//	for (int row = 0; row != 4; ++row) {
		//		element[row] = new T[4];
		//		for (int col = 0; col != 4; ++col)
		//			element[row][col] = matrix4f[row][col];
		//	}
		//}

		//Matrix4f(Matrix4f&& matrix4f)noexcept :element(matrix4f.element) {
		//	matrix4f.element = 0;
		//}

		//Matrix4f& operator=(const Matrix4f& matrix4f) {
		//	T** ele = new T* [4];
		//	for (int row = 0; row != 4; ++row) {
		//		ele[row] = new T[4];
		//		for (int col = 0; col != 4; ++col)
		//			ele[row][col] = matrix4f[row][col];
		//	}
		//	destroy();
		//	element = ele;
		//}

		//Matrix4f& operator=(Matrix4f&& matrix4f)noexcept {
		//	if (this != &matrix4f) {
		//		destroy();
		//		element = matrix4f.element;
		//		matrix4f.element = 0;
		//	}
		//	return *this;
		//}

		//~Matrix4f() {
		//	destroy();
		//}


		T* operator[](int row) {
			if (row > 3 || row < 0)
				throw std::out_of_range("the subscript of matrix out of range");
			return element[row];
		}

		const T* operator[](int row)const {
			if (row > 3 || row < 0)
				throw std::out_of_range("the subscript of matrix out of range");
			return element[row];
		}

		Matrix4& operator+=(const Matrix4& matrix) {
			for (int row = 0; row != 4; ++row) {
				for (int col = 0; col != 4; ++col)
					element[row][col] += matrix[row][col];
			}
			return *this;
		}

		Matrix4 operator+(const Matrix4& matrix)const {
			Matrix4 m = *this;
			return m += matrix;
		}

		Matrix4 operator-() {
			Matrix4 matrix = *this;
			for (int row = 0; row != 4; ++row) {
				for (int col = 0; col != 4; ++col)
					matrix[row][col] = -matrix[row][col];
			}
			return matrix;
		}

		Matrix4& operator-=(const Matrix4& matrix) {
			for (int row = 0; row != 4; ++row) {
				for (int col = 0; col != 4; ++col)
					element[row][col] -= matrix[row][col];
			}
			return *this;
		}

		Matrix4 operator-(const Matrix4& matrix)const {
			Matrix4 m = *this;
			return m -= matrix;
		}

		Matrix4& operator*=(const Matrix4& matrix) {
			for (int row = 0;row != 4;++row) {
				T thisRow[4]{element[row][0] ,element[row][1] ,element[row][2] ,element[row][3]};
				for (int col = 0;col != 4;++col) {
					element[row][col] =
						thisRow[0] * matrix[0][col] + thisRow[1] * matrix[1][col] +
						thisRow[2] * matrix[2][col] + thisRow[3] * matrix[3][col];
				}
			}
			return *this;
		}

		Matrix4 operator*(const Matrix4& matrix)const {
			Matrix4 m = *this;
			return m *= matrix;
		}

		Matrix4& operator*=(T num) {
			for (int row = 0; row != 4; ++row) {
				for (int col = 0; col != 4; ++col)
					element[row][col] *= num;
			}
			return *this;
		}

		Matrix4 operator*(T num)const {
			Matrix4 matrix = *this;
			return matrix*=num;
		}

		Matrix4 transpose()const {
			Matrix4 matrix = *this;
			for (int row = 0;row != 3;++row) {
				for (int col = row + 1;col != 4;++col)
					std::swap(matrix[row][col], matrix[col][row]);
			}
			return matrix;
		}

		//Matrix4f inverse()const {
		//	Matrix4f matrix;
		//	//todo
		//	return matrix;
		//}
	};

	using Matrix4f = Matrix4<float>;
	using Matrix4i = Matrix4<int>;
}


