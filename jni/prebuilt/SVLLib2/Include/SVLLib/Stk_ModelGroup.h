// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		Stk_ModelGroup.h
*
*	@brief	ģ�ͷ�����ʵ��
*
*	@par	��ʷ:
*		2017-07-18 Create by likun
****************************************************************************/
#ifndef _SVLLIB_MODELGROUP_H_
#define _SVLLIB_MODELGROUP_H_

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Stk_Object.h"
#include "Stk_Camera.h"

using namespace std;
namespace HoteamSoft {
	namespace SVLLib {
		class STK_TOOLKIT_EXPORT Stk_ModelGroup
		{
		public:
			Stk_ModelGroup();
			Stk_ModelGroup(string& strName, int iType = 0, int iPriority = 1, int m_iPrecise = 1);
			Stk_ModelGroup(Stk_ModelGroup* pGroup);
			~Stk_ModelGroup();

		public:
			int GetID() { return m_ID; }
			void SetID(int iID) { m_ID = iID; }

			Stk_CameraPtr GetCamera() { return m_Camera; }
			void SetCamera(Stk_CameraPtr camera) { m_Camera = camera; }

			int GetType() { return m_iType; }
			void SetType(int iType) { m_iType = iType; }

			string& GetName() { return m_strName; }
			void SetName(string& strName) { m_strName = strName; }

			int GetPriority() { return m_iPriority; }
			void SetPriority(int iPriority) { m_iPriority = iPriority; }

			int GetPrecise() { return m_iPrecise; }
			void SetPrecise(int iPrecise) { m_iPrecise = iPrecise; }

			std::vector<string>& GetInsPlcPath() { return m_vecPlcPath; }
			void SetInsPlcPath(std::vector<string>& vecPlcPath) { m_vecPlcPath = vecPlcPath; }

		private:
			int			m_ID;			//Ψһ��ʶ
			Stk_CameraPtr	m_Camera;
			int			m_iType;		//��������ͣ�0-Ĭ�Ϸ��飻1-ϵͳ���飻2-�Զ������
			string		m_strName;		//���������,Ψһ��ʶ
			int			m_iPriority;	//��������ȼ�:�ȼ���1-5��
			int			m_iPrecise;		//��ʾ����:0���� 1���� 2����
			std::vector<string>		 m_vecPlcPath;	//�������������������·��

		};
	}
}
#endif
