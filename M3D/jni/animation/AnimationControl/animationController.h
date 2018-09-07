#ifndef AnimationControl_AnimationController_H
#define AnimationControl_AnimationController_H

#include "animationNode.h"
#include "animationControlParser.h"

class SA_API AnimationController{
public:
	AnimationController();
	~AnimationController();
	void initialize(AnimationControlParser* i_parser);
	void play();
	void pause();
	void resume();
	void kill();

	// 重置动画状态
	void reset();
	// 暂停、继续某一模型的动画
	void pauseModel(ModelID i_mID);
	void resumeModel(ModelID i_mID);
	// 循环监听，更新正在运行动画节点
	void monitorRunningNodes();
	// 寻找正在运行的节点
	void findRunningNodes(AnimationNode* i_topNode, vector<StepNode*>& o_nodeList);
	// 获取正在播放节点
	inline vector<StepNode*> getCurPlayingNodes(){ return curPlayingNodes; }

	//设置播放处理消息
	void setPlayNotifyMsg();

public:
	AnimationState myState;

private:
	/// 顶级节点
	AnimationNode* topNode;
	/// 当前播放中的节点
	vector<StepNode*> curPlayingNodes;
	/// 模型与节点对应关系
	map<ModelID, vector<StepNode*>> modelAnimationMap;
	//std::thread monitorThread;
	bool isPlaying;
};

#endif