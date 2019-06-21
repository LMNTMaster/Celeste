#pragma once
#include "Component.h"
#include "../Client.h"

LMNT_USING
enum PLAYER_ANIMATION_STATE
{
	AS_IDLE_CARRY = 0,
	AS_RUN_CARRY,
	AS_JUMP_SLOW_CARRY,
	AS_FALL_CARRY,
	AS_PICK_UP,
	AS_THROW,
	AS_IDLE,
	AS_IDLE_A,
	AS_IDLE_B ,
	AS_IDLE_C ,
	AS_LOOK_UP,
	AS_WALK,
	AS_PUSH,
	AS_RUN_SLOW,
	AS_RUN_FAST,
	AS_RUN_STRUMBLE,
	AS_RUN_WIND,
	AS_DASH,
	AS_DREAM_DASH_START,
	AS_DREAM_DASH,
	AS_DREAM_DASH_EXIT,
	AS_SLIDE,
	AS_JUMP_SLOW_UP,
	AS_JUMP_SLOW_DOWN,
	AS_JUMP_FAST_UP,
	AS_JUMP_FAST_DOWN,
	AS_TIRED,
	AS_TIRED_STILL,
	AS_CLIMB_WALL_SLIDE,
	AS_CLIMB_LOOK_BACK_START, 
	AS_CLIMB_LOOK_BACK,
	AS_CLIMB_UP,
	AS_CLIMB_PUSH,
	AS_CLIMB_PULL,
	AS_DUCK,
	AS_FALL_POSE,
	AS_EDGE,
	AS_EDGE_BACK,
	AS_FAINT_START,
	AS_FAINT,
	AS_FLIP,
	AS_FLIP_SKID,
	AS_DANGLING,
	AS_DEATH_SIDE,
	AS_DEATH_UP,
	AS_DEATH_DOWN,
	AS_SWIM_IDLE,
	AS_SWIM_UP,
	AS_SWIM_DOWN,
	AS_STARFLY_START,
	AS_STARFLY,
	AS_BUBBLE,
	AS_FALL,
	AS_BIG_FALL,
	AS_BIG_FALL_RECOVER,
	AS_SLEEP,
	AS_SLEEP_BAG_DOWN,
	AS_WAKE_UP,
	AS_WAKE_UP_HALF,
	AS_SPIN,
	AS_SHAKING,
	AS_HUG,
	AS_STAR_MORPH,
	AS_STAR_MORPH_IDLE,
	AS_WALK_CARRY_THEO,
	AS_WALK_CARRY_THEO_COLLAPSE,
	AS_TENTACLE_GRAB,
	AS_TENTACLE_GRABBED,
	AS_TENTACLE_PULL,
	AS_TENTACLE_DANGLING,
	AS_SIT_DOWN,
	AS_LAUNCH,
	AS_LAUNCH_RECOVER


};

class CPlayer :
	public CComponent
{
private:
	friend class CGameObject;
	friend class CStageManager00;

private:
	CPlayer();
	CPlayer(const CPlayer& component);
	~CPlayer();

private:
	float m_fDebugTimer;

	Vector3 m_vBeginPosition;
	Vector4 m_vHairColor;
	bool m_bInitialized;
	bool m_bAwake;
	bool m_bEvent;
	bool m_bAreaChanged;
	bool m_bInteraction;

	//class CGameObject* m_pHair;
	PLAYER_ANIMATION_STATE m_eState;
	Vector3 m_vOriginPosition;
	class CCollider* m_pBodyBottom;
	class CCollider* m_pBodyTop;
	class CCollider* m_pBodyRight;
	class CCollider* m_pBodyLeft;
	class CGameObject* m_pClimbWall;
	vector<class CGameObject*> m_vecHair;
	vector<class CGameObject*> m_vecHairShadow;


	bool m_bDeath;
	bool m_bSpawned;
	bool m_bBoostBuff;
	bool m_bAffactBoostable;
	bool m_bBoostable;
	bool m_bPrevBoostable;
	float m_fBoostBuffTimer;
	PAREA m_pArea;
	Vector3 m_vCheckPoint;

//Scaler
	bool m_bOnScaling;
	Vector3 m_vBeginScale;

//Gravity
	bool m_bAffactGravity;
	bool m_bOnGround;
	Vector3 m_vPotential;
	float m_fForce;
	float m_fAcceleration;
	float m_fGravityThreshold;

//Jump
	float m_fTotalJumpForce;
	float m_fBeginJumpHeight;
	bool m_bJump;
	bool m_bJumpAuto;
	bool m_bJumpTransitioned;
	float m_fJumpPressTimerMinThreshold;
	float m_fJumpPressTimerMaxThreshold;
	float m_fJumpPressTimer;
	float m_fJumpHeight;
	float m_fJumpMinWeight;
	float m_fJumpDuration;
	float m_fJumpTimer;

//Move
	bool m_bMoveAuto;
	float m_bAutoMoveScale;
	
	float m_fMoveHorizontalAccelerationOffset;
	float m_fMoveHorizontalAcceleration;

	float m_fMoveRightAcceleration;
	float m_fMoveLeftAcceleration;
	float m_fMoveMaxAcceleration;
	float m_fMoveMaxSpeed;
	float m_fMoveRightPressTimer;
	float m_fMoveLeftPressTimer;
	float m_fMoveAxisWeight;
	float m_fMoveAxisWeightMinThreshold;
	float m_fMoveAxisWeightMaxThreshold;

//Push
	bool m_bPushable;

//Climb
	bool m_bHeadColliding;
	DIRECTION m_eClimbDirection;
	bool m_bOnClimb;
	bool m_bOnSlide;
	float m_fClimbSpeed;
	float m_fSlideSpeed;


//Dash
	bool m_bOnDash;
	Vector3 m_vDashDirection;
	int m_iDashCountMax;
	int m_iDashCount;
	float m_fDashDistance;
	float m_fDashDuration;
	float m_fDashTimer;
	float m_fDashEffectTimer;
	float m_fDashEffectTimerMax;


//Dream Dash



private:
	float JumpWeight(float fDeltaTime);
	void AddForce(float fForce);


//Scale
	void SetScaling(const Vector3& vBeginScale );
	void UpdateScaling(float fTimeScale, float fDeltaTime);

//Jump


//Move
	float MoveRightWeight();
	float MoveLeftWeight();
	float MoveAcceleration(float fTime);

//Dash
	float DashAcceleration(float fElapseTime);
	void CreateDummy(const Vector4& vColor);


public:
	bool OnGround() const;
	bool OnDash() const;
	bool GetInteraction() const;
	void SetInteraction(bool bInteraction);


public:
	
	PAREA GetCurrentArea() const;
	bool GetAreaChanged() const;
	bool GetSpawned() const;
	void SetBoost(bool bBoost);
	void ResetPotentialForce();
	void SetPotentialForce(const Vector3& fForce);	
	void SetBeginPosition(const Vector3& vPosition);
	void AddHair(class CGameObject* pHair, bool bShadow = false);
	void SetHairColor();
	void SetBottomCollider(class CCollider* pCollider);
	void SetTopCollider(class CCollider* pCollider);
	void SetLeftCollider(class CCollider* pCollider);
	void SetRightCollider(class CCollider* pCollider);
	void ResetJumpOptions();
	void RenderHair(bool bRender);

public:
	void JumpForced(float fForce, bool bAutoMove = true, float fAutoMoveScale = 0.5f, bool bTrailingForce = false);
	void SetMaxDash(int iCount);
	void ReduceDash();
	void SetAnimationState(PLAYER_ANIMATION_STATE eState);
	void SetEvent(bool bEvent);
	void Reset();
	void ResetCurrentPosition();
	void ResetDashCount();
	void ResetPhysics();
	void SetAwake(bool bAwake);

public:
	bool Initialize()						override;
	int Input(float fTime)				override;
	int Update(float fTime)			override;
	int LateUpdate(float fTime)		override;
	int OnCollisionEnter(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
	int OnCollisionStay(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
	int OnCollisionExit(class CCollider* pThis, class CCollider* pTarget, float fTime) override;
	void Render(float fTime)			override;
	CPlayer* Clone()	const			override;	



public:
	class CGameObject* m_pDebugText;


};
