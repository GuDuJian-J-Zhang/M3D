#ifndef AnimationControl_AnimationControlParser_H
#define AnimationControl_AnimationControlParser_H

#include "animationNode.h"

// �����߼�����ָ�

//		TLframe t = 10;
//		Node s = bind(PLCID:1\0, animationID:1\0);// ָ��ģ���붯������Ķ�Ӧ��ϵ��һ��ģ�Ϳ��Զ�Ӧ��β��衣
//										// ģ����PLCID��ʾ,�����ù���id�Ͳ���id��ʾ
//		Node r1 = serial[s1, 10,s3];// ���մ��й�ϵ, TLframe������dummystep
//		Node r2 = parallel[s1, s2, serial[s3, s4]];
//		Node r3 = serial[r1, r2];

//		serial[s1,s2] or parallel[s1,s2] û�и�ֵ���������������animationGraph, �����塣

//		attach(s1,s2); attach(s1, {s2,s3,..}) // ������ϵ��s2������s1: s1ֹͣ��������s2��ֹͣ��������
//		delay(s1, t, s2); // ���轻����ϵ�� s1ִ��tʱ���ִ��s2
//		conditionalJump(s1, stateTransition, s2); // ��ת��ϵ����������״̬��ת����ǰs1, stateTransition ={starting, pausing, resuming, finishing}
//		adjustTimeFrame(s, t);// ����ʱ������
//		repeat(s, nTimes); // �����ظ�����


//		animationTopNode = ? // �������
//		extras: onPausedDo(), onResumeDo() ...


// �������
// r1 = serial[s1,s2]; r2 = serial[s1,s3]; r3 = parallel[r1,r2]; // s1��ͬʱִ�����Σ�������ظ����
// parallel to serial : �ȴ�����parallel����ɣ��ټ���
// ��λ������װ���빤������Լ����װ����������������װ������λ������

// �ؼ���keyword
enum Keyword{COMMENT, DEFINE, NODE, TLFRAME, BIND, SERIAL, PARALLEL, ATTACH,
			MODELID, ANIMID, DELAY, ADJTL, REPEAT, TOPNODE, CONDJUMP, UNKNOWN};

// �����߼�����ָ�������
class SA_API AnimationControlParser{
public:
	AnimationControlParser();
	~AnimationControlParser();
	/// ��ʼ��parser, �����ؼ��ֱ�
	void initParser(); 
	/// ������ں����������ļ���ַ
	void parseAnimationInstructions(string filePath);
	/// ��������ָ��
	void parseInstruction(string i_instruction);
	/// ����������ָ��
	void parseDefinition(string i_instruction);
	/// ����ʱ�䶨��
	void parseTimeFrame(string i_instruction);
	/// �����ڵ���ʽ
	AnimationNode* parseNodeExpression(string i_instruction);
	/// �������б��ʽ
	SerialNode* parseSerial(string i_instruction);
	/// �������б��ʽ
	ParallelNode* parseParallel(string i_instruction);
	/// ����ģ�Ͷ����󶨱��ʽ
	StepNode* parseModelAnimationBinding(string i_instruction);
	/// ��������ָ��
	void parseAttachment(string i_instruction);
	/// �����ӳ�ָ��
	void parseDelay(string i_instruction);
	/// ����ʱ�������ָ��
	void parseTimeFrameAdjustment(string i_instruction);
	/// �����ظ�����ָ��
	void parseRepeat(string i_instruction);
	/// ������������ָ��
	void parseConditionalJump(string i_instruction);

	// ��ȡ���������ڵ�
	inline AnimationNode* getTopNode(){return topNode;}
	// ��ȡģ���붯����Ӧ��ϵ
	inline map<ModelID, vector<StepNode*>> getModelAnimationMap(){return modelAnimationMap;}

	/*! This method Sets the pointer to the CSimulationAnimationManager object associated with this animationStep. */
	void SetSimulationAnimationManager(NS_SimulationAnimation::CSimulationAnimationManager*pSA) { m_pSA = pSA; }

	AnimationID parseAnimationID(string i_str);
	ModelID parseModelID(string i_str);

	// ���ú���
	static string trimSpace(string i_str);
	inline static float StrToFloat(const string i_str){
		return atof(i_str.c_str());
	}
	inline static string floatToStr(float f){
		ostringstream buffer;
		buffer << f;
		return buffer.str();
	}

	static string::size_type getRightBracket(string i_str, string::size_type left);
	static string getNextPartInsideBracket(string& i_str);
	static bool isNumeric(string i_str);

private:
	/// ��ȡָ��ؼ�������
	Keyword getInstructionKeyword(string i_instruction);
	AnimationNode* getNodeFromVar(string i_varStr);

	map<Keyword, string> keywordMap;//�ؼ���map
	map<ModelID, vector<StepNode*>> modelAnimationMap;
	map<string, AnimationNode*> varNodes; // �ڵ����map
	map<string, TLframe> varTLframes;
	vector<AnimationNode*> aniNodes; // ����new�����Ķ����ڵ㣬����Ұָ��
	AnimationNode* topNode;
	//������Ϣ������
	NS_SimulationAnimation::CSimulationAnimationManager * m_pSA;
};

#endif