#pragma once
#include<math.h>
/// <summary>
/// 行列
/// </summary>
class Matrix4 {
public:
	// 行x列
	float m[4][4];


public:
	// コンストラクタ
	Matrix4();
	// 成分を指定しての生成
	Matrix4(
		float m00, float m01, float m02, float m03,
		float m10, float m11, float m12, float m13,
		float m20, float m21, float m22, float m23,
		float m30, float m31, float m32, float m33);

	// 代入演算子オーバーロード
	Matrix4& operator*=(const Matrix4& m2);


	Matrix4 Scaliing(float sx, float sy, float sz)
	{
		Matrix4 m = {};

		m.m[0][0] = sx;
		m.m[1][1] = sy;
		m.m[2][2] = sz;

		return m;

	};
	void Scaliing(Matrix4 m, float sx, float sy, float sz)
	{
		m.m[0][0] = sx;
		m.m[1][1] = sy;
		m.m[2][2] = sz;

	};

	Matrix4 MatRotX(float rotX) {

		Matrix4 m = {};

		m.m[1][1] = cos(rotX);
		m.m[1][2] = sin(rotX);
		m.m[2][1] = sin(-rotX);
		m.m[2][2] = cos(rotX);
		m.m[0][0] = 1;
		m.m[3][3] = 1;

		return m;

	};

	void MatRotX(Matrix4& m, float rotX) {

		m.m[1][1] = cos(rotX);
		m.m[1][2] = sin(rotX);
		m.m[2][1] = sin(-rotX);
		m.m[2][2] = cos(rotX);
		m.m[0][0] = 1;
		m.m[3][3] = 1;

	};

	Matrix4 MatRotY(float rotY) {

		Matrix4 m = {};

		m.m[0][0] = cos(rotY);
		m.m[0][2] = sin(rotY);
		m.m[2][0] = sin(-rotY);
		m.m[2][2] = cos(rotY);
		m.m[1][1] = 1;
		m.m[3][3] = 1;

		return m;
	};


	void MatRotY(Matrix4& m, float rotY) {

		m.m[0][0] = cos(rotY);
		m.m[0][2] = sin(rotY);
		m.m[2][0] = sin(-rotY);
		m.m[2][2] = cos(rotY);
		m.m[1][1] = 1;
		m.m[3][3] = 1;

	};


	Matrix4 MatRotZ(float rotZ) {

		Matrix4 m = {};

		m.m[0][0] = cos(rotZ);
		m.m[0][1] = sin(rotZ);
		m.m[1][0] = sin(-rotZ);
		m.m[1][1] = cos(rotZ);
		m.m[2][2] = 1;
		m.m[3][3] = 1;

		return m;

	};

	void MatRotZ(Matrix4& m, float rotZ) {

		m.m[0][0] = cos(rotZ);
		m.m[0][1] = sin(rotZ);
		m.m[1][0] = sin(-rotZ);
		m.m[1][1] = cos(rotZ);
		m.m[2][2] = 1;
		m.m[3][3] = 1;

	};

	Matrix4 MatTrans(float tx, float ty, float tz)
	{
		Matrix4 m = {};

		MatIdentity(m);

		m.m[3][0] = tx;
		m.m[3][1] = ty;
		m.m[3][2] = tz;

		return m;
	};

	void MatTrans(Matrix4& m, float tx, float ty, float tz)	{

		MatIdentity(m);

		m.m[3][0] = tx;
		m.m[3][1] = ty;
		m.m[3][2] = tz;


	};


	Matrix4 MatIdentity() {
		Matrix4 mat;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; 0 < 4; j++) {
				if (i != j && mat.m[i][j] != 0)mat.m[i][j] = 0;
				else if (i == j && mat.m[i][j] != 1)mat.m[i][j] = 1;
			}
		}
		return mat;
	}

	void MatIdentity(Matrix4& m) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j< 4; j++) {
				if (i != j && m.m[i][j] != 0) { m.m[i][j] = 0; }
				else if (i == j && m.m[i][j] != 1){m.m[i][j] = 1;}
			}
		}
	}

	;
	float ToRadian(float angle) { return angle * 3.14 / 180; }











};




