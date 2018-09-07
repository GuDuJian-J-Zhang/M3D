#ifndef AnimationControl_AnimationControlParser_H
#define AnimationControl_AnimationControlParser_H

#include "animationNode.h"

// 动画逻辑控制指令：

//		TLframe t = 10;
//		Node s = bind(PLCID:1\0, animationID:1\0);// 指定模型与动画步骤的对应关系。一个模型可以对应多段步骤。
//										// 模型用PLCID表示,动画用过程id和步骤id表示
//		Node r1 = serial[s1, 10,s3];// 紧凑串行关系, TLframe会生成dummystep
//		Node r2 = parallel[s1, s2, serial[s3, s4]];
//		Node r3 = serial[r1, r2];

//		serial[s1,s2] or parallel[s1,s2] 没有赋值到变量，不会加入animationGraph, 无意义。

//		attach(s1,s2); attach(s1, {s2,s3,..}) // 附属关系。s2附属到s1: s1停止、继续，s2亦停止、继续。
//		delay(s1, t, s2); // 步骤交叠关系。 s1执行t时间后执行s2
//		conditionalJump(s1, stateTransition, s2); // 跳转关系。根据运行状态跳转，当前s1, stateTransition ={starting, pausing, resuming, finishing}
//		adjustTimeFrame(s, t);// 调整时间区间
//		repeat(s, nTimes); // 设置重复次数


//		animationTopNode = ? // 最终输出
//		extras: onPausedDo(), onResumeDo() ...


// 问题解释
// r1 = serial[s1,s2]; r2 = serial[s1,s3]; r3 = parallel[r1,r2]; // s1会同时执行两次，需进行重复检查
// parallel to serial : 等待所有parallel都完成，再继续
// 单位动画：装备与工件不做约束，装备动画不可与其他装备做单位动画。

// 关键字keyword
enum Keyword{COMMENT, DEFINE, NODE, TLFRAME, BIND, SERIAL, PARALLEL, ATTACH,
			MODELID, ANIMID, DELAY, ADJTL, REPEAT, TOPNODE, CONDJUMP, UNKNOWN};

// 动画逻辑控制指令解析器
class SA_API AnimationControlParser{
public:
	AnimationControlParser();
	~AnimationControlParser();
	/// 初始化parser, 创建关键字表
	void initParser(); 
	/// 解析入口函数，输入文件地址
	void parseAnimationInstructions(string filePath);
	/// 解析单行指令
	void parseInstruction(string i_instruction);
	/// 解析定义类指令
	void parseDefinition(string i_instruction);
	/// 解析时间定义
	void parseTimeFrame(string i_instruction);
	/// 解析节点表达式
	AnimationNode* parseNodeExpression(string i_instruction);
	/// 解析串行表达式
	SerialNode* parseSerial(string i_instruction);
	/// 解析并行表达式
	ParallelNode* parseParallel(string i_instruction);
	/// 解析模型动画绑定表达式
	StepNode* parseModelAnimationBinding(string i_instruction);
	/// 解析附属指令
	void parseAttachment(string i_instruction);
	/// 解析延迟指令
	void parseDelay(string i_instruction);
	/// 解析时间轴调整指令
	void parseTimeFrameAdjustment(string i_instruction);
	/// 解析重复播放指令
	void parseRepeat(string i_instruction);
	/// 解析条件调整指令
	void parseConditionalJump(string i_instruction);

	// 获取顶级动画节点
	inline AnimationNode* getTopNode(){return topNode;}
	// 获取模型与动画对应关系
	inline map<ModelID, vector<StepNode*>> getModelAnimationMap(){return modelAnimationMap;}

	/*! This method Sets the pointer to the CSimulationAnimationManager object associated with this animationStep. */
	void SetSimulationAnimationManager(NS_SimulationAnimation::CSimulationAnimationManager*pSA) { m_pSA = pSA; }

	AnimationID parseAnimationID(string i_str);
	ModelID parseModelID(string i_str);

	// 功用函数
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
	/// 获取指令关键字类型
	Keyword getInstructionKeyword(string i_instruction);
	AnimationNode* getNodeFromVar(string i_varStr);

	map<Keyword, string> keywordMap;//关键字map
	map<ModelID, vector<StepNode*>> modelAnimationMap;
	map<string, AnimationNode*> varNodes; // 节点变量map
	map<string, TLframe> varTLframes;
	vector<AnimationNode*> aniNodes; // 管理new出来的动画节点，避免野指针
	AnimationNode* topNode;
	//动画信息管理器
	NS_SimulationAnimation::CSimulationAnimationManager * m_pSA;
};

#endif