#pragma once
#include "Macro.h"

LMNT_BEGIN

typedef union LMNT_DLL _tagMatrix
{

	XMMATRIX	mat;

	struct
	{
		float	_11, _12, _13, _14;
		float	_21, _22, _23, _24;
		float	_31, _32, _33, _34;
		float	_41, _42, _43, _44;
	};

	struct
	{
		float	_m00, _m01, _m02, _m03;
		float	_m10, _m11, _m12, _m13;
		float	_m20, _m21, _m22, _m23;
		float	_m30, _m31, _m32, _m33;
	};

	float fMat[4][4];

	_tagMatrix();
	_tagMatrix(const _tagMatrix& m);
	_tagMatrix(const XMMATRIX& m);
	operator XMMATRIX();
	void* operator new(size_t size);
	void operator delete(void* p);
	void operator =(const XMMATRIX& m);
	void operator =(const _tagMatrix& m);
	XMMATRIX operator *(const _tagMatrix& m);
	XMMATRIX operator *(const XMMATRIX& m);
	XMMATRIX& operator *=(const _tagMatrix& m);
	XMMATRIX& operator *=(const XMMATRIX& m);

	XMMATRIX& Identity();
	XMMATRIX& Transpose();
	XMMATRIX& Inverse();


	XMMATRIX& Position();
	XMMATRIX& Rotation();
	XMMATRIX& Scale();

	static XMMATRIX IdentityMatrix();
	static XMMATRIX Transpose(const _tagMatrix& m);
	static XMMATRIX Inverse(const _tagMatrix& m);
	static XMMATRIX Scaling(const struct _tagVector3& vScale);
	static XMMATRIX Translation(const struct _tagVector3& vPos);

}Matrix, *PMatrix;


LMNT_END