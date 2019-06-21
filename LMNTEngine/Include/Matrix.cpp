#include "Matrix.h"
#include "Vector3.h"

LMNT_USING

_tagMatrix::_tagMatrix()
{
	// XMMatrixIdentity() : 항등행렬 (E)을 만들어주는 함수이다.
	mat = XMMatrixIdentity();
}

_tagMatrix::_tagMatrix(const _tagMatrix & m)
{
	*this = m;
}
_tagMatrix::_tagMatrix(const XMMATRIX & m)
{
	mat = m;
}

_tagMatrix::operator XMMATRIX()
{
	return mat;
}

void * _tagMatrix::operator new(size_t size)
{
	_tagMatrix* pMatrix = (_tagMatrix*)_aligned_malloc(size, 16);

	pMatrix->mat = XMMatrixIdentity();

	return pMatrix;
}

void _tagMatrix::operator delete(void * p)
{
	_aligned_free(p);
}

void _tagMatrix::operator=(const XMMATRIX & m)
{
	mat = m;
}

void _tagMatrix::operator=(const _tagMatrix & m)
{
	mat = m.mat;
}

XMMATRIX _tagMatrix::operator*(const _tagMatrix & m)
{
	return mat * m.mat;
}

XMMATRIX _tagMatrix::operator*(const XMMATRIX & m)
{
	return mat * m;
}

XMMATRIX & _tagMatrix::operator*=(const _tagMatrix & m)
{
	*this = mat * m.mat;
	return mat;
}

XMMATRIX & _tagMatrix::operator*=(const XMMATRIX & m)
{
	*this = mat * m;
	return mat;
}

XMMATRIX& _tagMatrix::Identity()
{
	return XMMatrixIdentity();
}

XMMATRIX& _tagMatrix::Transpose()
{
	mat = XMMatrixTranspose(mat);
	return mat;
}

XMMATRIX & _tagMatrix::Inverse()
{
	mat = XMMatrixInverse(&XMMatrixDeterminant(mat), mat);
	return mat;
}

XMMATRIX& _tagMatrix::Position()
{
	return XMMATRIX();
}

XMMATRIX& _tagMatrix::Rotation()
{
	return XMMATRIX();
}

XMMATRIX& _tagMatrix::Scale()
{
	return XMMATRIX();
}

XMMATRIX _tagMatrix::IdentityMatrix()
{
	return XMMatrixIdentity();
}

XMMATRIX _tagMatrix::Transpose(const _tagMatrix & m)
{
	return XMMatrixTranspose(m.mat);
}

XMMATRIX _tagMatrix::Inverse(const _tagMatrix & m)
{
	return XMMatrixInverse(&XMMatrixDeterminant(m.mat), m.mat);
}

XMMATRIX _tagMatrix::Scaling(const _tagVector3 & vScale)
{
	return XMMatrixScaling(vScale.x, vScale.y, vScale.z);
}

XMMATRIX _tagMatrix::Translation(const _tagVector3 & vPos)
{
	return XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
}

