#pragma once

#include "Macro.h"
#include "Flag.h"

LMNT_BEGIN


typedef struct LMNT_DLL _tagVector3
{
	float x, y, z;

	_tagVector3();
	_tagVector3(float _x, float _y, float _z);
	_tagVector3(const _tagVector3& _v);
	_tagVector3(const float f[3]);
	_tagVector3(const int i[3]);
	_tagVector3(const XMVECTOR& _v);

	XMVECTOR Convert()	const;
	void Convert(const XMVECTOR& v);

	// ==============  operator =  =================
	void operator =(const _tagVector3& _v);
	void operator =(const XMVECTOR& _v);
	void operator =(const float f[3]);
	void operator =(const int i[3]);

	// ==============  operator +  =================
	_tagVector3 operator +(const _tagVector3& _v)	const;
	_tagVector3 operator +(const XMVECTOR& _v)	const;
	_tagVector3 operator +(const float f[3])	const;
	_tagVector3 operator +(const int i[3])	const;
	_tagVector3 operator +(float f)	const;
	_tagVector3 operator +(int i)	const;

	// ==============  operator +=  =================
	_tagVector3& operator +=(const _tagVector3& _v);
	_tagVector3& operator +=(const XMVECTOR& _v);
	_tagVector3& operator +=(const float f[3]);
	_tagVector3& operator +=(const int i[3]);
	_tagVector3& operator +=(float f);
	_tagVector3& operator +=(int i);

	// ==============  operator -  =================
	//Inverse Vector

	_tagVector3 operator -();

	_tagVector3 operator -(const _tagVector3& _v)	const;
	_tagVector3 operator -(const XMVECTOR& _v)	const;
	_tagVector3 operator -(const float f[3])	const;
	_tagVector3 operator -(const int i[3])	const;
	_tagVector3 operator -(float f)	const;
	_tagVector3 operator -(int i)	const;

	// ==============  operator -=  =================
	_tagVector3& operator -=(const _tagVector3& _v);
	_tagVector3& operator -=(const XMVECTOR& _v);
	_tagVector3& operator -=(const float f[3]);
	_tagVector3& operator -=(const int i[3]);
	_tagVector3& operator -=(float f);
	_tagVector3& operator -=(int i);


	// ==============  operator *  =================
	_tagVector3 operator *(float f)	const;
	_tagVector3 operator *(int i)	const;
	_tagVector3 operator * (_tagVector3 _v) const;

	// ==============  operator *=  =================
	_tagVector3& operator *=(float f);
	_tagVector3& operator *=(int i);


	// ==============  operator /  =================
	_tagVector3 operator /(float f)	const;
	_tagVector3 operator /(int i)	const;

	// ==============  operator /=  =================
	_tagVector3& operator /=(float f);
	_tagVector3& operator /=(int i);

	// ==============  operator ++, --  =================
	_tagVector3& operator ++();
	_tagVector3& operator --();

	bool operator ==(const _tagVector3& _v)	const;
	bool operator ==(const XMVECTOR& _v)	const;
	bool operator !=(const _tagVector3& _v)	const;
	bool operator !=(const XMVECTOR& _v)	const;

	// =============== Conversion ================
	operator XMVECTOR ()
	{
		return Convert();
	}
	
	// =============== 벡터 관련 함수들 ================
	float Length()	const;
	_tagVector3 Normalize()	const;
	void normalize();
	float Distance(_tagVector3 _v);
	static float Distance(_tagVector3 _v1, _tagVector3 _v2);
	static _tagVector3 Lerp(_tagVector3 v1, _tagVector3 v2, float fElapse);

	float Dot(const _tagVector3& _v)	const;
	float Dot(const XMVECTOR& _v)	const;
	float Angle(const _tagVector3& _v)	const;
	float Angle(const XMVECTOR& _v)	const;
	static _tagVector3 RandomDirection();
	static _tagVector3 DirectionToAngle(_tagVector3 vDirection);

	_tagVector3 TransformNormal(const union _tagMatrix& _m)	const;
	_tagVector3 TransformCoord(const union _tagMatrix& _m)	const;


	static const _tagVector3	Zero;
	static const _tagVector3	One;
	static const _tagVector3	Half;
	static const _tagVector3	Axis[AX_END];

	_tagVector3 Cross(const _tagVector3& _v)	const;
	_tagVector3 Cross(const XMVECTOR& _v)	const;

}Vector3, *PVector3;

LMNT_END
