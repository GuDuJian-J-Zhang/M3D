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

	//// ���Ÿ����ڵ�,ÿ���ڵ��Ӧһ���̣߳����join
	//vector<std::thread> threads;
	//for (int i = 0; i < attachedNodes.size(); ++i){
	//	AnimationNode* node = attachedNodes[i];
	//	//threads.push_back(std::thread(&AnimationNode::playAnimation, node));
	//}
	////������ʱ�ڵ�
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
	//		/* ��¼��ǰ֡curFrame*/
	//		curFrame = m_pBehaviorAction->GetCurrentTick();
	//		break;
	//	}
	//	leftRepeatTimes--;
	//}
	//// Ŀǰδ�������ڵ���ýڵ�ʱ�䳤�Ȳ�һ���������ȴ������ڵ���ᣬ�ýڵ����ᡣ
	//for (auto &t : threads){
	//	t.join();
	//}
	//if (curState == PAUSED){ // �����������߳�join֮��return
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
		// ��ͣ��ʱ�ڵ�
		for (int i = 0; i < delayedNodes.size(); ++i) {
			delayedNodes[i].first->pauseAnimation(isGlobal);
		}
	}
	//���û����еĽڵ���Ա���ͣ
	if (curState == IDLE || curState == RUNNING){
		lastState = curState;
		curState = PAUSED;
		// SVIEW ��ͣ����
		m_pBehaviorAction->Stop();
		//if (m_pAnimationPlayWnd)
		//{
		//	m_pAnimationPlayWnd->PostMessage(WM_ANIMATION_STOP, (WPARAM)m_pBehaviorAction, NULL);
		//}

		// ��ͣ�����ڵ�
		for (int i = 0; i < attachedNodes.size(); ++i){
			attachedNodes[i]->pauseAnimation(isGlobal);
		}
	}
}

// ����������������(blocking),ֱ�����и�������join���˳�
// �ӵ�ǰ֡curFrame��������, non-blocking; ���blocking,���������߳�
void StepNode::resumeAnimation(){
	if (curState != PAUSED || lastState != RUNNING) {// ��ǰ����ͣ �� ��ͣǰ״̬Ϊ����, ֱ�ӷ���
		// ������ʱ�ڵ�
		playDelayedNodes();
		return;
	} 

	if (!m_pBehaviorAction)
	{
		return;
	}

	lastState = curState;
	curState = RUNNING;
	// ������ʱ�ڵ�
	playDelayedNodes();
	// ���������ڵ�
	//vector<std::thread> threads;
	for (int i = 0; i < attachedNodes.size(); ++i) {
	//	threads.push_back(std::thread(&AnimationNode::resumeAnimation, attachedNodes[i]));
	}

	// sview ��������
	while (leftRepeatTimes > 0) {
		m_pBehaviorAction->Continue();
	//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//if (m_pAnimationPlayWnd)
		//{
		//	// sendMessage��Ϣ��������ΪpostMessage
		//	m_pAnimationPlayWnd->PostMessage(WM_ANIMATION_CONTINUE, (WPARAM)m_pBehaviorAction, NULL);
		//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
		//}
		
		while (m_pBehaviorAction->IsPlaying() && curState != PAUSED) {
	//		std::this_thread::sleep_for(std::chrono::milliseconds(30));
		}
		if (curState == PAUSED) {
			/* ��¼��ǰ֡curFrame*/
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
	// SVIEW ��λ����
	if(m_pBehaviorAction)
	{
		m_pBehaviorAction->Rewind();
	}
	//if (m_pBehaviorAction && m_pAnimationPlayWnd)
	//{
	//	m_pAnimationPlayWnd->SendMessage(WM_ANIMATION_REWIND, (WPARAM)m_pBehaviorAction, NULL);
	//}
	// �����ڵ㸴λ
	for (int i = 0; i < attachedNodes.size(); ++i){
		attachedNodes[i]->resetStates(repeatSwitch); // ?
	}
	// ��ʱ�ڵ㸴λ
	for (int i = 0; i < delayedNodes.size(); ++i){
		delayedNodes[i].first->resetStates(repeatSwitch); // ?
	}
}

// ����delay�ڵ�, �����̣߳�detach
// ���ǵ�ǰ֡��
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
	(1) ����Ѿ����Ź������ö���״̬
	(1) ���curIndex >= �ڵ���Ŀ����ȫ������
	(2) ����curIndex��õ�ǰ�����ڵ㣬���״̬Ϊidle����������thread,��������t.join()
	(3) ���״̬Ϊfinished��curIndex++
	(4) ���״̬Ϊpaused, ����
	
*/
void SerialNode::playAnimation(){
	// ����Ѿ����Ź������ö���״̬
	if (curState == FINISHED){
		resetStates(true);
		//leftRepeatTimes = repeatTimes;
		curIndex = 0;
	}
	//lastState = curState;
	//curState = RUNNING;
	while (leftRepeatTimes > 0){
		resetStates(false);
		curState = RUNNING; //��ǰ״̬��Ĩ��������
		//curIndex = 0;
		
		//while (curIndex < nodeList.size()){
		//	AnimationNode* curNode = nodeList[curIndex];
		//	AnimationState curNodeState = curNode->getCurState();
		//	if (curNodeState == IDLE){ // ��ǰ�����ڵ���У���ʼ���Ŷ���
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
	//�����꣬����finished
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
	// ������ǰ�ڵ�
	AnimationNode* curNode = nodeList[curIndex];
	curNode->resumeAnimation();
	if (curNode->getCurState() == FINISHED) {
		curIndex++;
	}
	
	// ���������ڵ�
	while (leftRepeatTimes > 0){	
		//while (curIndex < nodeList.size()){
		//	AnimationNode* curNode = nodeList[curIndex];
		//	AnimationState curNodeState = curNode->getCurState();
		//	if (curNodeState == IDLE){ // ��ǰ�����ڵ���У���ʼ���Ŷ���
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

	// ���ĩβ�ڵ��Ƿ���ᣬ�Ӷ����øýڵ����
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
	(1) ����n�����е�thread
	(2) �������ȫ��join
	(3) �ڲ��źͼ����У�����������ͣ״̬��������ظ�����laststate
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
	//	// ����ӽڵ��Ƿ���ᣬ�������δ����ӽڵ㣬��ýڵ㲻����leftRepeatTimes--
	//	bool isAllFinished = true;
	//	for (int i = 0; i < nodeList.size(); ++i) {
	//		if (nodeList[i]->getCurState() != FINISHED) {
	//			isAllFinished = false;
	//			// ��ͣ������
	//			return;
	//		}
	//	}
	//	if(isAllFinished)
	//		leftRepeatTimes--;
	//}
	
	// ����ӽڵ��Ƿ���ᣬ�������δ����ӽڵ㣬��ýڵ㱻��ͣ
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
	//	// ����ӽڵ��Ƿ���ᣬ�������δ����ӽڵ㣬��ýڵ㲻����leftRepeatTimes--
	//	bool isAllFinished = true;
	//	for (int i = 0; i < nodeList.size(); ++i) {
	//		if (nodeList[i]->getCurState() != FINISHED) {
	//			isAllFinished = false;
	//			// ��ͣ������
	//			return;
	//		}
	//	}
	//	if (isAllFinished)
	//		leftRepeatTimes--;
	//}
	// ����ӽڵ��Ƿ���ᣬ�������δ����ӽڵ㣬��ýڵ㱻��ͣ
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
	timer��ʱ��timer���������ö���״̬Ϊfinished
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
