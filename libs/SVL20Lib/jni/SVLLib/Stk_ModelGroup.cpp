// All Rights Reserved. Copyright (C) 2017 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file		Stk_ModelGroup.cpp
*
*	@brief	ƒ£–Õ∑÷◊È¿‡ µœ÷
*
*	@par	¿˙ ∑:
*		2017-07-18 Create by likun
****************************************************************************/

#include "Stk_ModelGroup.h"
namespace HoteamSoft {
	namespace SVLLib {
		Stk_ModelGroup::Stk_ModelGroup()
        {
            m_strName = "";
            m_iType = 0;
			m_iPriority = 1;
			m_iPrecise = 0;
		}

		Stk_ModelGroup::Stk_ModelGroup(string& strName, int iType, int iPriority, int iPrecise)
		{
			m_strName = strName;
			m_iType = iType;
			m_iPriority = iPriority;
			m_iPrecise = iPrecise;
		}

		Stk_ModelGroup::Stk_ModelGroup(Stk_ModelGroup* pGroup)
		{
			m_strName = pGroup->m_strName;
			m_iType = pGroup->m_iType;
			m_iPriority = pGroup->m_iPriority;
			m_iPrecise = pGroup->m_iPrecise;
			
			vector<string>::iterator itorPlcPath = pGroup->m_vecPlcPath.begin();
			while (itorPlcPath != pGroup->m_vecPlcPath.end())
			{
				m_vecPlcPath.push_back(*itorPlcPath);
				itorPlcPath++;
			}


		}

		Stk_ModelGroup::~Stk_ModelGroup()
		{
			m_vecPlcPath.clear();
		}
	}
}
