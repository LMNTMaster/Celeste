#pragma once
#include "Ref.h"

LMNT_BEGIN

typedef struct LMNT_DLL _tagVertexBuffer
{
	unsigned int	iCount;
	unsigned int	iSize;
	D3D11_USAGE		eUsage;
	// 도형 위상구조. 정점버퍼에 있는 정점을 어떤 도형으로 그려낼지
	// 결정하는 기준이 된다.
	D3D11_PRIMITIVE_TOPOLOGY	ePrimitiveType;
	void*			pData;
	ID3D11Buffer*	pVB;
}VERTEXBUFFER, *PVERTEXBUFFER;

typedef struct LMNT_DLL _tagIndexBuffer
{
	unsigned int		iCount;
	unsigned int		iSize;
	D3D11_USAGE	eUsage;
	DXGI_FORMAT	eFormat;
	void*					pData;
	ID3D11Buffer*	pIB;
}INDEXBUFFER, *PINDEXBUFFER;

//각각의 컨테이너는 여러개의 서브셋을 가질 수 있다.
typedef struct LMNT_DLL _tagMeshContainer
{
	VERTEXBUFFER	tVB;
	vector<INDEXBUFFER> vecIB;

}MESHCONTAINER, *PMESHCONTAINER;


class LMNT_DLL CMesh :
	public CRef
{
private:
	friend class CResourcesManager;

private:
	vector<PMESHCONTAINER> m_vecMeshContainer;
	Vector3					m_vMin;
	Vector3					m_vMax;
	Vector3					m_vCenter;
	Vector3					m_vLength;

private:
	CMesh();
	~CMesh();

public:
	Vector3 GetMin() const;
	Vector3 GetMax() const;
	Vector3 GetLength() const;
	Vector3 GetCenter() const;
	size_t GetContainerCount() const;
	size_t GetSubsetCount(size_t iContainerIndex) const;

public:	
	bool CreateMesh(const string& strKey, UINT iVtxCount,
		UINT iVtxSize, D3D11_USAGE eVtxUsage, void* pVtxData,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
		UINT iIdxCount = 0, UINT iIdxSize = 0,
		D3D11_USAGE eIdxUsage = D3D11_USAGE_DEFAULT,
		DXGI_FORMAT eIdxFmt = DXGI_FORMAT_R32_UINT,
		void* pIdxData = NULL);

	//출력
	void Render(int iContainer, int iSubset);
	void Render();
	void UpdateVertexBuffer(void* pVtxData);
	void MoveVertexBuffer(const Vector3& vMove);

private:
	//Create Vertex Buffer
	bool CreateVertexBuffer(UINT iVtxCount,
		UINT iVtxSize, D3D11_USAGE eVtxUsage, void* pVtxData,
		D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType,
		PMESHCONTAINER pContainer);

	//Create Index Buffer
	bool CreateIndexBuffer(UINT iIdxCount, UINT iIdxSize,
		D3D11_USAGE eIdxUsage, DXGI_FORMAT eIdxFmt,
		void* pIdxData, PMESHCONTAINER pContainer);
	
};

LMNT_END