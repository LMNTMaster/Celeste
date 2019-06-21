#include "Pivot.h"
#include "Transform.h"
#include "GameObject.h"


CPivot::CPivot()
{
}


CPivot::~CPivot()
{
}

bool CPivot::Initialize()
{
	return true;
}

int CPivot::Update(float fTime)
{
	//CTransform* pParentTransform = m_pTransform->GetParent();
	//
	//Vector3 parentPosition = pParentTransform->GetWorldPosition();
	//Vector3 parentScale = pParentTransform->GetWorldScale();
	//Vector3 parentPivot = pParentTransform->GetPivot();
	//Vector3 originPosition = parentPosition - (parentScale * parentPivot);
	//


	//Vector3 point = parentPosition * parentScale

	return 0;
}
