#include "animationController.h"
//#include <thread>
#include <iostream>
AnimationController::AnimationController(){
	myState = IDLE;
	topNode = NULL;
}

AnimationController::~AnimationController(){
	
}

void AnimationController::initialize(AnimationControlParser* i_parser){
	topNode = i_parser->getTopNode();
	modelAnimationMap = i_parser->getModelAnimationMap();
}

void AnimationController::play(){
	//std::thread t(&AnimationController::monitorRunningNodes, this);
	//t.detach();
	topNode->playAnimation();
	// �����ڵ��������ʱ�ڵ��޹أ�����迼���Ƿ�����ʱ�ڵ��ڲ���
	if (topNode->getCurState() == FINISHED && curPlayingNodes.size() == 0) {
		myState = FINISHED;
	}
}

void AnimationController::pause(){
	topNode->pauseAnimation(true);
}

void AnimationController::resume(){
	//std::thread t(&AnimationController::monitorRunningNodes, this);
	//t.detach();
	topNode->resumeAnimation();
	if (topNode->getCurState() == FINISHED && curPlayingNodes.size() == 0) {
		myState = FINISHED;
	}
}

void AnimationController::kill(){
	if(topNode)
	{
		topNode->pauseAnimation(true);
		//std::this_thread::sleep_for(std::chrono::milliseconds(50));
		topNode->resetStates(true);
	}
}

void AnimationController::pauseModel(ModelID i_mID){
	if (modelAnimationMap.find(i_mID) != modelAnimationMap.end()){
		vector<StepNode*> animations = modelAnimationMap[i_mID];
		for (int i = 0; i < animations.size(); ++i){
			animations[i]->pauseAnimation(false);
		}
	}
}

void AnimationController::resumeModel(ModelID i_mID){
	if (modelAnimationMap.find(i_mID) != modelAnimationMap.end()){
		vector<StepNode*> animations = modelAnimationMap[i_mID];
		for (int i = 0; i < animations.size(); ++i){
			animations[i]->resumeAnimation();
		}
	}
}

/*
	�������ź������������߳�
	����topNode�ṹ����ȡ��ǰ���ŵĽڵ㣬���û�У����˳�
	ÿ�α���ǰ�������curPlayingNodes
*/
void AnimationController::monitorRunningNodes(){
	////std::this_thread::sleep_for(std::chrono::milliseconds(100)); // ���ڶ�������һ���
	//findRunningNodes(topNode, curPlayingNodes);
	//while (!curPlayingNodes.empty()){ // ��ֹ����
	//	curPlayingNodes.clear();
	//	findRunningNodes(topNode, curPlayingNodes);
	//	std::this_thread::sleep_for(std::chrono::milliseconds(50));
	//}
}

// ����topNode�ṹ����ȡ��ǰ���ŵĽڵ�, �ݹ�
// ����ÿ���ڵ㶼��������ʱ�ڵ㣬����Ҫ�������нڵ�
void AnimationController::findRunningNodes(AnimationNode* i_topNode, vector<StepNode*>& o_nodeList){
	AnimationNodeType nodeType = i_topNode->getNodeType();
	AnimationState nodeState = i_topNode->getCurState();

	switch (nodeType){
		case SERIAL_NODE:{
			SerialNode* sNode = static_cast<SerialNode*>(i_topNode);
			vector<AnimationNode*> sNodeList = sNode->getNodeList();
			for (int i = 0; i < sNodeList.size(); ++i){
				findRunningNodes(sNodeList[i], o_nodeList);
			}
			break;
		}
		case PARALLEL_NODE:{
			ParallelNode* sNode = static_cast<ParallelNode*>(i_topNode);
			vector<AnimationNode*> sNodeList = sNode->getNodeList();
			for (int i = 0; i < sNodeList.size(); ++i){
				findRunningNodes(sNodeList[i], o_nodeList);
			}
			break;
		}
		case STEP_NODE:{
			StepNode* stepNode = static_cast<StepNode*>(i_topNode);
			if (nodeState == RUNNING){
				o_nodeList.push_back(stepNode);
				vector<AnimationNode*> attachedNodes = stepNode->getAttachedNodes();
				// �����ڵ�
				for (int i = 0; i < attachedNodes.size(); ++i){
					//o_nodeList.push_back(attachedNodes[i]);
					findRunningNodes(attachedNodes[i], o_nodeList);
				}
			}
			// �ӳٽڵ�
			std::vector<pair<AnimationNode*, TLframe>> delayedNodes = stepNode->getDelayedNodes();
			for (int i = 0; i < delayedNodes.size(); ++i){
				findRunningNodes(delayedNodes[i].first, o_nodeList);
			}
			break;
		}
	}
}

void AnimationController::setPlayNotifyMsg()
{
	//map<ModelID, vector<StepNode*>>::iterator it = modelAnimationMap.begin();
	//while (it!= modelAnimationMap.end())
	//{
	//	for(int i = 0; i < it->second.size(); i++)
	//	{
	//		(it->second.at(i))->SetPlayNotifyMsg(hWnd, nMsg);
	//	}
	//}
}
