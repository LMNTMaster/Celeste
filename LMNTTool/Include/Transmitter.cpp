#include "stdafx.h"
#include "Transmitter.h"

DEFINITION_SINGLE(CTransmitter)

CTransmitter::CTransmitter()
{
}


CTransmitter::~CTransmitter()
{
}

CMainFrame * CTransmitter::GetMainFrame() const
{
	return m_pMainFrame;
}

bool CTransmitter::Initialize()
{
	return true;
}
