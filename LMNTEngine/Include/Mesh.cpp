#include "Mesh.h"
#include "Device.h"

//Transform, Projection 이후 제거. Render 함수 수정
#include "Core.h"

LMNT_USING

CMesh::CMesh()
{
}

CMesh::~CMesh()
{
	vector<PMESHCONTAINER>::iterator iter;

	vector<PMESHCONTAINER>::iterator iterEnd = m_vecMeshContainer.end();

	for (iter = m_vecMeshContainer.begin(); iter != iterEnd; ++iter)
	{
		SAFE_RELEASE((*iter)->tVB.pVB);
		SAFE_DELETE_ARRAY((*iter)->tVB.pData);
	
		for (size_t i = 0; i < (*iter)->vecIB.size(); ++i)
		{
			SAFE_RELEASE((*iter)->vecIB[i].pIB);
			SAFE_DELETE_ARRAY((*iter)->vecIB[i].pData);
		}

		SAFE_DELETE((*iter));

	}

	m_vecMeshContainer.clear();

}

Vector3 CMesh::GetMin() const
{
	return m_vMin;
}

Vector3 CMesh::GetMax() const
{
	return m_vMax;
}

Vector3 CMesh::GetLength() const
{
	return m_vLength;
}

Vector3 CMesh::GetCenter() const
{
	return m_vCenter;
}

size_t CMesh::GetContainerCount() const
{
	return m_vecMeshContainer.size();
}

size_t CMesh::GetSubsetCount(size_t iContainerIndex) const
{
	return m_vecMeshContainer[iContainerIndex]->vecIB.size();
}

bool CMesh::CreateMesh(const string & strKey, UINT iVertexCount, UINT iVertexSize, D3D11_USAGE eVertexUsage,
	void * pVertexData, D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType, UINT iIndexCount, UINT iIndexSize,
	D3D11_USAGE eIndexUsage, DXGI_FORMAT eIndexFormat, void * pIndexData)
{
	SetTag(strKey);

	PMESHCONTAINER pContainer = new MESHCONTAINER;

	m_vecMeshContainer.push_back(pContainer);

	if (!CreateVertexBuffer(iVertexCount, iVertexSize, eVertexUsage, pVertexData,ePrimitiveType, pContainer))	
	{
		return false;
	}

	if (!CreateIndexBuffer(iIndexCount, iIndexSize, eIndexUsage, eIndexFormat, pIndexData, pContainer))
	{
		return false;
	}


	return true;
}

void CMesh::Render(int iContainer, int iSubset)
{
	UINT	iOffset = 0;
	PMESHCONTAINER	pContainer = m_vecMeshContainer[iContainer];
	
	// 도형 위상구조를 설정한다.
	DEVICE_CONTEXT->IASetPrimitiveTopology(pContainer->tVB.ePrimitiveType);
	DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &pContainer->tVB.pVB, &pContainer->tVB.iSize, &iOffset);

	if (pContainer->vecIB.empty())
	{
		DEVICE_CONTEXT->Draw(pContainer->tVB.iCount, 0);
	}
	else
	{
		DEVICE_CONTEXT->IASetIndexBuffer(pContainer->vecIB[iSubset].pIB, pContainer->vecIB[iSubset].eFormat, 0);
		DEVICE_CONTEXT->DrawIndexed(pContainer->vecIB[iSubset].iCount, 0, 0);	
	}
}

void CMesh::Render()
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		UINT	iOffset = 0;
		PMESHCONTAINER	pContainer = m_vecMeshContainer[i];


		// 도형 위상구조를 설정한다.
		DEVICE_CONTEXT->IASetPrimitiveTopology(pContainer->tVB.ePrimitiveType);
		DEVICE_CONTEXT->IASetVertexBuffers(0, 1, &pContainer->tVB.pVB, &pContainer->tVB.iSize, &iOffset);
		
		if (pContainer->vecIB.empty())
		{
			DEVICE_CONTEXT->Draw(pContainer->tVB.iCount, 0);
		}
		else
		{
			for (size_t j = 0; j < pContainer->vecIB.size(); ++j)
			{
				DEVICE_CONTEXT->IASetIndexBuffer(	pContainer->vecIB[j].pIB, pContainer->vecIB[j].eFormat,	0);
				DEVICE_CONTEXT->DrawIndexed(pContainer->vecIB[j].iCount, 0, 0);
			}
		}
	}
}

void CMesh::UpdateVertexBuffer(void * pVtxData)
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		int	iSize = m_vecMeshContainer[i]->tVB.iSize *
			m_vecMeshContainer[i]->tVB.iCount;

		memcpy(m_vecMeshContainer[i]->tVB.pData, pVtxData, iSize);

		switch (m_vecMeshContainer[i]->tVB.eUsage)
		{
		case D3D11_USAGE_DEFAULT:
			DEVICE_CONTEXT->UpdateSubresource(m_vecMeshContainer[i]->tVB.pVB,
				0, NULL, pVtxData, 0, 0);
			break;
		case D3D11_USAGE_DYNAMIC:
		case D3D11_USAGE_STAGING:
		{
			D3D11_MAPPED_SUBRESOURCE	tMap = {};
			DEVICE_CONTEXT->Map(m_vecMeshContainer[i]->tVB.pVB, 0,
				D3D11_MAP_WRITE_DISCARD, 0, &tMap);

			memcpy(tMap.pData, pVtxData, iSize);

			DEVICE_CONTEXT->Unmap(m_vecMeshContainer[i]->tVB.pVB, 0);
		}
		break;
		}
	}
}

void CMesh::MoveVertexBuffer(const Vector3 & vMove)
{
	for (size_t i = 0; i < m_vecMeshContainer.size(); ++i)
	{
		int	iSize = m_vecMeshContainer[i]->tVB.iSize *
			m_vecMeshContainer[i]->tVB.iCount;

		char*	pData = (char*)m_vecMeshContainer[i]->tVB.pData;

		for (int j = 0; j < m_vecMeshContainer[i]->tVB.iCount; ++j)
		{
			PVector3	pPos = (PVector3)(pData + (j * m_vecMeshContainer[i]->tVB.iSize));

			(*pPos) += vMove;
		}

		switch (m_vecMeshContainer[i]->tVB.eUsage)
		{
		case D3D11_USAGE_DEFAULT:
			DEVICE_CONTEXT->UpdateSubresource(m_vecMeshContainer[i]->tVB.pVB,
				0, NULL, m_vecMeshContainer[i]->tVB.pData, 0, 0);
			break;
		case D3D11_USAGE_DYNAMIC:
		case D3D11_USAGE_STAGING:
		{
			D3D11_MAPPED_SUBRESOURCE	tMap = {};
			DEVICE_CONTEXT->Map(m_vecMeshContainer[i]->tVB.pVB, 0,
				D3D11_MAP_WRITE_DISCARD, 0, &tMap);

			memcpy(tMap.pData, m_vecMeshContainer[i]->tVB.pData,
				iSize);

			DEVICE_CONTEXT->Unmap(m_vecMeshContainer[i]->tVB.pVB, 0);
		}
		break;
		}
	}
}

bool CMesh::CreateVertexBuffer(UINT iVertexCount, UINT iVertexSize, D3D11_USAGE eVertexUsage,
	void * pVertexData, D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType, PMESHCONTAINER pContainer)
{

	char*	pVertices = (char*)pVertexData;

	for (int i = 0; i < iVertexCount; ++i)
	{
		PVector3	pPos = (PVector3)(pVertices + (iVertexSize * i));

		if (m_vMin.x > pPos->x)
			m_vMin.x = pPos->x;

		if (m_vMin.y > pPos->y)
			m_vMin.y = pPos->y;

		if (m_vMin.z > pPos->z)
			m_vMin.z = pPos->z;

		if (m_vMax.x < pPos->x)
			m_vMax.x = pPos->x;

		if (m_vMax.y < pPos->y)
			m_vMax.y = pPos->y;

		if (m_vMax.z < pPos->z)
			m_vMax.z = pPos->z;
	}

	m_vLength = m_vMax - m_vMin;
	m_vCenter = (m_vMin + m_vMax) / 2.f;



	pContainer->tVB.ePrimitiveType = ePrimitiveType;
	pContainer->tVB.eUsage = eVertexUsage;
	pContainer->tVB.iCount = iVertexCount;
	pContainer->tVB.iSize = iVertexSize;
	pContainer->tVB.pData = new char[iVertexCount * iVertexSize];

	//Copy Vertex Data to Dynamicaly allocated Storage
	memcpy(pContainer->tVB.pData, pVertexData, iVertexCount * iVertexSize);

	D3D11_BUFFER_DESC tDescription = {};

	tDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	tDescription.ByteWidth = iVertexCount * iVertexSize;
	tDescription.Usage = eVertexUsage;

	//버텍스의 용도가 Dynamic 이면 CPU에서 버텍스 정보 변경 가능함.
	//분리한 이유는, 메모리 자원을 비디오카드 메모리에 복사해주기 떄문에, 메모리가 분리되어 있어서 다른 접근 방식이 필요함
	if (eVertexUsage == D3D11_USAGE_DYNAMIC)
	{
		tDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	/*실제 버텍스의 정보를 메모리에 얹음*/
	D3D11_SUBRESOURCE_DATA tSubResource = {};
	tSubResource.pSysMem = pContainer->tVB.pData;



	if (FAILED(DEVICE->CreateBuffer(&tDescription, &tSubResource, &pContainer->tVB.pVB)))
	{
		return false;
	}
	return true;
}

bool CMesh::CreateIndexBuffer(UINT iIndexCount, UINT iIndexSize, D3D11_USAGE eIndexUsage,
	DXGI_FORMAT eIndexFomat, void * pIndexData, PMESHCONTAINER pContainer)
{

	if (iIndexCount == 0)
		return true;

	INDEXBUFFER tIndexBuffer = {};

	tIndexBuffer.eFormat = eIndexFomat;
	tIndexBuffer.eUsage = eIndexUsage;
	tIndexBuffer.iCount = iIndexCount;
	tIndexBuffer.iSize = iIndexSize;
	tIndexBuffer.pData = new char[iIndexCount * iIndexSize];

	memcpy(tIndexBuffer.pData, pIndexData, iIndexCount * iIndexSize);


	D3D11_BUFFER_DESC tDescription = {};
	tDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
	tDescription.ByteWidth = iIndexCount * iIndexSize;
	tDescription.Usage = eIndexUsage;

	if (tDescription.Usage == D3D11_USAGE_DYNAMIC)
	{
		tDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}

	D3D11_SUBRESOURCE_DATA pSubResource = {};
	pSubResource.pSysMem = tIndexBuffer.pData;


	if (FAILED(DEVICE->CreateBuffer(&tDescription, &pSubResource,
		&tIndexBuffer.pIB)))
		return false;

	pContainer->vecIB.push_back(tIndexBuffer);

	return true;
}


