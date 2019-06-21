#include "Hair.h"
#include "../Client.h"
#include "Transform.h"
#include "GameObject.h"
#include "Component/Animator.h"
#include "AnimationClip2D.h"
#include "AnimationState.h"
#include "AnimationController.h"
#include "Component/Renderer.h"
#include "SoundManager.h"

#include "../Option.h"
#include "Component/Text.h"
#include "Input.h"

CHair::CHair():
	m_bIsShadow(false)
{
	m_eComponentType = (int)CT_HAIR;
	SetTag("Hair");
}


CHair::~CHair()
{
}

void CHair::UseAsShadow()
{
	m_bIsShadow = true;
}

void CHair::SetBackground(CGameObject * pBackground)
{
	CTransform* pTransform = pBackground->GetTransform();	
	m_vOutlineOriginScale = pTransform->GetWorldScale();
	SAFE_RELEASE(pTransform);

	m_pBackground = pBackground;
}

void CHair::SetBody(CGameObject * pBody)
{
	m_pBody = pBody;
}

bool CHair::Initialize()
{

	m_vOriginScale = m_pTransform->GetWorldScale();
	m_vOffset = Vector3(-10, -26, 0);
	return true;
}

int CHair::LateUpdate(float fTime)
{
	CRenderer* pHairRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
	SAFE_RELEASE(pHairRenderer);


	CTransform* pBodyTransform = m_pBody->GetTransform();
	CAnimator* pAnimator = m_pBody->GetComponentFromType<CAnimator>(CT_ANIMATOR);	
	string strClipName = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetClipName();
	m_pTransform->SetWorldScale(pAnimator->GetLocalScaleFactor() * m_vOriginScale);

	float fRight = pBodyTransform->right().x;


	if (GETKEYDOWN(VK_NUMPAD4))
	{
		m_vOffset += Vector3(-1.0f, 0.0f, 0.0f);
	}
	if (GETKEYDOWN(VK_NUMPAD6))
	{
		m_vOffset += Vector3(1.0f, 0.0f, 0.0f);
	}
	if (GETKEYDOWN(VK_NUMPAD8))
	{
		m_vOffset += Vector3(0.0f, -1.0f, 0.0f);
	}
	if (GETKEYDOWN(VK_NUMPAD2))
	{
		m_vOffset += Vector3(0.0f, 1.0f, 0.0f);
	}

	//pText->SetText(to_wstring(m_vOffset.x) + L" Èë, " + to_wstring(m_vOffset.y) + L" , " + to_wstring(m_vOffset.z));

	Vector3 vOffset = m_vOffset;
	vOffset.x = vOffset.x * fRight;

	if (strClipName == "Stand")
	{
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);

		if(!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
			case 0: vOffset += Vector3(0, -2 * CELESTE_PIXEL, 0); break;
			case 1: vOffset += Vector3(0, -2 * CELESTE_PIXEL, 0); break;
			case 2: vOffset += Vector3(0, -2 * CELESTE_PIXEL, 0); break;
			case 3: vOffset += Vector3(0, -2 * CELESTE_PIXEL, 0); break;
			case 4: vOffset += Vector3(0, -1 * CELESTE_PIXEL, 0); break;
			case 5: vOffset += Vector3(0, -1 * CELESTE_PIXEL, 0); break;
			case 6: vOffset += Vector3(0, -1 * CELESTE_PIXEL, 0); break;
			case 7: vOffset += Vector3(0, -1 * CELESTE_PIXEL, 0); break;
			case 8: vOffset += Vector3(0, -1 * CELESTE_PIXEL, 0); break;
		}																				   
	}
	else	if (strClipName == "Run Fast")									
	{
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 1: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 2: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 3: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 4: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -3 * CELESTE_PIXEL, 0); break;
		case 5: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 6: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 7: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 8: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 9: vOffset	+= Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 10: vOffset	+= Vector3(0.0f * fRight * CELESTE_PIXEL, -3 * CELESTE_PIXEL, 0); break;
		case 11: vOffset	+= Vector3(0.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		}
	}
	else	if (strClipName == "Run Slow")
	{
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 1: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 2: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 3: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 4: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -3 * CELESTE_PIXEL, 0); break;
		case 5: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 6: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 7: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 8: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 9: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 10: vOffset += Vector3(0.0f * fRight * CELESTE_PIXEL, -3 * CELESTE_PIXEL, 0); break;
		case 11: vOffset += Vector3(0.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Jump Fast Up")
	{
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{//1,-3 | 1,-3
		case 0: vOffset += Vector3(0.5f * fRight * CELESTE_PIXEL, -3 * CELESTE_PIXEL, 0); break;
		case 1: vOffset += Vector3(0.5f * fRight * CELESTE_PIXEL, -3 * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Jump Fast Down")
	{
		//1,-2 | 0,-2
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
			case 0: vOffset += Vector3(0.0f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
			case 1: vOffset += Vector3(-0.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
			//case 0: vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
			//case 1: vOffset += Vector3(0.0f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Jump Slow Up")
	{
		//1,-3|1,-3|
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0: vOffset += Vector3(0.5f * fRight * CELESTE_PIXEL, -3 * CELESTE_PIXEL, 0); break;
		case 1: vOffset += Vector3(0.5f * fRight * CELESTE_PIXEL, -3 * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Jump Slow Down")
	{
		//1,-2|0,-2
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0: vOffset += Vector3(0.0f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 1: vOffset += Vector3(0.0f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Push")
	{
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 1:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 2:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 3:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 4:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 5:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 6:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 7:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 8:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 9:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 10:	vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 11:	vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -1 * CELESTE_PIXEL, 0); break;
		case 12:	vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 13:	vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 14:	vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		case 15:	vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, -2 * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Climb Wall Slide")
	{
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0:		vOffset += Vector3(-1.5f * fRight * CELESTE_PIXEL, -1.0f * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Climb Up")
	{
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0:		vOffset += Vector3(-1.5f * fRight * CELESTE_PIXEL, -1.0f * CELESTE_PIXEL, 0); break;
		case 1:		vOffset += Vector3(-1.5f * fRight * CELESTE_PIXEL, 0.0f * CELESTE_PIXEL, 0); break;
		case 2:		vOffset += Vector3(-1.5f * fRight * CELESTE_PIXEL, 0.0f * CELESTE_PIXEL, 0); break;
		case 3:		vOffset += Vector3(-1.5f * fRight * CELESTE_PIXEL, -1.0f * CELESTE_PIXEL, 0); break;
		case 4:		vOffset += Vector3(-2.0f * fRight * CELESTE_PIXEL, -1.0f * CELESTE_PIXEL, 0); break;
		case 5:		vOffset += Vector3(-2.0f * fRight * CELESTE_PIXEL, -1.0f * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Dash")
	{
		//2,0|2,0|2,0|2,1
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, 0.0f * CELESTE_PIXEL, 0); break;
		case 1:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, 0.0f * CELESTE_PIXEL, 0); break;
		case 2:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, 0.0f * CELESTE_PIXEL, 0); break;
		case 3:		vOffset += Vector3(1.5f * fRight * CELESTE_PIXEL, 1.0f * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Duck")
	{
		//2,0|2,0|2,0|2,1
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();

		CRenderer* pRenderer = m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		if (!m_bIsShadow)
			pRenderer->SetImageInfo("characters\\player\\bangs00");
		SAFE_RELEASE(pRenderer);
		switch (iFrame)
		{
		case 0:		vOffset += Vector3(0.0f * fRight * CELESTE_PIXEL, 2.0f * CELESTE_PIXEL, 0); break;
		}
	}
	else if (strClipName == "Climb Look Back")
	{
		//2,0|2,0|2,0|2,1
		unsigned int iFrame = pAnimator->GetAnimationController()->GetCurrentState()->GetClip()->GetCurrentFrameCount();
		CRenderer* pRenderer =m_pGameObject->GetComponentFromType<CRenderer>(CT_RENDERER);
		switch (iFrame)
		{
		case 0:
		{
			vOffset += Vector3(-0.5f * fRight * CELESTE_PIXEL, -0.5f * CELESTE_PIXEL, 0);
			if (!m_bIsShadow)
				pRenderer->SetImageInfo("characters\\player\\bangs00");
		}break; 
		case 1:
		{
			vOffset += Vector3(-0.5f * fRight * CELESTE_PIXEL, -0.5f * CELESTE_PIXEL, 0);
			if (!m_bIsShadow)
				pRenderer->SetImageInfo("characters\\player\\bangs01");
		}break; 
		case 2:
		{
			vOffset += Vector3(-0.5f * fRight * CELESTE_PIXEL, -0.5f * CELESTE_PIXEL, 0);
			if (!m_bIsShadow)
				pRenderer->SetImageInfo("characters\\player\\bangs02");
		}break;
		}
		SAFE_RELEASE(pRenderer);
	}

	//CTransform* pBackgroundTransform = m_pBackground->GetTransform();


	m_pTransform->SetLocalPosition(vOffset);
	Matrix matPosition = pBodyTransform->GetWorldPositionMatrix() * m_pTransform->GetLocalPositionMatrix();
	Matrix matRotation = pBodyTransform->GetWorldRotationMatrix() * m_pTransform->GetLocalRotationMatrix();
	m_pTransform->SetWorldPosition(matPosition);
	m_pTransform->SetWorldRotation(matRotation);

	if (m_bIsShadow)
	{
		m_pTransform->SetWorldPosition(m_pTransform->GetWorldPosition() - Vector3(fRight * CELESTE_PIXEL, 0.0f, 0.0f));
	}


	SAFE_RELEASE(pAnimator);
	SAFE_RELEASE(pBodyTransform);
	//SAFE_RELEASE(pBackgroundTransform);
	return 0;

/*
<Frames path="idle" hair="0,-2|0,-2|0,-2|0,-2|0,-1|0,-1|0,-1|0,-1|0,-1"/>
<Frames path="idleA" hair="0,-2|-1,-2:1|-1,-2:2|-1,-2:2|-1,-2:2|-1,-1:2|-1,-1:2|-1,-1:2|-1,-1:1|-1,-1:1|-1,-1|0,-1|0,-1"/>
<Frames path="idleB" hair="0,-2|-1,-2|-1,-2|-1,-2|-1,-2|-1,-2|-1,-2|-1,-2|-1,-2:1|-1,-2:2|-1,-2:2|-1,-2:2|-1,-2:2|-1,-2:1|-1,-2|-1,-2|-1,-2|0,-2|0,-2|0,-1|0,-1|0,-1|0,-1|0,-1"/>
<Frames path="idleC" hair="0,-2|-1,-2|-2,-2|-2,-2|-2,-2|-2,-2|1,-1|2,0|0,0|-1,-1|0,-1|0,-1"/>
<Frames path="lookUp" hair="0,-2|0,-2|0,-2|0,-2|-1,-2:1|-1,-2:1|-1,-2:1|-1,-2:1|-1,-3:1"/>
<Frames path="edge" hair="-1,-2|-1,-2|-1,-2|-1,-1|-1,-1|-1,-1|-1,-2|0,-2|1,-2|0,-2|0,-1|-1,-1|-1,-1|-1,-2"/>
<Frames path="edge_back" hair="1,-2:1|1,-2:2|0,-2:2|0,-1:2|-1,-2:2|0,-2:2|1,-2:2|1,-2:2|1,-2:2|1,-2:2|2,-1|2,-1|2,-1|2,-2"/>
<Frames path="walk" hair="0,-2|0,-1|0,-1|0,-1|0,-3|0,-2|0,-1|0,-1|0,-1|0,-1|0,-3|0,-2"/>
<Frames path="push" hair="1,-2|1,-1|1,-1|1,-1|1,-2|1,-2|1,-2|1,-2|1,-1|1,-1|1,-1|1,-1|1,-2|1,-2|1,-2|1,-2"/>
<Frames path="runSlow" hair="1,-2|1,-1|1,-1|1,-1|1,-3|1,-2|1,-1|1,-1|1,-1|1,-1|1,-3|1,-2"/>
<Frames path="runFast" hair="1,-2|1,-1|1,-1|1,-1|1,-3|1,-2|1,-1|1,-1|1,-1|1,-1|1,-3|1,-2"/>
<Frames path="run_wind" hair="4,-1|4,0|5,0|5,0|4,-2|4,-1|4,0|4,0|4,0|4,0|4,-2|4,-1"/>
<Frames path="runStumble" hair="1, 1|1,2|1,2|1,0|1,-2|1,-2|1,-1|1,-1|1,-1|1,-1|1,-3|1,-2"/>
<Frames path="dash" hair="2,0|2,0|2,0|2,1"/>
<Frames path="dreamDash" hair="2,0|1,0|0,0|0,0|1,1|2,1|2,1|2,0|1,0|1,0|1,0|1,0|1,0|1,0|1,0|1,0|1,0|0,0|1,0|1,0|2,1"/>
<Frames path="slide" hair="0,3|0,3|0,3"/>
<Frames path="jumpSlow" hair="1,-3|1,-3|1,-2|0,-2"/>
<Frames path="jumpFast" hair="1,-3|1,-3|1,-2|0,-2"/>
<Frames path="tired" hair="0,1|0,2|0,1|0,0"/>
<Frames path="climb" hair="0,-2|0,-2|0,-2|0,-2|-1,-2|-1,-2|0,-1|1,-2:1|2,-2:2"/>
<Frames path="duck" hair="0,3"/>
<Frames path="fallPose" hair="x|x|x|x|x|x|x|x|x"/>
<Frames path="hug" hair="0,2"/>
<Frames path="sleep" hair=""/>
<Frames path="flip" hair="-1,-1:2|-1,-1:1|0,-1:1|1,-1:0|1,-1:0|1,0:0|2,0:0|2,0:0|2,0:0"/>
<Frames path="faint" hair="0,1|0,1|0,1|0,1|1,1|2,1|2,2|5,5|6,8|6,7"/>
<Frames path="fainted" hair="6,7"/>
<Frames path="dangling" hair="0,-2|0,-2|0,-2|0,-2|-1,-2|-1,-2|-1,-2|-1,-2|-1,-2|-1,-2"/>
<Frames path="shaking" hair="0,-1"/>
<Frames path="idle_carry" hair="0,-2|0,-2|0,-2|0,-2|0,-1|0,-1|0,-1|0,-1|0,-1" carry="-1,-1,-1,0,0,0,0,0,-1"/>
<Frames path="jump_carry" hair="1,-3|1,-3|1,-2|0,-2" carry="-3,-3,-1,-1"/>
<Frames path="run_carry" hair="1,-2|1,-1|1,-1|1,-1|1,-3|1,-2|1,-1|1,-1|1,-1|1,-1|1,-3|1,-2" carry="-1,0,0,0,-3,-2,-1,0,0,0,-3,-1"/>
<Frames path="pickup" hair="2,0|1,-1|0,-2|0,-2|0,-2"/>
<Frames path="throw" hair="0,-3|2,-2|2,-2|1,-2"/>
<Frames path="swim" hair="0,-2|0,-2|0,-2|0,-2|0,-2|0,-2|0,-3|0,-3|0,-2|0,-2|0,-2|0,-2|1,-1|1,-1|1,0|1,0|1,0|1,0"/>
<Frames path="fall" hair="1,-2|1,-2|1,-2|1,-2|1,-2|1,-2|1,-2|1,-2"/>
<Frames path="bigFall" hair="1,-2|1,-2|1,-2|1,-2|1,-2|1,-2|1,-2|1,-2|1,-2|1,-2|1,-2"/>
<Frames path="startStarFly" hair="0,-2|0,-2|0,-2|0,-2"/>
<Frames path="starFly" hair="0,3"/>
<Frames path="starMorph" hair="0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0|0,0"/>
*/

}

void CHair::Render(float fTime)
{
}
