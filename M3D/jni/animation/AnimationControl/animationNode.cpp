#include "stdafx.h"
#include "animationNode.h"
//#include "Threads.h"
#include <iostream>
#include "../SimulationAnimation/AnimationStepManager.h"

AnimationNode::AnimationNode(){
	myNodeType = GENERIC_NODE;
	curState = IDLE;
	lastState = IDLE;
	duration = 0;
	repeatTimes = 1;
	leftRepeatTimes = 1;
}

AnimationNode::~AnimationNode(){
}

void AnimationNode::resetStates(bool repeatSwitch){
	lastState = IDLE;
	curState = IDLE;
	if(repeatSwitch)
		leftRepeatTimes = repeatTimes;
}

StepNode::StepNode(ModelID i_mID, AnimationID i_aniID, NS_SimulationAnimation::CSimulationAnimationManager * pSA){
	myNodeType = STEP_NODE; 
	m_modelID = i_mID;
	m_aniID = i_aniID;
	curFrame = 0;
	//m_pAnimationPlayWnd = NULL;
	m_pBehaviorAction = NULL;
	NS_SimulationAnimation::CAnimationStepManager * pAnimationStepManager = NULL;
	if (pSA)
		pAnimationStepManager = pSA->GetAnimationStepManager();
	if (pAnimationStepManager)
	{
		NS_SimulationAnimation::CProcessManager * pProcessManager = pAnimationStepManager->FindProcessManagerByID(m_aniID.processID);
		if (pProcessManager)
		{
			NS_SimulationAnimation::CProcess* pProcess = pProcessManager->FindProcessByID(m_aniID.stepID);
			if (pProcess)
			{
				m_pBehaviorAction = pProcess->GetBehaviorAction();
			}
		}
	}
}

StepNode::~StepNode(){
}

void StepNode::playAnimation(){
	//if (!m_pBehaviorAction)
	//{
	//	return;
	//}
	//if (curState == FINISHED){
	//	lastState = curState;
	//	curState = RUNNING;
	//	leftRepeatTimes = repeatTimes;
	//}
	//lastState = curState;
	//curState = RUNNING;

	//// 播放附属节点,每个节点对应一个线程，最后join
	//vector<std::thread> threads;
	//for (int i = 0; i < attachedNodes.size(); ++i){
	//	AnimationNode* node = attachedNodes[i];
	//	//threads.push_back(std::thread(&AnimationNode::playAnimation, node));
	//}
	////播放延时节点
	//playDelayedNodes();

	//while (leftRepeatTimes > 0){

	//	m_pBehaviorAction->Rewind();
	//	m_pBehaviorAction->Continue();
	//	//std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//	//if (m_pAnimationPlayWnd)
	//	//{
	//	//	PostMessage(m_pAnimationPlayWnd, WM_ANIMATION_PLAY, (WPARAM)m_pBehaviorAction, NULL);
	//	//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	//	//}
	//	while (m_pBehaviorAction->IsPlaying() && curState != PAUSED){
	//	//	std::this_thread::sleep_for(std::chrono::milliseconds(30));
	//	}
	//	if (curState == PAUSED){
	//		/* 记录当前帧curFrame*/
	//		curFrame = m_pBehaviorAction->GetCurrentTick();
	//		break;
	//	}
	//	leftRepeatTimes--;
	//}
	//// 目前未处理附属节点与该节点时间长度不一致情况。需等待附属节点完结，该节点才完结。
	//for (auto &t : threads){
	//	t.join();
	//}
	//if (curState == PAUSED){ // 必须在所有线程join之后return
	//	return;
	//}
	//curFrame = m_pBehaviorAction->GetCurrentTick();
	//lastState = curState;
	//curState = FINISHED;
}

void StepNode::pauseAnimation(bool isGlobal){
	if (!m_pBehaviorAction)
	{
		return;
	}
	if (isGlobal) {
		// 暂停延时节点
		for (int i = 0; i < delayedNodes.size(); ++i) {
			delayedNodes[i].first->pauseAnimation(isGlobal);
		}
	}
	//闲置或运行的节点可以被暂停
	if (curState == IDLE || curState == RUNNING){
		lastState = curState;
		curState = PAUSED;
		// SVIEW 暂停动画
		m_pBehaviorAction->Stop();
		//if (m_pAnimationPlayWnd)
		//{
		//	m_pAnimationPlayWnd->PostMessage(WM_ANIMATION_STOP, (WPARAM)m_pBehaviorAction, NULL);
		//}

		// 暂停附属节点
		for (int i = 0; i < attachedNodes.size(); ++i){
			attachedNodes[i]->pauseAnimation(isGlobal);
		}
	}
}

// 继续操作是阻塞的(blocking),直到所有附属动画join才退出
// 从当前帧curFrame继续播放, non-blocking; 如果blocking,单独开启线程
void StepNode::resumeAnimation(){
	if (curState != PAUSED || lastState != RUNNING) {// 当前不暂停 或 暂停前状态为空闲, 直接返回
		// 继续延时节点
		playDelayedNodes();
		return;
	} 

	if (!m_pBehaviorAction)
	{
		return;
	}

	lastState = curState;
	curState = RUNNING;
	// 继续延时节点
	playDelayedNodes();
	// 继续附属节点
	//vector<std::thread> threads;
	for (int i = 0; i < attachedNodes.size(); ++i) {
	//	threads.push_back(std::thread(&AnimationNode::resumeAnimation, attachedNodes[i]));
	}

	// sview 继续播放
	while (leftRepeatTimes > 0) {
		m_pBehaviorAction->Continue();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//if (m_pAnimationPlayWnd)
		//{
		//	// sendMessage消息阻塞，改为postMessage
		//	m_pAnimationPlayWnd->PostMessage(WM_ANIMATION_CONTINUE, (WPARAM)m_pBehaviorAction, NULL);
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//}
		
		while (m_pBehaviorAction->IsPlaying() && curState != PAUSED) {
	//		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		if (curState == PAUSED) {
			/* 记录当前帧curFrame*/
			curFrame = m_pBehaviorAction->GetCurrentTick();
			break;
		}
		leftRepeatTimes--;
	}
	
	//for (auto &thread : threads){
	//	thread.join();
	//}
	if (curState == PAUSED) {
		return;
	}
	curFrame = m_pBehaviorAction->GetCurrentTick();
	lastState = curState;
	curState = FINISHED;
}

void StepNode::resetStates(bool repeatSwitch){
	lastState = IDLE;
	curState = IDLE;
	curFrame = 0;
	if (repeatSwitch) {
		leftRepeatTimes = repeatTimes;
	}
	// SVIEW 复位动画
	if(m_pBehaviorAction)
	{
		m_pBehaviorAction->Rewind();
	}
	//if (m_pBehaviorAction && m_pAnimationPlayWnd)
	//{
	//	m_pAnimationPlayWnd->SendMessage(WM_ANIMATION_REWIND, (WPARAM)m_pBehaviorAction, NULL);
	//}
	// 附属节点复位
	for (int i = 0; i < attachedNodes.size(); ++i){
		attachedNodes[i]->resetStates(repeatSwitch); // ?
	}
	// 延时节点复位
	for (int i = 0; i < delayedNodes.size(); ++i){
		delayedNodes[i].first->resetStates(repeatSwitch); // ?
	}
}

// 播放delay节点, 独立线程，detach
// 考虑当前帧数
void StepNode::playDelayedNodes(){
	//for (int i = 0; i < delayedNodes.size(); ++i){
	//	AnimationNode* iNode = delayedNodes[i].first;
	//	if (curFrame < delayedNodes[i].second) { //iNode->getCurState() == IDLE && iNode->getLastState() == RUNNING?
	//		std::thread t(&StepNode::playDelayedThread, this, iNode, delayedNodes[i].second - curFrame, this);
	//		t.detach();
	//	}
	//	else if (iNode->getCurState() == PAUSED){ //
	//		std::thread t(&AnimationNode::resumeAnimation, iNode);
	//		t.detach();
	//	}
	//}
}

void StepNode::playDelayedThread(AnimationNode* i_node, TLframe i_t, AnimationNode* parentNode){
	/*if (i_t <= 0)
		return;
	while (i_t > 0 && parentNode->getCurState() != PAUSED) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		i_t--;
	}
	if (parentNode->getCurState() != PAUSED){
		i_node->playAnimation();
	}*/
}

CompositeNode::CompositeNode(){
	myNodeType = COMPOSITE_NODE;
}

CompositeNode::~CompositeNode(){
	for (int i = 0; i < nodeList.size(); ++i){
		if (nodeList[i] != NULL){
			delete nodeList[i];
			nodeList[i] = NULL;
		}
	}
}

void CompositeNode::resetStates(bool repeatSwitch){
	for (int i = 0; i < nodeList.size(); ++i){
		nodeList[i]->resetStates(true);
	}
	if (repeatSwitch) {
		leftRepeatTimes = repeatTimes;
	}
	lastState = curState;
	curState = IDLE;
}

SerialNode::SerialNode(){
	myNodeType = SERIAL_NODE;
	curIndex = 0;
}

SerialNode::~SerialNode(){}

/*
	(1) 如果已经播放过，重置动画状态
	(1) 如果curIndex >= 节点数目，则全部播完
	(2) 根据curIndex获得当前动画节点，如果状态为idle，开启独立thread,紧接设置t.join()
	(3) 如果状态为finished，curIndex++
	(4) 如果状态为paused, 跳出
	
*/
void SerialNode::playAnimation(){
	// 如果已经播放过，重置动画状态
	if (curState == FINISHED){
		resetStates(true);
		//leftRepeatTimes = repeatTimes;
		curIndex = 0;
	}
	//lastState = curState;
	//curState = RUNNING;
	while (leftRepeatTimes > 0){
		resetStates(false);
		curState = RUNNING; //当前状态被抹掉，重置
		//curIndex = 0;
		
		//while (curIndex < nodeList.size()){
		//	AnimationNode* curNode = nodeList[curIndex];
		//	AnimationState curNodeState = curNode->getCurState();
		//	if (curNodeState == IDLE){ // 当前动画节点空闲，开始播放动画
		//		std::thread t(&AnimationNode::playAnimation, curNode);
		//		t.join();
		//	}
		//	else if (curNodeState == FINISHED){ // 
		//		curIndex++;
		//	}
		//	else if (curNodeState == PAUSED){
		//		return;
		//	}
		//}	
		leftRepeatTimes--;
	}
	//播放完，设置finished
	if (curIndex >= nodeList.size() && leftRepeatTimes == 0){
		curState = FINISHED;
	}
}

void SerialNode::pauseAnimation(bool isGlobal){
	lastState = curState;
	curState = PAUSED;
	if (curIndex < nodeList.size()) {
		nodeList[curIndex]->pauseAnimation(isGlobal);
	}
}

void SerialNode::resumeAnimation(){
	if (curState != PAUSED || lastState != RUNNING)
		return;
	lastState = curState;
	curState = RUNNING;
	// 继续当前节点
	AnimationNode* curNode = nodeList[curIndex];
	curNode->resumeAnimation();
	if (curNode->getCurState() == FINISHED) {
		curIndex++;
	}
	
	// 继续后续节点
	while (leftRepeatTimes > 0){	
		//while (curIndex < nodeList.size()){
		//	AnimationNode* curNode = nodeList[curIndex];
		//	AnimationState curNodeState = curNode->getCurState();
		//	if (curNodeState == IDLE){ // 当前动画节点空闲，开始播放动画
		//		std::thread t(&AnimationNode::playAnimation, curNode);
		//		t.join();
		//	}
		//	else if (curNodeState == FINISHED){ // 
		//		curIndex++;
		//	}
		//	else if (curNodeState == PAUSED){
		//		return;
		//	}
		//}
		leftRepeatTimes--;
		if (leftRepeatTimes > 0) {
			resetStates(false);
			curState = RUNNING;
		}
	}

	// 检查末尾节点是否完结，从而设置该节点完结
	if (nodeList[nodeList.size() - 1]->getCurState() == FINISHED) {
		lastState = curState;
		curState = FINISHED;
	}
}

void SerialNode::resetStates(bool repeatSwitch) {
	for (int i = 0; i < nodeList.size(); ++i) {
		nodeList[i]->resetStates(true);
	}
	if (repeatSwitch) {
		leftRepeatTimes = repeatTimes;
	}
	curIndex = 0;
	lastState = curState;
	curState = IDLE;
}

ParallelNode::ParallelNode(){
	myNodeType = PARALLEL_NODE;
}

ParallelNode::~ParallelNode(){

}
/*
	(1) 创建n个并行的thread
	(2) 创建完后，全部join
	(3) 在播放和继续中，不可设置暂停状态。否则会重复设置laststate
*/
void ParallelNode::playAnimation(){
	if (curState == FINISHED){
		resetStates(true);
	}
	lastState = curState;
	curState = RUNNING;
	//while (leftRepeatTimes > 0){
	//	vector<std::thread> threads;
	//	for (int i = 0; i < nodeList.size(); ++i){
	//		AnimationNode* node = nodeList[i];
	//		threads.push_back(std::thread(&AnimationNode::playAnimation, node));
	//	}
	//	for (auto &t : threads){
	//		t.join();
	//	}
	//	// 检查子节点是否都完结，如果存在未完结子节点，则该节点不设置leftRepeatTimes--
	//	bool isAllFinished = true;
	//	for (int i = 0; i < nodeList.size(); ++i) {
	//		if (nodeList[i]->getCurState() != FINISHED) {
	//			isAllFinished = false;
	//			// 暂停，返回
	//			return;
	//		}
	//	}
	//	if(isAllFinished)
	//		leftRepeatTimes--;
	//}
	
	// 检查子节点是否都完结，如果存在未完结子节点，则该节点被暂停
	for (int i = 0; i < nodeList.size(); ++i) {
		if (nodeList[i]->getCurState() != FINISHED) {
			return;
		}
	}
	lastState = curState;
	curState = FINISHED;
}

void ParallelNode::pauseAnimation(bool isGlobal){
	for (int i = 0; i < nodeList.size(); ++i){
		nodeList[i]->pauseAnimation(isGlobal);
	}
	if (curState != PAUSED) {
		lastState = curState;
		curState = PAUSED;
	}
}

void ParallelNode::resumeAnimation(){
	if (curState != PAUSED || lastState != RUNNING)
		return;

	//while (leftRepeatTimes > 0){
	//	vector<std::thread> threads;
	//	for (int i = 0; i < nodeList.size(); ++i){
	//		threads.push_back(std::thread(&AnimationNode::resumeAnimation, nodeList[i]));
	//	}
	//	for (auto &thread : threads){
	//		thread.join();
	//	}
	//	// 检查子节点是否都完结，如果存在未完结子节点，则该节点不设置leftRepeatTimes--
	//	bool isAllFinished = true;
	//	for (int i = 0; i < nodeList.size(); ++i) {
	//		if (nodeList[i]->getCurState() != FINISHED) {
	//			isAllFinished = false;
	//			// 暂停，返回
	//			return;
	//		}
	//	}
	//	if (isAllFinished)
	//		leftRepeatTimes--;
	//}
	// 检查子节点是否都完结，如果存在未完结子节点，则该节点被暂停
	for (int i = 0; i < nodeList.size(); ++i) {
		if (nodeList[i]->getCurState() != FINISHED) {
			return;
		}
	}
	lastState = curState;
	curState = FINISHED;
}

DummyStepNode::DummyStepNode(){
	myNodeType = DUMMYSTEP_NODE;
	duration = 0;
}

DummyStepNode::DummyStepNode(TLframe i_duration){
	myNodeType = DUMMYSTEP_NODE;
	duration = i_duration;
	curFrame = 0;
}

DummyStepNode::~DummyStepNode(){}

/*
	timer计时，timer结束后设置动画状态为finished
*/
void DummyStepNode::playAnimation(){
	lastState = curState;
	curState = RUNNING;
	curFrame = 0;
	/*while (curState == RUNNING && curFrame < duration) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		curFrame++;
	}*/
	if (curState == PAUSED) {
		return;
	}
	lastState = curState;
	curState = FINISHED;
}

void DummyStepNode::pauseAnimation(bool isGlobal){
	lastState = curState;
	curState = PAUSED;
}

void DummyStepNode::resumeAnimation(){
	lastState = curState;
	curState = RUNNING;
	/*while (curState == RUNNING && curFrame < duration) {
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		curFrame++;
	}*/
	if (curState == PAUSED) {
		return;
	}
	lastState = curState;
	curState = FINISHED;
}

void DummyStepNode::waitDuration(TLframe t) {
	/*std::this_thread::sleep_for(std::chrono::milliseconds(t * 100));
	lastState = curState;
	curState = FINISHED;*/
}
