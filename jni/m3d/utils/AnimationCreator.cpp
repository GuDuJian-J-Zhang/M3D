#include "m3d/utils/AnimationCreator.h"
#include "m3d/model/ModelView.h"
#include "m3d/SceneManager.h"
#include "m3d/scene/SceneNode.h"
#include "m3d/scene/GroupNode.h"

namespace M3D
{

void Animationcreator::MatrixInfoSlerp(MovedMatrixInfo& fromState,
		MovedMatrixInfo& toState, MovedMatrixInfo& midStates, float interPos)
{
//	midStates.m_MovedMatrix = Matrix::MatrixSlerp(fromState.m_MovedMatrix,
//			toState.m_MovedMatrix, interPos);
//	midStates.m_PlcMatrx = Matrix::MatrixSlerp(fromState.m_PlcMatrx, toState.m_PlcMatrx,
//			interPos);
}

void Animationcreator::CameraMatrixSlerp(Matrix4& beginState, Matrix4& endState,
		Matrix4& midStates, float interPos)
{
//	float* mm = beginState.GetData();
//
//	Quaternion rotationB;
//	Vector3 positionB;
//	Vector3 rotateCenterB;
//
//	float scaleFactorB;
//
//	rotationB.axis.x = mm[0];
//	rotationB.axis.y = mm[1];
//	rotationB.axis.z = mm[2];
//	rotationB.angle = mm[3];
//
//	positionB.x = mm[4 + 0];
//	positionB.y = mm[4 + 1];
//	positionB.z = mm[4 + 2];
//
//	scaleFactorB = mm[8 + 0];
//	//	 0;						  	 =mm[2][1] ;
//	//	 0;						  	 =mm[2][2] ;
//
//	rotateCenterB.x = mm[12 + 0];
//	rotateCenterB.y = mm[12 + 1];
//	rotateCenterB.z = mm[12 + 2];
//
//	mm = endState.GetData();
//
//	Quaternion rotationE;
//	Vector3 positionE;
//	Vector3 rotateCenterE;
//
//	float scaleFactorE;
//
//	rotationE.axis.x = mm[0];
//	rotationE.axis.y = mm[1];
//	rotationE.axis.z = mm[2];
//	rotationE.angle = mm[3];
//
//	positionE.x = mm[4 + 0];
//	positionE.y = mm[4 + 1];
//	positionE.z = mm[4 + 2];
//
//	scaleFactorE = mm[8 + 0];
//	//	 0;						  	 =mm[2][1] ;
//	//	 0;						  	 =mm[2][2] ;
//
//	rotateCenterE.x = mm[12 + 0];
//	rotateCenterE.y = mm[12 + 1];
//	rotateCenterE.z = mm[12 + 2];
//
//	Quaternion rotationM;
//	Vector3 positionM;
//	Vector3 rotateCenterM;
//	float scaleFactorM;
//
//	RotationSlerp(rotationB, rotationE, &rotationM, interPos);
//	VecSlerp(positionB, positionE, &positionM, interPos);
//
//	FloatSlerp(scaleFactorB, scaleFactorE, &scaleFactorM, interPos);
//
//	mm = midStates.GetData();
//
//	mm[0] = rotationM.axis.x;
//	mm[1] = rotationM.axis.y;
//	mm[2] = rotationM.axis.z;
//	mm[3] = rotationM.angle;
//
//	mm[4 + 0] = positionM.x;
//	mm[4 + 1] = positionM.y;
//	mm[4 + 2] = positionM.z;
//
//	mm[8 + 0] = scaleFactorM;
//	mm[8 + 1] = 0;
//	mm[8 + 2] = 0;
//
//	mm[12 + 0] = rotateCenterB.x;
//	mm[12 + 1] = rotateCenterB.y;
//	mm[12 + 2] = rotateCenterB.y;
}

void Animationcreator::CameraStateSlerp(Matrix4& beginState, Matrix4& endState,
		vector<Matrix4>& midStates, int times)
{
	for (int i = 0; i < times; i++)
	{
		Matrix4 m;
		CameraMatrixSlerp(beginState, endState, m, (float(i)) / (times - 1));
		midStates.push_back(m);
	}
}

void Animationcreator::StateSlerp(NODEINFO& beginState, NODEINFO& endState,
		vector<NODEINFO>& midStates, int times, SceneManager* scene)
{
	//beginstate 当前状态，endstate xml保存状态
	//首先在endstate中找当前状态中的变化如果找不到，说明xml保存的该node当时没有发生变换。
	//然后再endstate中区找beginstate中的不同量，如果有，则取出该node的当前状态加入endstate中。
//	NODEINFO::iterator it = beginState.begin();
//	for (; it != beginState.end(); it++)
//	{
//		if (endState.find(it->first) == endState.end())
//		{
//			MovedMatrixInfo m;
//			SceneNode* node = scene->m_NodesMgr->GetNode(it->first);
//			node->GetOrigMatrix(m.m_PlcMatrx);
//
//			if (node->GetType() == GROUP_NODE)
//			{
//				GroupNode* group = (GroupNode*) node;
//				SceneNode* childNode;// = group->GetChildrenNode();
//
//				for (int i = 0; i < group->Size(); i++)
//				{
//					childNode = group->GetChild(i);
//
//					if (childNode->GetType() == SHAPE_NODE)
//					{
//						 childNode->GetOrigMatrix(m.m_MovedMatrix);
//						break;
//					}
//				}
//			}
//
//			endState.insert(NODEINFO::value_type(it->first, m));
//		}
//	}
//
//	it = endState.begin();
//	for (; it != endState.end(); it++)
//	{
//		if (beginState.find(it->first) == beginState.end())
//		{
//			MovedMatrixInfo m;
//			SceneNode* node = scene->m_NodesMgr->GetNode(it->first);
//			Matrix& plcMatrix = node->GetPlcMatrix();
//			m.m_PlcMatrx.Set(plcMatrix);
//			//node->GetCurrentMatrix(m.m_PlcMatrx);
//
//			if (node->GetType() == GROUP_NODE)
//			{
//				GroupNode* group = (GroupNode*) node;
//				SceneNode* childNode;//= group->GetChildrenNode();
//				for (int i = 0; i < group->Size(); i++)
//				{
//					childNode = group->GetChild(i);
//
//					if (childNode->GetType() == SHAPE_NODE)
//					{
//						//childNode->GetCurrentMatrix(m.m_MovedMatrix);
//						Matrix& plcMatrix = childNode->GetPlcMatrix();
//						m.m_MovedMatrix.Set(plcMatrix);
//						break;
//					}
//				}
//			}
//
//			beginState.insert(NODEINFO::value_type(it->first, m));
//		}
//	}
//
//	for (int i = 0; i < times; i++)
//	{
//		NODEINFO nodesStateMap;
//		it = beginState.begin();
//		for (; it != beginState.end(); it++)
//		{
//			MovedMatrixInfo mBegin = it->second;
//			MovedMatrixInfo mEnd = endState.find(it->first)->second;
//			MovedMatrixInfo midStateInfo;
//			MatrixInfoSlerp(mBegin, mEnd, midStateInfo, i * 1.0 / (times - 1));
//
//			nodesStateMap.insert(NODEINFO::value_type(it->first, midStateInfo));
//
//		}
//		midStates.push_back(nodesStateMap);
//	}
}

}

