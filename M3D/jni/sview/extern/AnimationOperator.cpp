#include "Utility.h"

#include "sview/views/View.h"

#include "sview/extern/BaseAnimation.h"
#include "sview/extern/AnimationOperator.h"
#include "m3d/scene/SceneNode.h"

#include "m3d/scene/ShapeNode.h"
#include "m3d/base/Mutex.h"

using namespace SVIEW;

namespace M3D
{
const int AnimationOperator::TIMER_INTER = 10;

const int AnimationOperator::M_EFFECT_SPEED = 3;
const int AnimationOperator::M_EFFECT_KEEPTIME = 2;

AnimationOperator* AnimationOperator::instance = NULL;

AnimationOperator* AnimationOperator::GetInstance(View* view)
{
	if (AnimationOperator::instance == NULL)
	{
		AnimationOperator::instance = new AnimationOperator();
		instance->SetView(view);
	}

	return AnimationOperator::instance;
}

AnimationOperator* AnimationOperator::GetInstance()
{
	if (AnimationOperator::instance == NULL)
	{
		AnimationOperator::instance = new AnimationOperator();
	}
	return AnimationOperator::instance;
}

AnimationOperator::AnimationOperator()
{
	this->Reset();
}

AnimationOperator::~AnimationOperator()
{
	for (int i = 0; i < this->m_animations.size(); i++)
	{
		SequentialAnimationQueue& sequentialAnimations = this->m_animations[i];

		for (int j = 0; j < sequentialAnimations.size(); j++)
		{
			BaseAnimation* animation = sequentialAnimations[i];
			delete animation;
			sequentialAnimations[i] = NULL;
		}
		sequentialAnimations.clear();
	}
	this->m_animations.clear();

}

bool AnimationOperator::Add(BaseAnimation*animation)
{
	bool addState = false;

	if (animation)
	{
		ParallelAnimationQueue animations;
		animations.push_back(animation);
		addState = this->Add(animations);
	}

	return addState;
}

bool AnimationOperator::Remove(BaseAnimation* animation)
{

	bool removeState = false;

	if (animation)
	{
		vector<BaseAnimation*> animations;
		animations.push_back(animation);
		removeState = this->Remove(animations);
	}

	return removeState;
}

bool AnimationOperator::Add(vector<BaseAnimation*>& animations)
{

	bool addState = false;
	this->Pause();

	for (int i = 0; i < animations.size(); i++)
	{
		BaseAnimation* animation = animations[i];
		if (animation)
		{
			SequentialAnimationQueue newSequentialAnimations;
			animation->SetAnimationOperator(this);
			newSequentialAnimations.push_back(animation);

			this->m_animations.push_back(newSequentialAnimations);
		}
	}
	this->Continue();

	if (this->m_pView)
	{
		this->m_pView->RequestDraw();
	}

	return addState;
}

bool AnimationOperator::Add(vector<SequentialAnimationQueue>& animations)
{
	bool addState = false;
	this->Pause();

	for (int i = 0; i < animations.size(); i++)
	{

		SequentialAnimationQueue& sequentialAnimations = animations[i];

		SequentialAnimationQueue newSequentialAnimations;

		for (int j = 0; j < sequentialAnimations.size(); j++)
		{
			BaseAnimation* animation = sequentialAnimations[j];
			if (animation)
			{
				animation->SetAnimationOperator(this);
				newSequentialAnimations.push_back(animation);
			}
		}

		if (newSequentialAnimations.size() > 0)
		{
			this->m_animations.push_back(newSequentialAnimations);
		}
	}
	this->Continue();

	if (this->m_pView)
	{
		this->m_pView->RequestDraw();
	}
	return addState;
}

bool AnimationOperator::Remove(vector<BaseAnimation*>& animations)
{
	bool removeState = false;
	for (int i = 0; i < animations.size(); i++)
	{
		BaseAnimation* animation = animations[i];
		if (animation)
		{
			RemoveSequential: vector<SequentialAnimationQueue>::iterator it =
					m_animations.begin();
			while (it != m_animations.end())
			{
				RemoveFromSequential: SequentialAnimationQueue::iterator seqIt =
						(*it).begin();
				while (seqIt != (*it).end())
				{
					if (*seqIt == animation)
					{
						removeState = true;
						animation->Stop();
						delete *seqIt;
						*seqIt = NULL;
						(*it).erase(seqIt);
						goto RemoveFromSequential;
						//再次开始去查找下一个
					}
					else
					{
						seqIt++;
					}
				}
				//队列下面所有的动画全部删除了，则删除该队列
				if ((*it).size() == 0)
				{
					m_animations.erase(it);
					goto RemoveSequential;
					//再次
				}
				else
				{
					it++;
				}
			}
		}
	}

	if (m_animations.size() <= 0)
	{
		this->Pause();
	}

	if (this->m_pView)
	{
		this->m_pView->RequestDraw();
	}

	return removeState;
}

void AnimationOperator::Stop()
{

}

void AnimationOperator::SetView(View* view)
{
	this->m_pView = view;
}

void AnimationOperator::Pause()
{
	if (this->m_timer.IsStart())
	{
		this->m_timer.StopTimer();
	}
}

void AnimationOperator::Continue()
{
	if ((!this->m_timer.IsStart()) && this->m_animations.size() > 0)
	{
		//设置触发次数，为每秒10次，太快了会怎样？
		m_timer.SetTimer(OperatorTask, this, 0, TIMER_INTER);
		this->m_timer.StartTimer();
	}
}

void AnimationOperator::Reset()
{
	m_animations.clear();
	m_pView = NULL;
	m_finish = false;
}

bool AnimationOperator::IsFinish()
{
	return false;
}

void* AnimationOperator::OperatorTask(void* data)
{
	AnimationOperator* animationOperator = (AnimationOperator*) data;

	vector<SequentialAnimationQueue>& animations =
			animationOperator->m_animations;

	vector<BaseAnimation*> finishedanimations;
	//判断任务是否结束
	for (int i = 0; i < animations.size(); i++)
	{
		SequentialAnimationQueue& sequentialAnimations = animations[i];
		for (int j = 0; j < sequentialAnimations.size(); j++)
		{
			BaseAnimation* animation = sequentialAnimations[j];

			if (animation && animation->IsFinish())
			{
				finishedanimations.push_back(animation);
			}
		}
	}
	//如果结束了，则首先将任务从队列中移除
	if (finishedanimations.size() > 0)
	{
		animationOperator->Remove(finishedanimations);
	}

//	LOGE("task ++");
	//执行任务
	for (int i = 0; i < animations.size(); i++)
	{
		SequentialAnimationQueue& sequentialAnimations = animations[i];
		if (sequentialAnimations.size() > 0)
		{
			BaseAnimation* animation = sequentialAnimations[0];
			if (animation)
			{
				animation->Frame();
			}
		}
	}

	if (animationOperator->m_pView)
	{
		animationOperator->m_pView->RequestDraw();
	}
	return 0;
}

}

