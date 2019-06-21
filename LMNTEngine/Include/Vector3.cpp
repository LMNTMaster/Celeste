
#include "Vector3.h"
#include "Engine.h"

LMNT_USING



const _tagVector3	_tagVector3::Zero = Vector3(0, 0, 0);
const _tagVector3	_tagVector3::One = Vector3(1, 1, 1);
const _tagVector3	_tagVector3::Half = Vector3(0.5f, 0.5f, 0.5f);
const _tagVector3	_tagVector3::Axis[AX_END] =
{
	_tagVector3(1.f, 0.f, 0.f),
	_tagVector3(0.f, 1.f, 0.f),
	_tagVector3(0.f, 0.f, 1.f)
};

_tagVector3::_tagVector3() :
	x(0.f),
	y(0.f),
	z(0.f)
{
}

_tagVector3::_tagVector3(float _x, float _y, float _z) :
	x(_x),
	y(_y),
	z(_z)
{
}

_tagVector3::_tagVector3(const _tagVector3& _v) :
	x(_v.x),
	y(_v.y),
	z(_v.z)
{
}

_tagVector3::_tagVector3(const float f[3]) :
	x(f[0]),
	y(f[1]),
	z(f[2])
{
}

_tagVector3::_tagVector3(const int i[3]) :
	x((float)i[0]),
	y((float)i[1]),
	z((float)i[2])
{
}

_tagVector3::_tagVector3(const XMVECTOR& _v)
{
	XMStoreFloat3((XMFLOAT3*)this, _v);
}

XMVECTOR _tagVector3::Convert() const
{
	return XMLoadFloat3((XMFLOAT3*)this);
}

void _tagVector3::Convert(const XMVECTOR & v)
{
	XMStoreFloat3((XMFLOAT3*)this, v);
}

// ==============  operator =  =================
void _tagVector3::operator =(const _tagVector3& _v)
{
	x = _v.x;
	y = _v.y;
	z = _v.z;
}

void _tagVector3::operator =(const XMVECTOR& _v)
{
	XMStoreFloat3((XMFLOAT3*)this, _v);
}

void _tagVector3::operator=(const float f[3])
{
	x = f[0];
	y = f[1];
	z = f[2];
}

void _tagVector3::operator=(const int i[3])
{
	x =(float)i[0];
	y =(float)i[1];
	z =(float)i[2];
}

// ==============  operator +  =================
_tagVector3 _tagVector3::operator+(const _tagVector3 & _v)	const
{
	/*_tagVector3	v;
	v.Convert(Convert() + _v.Convert());
	return v;*/
	return _tagVector3(x + _v.x, y + _v.y, z + _v.z);
}

_tagVector3 _tagVector3::operator+(const XMVECTOR & _v)	const
{
	return _tagVector3(Convert() + _v);
}

_tagVector3 _tagVector3::operator+(const float f[3])	const
{
	return _tagVector3(x + f[0], y + f[1], z + f[2]);
}

_tagVector3 _tagVector3::operator+(const int i[3])	const
{
	return _tagVector3(x + i[0], y + i[1], z + i[2]);
}

_tagVector3 _tagVector3::operator+(float f)	const
{
	return _tagVector3(x + f, y + f, z + f);
}

_tagVector3 _tagVector3::operator+(int i)	const
{
	return _tagVector3(x + i, y + i, z + i);
}

// ==============  operator +=  =================
_tagVector3& _tagVector3::operator+=(const _tagVector3 & _v)
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
	return *this;
}

_tagVector3& _tagVector3::operator+=(const XMVECTOR & _v)
{
	Convert(Convert() + _v);
	return *this;
}

_tagVector3& _tagVector3::operator+=(const float f[3])
{
	x += f[0];
	y += f[1];
	z += f[2];
	return *this;
}

_tagVector3& _tagVector3::operator+=(const int i[3])
{
	x += i[0];
	y += i[1];
	z += i[2];
	return *this;
}

_tagVector3& _tagVector3::operator+=(float f)
{
	x += f;
	y += f;
	z += f;
	return *this;
}

_tagVector3& _tagVector3::operator+=(int i)
{
	x += i;
	y += i;
	z += i;
	return *this;
}

// ==============  operator -  =================

_tagVector3 _tagVector3::operator-()
{
	*this *= -1;
	return *this;
}

_tagVector3 _tagVector3::operator-(const _tagVector3 & _v)	const
{
	/*_tagVector3	v;
	v.Convert(Convert() - _v.Convert());
	return v;*/
	return _tagVector3(x - _v.x, y - _v.y, z - _v.z);
}

_tagVector3 _tagVector3::operator-(const XMVECTOR & _v)	const
{
	return _tagVector3(Convert() - _v);
}

_tagVector3 _tagVector3::operator-(const float f[3])	const
{
	return _tagVector3(x - f[0], y - f[1], z - f[2]);
}

_tagVector3 _tagVector3::operator-(const int i[3])	const
{
	return _tagVector3(x - i[0], y - i[1], z - i[2]);
}

_tagVector3 _tagVector3::operator-(float f)	const
{
	return _tagVector3(x - f, y - f, z - f);
}

_tagVector3 _tagVector3::operator-(int i)	const
{
	return _tagVector3(x - i, y - i, z - i);
}

// ==============  operator -=  =================
_tagVector3& _tagVector3::operator-=(const _tagVector3 & _v)
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
	return *this;
}

_tagVector3& _tagVector3::operator-=(const XMVECTOR & _v)
{
	Convert(Convert() - _v);
	return *this;
}

_tagVector3& _tagVector3::operator-=(const float f[3])
{
	x -= f[0];
	y -= f[1];
	z -= f[2];
	return *this;
}

_tagVector3& _tagVector3::operator-=(const int i[3])
{
	x -= i[0];
	y -= i[1];
	z -= i[2];
	return *this;
}

_tagVector3& _tagVector3::operator-=(float f)
{
	x -= f;
	y -= f;
	z -= f;
	return *this;
}

_tagVector3& _tagVector3::operator-=(int i)
{
	x -= i;
	y -= i;
	z -= i;
	return *this;
}


// ==============  operator *  =================
_tagVector3 _tagVector3::operator*(float f)	const
{
	return _tagVector3(x * f, y * f, z * f);
}

_tagVector3 _tagVector3::operator*(int i)	const
{
	return _tagVector3(x * i, y * i, z * i);
}

_tagVector3 _tagVector3::operator*(_tagVector3 _v) const
{
	return _tagVector3(_v.x * x, _v.y * y, _v.z * z);
}

// ==============  operator *=  =================

_tagVector3& _tagVector3::operator*=(float f)
{
	x *= f;
	y *= f;
	z *= f;
	return *this;
}

_tagVector3& _tagVector3::operator*=(int i)
{
	x *= i;
	y *= i;
	z *= i;
	return *this;
}


// ==============  operator /  =================

_tagVector3 _tagVector3::operator/(float f)	const
{
	return _tagVector3(x / f, y / f, z / f);
}

_tagVector3 _tagVector3::operator/(int i)	const
{
	return _tagVector3(x / i, y / i, z / i);
}

// ==============  operator /=  =================

_tagVector3& _tagVector3::operator/=(float f)
{
	x /= f;
	y /= f;
	z /= f;
	return *this;
}

_tagVector3& _tagVector3::operator/=(int i)
{
	x /= i;
	y /= i;
	z /= i;
	return *this;
}

_tagVector3 & _tagVector3::operator++()
{
	++x;
	++y;
	++z;
	return *this;
}

_tagVector3 & _tagVector3::operator--()
{
	--x;
	--y;
	--z;
	return *this;
}

bool _tagVector3::operator==(const _tagVector3 & _v) const
{
	return x == _v.x && y == _v.y && z == _v.z;
}

bool _tagVector3::operator==(const XMVECTOR & _v) const
{
	_tagVector3	v;
	v.Convert(_v);
	return x == v.x && y == v.y && z == v.z;
}

bool _tagVector3::operator!=(const _tagVector3 & _v) const
{
	return x != _v.x || y != _v.y || z != _v.z;
}

bool _tagVector3::operator!=(const XMVECTOR & _v) const
{
	_tagVector3	v;
	v.Convert(_v);
	return x != v.x || y != v.y || z != v.z;
}

float _tagVector3::Length() const
{
	return sqrtf(x * x + y * y + z * z);
}

_tagVector3 _tagVector3::Normalize()	const
{
	_tagVector3	v;
	v.Convert(XMVector3Normalize(Convert()));
	return v;
}

void _tagVector3::normalize()
{
	*this = XMVector3Normalize(Convert());
}

float _tagVector3::Distance(_tagVector3 _v)
{
	return (*this - _v).Length();
}

float _tagVector3::Distance(_tagVector3 _v1, _tagVector3 _v2)
{
	return (_v1 - _v2).Length();
}

_tagVector3 _tagVector3::Lerp(_tagVector3 v1, _tagVector3 v2, float fElapse)
{	
	return XMVectorLerp(v1, v2, fElapse);
}

float _tagVector3::Dot(const _tagVector3 & _v) const
{
	return x * _v.x + y * _v.y + z * _v.z;
}

float _tagVector3::Dot(const XMVECTOR & _v) const
{
	_tagVector3	v;
	v.Convert(XMVector3Dot(Convert(), _v));
	return v.x;
}

float _tagVector3::Angle(const _tagVector3 & _v) const
{
	_tagVector3	v = Normalize();
	float	fAngle = v.Dot(_v.Normalize());
	return acosf(fAngle);
}

float _tagVector3::Angle(const XMVECTOR & _v) const
{
	_tagVector3	v = Normalize();
	_tagVector3	v1(_v);
	float	fAngle = v.Dot(v1.Normalize());
	return acosf(fAngle);
}

_tagVector3 _tagVector3::RandomDirection()
{
	return _tagVector3(RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f), RandomRange(-1.0f, 1.0f)).Normalize();
}

_tagVector3 _tagVector3::DirectionToAngle(_tagVector3 vDirection)
{
	_tagVector3 rt;
	return rt;
}

_tagVector3 _tagVector3::TransformNormal(const _tagMatrix & _m) const
{
	// 3차원 벡터와 4x4 행렬은 곱할 수 없다. Normald은 w를 0으로 해서 곱한다.
	_tagVector3	v;
	v = XMVector3TransformNormal(Convert(), _m.mat);
	return v;
}

_tagVector3 _tagVector3::TransformCoord(const _tagMatrix & _m) const
{
	// Coord는 w를 1로 해서 곱한다.
	_tagVector3	v;
	v = XMVector3TransformCoord(Convert(), _m.mat);
	return v;	
}



_tagVector3 _tagVector3::Cross(const _tagVector3 & _v) const
{
	XMVECTOR	v = XMVector3Cross(Convert(), _v.Convert());
	return _tagVector3(v);
}

_tagVector3 _tagVector3::Cross(const XMVECTOR & _v) const
{
	XMVECTOR	v = XMVector3Cross(Convert(), _v);
	return _tagVector3(v);
}

