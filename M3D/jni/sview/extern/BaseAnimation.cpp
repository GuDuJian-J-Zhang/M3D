#include "Utility.h"

#include "m3d/scene/SceneNode.h"

#include "sview/views/View.h"
#include "m3d/scene/GroupNode.h"
#include "m3d/scene/ShapeNode.h"

#include "sview/extern/BaseAnimation.h"
#include "sview/extern/AnimationOperator.h"

using namespace SVIEW;

namespace M3D
{

const int BaseAnimation::M_EFFECT_SPEED = 10;
const int BaseAnimation::M_EFFECT_KEEPTIME = 1;

AnimtaionListener::AnimtaionListener()
{

}

AnimtaionListener::~AnimtaionListener()
{

}

void AnimtaionListener::OnFrame()
{

}

void AnimtaionListener::OnStop()
{

}

BaseAnimation::BaseAnimation()
{
	this->Init();
}

BaseAnimation::~BaseAnimation()
{
	for (int i = 0; i < m_listeners.size(); i++)
	{
		AnimtaionListener* listener = m_listeners[i];
		if (listener)
		{
			delete listener;
		}
		m_listeners[i] = NULL;
	}
	m_listeners.clear();
}

void BaseAnimation::SetAnimationOperator(AnimationOperator* aniOperator)
{
	this->m_animationOperator = aniOperator;
}
void BaseAnimation::Frame()
{
	//判断多长时间执行一次
	if (m_tickTime++ > this->m_speed)
	{
		m_tickTime = -1;

		this->m_currentFrameIndex++;

		this->OnFrame();

		for (int i = 0; i < m_listeners.size(); i++)
		{
			AnimtaionListener* listener = m_listeners[i];
			if (listener)
			{
				listener->OnFrame();
			}
		}
	}
}

void BaseAnimation::OnFrame()
{

}

void BaseAnimation::OnStop()
{

}

void BaseAnimation::RemoveFromOperator()
{
	if (this->m_animationOperator)
	{
		vector<BaseAnimation*> animations;
		animations.push_back(this);
		this->m_animationOperator->Remove(animations);
	}
}

bool BaseAnimation::IsFinish()
{
	bool finishState = this->m_finish;

	if (m_currentFrameIndex >= m_allFrameCount)
	{
		finishState |= true;
	}

	return finishState;
}

bool BaseAnimation::AddListener(AnimtaionListener* listener)
{
	bool addState = false;
	AnimtaionListener* _listener = listener;
	if (_listener)
	{
		addState = true;
		m_listeners.push_back(_listener);
	}
	return addState;
}

bool BaseAnimation::RemoveListener(AnimtaionListener* listener)
{
	bool removeState = false;
	AnimtaionListener* _listener = listener;
	if (_listener)
	{
		vector<AnimtaionListener*>::iterator it = m_listeners.begin();

		while (it != m_listeners.end())
		{
			if (*it == _listener)
			{
				removeState = true;
				m_listeners.erase(it);
				break;
			}
			it++;
		}

	}
	return removeState;
}

void BaseAnimation::Start()
{
	if (this->m_animationOperator)
	{
		ParallelAnimationQueue animation;
		animation.push_back(this);
		this->m_animationOperator->Add(animation);
	}
}

void BaseAnimation::Stop()
{
	//this->RemoveFromOperator();
	for (int i = 0; i < m_listeners.size(); i++)
	{
		AnimtaionListener* listener = m_listeners[i];
		if (listener)
		{
			listener->OnStop();
		}
	}
	this->OnStop();
}

void BaseAnimation::SetSpeedAndKeepTime(int speed, int keepTime)
{
	if (speed <= 0 || speed > 50)
	{
		speed = M_EFFECT_SPEED;
	}

	if (keepTime <= 0)
	{
		keepTime = M_EFFECT_KEEPTIME;
	}

	this->m_speed = 100 / speed;

	this->m_allFrameCount = ((int) keepTime * 1.0f / 10 * speed) / 2 * 2; //让其变成偶数

	m_dirt = true;
}

void BaseAnimation::Init()
{
	this->Reset();
}

void BaseAnimation::Reset()
{
	m_finish = false;

	m_listeners.clear();

	m_allFrameCount = M_EFFECT_KEEPTIME * M_EFFECT_SPEED;
	m_currentFrameIndex = -1;

	SetSpeedAndKeepTime();
	m_animationOperator = NULL;
	m_currentFrameIndex = -1; //当前执行到的帧数
	m_tickTime = -1;
}

}

