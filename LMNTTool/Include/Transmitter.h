#pragma once
#include "Engine.h"

class CTransmitter
{
	DECLARE_SINGLE(CTransmitter)


public:
	class CMainFrame* m_pMainFrame;

public:
	CMainFrame* GetMainFrame() const;

public:
	bool Initialize();

};

