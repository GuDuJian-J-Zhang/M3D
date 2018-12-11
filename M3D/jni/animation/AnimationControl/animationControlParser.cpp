#include "stdafx.h"
#include "animationControlParser.h"
#include "animationNode.h"
#include <sstream>
#include <iostream>
#include <fstream>

AnimationControlParser::AnimationControlParser(){
	topNode = NULL;
	initParser();
	m_pSA = NULL;
}

AnimationControlParser::~AnimationControlParser(){
	for (int i = 0; i < aniNodes.size(); ++i) {
		if (aniNodes[i] != NULL) {
			delete aniNodes[i];
			aniNodes[i] = NULL;
		}
	}
}

void AnimationControlParser::initParser(){
	keywordMap.insert(pair<Keyword,string>(COMMENT, "//"));
	keywordMap.insert(pair<Keyword,string>(DEFINE, "def"));
	keywordMap.insert(pair<Keyword,string>(TLFRAME, "TLframe"));
	keywordMap.insert(pair<Keyword,string>(NODE, "Node"));
	keywordMap.insert(pair<Keyword,string>(BIND, "bind"));
	keywordMap.insert(pair<Keyword,string>(SERIAL, "serial"));
	keywordMap.insert(pair<Keyword,string>(PARALLEL, "parallel"));
	keywordMap.insert(pair<Keyword,string>(ATTACH, "attach"));
	keywordMap.insert(pair<Keyword,string>(DELAY, "delay"));
	keywordMap.insert(pair<Keyword,string>(ADJTL, "adjTimeFrame"));
	keywordMap.insert(pair<Keyword,string>(REPEAT, "repeat"));
	keywordMap.insert(pair<Keyword,string>(CONDJUMP, "condJump"));
	keywordMap.insert(pair<Keyword,string>(TOPNODE, "topNode"));
	keywordMap.insert(pair<Keyword,string>(MODELID, "PLCID:"));
	keywordMap.insert(pair<Keyword,string>(ANIMID, "AnimID:"));
}

void AnimationControlParser::parseAnimationInstructions(string filePath){
	ifstream infile;
	string curLine;
	infile.open(filePath);
	if(!infile.is_open())
		return;
	while(infile.good()){
		getline(infile, curLine);
		parseInstruction(curLine);
	}
	// 如果顶级节点为空，取最后一个变量为顶级节点
	if (topNode == NULL) {
		topNode = (*varNodes.end()).second;
	}
	infile.close();
}

Keyword AnimationControlParser::getInstructionKeyword(string i_instruction){
	 
	map<Keyword, string>::iterator itor = keywordMap.begin();
	Keyword result = UNKNOWN;
	string::size_type resultP = i_instruction.find((*itor).second);
	while(itor != keywordMap.end()){
		string::size_type temp = i_instruction.find((*itor).second);
		if(temp < resultP){
			resultP = temp;
			result = (*itor).first;
		}
		itor++;
	}
	return result;
}

AnimationNode* AnimationControlParser::getNodeFromVar(string i_varStr){
	if (varNodes.find(i_varStr) != varNodes.end()) {
		return varNodes[i_varStr];
	}
	else {
		return NULL;
	}
}


void AnimationControlParser::parseInstruction(string i_instruction){
	Keyword k = getInstructionKeyword(i_instruction);
	switch(k) {
	case COMMENT:
		break;
	case NODE:
	case TLFRAME:
	case TOPNODE:
		parseDefinition(i_instruction);
		break;
	case ADJTL:
		parseTimeFrameAdjustment(i_instruction);
		break;
	case ATTACH:
		parseAttachment(i_instruction);
		break;
	case DELAY:
		parseDelay(i_instruction);
		break;
	case REPEAT:
		parseRepeat(i_instruction);
		break;
	case CONDJUMP:
		parseConditionalJump(i_instruction);
		break;
	default:
		break;
	}
}

void AnimationControlParser::parseDefinition(string i_instruction){
	string::size_type equalDelimiter = i_instruction.find("=");
	string varPart = i_instruction.substr(0, equalDelimiter);
	string exprPart  = trimSpace(i_instruction.substr(equalDelimiter+1, i_instruction.size()-equalDelimiter-1));
	Keyword k_var = getInstructionKeyword(varPart);
	if(k_var == NODE){
		string::size_type varDelimiter = varPart.find(keywordMap[NODE]) + keywordMap[NODE].size();
		string varStr = trimSpace(varPart.substr(varDelimiter, varPart.size()-varDelimiter));
		AnimationNode* node = parseNodeExpression(exprPart);
		varNodes.insert(pair<string, AnimationNode*>(varStr, node));
	}else if(k_var == TLFRAME){
		string::size_type varDelimiter = varPart.find(keywordMap[TLFRAME]) + keywordMap[TLFRAME].size();
		string varStr = trimSpace(varPart.substr(varDelimiter, varPart.size()-varDelimiter));
		string::size_type semicolon = exprPart.find(";");
		string frameStr = exprPart.substr(0, semicolon);
		TLframe frameValue = StrToFloat(frameStr);
		varTLframes.insert(pair<string, TLframe>(varStr, frameValue));
	}else if(k_var == TOPNODE){
		topNode = parseNodeExpression(exprPart);
		varNodes.insert(pair<string, AnimationNode*>(keywordMap[TOPNODE], topNode));
	}
}

AnimationNode* AnimationControlParser::parseNodeExpression(string i_instruction){
	AnimationNode* result = NULL;
	Keyword k = getInstructionKeyword(i_instruction);
	switch(k){
	case SERIAL:
		result = parseSerial(i_instruction);
		aniNodes.push_back(result);
		break;
	case PARALLEL:
		result = parseParallel(i_instruction);
		aniNodes.push_back(result);
		break;
	case BIND:
		result = parseModelAnimationBinding(i_instruction);
		aniNodes.push_back(result);
		break;
	default:
		string::size_type semicolon = i_instruction.find(";");
		string varStr = trimSpace(i_instruction.substr(0, semicolon));
		if (varNodes.find(varStr) != varNodes.end()) {
			result = varNodes[varStr];
		}
		break;
	}
	return result;
}

/*
	(1) 检查关键字
	(2) 如果不是末尾，获取括号内下一部份
	(3) 如果是节点变量或时间变量，从map中获取指针;如果是数字，构建dummynode;如果是expr,解析
*/
SerialNode* AnimationControlParser::parseSerial(string i_instruction){
	SerialNode* result = new SerialNode();
	Keyword k = getInstructionKeyword(i_instruction);
	if(k != SERIAL)
		return NULL;
	string::size_type leftBracket = i_instruction.find_first_of("(");
	string linepart = i_instruction.substr(leftBracket+1, i_instruction.size()-leftBracket-1);

	while(!linepart.empty()){
		string strpart = getNextPartInsideBracket(linepart);
		Keyword kPart = getInstructionKeyword(strpart);
		if(kPart == UNKNOWN){
			map<string, AnimationNode*>::iterator itor = varNodes.find(strpart);
			if(itor != varNodes.end()){
				AnimationNode* node = varNodes[strpart];
				result->addNode(node);
			}
			map<string, TLframe>::iterator itor2 = varTLframes.find(strpart);
			if(itor2 != varTLframes.end()){
				DummyStepNode* node = new DummyStepNode(itor2->second);
				result->addNode(node);
			}
			if(isNumeric(strpart)){
				TLframe t = (int)StrToFloat(strpart);
				DummyStepNode* node = new DummyStepNode(t);
				result->addNode(node);
			}
		}else if(kPart == PARALLEL){
			ParallelNode* node = parseParallel(strpart);
			result->addNode(node);
		}else if(kPart == SERIAL){
			SerialNode* node = parseSerial(strpart);
			result->addNode(node);
		}
	}
	return result;
}

//与解析serial类似
ParallelNode* AnimationControlParser::parseParallel(string i_instruction){
	ParallelNode* result = new ParallelNode();
	Keyword k = getInstructionKeyword(i_instruction);
	if(k != PARALLEL)
		return NULL;
	string::size_type leftBracket = i_instruction.find_first_of("(");
	string linepart = i_instruction.substr(leftBracket+1, i_instruction.size()-leftBracket-1);

	while(!linepart.empty()){
		string strpart = getNextPartInsideBracket(linepart);
		Keyword kPart = getInstructionKeyword(strpart);
		if(kPart == UNKNOWN){
			map<string, AnimationNode*>::iterator itor = varNodes.find(strpart);
			if(itor != varNodes.end()){
				AnimationNode* node = varNodes[strpart];
				result->addNode(node);
			}
			map<string, TLframe>::iterator itor2 = varTLframes.find(strpart);
			if(itor2 != varTLframes.end()){
				DummyStepNode* node = new DummyStepNode(itor2->second);
				result->addNode(node);
			}
			if(isNumeric(strpart)){
				TLframe t = (int)StrToFloat(strpart);
				DummyStepNode* node = new DummyStepNode(t);
				result->addNode(node);
			}
		}else if(kPart == PARALLEL){
			ParallelNode* node = parseParallel(strpart);
			result->addNode(node);
		}else if(kPart == SERIAL){
			SerialNode* node = parseSerial(strpart);
			result->addNode(node);
		}
	}
	return result;
}

StepNode* AnimationControlParser::parseModelAnimationBinding(string i_instruction){
	StepNode* result = NULL;
	Keyword k = getInstructionKeyword(i_instruction);
	if(k != BIND)
		return NULL;
	string::size_type left_bracket = i_instruction.find("(");
	string::size_type comma = i_instruction.find(",");
	string::size_type right_bracket = i_instruction.find(")");

	string modelIDStr = i_instruction.substr(left_bracket+1, comma-left_bracket-1);
	string animIDStr = i_instruction.substr(comma+1, right_bracket-comma-1);
	//ModelID* p_mID = parseModelID(modelIDStr);
	ModelID mID = parseModelID(modelIDStr);
	AnimationID aID = parseAnimationID(animIDStr);
	result = new StepNode(mID, aID,m_pSA);
	if (modelAnimationMap.find(mID) == modelAnimationMap.end()){
		vector<StepNode*> anims;
		anims.push_back(result);
		modelAnimationMap.insert(pair<ModelID, vector<StepNode*>>(mID, anims));
	}
	else{
		modelAnimationMap[mID].push_back(result);
	}
	
	return result;
}

// attach(s1, s2); attach(s1, {s2,s3,s4});
/*
	(1) 检查关键字
	(2) 获取被附属对象
	(3) 获取附属对象
*/
void AnimationControlParser::parseAttachment(string i_instruction){
	Keyword k = getInstructionKeyword(i_instruction);
	if(k != ATTACH)
		return;
	string::size_type leftBracket = i_instruction.find("(");
	string::size_type rightBracket = i_instruction.find(")");
	string::size_type comma = i_instruction.find_first_of(",");
	string mainNodeStr = trimSpace(i_instruction.substr(leftBracket+1, comma-leftBracket-1));
	string attachNodeStr = trimSpace(i_instruction.substr(comma + 1, rightBracket - comma - 1));
	if(varNodes.find(mainNodeStr) != varNodes.end()){
		AnimationNode* mainNode = varNodes[mainNodeStr];
		if(mainNode->getNodeType() == STEP_NODE){
			StepNode* mNode = static_cast<StepNode*>(mainNode);
			if(varNodes.find(attachNodeStr) != varNodes.end()){
				AnimationNode* varNode = varNodes[attachNodeStr];
				mNode->addAttachedNode(varNode);
				/*if (varNode->getNodeType() == STEP_NODE){
					StepNode* attachNode = static_cast<StepNode*>(varNode);
					mNode->addAttachedNode(attachNode);
				}*/
			}
		}
	}
}

// 目前只有stepnode后才可以跟延时节点
void AnimationControlParser::parseDelay(string i_instruction){
	Keyword k = getInstructionKeyword(i_instruction);
	if(k != DELAY)
		return;
	string::size_type leftBrackt = i_instruction.find_first_of("(");
	string::size_type comma1 = i_instruction.find(",");
	string::size_type comma2 = i_instruction.find(",", comma1+1);
	string::size_type rightBracket = i_instruction.find_first_of(")");

	string varStr1 = trimSpace(i_instruction.substr(leftBrackt+1, comma1-leftBrackt-1));
	string countStr = trimSpace(i_instruction.substr(comma1+1, comma2-comma1-1));
	string varStr2 = trimSpace(i_instruction.substr(comma2+1, rightBracket-comma2-1));

	if(varNodes.find(varStr1) != varNodes.end() && varNodes.find(varStr2) != varNodes.end()){
		if(varNodes[varStr1]->getNodeType() == STEP_NODE){ 
			StepNode* mNode = static_cast<StepNode*>(varNodes[varStr1]);
			TLframe t = (int)StrToFloat(countStr);
			mNode->addDelayedNode(varNodes[varStr2], t);
		}
	}
}

void AnimationControlParser::parseRepeat(string i_instruction){
	Keyword k = getInstructionKeyword(i_instruction);
	if(k != REPEAT)
		return;
	string::size_type leftBrackt = i_instruction.find_first_of("(");
	string::size_type comma = i_instruction.find(",");
	string::size_type rightBracket = i_instruction.find_first_of(")");

	string varStr = i_instruction.substr(leftBrackt+1, comma-leftBrackt-1);
	string countStr = trimSpace(i_instruction.substr(comma+1, rightBracket-comma-1));
	int nTimes = (int)StrToFloat(countStr);
	getNodeFromVar(varStr)->setRepeatTimes(nTimes);
}
/*
	设置duration
*/
void AnimationControlParser::parseTimeFrameAdjustment(string i_instruction){
	Keyword k = getInstructionKeyword(i_instruction);
	if(k != ADJTL)
		return;
	string::size_type leftBrackt = i_instruction.find_first_of("(");
	string::size_type comma = i_instruction.find(",");
	string::size_type rightBracket = i_instruction.find_first_of(")");

	string varStr = i_instruction.substr(leftBrackt+1, comma-leftBrackt-1);
	string durationStr = trimSpace(i_instruction.substr(comma+1, rightBracket-comma-1));
	int t = (int)StrToFloat(durationStr);
	if(varNodes.find(varStr) != varNodes.end()){
		varNodes[varStr]->setDuration(t);
	}
}

void AnimationControlParser::parseConditionalJump(string i_instruction){

}

/*
	(1) 获取关键字"PLCID:"并检查
	(2) 获取1\0\1字符串
	(3) 找"\",分割字符串，循环依次加入，直到结束
*/
ModelID AnimationControlParser::parseModelID(string i_str){
	ModelID mID;
	Keyword k = getInstructionKeyword(i_str);
	if (k != MODELID)
		return mID;
	
	string modelIDKeyword = keywordMap[MODELID];
	string::size_type delimiter = i_str.find(modelIDKeyword) + modelIDKeyword.size();

	string plcStr = i_str.substr(delimiter, i_str.size() - delimiter);
	string::size_type slash = plcStr.find("|");

	while (slash != string::npos){
		string varStr = plcStr.substr(0, slash);
		int num = (int)StrToFloat(varStr);
		mID.addPathItem(num);
		plcStr = plcStr.substr(slash + 1, plcStr.size() - slash - 1);
		slash = plcStr.find("|");
	}
	// 最后一个
	if (!plcStr.empty()){
		int num = (int)StrToFloat(plcStr);
		mID.addPathItem(num);
	}
	return mID;
}

/*
	(1) 获取关键字"AnimID:"并检查
	(2) 获取1\0字符串
	(3) 找"\",分割字符串，获得procID,stepID
*/
AnimationID AnimationControlParser::parseAnimationID(string i_str){
	AnimationID aID;
	Keyword k = getInstructionKeyword(i_str);
	if(k != ANIMID)
		return aID;

	string animIDKeyword = keywordMap[ANIMID];
	string::size_type delimiter = i_str.find(animIDKeyword) + animIDKeyword.size();
	string idStr = i_str.substr(delimiter, i_str.size() - delimiter);
	string::size_type slash = idStr.find("|");
	string procStr = idStr.substr(0, slash);
	string stepStr = idStr.substr(slash+1, idStr.size()-slash-1);
	int procID = (int)StrToFloat(procStr);
	int stepID = (int)StrToFloat(stepStr);
	//AnimationID* p_aID = new AnimationID(procID, stepID);
	aID.processID = procID;
	aID.stepID = stepID;
	return aID;
}

string AnimationControlParser::trimSpace(string io_str){
	if (io_str.empty())
	{
		return io_str;
	}
	io_str.erase(0, io_str.find_first_not_of(" ")); // 消除前空格
	io_str.erase(0, io_str.find_first_not_of("\t")); // 消除前tab
	io_str.erase(io_str.find_last_not_of(" ") + 1);  // 消除后空格
	io_str.erase(io_str.find_last_not_of("\t") + 1); // 消除后tab
	return io_str;
}


// 遍历string, 遇到"(" diff++,遇到")" diff--; diff ==0 时 break;
string::size_type AnimationControlParser::getRightBracket(string i_str, string::size_type left){
	int diff = 1;	
	int cur = left+1;
	for( ;cur<i_str.size(); ++cur){
		if(i_str[cur] == '(')
			++diff;
		if(i_str[cur] == ')')
			--diff;
		if(diff == 0)
			break;
	}
	return cur;
}

/*
	(1)找comma 和 leftbracket
	(2)比较大小，找到小的
	(3)如果comma小，分割字符串；
	(4)如果leftbracket小，找到rightbracket,分割
*/ 
string AnimationControlParser::getNextPartInsideBracket(string &linepart){
	linepart = trimSpace(linepart);
	if(linepart == "")
		return linepart;
	string curPart;
	string::size_type comma = linepart.find(",");
	string::size_type leftbracket = linepart.find("(");
	
	// 处理尾部
	if(comma == string::npos && leftbracket == string::npos){
		string::size_type rightbracket = linepart.find(")");
		curPart = linepart.substr(0, rightbracket);
		linepart = "";
		return curPart;
	}
	if(comma < leftbracket){
		curPart = linepart.substr(0, comma);
		linepart = linepart.substr(comma+1, linepart.size()-comma-1);
	}else{
		string::size_type rightbracket = getRightBracket(linepart, leftbracket+1);
		comma = rightbracket + 1; 
		curPart = linepart.substr(0, comma);
		linepart = linepart.substr(comma+1, linepart.size()-comma-1);
	}


	return curPart;
}

bool AnimationControlParser::isNumeric(string i_str){
	if(i_str.empty())
		return false;
	string base = "0123456789";
	return i_str.find_first_not_of(base) == string::npos;
}
