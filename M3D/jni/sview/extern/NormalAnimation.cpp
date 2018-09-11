#include <stdlib.h>

#include "Utility.h"

#include "sview/extern/BaseAnimation.h"
#include "sview/extern/NormalAnimation.h"

#include "m3d/scene/SceneNode.h"
#include "sview/views/View.h"

#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ShapeNode.h"
#include "m3d/model/ModelShape.h"

using namespace SVIEW;

namespace M3D
{

HideShowAnimation::HideShowAnimation(IShape* shape,bool from, bool to)
:BaseAnimation()
{
	this->Reset();
	this->SetState(shape,from,to);
}


HideShowAnimation::~HideShowAnimation()
{

}

void HideShowAnimation::OnFrame()
{
	if (this->m_shape)
	{
		this->m_shape->SetVisible(this->GetState());
	}
}
void HideShowAnimation::OnStop()
{
	if (this->m_shape)
	{
	   this->m_shape->SetVisible(this->m_toState);
	}
}

bool HideShowAnimation::GetState()
{
	bool isShow = false;
	if(this->m_currentFrameIndex %2 != 0)
	{
		isShow = this->m_fromState;
	}else
	{
		isShow = this->m_toState;
	}
	return isShow;
}

void HideShowAnimation::Reset()
{
	this->m_shape = NULL;
	this->m_fromState = true;
	this->m_toState = true;

	this->SetSpeedAndKeepTime();
}

void HideShowAnimation::SetState(IShape* shape, bool from,
		bool to)
{

	bool m_fromState = from;
	bool m_toState = to;
	this->m_shape = shape;
}

ChangeColorAnimation::ChangeColorAnimation(IShape* shape,const Color& distColor):
	BaseAnimation()
{
	this->Reset();
	this->SetValue(shape,distColor);
}

ChangeColorAnimation::~ChangeColorAnimation()
{

}

void ChangeColorAnimation::OnFrame()
{
	if (this->m_shape)
	{
            if(this->m_currentFrameIndex %2 != 0)
			{
				this->m_shape->ResetColor();
			}else
			{
				Color c = this->GetState();
			    this->m_shape->SetColor(c);
			}
	}
}

void ChangeColorAnimation::OnStop()
{
	if (this->m_shape)
	{
		this->m_shape->ResetColor();
	}
}

Color ChangeColorAnimation::GetState()
{
	Color c (1.0f,1.0f,1.0f,0.5f);
	if(this->frameStates.size()>0)
	{
		c = this->frameStates[0];
	}
	return c;
}


void ChangeColorAnimation::Reset()
{
	this->m_shape = NULL;
	this->frameStates.clear();
	m_useOrignColor = true;

	this->SetSpeedAndKeepTime();
}


void ChangeColorAnimation::SetValue(IShape* shape,
		const Color& distColor)
{
	vector<Color> states;
	states.push_back(distColor);

	this->SetValue(shape, states);
}

void ChangeColorAnimation::SetValue(IShape* shape,
		const vector<Color>& frameStates)
{
	this->m_shape = shape;
	this->frameStates = frameStates;
	this->m_stateIndex = 0;
}


ChangePosAnimation::ChangePosAnimation(Model* shapeNode,const Vector3& fromPos, const Vector3& disPos)
:	BaseAnimation()
{
	this->Reset();
	this->SetValue(shapeNode,fromPos,disPos);
}

ChangePosAnimation::~ChangePosAnimation()
{

}
void ChangePosAnimation::OnFrame()
{
	if (this->shapeNode)
	{
		ModelShape* modelShape =shapeNode->GetModelShape();
		if(modelShape)
		{
			Matrix3x4 origMatrix ;
			(origMatrix).MultiTranslate(this->GetState());
			modelShape->SetWorldMatrix(origMatrix);
		}
	}
}

void ChangePosAnimation::OnStop()
{
	if (this->shapeNode)
	{
		ModelShape* modelShape =shapeNode->GetModelShape();
		if(modelShape)
		{
			Matrix3x4 origMatrix ;
			(origMatrix).MultiTranslate(this->m_fromPos + this->m_distance);
			modelShape->SetWorldMatrix(origMatrix);
		}
	}
}

Vector3 ChangePosAnimation::GetState()
{
	Vector3 pos(0,0,0);

	float percent = this->m_currentFrameIndex*1.0f/this->m_allFrameCount;
	pos = this->m_fromPos + this->m_distance*percent;

	return pos;
}

void ChangePosAnimation::Reset()
{
	this->shapeNode = NULL;
	this->m_fromPos = Vector3(0,0,0);
	this->m_distance = Vector3(0,0,0);

	this->SetSpeedAndKeepTime();
}

void ChangePosAnimation::SetValue(Model* shapeNode,
		const Vector3& from, const Vector3& to)
{
	this->m_fromPos = from;
	this->m_distance = to-from;

	this->shapeNode = shapeNode;
}

}

