// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_Feat.cpp
 *
 *	@brief	特征类的实现
 *
 *	@par	历史:
 *		2014/05/30	创建。WangY@HOTEAMSOFT
****************************************************************************/

#ifndef __MOBILE__
#pragma once
#endif

#include "StdAfx.h"
#include "Stk_Feat.h"
#include "Stk_Manager.h"
#include "Stk_Platform.h"

/************************************************************************//**
 *	@brief		Stk_Feat构造函数
 *	@param[in]	i_stkPrototypeP		所属零件
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Feat::Stk_Feat(Stk_ProtoType* i_stkPrototypeP)
{
	m_stkProtoP = i_stkPrototypeP;
	m_ID = 0;
	m_eType = StkFeatTypeEnum::FEAT_TYPE_UNKNOWN;
	m_bActive = false;
	m_wstrName = L"";
	m_dataP = NULL;
}

/****************************************************************************
 *	@brief		Stk_Feat拷贝构造函数
 *	@param[in]	i_stkFeat		特征类对象
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//Stk_Feat::Stk_Feat(Stk_Feat& i_stkFeat)
//{
//	m_stkProtoP = new Stk_ProtoType();
//	memcpy(m_stkProtoP, i_stkFeat.m_stkProtoP, sizeof(*i_stkFeat.m_stkProtoP));
//	m_ID = i_stkFeat.m_ID;
//	m_eType = i_stkFeat.m_eType;
//	m_bActive = i_stkFeat.m_bActive;
//	m_wstrName = i_stkFeat.m_wstrName;
//	switch(i_stkFeat.GetFeatType())
//	{
//	case StkFeatTypeEnum::FEAT_TYPE_PIPE:
//		m_dataP = new Stk_Feat_Pipe();
//		memcpy(m_dataP, i_stkFeat.m_dataP, sizeof(*(Stk_Feat_Pipe*)m_dataP));
//		break;
//	default:
//		break;
//	}
//}

/************************************************************************//**
 *	@brief		Stk_Feat析构函数
 *	@param[in]	无
 *	@retval		无
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Feat::~Stk_Feat()
{
	if (m_dataP != NULL)
	{
		delete m_dataP;
		m_dataP = NULL;
	}
	m_stkProtoP->GetProtoIDManager()->DelFeatByID(m_ID);
}

/****************************************************************************
 *	@brief		Stk_Feat赋值构造函数
 *	@param[in]	i_stkFeat		特征类对象
 *	@retval		*this			当前特征类对象
 *	@note		无
 *	@attention	无
 ****************************************************************************/

//const Stk_Feat& Stk_Feat::operator = (Stk_Feat& i_stkFeat)
//{
//	if (this == &i_stkFeat)
//	{
//		return *this;
//	}
//
//	m_stkProtoP = new Stk_ProtoType();
//	memcpy(m_stkProtoP, i_stkFeat.m_stkProtoP, sizeof(*i_stkFeat.m_stkProtoP));
//	m_ID = i_stkFeat.m_ID;
//	m_eType = i_stkFeat.m_eType;
//	m_bActive = i_stkFeat.m_bActive;
//	m_wstrName = i_stkFeat.m_wstrName;
//	switch(i_stkFeat.GetFeatType())
//	{
//	case StkFeatTypeEnum::FEAT_TYPE_PIPE:
//		m_dataP = new Stk_Feat_Pipe();
//		memcpy(m_dataP, i_stkFeat.m_dataP, sizeof(*(Stk_Feat_Pipe*)m_dataP));
//		break;
//	default:
//		break;
//	}
//
//	return *this;
//}

/************************************************************************//**
 *	@brief		设置特征ID
 *	@param[in]	i_Id			特征ID
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat::SetID(STK_UINT32 i_Id)
{
	m_ID = i_Id;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取特征ID
 *	@param[in]	无
 *	@retval		m_ID		特征ID
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_ID Stk_Feat::GetID()
{
	return m_ID;
}

/************************************************************************//**
 *	@brief		注册特征ID
 *	@param[in]	i_idFeat		特征ID
 *	@retval		STK_SUCCESS		注册成功
 *	@retval		STK_ERROR		注册失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat::RegisterID(STK_ID i_idFeat)
{
	STK_STATUS ret = m_stkProtoP->GetProtoIDManager()->RegFeatID(this, i_idFeat);
	if (ret==STK_SUCCESS)
	{
		m_ID = i_idFeat;
		return STK_SUCCESS;
	} 
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		设置特征类型
 *	@param[in]	i_eType			特征类型
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat::SetFeatType(StkFeatTypeEnum i_eType)
{
	m_eType = i_eType;
	
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取特征类型
 *	@param[in]	无
 *	@retval		m_eType			特征类型
 *	@note		无
 *	@attention	无
 ****************************************************************************/

StkFeatTypeEnum Stk_Feat::GetFeatType()
{
	return m_eType;
}

/************************************************************************//**
 *	@brief		是否活动状态
 *	@param[in]	无
 *	@retval		m_bActive		是否活动状态
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_BOOL Stk_Feat::IsActiveed()
{
	return m_bActive;
}

/************************************************************************//**
 *	@brief		设置活动状态
 *	@param[in]	i_bActived		活动状态
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat::SetActiveed(STK_BOOL i_bActived)
{
	m_bActive = i_bActived;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		设置特证名
 *	@param[in]	i_wstrName		特证名
 *	@retval		STK_SUCCESS		设置成功
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat::SetFeatName(wstring i_wstrName)
{
	m_wstrName = i_wstrName;

	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		获取特证名
 *	@param[in]	无
 *	@retval		m_wstrName		特证名
 *	@note		无
 *	@attention	无
 ****************************************************************************/

wstring Stk_Feat::GetFeatName()
{
	return m_wstrName;
}

/************************************************************************//**
 *	@brief		设置线缆特征
 *	@param[in]	i_FeatPipe		线缆特征对象指针
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其他			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/

STK_STATUS Stk_Feat::SetFeatPipe(Stk_Feat_Pipe* i_FeatPipe)
{
	if (m_eType == FEAT_TYPE_PIPE)
	{
		m_dataP = i_FeatPipe;
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取线缆特征
 *	@param[in]	无
 *	@retval		m_dataP			线缆特征
 *	@note		无
 *	@attention	无
 ****************************************************************************/

Stk_Feat_Pipe* Stk_Feat::GetFeatPipe()
{
	if (m_eType == FEAT_TYPE_PIPE)
	{
		return (Stk_Feat_Pipe*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		设置磁力线特征
 *	@param[in]	i_MagneticLineP	磁力线特征对象
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其他			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat::SetMagneticLine(Stk_Feat_MagneticLine* i_MagneticLineP)
{
	if (m_eType == FEAT_TYPE_MAGNETIC_LINE)
	{
		m_dataP = i_MagneticLineP;
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取磁力线特征
 *	@param[in]	无
 *	@retval		磁力线特征
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_MagneticLine* Stk_Feat::GetMagneticLine()
{
	if (m_eType == FEAT_TYPE_MAGNETIC_LINE)
	{
		return (Stk_Feat_MagneticLine*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		设置箭头特征
 *	@param[in]	i_FeatArrowP	箭头特征对象
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其他			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat::SetArrow(Stk_Feat_Arrow* i_FeatArrowP)
{
	if (m_eType == FEAT_TYPE_ARROW)
	{
		m_dataP = i_FeatArrowP;
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取箭头特征
 *	@param[in]	无
 *	@retval		箭头特征
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Arrow* Stk_Feat::GetArrow()
{
	if (m_eType == FEAT_TYPE_ARROW)
	{
		return (Stk_Feat_Arrow*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		设置标记特征
 *	@param[in]	i_FeatMarkP		标记特征对象
 *	@retval		STK_SUCCESS		设置成功
 *	@retval		其他			设置失败
 *	@note		无
 *	@attention	无
 ****************************************************************************/
STK_STATUS Stk_Feat::SetFeatMark(Stk_Feat_Mark* i_FeatMarkP)
{
	if (m_eType == FEAT_TYPE_MARK)
	{
		m_dataP = i_FeatMarkP;
		return STK_SUCCESS;
	}
	else
	{
		return STK_ERROR;
	}
}

/************************************************************************//**
 *	@brief		获取标记特征
 *	@param[in]	无
 *	@retval		标记特征
 *	@note		无
 *	@attention	无
 ****************************************************************************/
Stk_Feat_Mark* Stk_Feat::GetFeatMark()
{
	if (m_eType == FEAT_TYPE_MARK)
	{
		return (Stk_Feat_Mark*)m_dataP;
	}
	else
	{
		return NULL;
	}
}

/************************************************************************//**
 *	@brief		读取特征文件
 *	@param[in]	i_stkFileP		文件指针
 *	@param[in]	i_FeatHeadSP	特征头记录
 *	@retval		STK_SUCCESS		读取成功
 *	@retval		其他			读取失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Feat::ReadFile(Stk_File *i_stkFileP, StkSegmentHead *i_FeatHeadSP)
{
	/*
	 *	变量定义
	 */
	STK_UINT32				ii;
	StkControl**			recordPP = NULL;
	StkFeatRec*				stkFeatRecP = NULL;
	StkFeatRec2*			stkFeatRec2P = NULL;
	STK_STATUS				eState = STK_SUCCESS;
	StkSegmentHead			head ={0};
	Stk_Feat_Pipe*			stkFeatPipe = NULL;
	Stk_Feat_Hole*			stkFeatHoleP = NULL;
	Stk_Feat_MagneticLine*	stkFeatMagneticLineP = NULL;
	Stk_Feat_LineArrow*			stkFeatLineArrowP = NULL;
	Stk_Feat_CircularArrow*		stkFeatCircularArrowP = NULL;
	Stk_Feat_RightAngleArrow*	stkFeatRightAngleArrowP = NULL;
	Stk_Mark_RectAngle*		stkMarkRectAngleP = NULL;
	Stk_Mark_Ellipse*		stkMarkEllipseP = NULL;
	Stk_Mark_FreeHand*		stkMarkFreeHandP = NULL;

	/*
	 *	读取Feat段的数据
	 */
	recordPP = (StkControl **)calloc(i_FeatHeadSP->recnum, sizeof(StkControl*));
	if (i_FeatHeadSP->recnum != 0 && recordPP == NULL)
	{
		return STK_ERROR_MEMORY;
	}
	eState = i_stkFileP->ReadSegmentAllRecordData(i_FeatHeadSP, (StkControl **)recordPP);
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	RegisterID(i_FeatHeadSP->id);	// 注册特征ID

	/*
	 *	读取Feat段各记录
	 */
	for (ii = 0; ii < i_FeatHeadSP->recnum; ii++)
	{
		if (recordPP[ii]->type == REC_TYPE_FEAT)
		{
			if (recordPP[ii]->version == 1)
			{
				stkFeatRecP =(StkFeatRec*)recordPP[ii];
				m_eType = (StkFeatTypeEnum)stkFeatRecP->nuType;
				m_bActive = (STK_BOOL)stkFeatRecP->nuActive;
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkFeatRecP->name);
#else
				m_wstrName = stkFeatRecP->name;
#endif
			}
			else if (recordPP[ii]->version == 2)
			{
				stkFeatRec2P =(StkFeatRec2*)recordPP[ii];
				m_eType = (StkFeatTypeEnum)stkFeatRec2P->nuType;
				m_bActive = (STK_BOOL)stkFeatRec2P->nuActive;
#ifdef __MOBILE__
				m_wstrName = Stk_Platform::STKWCHARStrToWString(stkFeatRec2P->atr);
#else
				m_wstrName = stkFeatRec2P->atr;
#endif
			}
		}
	}

	// 释放内存
	eState = i_stkFileP->ReadSegmentFinishData(i_FeatHeadSP, (StkControl **)recordPP);
	if (recordPP != NULL)
	{
		free(recordPP);
		recordPP = NULL;
	}
	if (eState!=STK_SUCCESS)
	{
		return eState;
	}

	/*
	 * 读取Feat段中的子段
	 */
	while (1)
	{
		eState = i_stkFileP->ReadSegmentHeadData( &head);
		if (eState!=STK_SUCCESS)
		{
			return eState;
		}

		switch(head.kind)
		{
		case SEG_TYPE_FEAT_PIPE:
			stkFeatPipe = new Stk_Feat_Pipe();
			eState = stkFeatPipe->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkFeatPipe;
				stkFeatPipe = NULL;
				return STK_ERROR;
			}
			m_dataP = stkFeatPipe;

			break;
		case SEG_TYPE_FEATHOLE:
			stkFeatHoleP = new Stk_Feat_Hole();
			eState = stkFeatHoleP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkFeatHoleP;
				stkFeatHoleP = NULL;
				return STK_ERROR;
			}
			m_dataP = stkFeatHoleP;

			break;
		case SEG_TYPE_FEAT_MAGNETICLINE:
			stkFeatMagneticLineP = new Stk_Feat_MagneticLine();
			eState = stkFeatMagneticLineP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkFeatMagneticLineP;
				stkFeatMagneticLineP = NULL;
				return STK_ERROR;
			}
			m_dataP = stkFeatMagneticLineP;

			break;
		case SEG_TYPE_FEAT_LINEARROW:
			stkFeatLineArrowP = new Stk_Feat_LineArrow();
			eState = stkFeatLineArrowP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkFeatLineArrowP;
				stkFeatLineArrowP = NULL;
				return STK_ERROR;
			}
			m_dataP = stkFeatLineArrowP;
			break;
		case SEG_TYPE_FEAT_CIRCULARARROW:
			stkFeatCircularArrowP = new Stk_Feat_CircularArrow();
			eState = stkFeatCircularArrowP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkFeatCircularArrowP;
				stkFeatCircularArrowP = NULL;
				return STK_ERROR;
			}
			m_dataP = stkFeatCircularArrowP;
			break;
		case SEG_TYPE_FEAT_RIGHTANGLEARROW:
			stkFeatRightAngleArrowP = new Stk_Feat_RightAngleArrow();
			eState = stkFeatRightAngleArrowP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkFeatRightAngleArrowP;
				stkFeatRightAngleArrowP = NULL;
				return STK_ERROR;
			}
			m_dataP = stkFeatRightAngleArrowP;
			break;
		case SEG_TYPE_MARK_RECTANGLE:
			stkMarkRectAngleP = new Stk_Mark_RectAngle();
			eState = stkMarkRectAngleP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkMarkRectAngleP;
				stkMarkRectAngleP = NULL;
				return STK_ERROR;
			}
			m_dataP = stkMarkRectAngleP;
			break;
		case SEG_TYPE_MARK_ELLIPSE:
			stkMarkEllipseP = new Stk_Mark_Ellipse();
			eState = stkMarkEllipseP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkMarkEllipseP;
				stkMarkEllipseP = NULL;
				return STK_ERROR;
			}
			m_dataP = stkMarkEllipseP;
			break;
		case SEG_TYPE_MARK_FREEHAND:
			stkMarkFreeHandP = new Stk_Mark_FreeHand();
			eState = stkMarkFreeHandP->ReadFile(i_stkFileP, &head);
			if (eState != STK_SUCCESS)
			{
				delete stkMarkFreeHandP;
				stkMarkFreeHandP = NULL;
				return STK_ERROR;
			}
			m_dataP = stkMarkFreeHandP;
			break;
		// 当下一段是FEAT-END表时
		case SEG_TYPE_FEAT_END:
			goto pt1;
			break;

		default:
			StkControl **TmpRecordPP = NULL;
			TmpRecordPP = (StkControl **)calloc(head.recnum, sizeof(StkControl*));
			if (head.recnum !=0 && TmpRecordPP == NULL)
			{
				return STK_ERROR_MEMORY;
			}
			eState = i_stkFileP->ReadSegmentAllRecordData(&head, (StkControl **)TmpRecordPP);
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			eState = i_stkFileP->ReadSegmentFinishData(&head, (StkControl **)TmpRecordPP);// 释放内存
			if (eState!=STK_SUCCESS)
			{
				return eState;
			}
			if (TmpRecordPP != NULL)
			{
				free(TmpRecordPP);
				TmpRecordPP = NULL;
			}
			break;
		}
	}

pt1:
	return STK_SUCCESS;
}

/************************************************************************//**
 *	@brief		写特征文件
 *	@param[in]	i_stkFileP		文件指针
 *	@retval		STK_SUCCESS		写入成功
 *	@retval		其他			写入失败
 *	@note		无
 *	@attention	内部接口
 ****************************************************************************/

STK_STATUS Stk_Feat::WriteFile(Stk_File *i_stkFileP)
{
	/*
	 *	变量定义
	 */
	STK_STATUS				ret;
	StkSegmentHead			featHead = {0};
	StkSegmentHead			endHead = {0};
	StkFeatRec2*			stkFeatRec2P = NULL;
	Stk_Feat*				stkFeatP = NULL;
	Stk_Feat_Arrow*			stkFeatArrowP = NULL;
	Stk_Feat_Mark*			stkFeatMarkP = NULL;
	StkArrowTypeEnum		eArrowType = ARROW_TYPE_LINE;
	STK_UINT32				nuFeatNameSize = 0;
	StkMarkTypeEnum			eMarkType = MARK_TYPE_RECTANGLE;

	if(i_stkFileP == NULL)
	{
		return STK_ERROR;
	}

	/*
	 *	【第1步】输出特征段头
	 */
	featHead.id = m_ID;
	featHead.kind = SEG_TYPE_FEAT;
	ret = i_stkFileP->WriteSegmentHeadData(&featHead);
	if (ret != STK_SUCCESS)
	{
		return ret;
	}

	/*
	 *	【第2步】输出特征基本记录
	 */
	nuFeatNameSize = m_wstrName.size() + 1;
	stkFeatRec2P = (StkFeatRec2 *)calloc(1, sizeof(StkFeatRec2) - sizeof(STK_WCHAR) + nuFeatNameSize * sizeof(STK_WCHAR));
	if(stkFeatRec2P == NULL)
	{
		return STK_ERROR;
	}

	stkFeatRec2P->Control.type = REC_TYPE_FEAT;
	stkFeatRec2P->Control.length = sizeof(StkFeatRec2) - sizeof(STK_WCHAR) + nuFeatNameSize * sizeof(STK_WCHAR) - sizeof(StkControl);
	stkFeatRec2P->Control.version = _VER_FEAT_RECORD;
#ifdef __MOBILE__
	memcpy(stkFeatRec2P->atr,Stk_Platform::WStringToSTKWCHARStr(m_wstrName),(m_wstrName.size()+1)*sizeof(STK_WCHAR));
#else
	wcscpy_s(stkFeatRec2P->atr, nuFeatNameSize, (WCHAR*)m_wstrName.c_str());
#endif
	
	stkFeatRec2P->nuType = m_eType;
	stkFeatRec2P->nuActive = m_bActive;
	stkFeatRec2P->nuCharSize = nuFeatNameSize;

	ret = i_stkFileP->WriteSegmentRecordData(&featHead, (StkControl*)stkFeatRec2P);
	if(ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}
	if(stkFeatRec2P != NULL)
	{
		free(stkFeatRec2P);
		stkFeatRec2P = NULL;
	}

	/*
	 *	【第3步】完成特征段的输出
	 */
	ret = i_stkFileP->WriteSegmentFinishData(&featHead);
	if( ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	/*
	 *	【第4步】输出复合条件类型的特征段
	 */
	switch(m_eType)
	{
	case FEAT_TYPE_PIPE:
		if (m_dataP != NULL)
		{
			ret = ((Stk_Feat_Pipe*)m_dataP)->WriteFile(i_stkFileP);
			if (ret != STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}

		break;
	case FEAT_TYPE_HOLE:
		if (m_dataP != NULL)
		{
			ret = ((Stk_Feat_Hole*)m_dataP)->WriteFile(i_stkFileP);
			if (ret != STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
		break;
	case FEAT_TYPE_MAGNETIC_LINE:
		if (m_dataP != NULL)
		{
			ret = ((Stk_Feat_MagneticLine*)m_dataP)->WriteFile(i_stkFileP);
			if (ret != STK_SUCCESS)
			{
				return STK_ERROR;
			}
		}
		break;
	case FEAT_TYPE_ARROW:
		if (m_dataP != NULL)
		{
			stkFeatArrowP = (Stk_Feat_Arrow*)m_dataP;
			eArrowType = stkFeatArrowP->GetType();
			switch(eArrowType)
			{
			case ARROW_TYPE_LINE:
				ret = ((Stk_Feat_LineArrow*)m_dataP)->WriteFile(i_stkFileP);
				if (ret != STK_SUCCESS)
				{
					return STK_ERROR;
				}
				break;
			case ARROW_TYPE_CIRCULAR:
				ret = ((Stk_Feat_CircularArrow*)m_dataP)->WriteFile(i_stkFileP);
				if (ret != STK_SUCCESS)
				{
					return STK_ERROR;
				}
				break;
			case ARROW_TYPE_RIGHTANGLE:
				ret = ((Stk_Feat_RightAngleArrow*)m_dataP)->WriteFile(i_stkFileP);
				if (ret != STK_SUCCESS)
				{
					return STK_ERROR;
				}
				break;
			default:
				break;
			}
		}
		break;
	case FEAT_TYPE_MARK:
		if (m_dataP != NULL)
		{
			stkFeatMarkP = (Stk_Feat_Mark*)m_dataP;
			eMarkType = stkFeatMarkP->GetType();
			switch(eMarkType)
			{
			case MARK_TYPE_RECTANGLE:
				ret = ((Stk_Mark_RectAngle*)m_dataP)->WriteFile(i_stkFileP);
				if (ret != STK_SUCCESS)
				{
					return STK_ERROR;
				}
				break;
			case MARK_TYPE_ELLIPSE:
				ret = ((Stk_Mark_Ellipse*)m_dataP)->WriteFile(i_stkFileP);
				if (ret != STK_SUCCESS)
				{
					return STK_ERROR;
				}
				break;
			case MARK_TYPE_FREEHAND:
				ret = ((Stk_Mark_FreeHand*)m_dataP)->WriteFile(i_stkFileP);
				if (ret != STK_SUCCESS)
				{
					return STK_ERROR;
				}
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}

	/*
	 *	【第5步】输出Feat-END段
	 */
	endHead.id = m_ID;
	endHead.kind = SEG_TYPE_FEAT_END;
	ret = i_stkFileP->WriteSegmentHeadData(&endHead);
	if( ret < STK_SUCCESS)
	{
		return STK_ERROR;
	}

	return ret;
}