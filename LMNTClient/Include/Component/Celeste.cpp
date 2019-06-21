#include "Celeste.h"



CCeleste::CCeleste(const CCeleste & component)
{
}

CCeleste::CCeleste()
{
}

CCeleste::~CCeleste()
{
}

void CCeleste::SetCamera(CCamera * pCamera)
{
	m_pCamera = pCamera;
}

void CCeleste::SetPlayer(CPlayer * pPlayer)
{
	m_pPlayer = pPlayer;
}

bool CCeleste::Initialize()
{
	return true;
}

int CCeleste::Update(float fTime)
{
	return 0;
}
