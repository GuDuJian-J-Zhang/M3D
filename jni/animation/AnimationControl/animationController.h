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

	// ���ö���״̬
	void reset();
	// ��ͣ������ĳһģ�͵Ķ���
	void pauseModel(ModelID i_mID);
	void resumeModel(ModelID i_mID);
	// ѭ�������������������ж����ڵ�
	void monitorRunningNodes();
	// Ѱ���������еĽڵ�
	void findRunningNodes(AnimationNode* i_topNode, vector<StepNode*>& o_nodeList);
	// ��ȡ���ڲ��Žڵ�
	inline vector<StepNode*> getCurPlayingNodes(){ return curPlayingNodes; }

	//���ò��Ŵ�����Ϣ
	void setPlayNotifyMsg();

public:
	AnimationState myState;

private:
	/// �����ڵ�
	AnimationNode* topNode;
	/// ��ǰ�����еĽڵ�
	vector<StepNode*> curPlayingNodes;
	/// ģ����ڵ��Ӧ��ϵ
	map<ModelID, vector<StepNode*>> modelAnimationMap;
	//std::thread monitorThread;
	bool isPlaying;
};

#endif