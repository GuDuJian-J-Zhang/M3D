// 动画步骤类
#ifndef AnimationControl_AnimationNode_H
#define AnimationControl_AnimationNode_H

#include <vector>
#include <map>
#include <sstream>
#include "../SimulationAnimation/AnimationStepManager.h"

using namespace std;

/// 定义时间帧类型
typedef unsigned int TLframe;

static string floatToStr(float f){
	ostringstream buffer;
	buffer << f;
	return buffer.str();
}

struct AnimationID{
	int processID;
	int stepID;
	AnimationID(){
		processID = 0;
		stepID = 0;
	}
	AnimationID(int i_procID, int i_stepID){
		processID = i_procID;
		stepID = i_stepID;
	}
	// 用于map的key,重载<运算
	bool operator<(const AnimationID& aID)const{
		if (processID < aID.processID){
			return true;
		}
		else if (processID == aID.processID){
			return stepID < aID.stepID;
		}
		else{
			return false;
		}
	}
	string toString(){
		return "AnimID(" + floatToStr(processID) + ", " + floatToStr(stepID) + ")";
	}
};

struct ModelID{
	vector<int> plcPath;
	void addPathItem(int item){
		plcPath.push_back(item);
	}
	// 用于map的key,重载<运算
	bool operator<(const ModelID& m)const{
		int size = plcPath.size() < m.plcPath.size() ? plcPath.size() : m.plcPath.size();
		int index = 0;
		while (index < size){
			if (plcPath[index] < m.plcPath[index]){
				return true;
			}
			else if (plcPath[index] == m.plcPath[index]){
				index++;
				continue;
			}
			else{
				return false;
			}
		}

		return plcPath.size() < m.plcPath.size();
	}
};

//动画节点类型。暂时排除OVERLAP_NODE, ATTACHED_NODE。交叠关系，附属关系放到步骤节点内
enum AnimationNodeType{GENERIC_NODE, STEP_NODE, COMPOSITE_NODE, SERIAL_NODE, PARALLEL_NODE, DUMMYSTEP_NODE};
//动画运行状态
enum AnimationState{IDLE, RUNNING, PAUSED, FINISHED};
// 动画过渡状态
//enum AnimationStateTransition{STARTING, PAUSING, RESUMING, FINISHING};

/// 动画节点基类
class SA_API AnimationNode{
public:
	AnimationNode();
	~AnimationNode();
	virtual void playAnimation() = 0;
	virtual void pauseAnimation(bool isGlobal) = 0;
	virtual void resumeAnimation() = 0;
	
	inline AnimationNodeType getNodeType(){return myNodeType;}
	inline AnimationState getCurState(){ return curState; }
	inline AnimationState getLastState() { return lastState; }
	inline void setDuration(TLframe t){duration = t;}
	inline void setAnimationState(AnimationState i_state){
		lastState = curState;
		curState = i_state;
	}
	inline void setRepeatTimes(int nTimes){
		repeatTimes = nTimes;
		leftRepeatTimes = nTimes;
	}
	// 如果repeatSwitch = true, 重置leftRepeatTimes
	virtual void resetStates(bool repeatSwitch);

protected:
	AnimationNodeType myNodeType; // 节点类型
	AnimationState curState; // 当前动画状态
	AnimationState lastState; // 上一个动画状态，状态变化(state transition) 和 resume 使用
	TLframe duration; // 时间长度
	int repeatTimes; // 重复播放次数
	int leftRepeatTimes; // 剩余播放次数 
	//map<AnimationNode*, TLframe> delayedNodes; // 该节点对应的延时节点
	//vector<pair<AnimationNode*, TLframe>> delayedNodes;
};

/// 动画步骤节点，内含动画步骤
class StepNode : public AnimationNode{
public:
	//StepNode(void* i_bh){bh = i_bh;}
	StepNode(ModelID mID, AnimationID aniID, NS_SimulationAnimation::CSimulationAnimationManager * pSA);
	~StepNode();
	void playAnimation();
	void pauseAnimation(bool isGlobal);
	void resumeAnimation();

	void playDelayedNodes();
	void playDelayedThread(AnimationNode* i_node, TLframe i_t, AnimationNode* i_parentNode);
	// TODO 使用AnimationID获取sview动画，初始化节点 
	void initialize(AnimationID);
	TLframe getStepDuration();
	inline void addAttachedNode(AnimationNode* i_node){attachedNodes.push_back(i_node);}
	inline vector<AnimationNode*> getAttachedNodes(){ return attachedNodes; }
	virtual void resetStates(bool repeatSwitch);
	inline void addDelayedNode(AnimationNode* i_node, TLframe i_t){
		delayedNodes.push_back(pair<AnimationNode*, TLframe>(i_node, i_t));
	}
	inline vector<pair<AnimationNode*, TLframe>> getDelayedNodes(){ return delayedNodes; }

private:
	ModelID m_modelID;
	AnimationID m_aniID;
	TLframe curFrame;
	NS_SimulationAnimation::CSBehaviorAction* m_pBehaviorAction; // behavior 动画步骤指针
	//HWND m_pAnimationPlayWnd;	//动画播放处理界面

	vector<AnimationNode*> attachedNodes;
	vector<pair<AnimationNode*, TLframe>> delayedNodes;
};

/// 复合节点抽象类
class CompositeNode : public AnimationNode{
public:
	CompositeNode();
	~CompositeNode();
	virtual void playAnimation() = 0;
	virtual void pauseAnimation(bool isGlobal) = 0;
	virtual void resumeAnimation() = 0;
	inline void addNode(AnimationNode* i_node){nodeList.push_back(i_node);}
	inline vector<AnimationNode*> getNodeList(){ return nodeList; }
	void resetStates(bool repeatSwitch);

protected:
	vector<AnimationNode*> nodeList;
};

/// 串行节点
class SerialNode : public CompositeNode{
public:
	SerialNode();
	~SerialNode();
	void playAnimation();
	void pauseAnimation(bool isGlobal);
	void resumeAnimation();
	void resetStates(bool repeatSwitch);

private:
	int curIndex;
};

class ParallelNode : public CompositeNode{
public:
	ParallelNode();
	~ParallelNode();
	void playAnimation();
	void pauseAnimation(bool isGlobal);
	void resumeAnimation();

private:

};

class DummyStepNode : public AnimationNode{
public:
	DummyStepNode();
	DummyStepNode(TLframe i_duration);
	~DummyStepNode();
	void playAnimation();
	void pauseAnimation(bool isGlobal);
	void resumeAnimation();
	void waitDuration(TLframe t);

private:
	TLframe duration;
	TLframe curFrame;
};

#endif


