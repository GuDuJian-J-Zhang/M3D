// All Rights Reserved. Copyright (C) 2013 HOTEAMSOFT, Ltd
/****************************************************************************
 *	@file	Stk_InsAttribute.cpp
 *
 *	@brief	实例属性
 *
 *	@par	历史:
 *		2015/03/24	创建。WangY@HOTEAMSOFT
****************************************************************************/

#include "Stk_InsAttribute.h"
#include "Stk_ObjectImpl.h"
#include "Stk_BaseDocumentImpl.h"
#include "ChineseCode.h"
#include "StringTools.h"

#define INSATTRIBUTE_NODE_NAME_PLCPATH				"plcPath"
#define INSATTRIBUTE_NODE_NAME_VISIBLE				"visible"
#define INSATTRIBUTE_NODE_NAME_COLOR				"color"
#define INSATTRIBUTE_NODE_NAME_MATIX				"matix"
#define INSATTRIBUTE_NODE_NAME_MATERIAL				"material"

namespace HoteamSoft {
	namespace SVLLib {

		class Stk_InsAttributeImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_InsAttribute)
		public:
			Stk_InsAttributeImpl();
			virtual ~Stk_InsAttributeImpl(void);

			/****************************************************************************
			*	@brief		读取文件中的Body数据
			*	@param[in]	i_pJsonNode			View 父Json节点
			*	@param[out]	无
			*	@retval		STK_SUCCESS			读取成功
			*	@retval		STK_ERROR			读取失败
			*	@retval		STK_ERROR_MEMORY	内存申请失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		写View数据
			*	@param[in]	Value* i_pJsonParentNode			View 父Json节点
			*	@param[out]	无
			*	@retval		STK_SUCCESS			写入成功
			*	@retval		其它				写入失败
			*	@note		无
			*	@attention	内部接口
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		public:
			StkDisplayStateEnum	m_eDisplayState;		// 实例显隐
			STK_BOOL			m_bHasColor;			// 是否有颜色
			STK_RGBA32			m_rgbaIns;				// 实例颜色
			STK_BOOL			m_bHasMaterial;			// 是否有材质
			STK_MAT32			m_matIns;				// 实例材质
			STK_BOOL			m_bHasPlacement;		// 是否有实例配置矩阵
			STK_ID				m_idPlacement;			// 实例配置矩阵ID
			STK_MTX32			m_mtxPlacement;			// 实例相对于父级的配置矩阵
		};

		
		STK_CLASS_IMPL_IMPLEMENT(Stk_InsAttribute)
		Stk_InsAttributeImpl::Stk_InsAttributeImpl()
		{
			m_eDisplayState = STK_DISPLAY;
			m_bHasColor = false;
			m_bHasMaterial = false;
			m_bHasPlacement = false;
			m_rgbaIns.Red = 0;
			m_rgbaIns.Green = 0;
			m_rgbaIns.Blue = 0;
			m_rgbaIns.Alpha = 0;
			m_idPlacement = 0;
			memset(&m_mtxPlacement, 0, sizeof(STK_MTX32));
		}
		Stk_InsAttributeImpl::~Stk_InsAttributeImpl(void)
		{

		}

		STK_STATUS Stk_InsAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			Value::MemberIterator plcPathNodeIter = i_pJsonNode->FindMember(INSATTRIBUTE_NODE_NAME_PLCPATH);
			if (plcPathNodeIter != i_pJsonNode->MemberEnd())
			{
				m_idPlacement = (*i_pJsonNode)[INSATTRIBUTE_NODE_NAME_PLCPATH].GetInt();
			}

			Value::MemberIterator visibleNodeIter = i_pJsonNode->FindMember(INSATTRIBUTE_NODE_NAME_VISIBLE);
			if (visibleNodeIter != i_pJsonNode->MemberEnd())
			{
				m_eDisplayState = (StkDisplayStateEnum)(*i_pJsonNode)[INSATTRIBUTE_NODE_NAME_VISIBLE].GetInt();
			}

			m_bHasColor = false;
			Value::MemberIterator colorNodeIter = i_pJsonNode->FindMember(INSATTRIBUTE_NODE_NAME_COLOR);
			if (colorNodeIter != i_pJsonNode->MemberEnd())
			{
				string strRGBA = colorNodeIter->value.GetString();
				std::vector<std::string> vecRGBAValue;
				CStringTools::StringSplit(strRGBA, " ", vecRGBAValue);
				if (vecRGBAValue.size() == 4)
				{
					m_rgbaIns.Red = atof(vecRGBAValue[0].c_str());
					m_rgbaIns.Green = atof(vecRGBAValue[1].c_str());
					m_rgbaIns.Blue = atof(vecRGBAValue[2].c_str());
					m_rgbaIns.Alpha = atof(vecRGBAValue[3].c_str());
					m_bHasColor = true;
				}
			}
			
			m_bHasMaterial = false;
			Value::MemberIterator materialNodeIter = i_pJsonNode->FindMember(INSATTRIBUTE_NODE_NAME_MATERIAL);
			if (materialNodeIter != i_pJsonNode->MemberEnd())
			{
				string strMaterial = materialNodeIter->value.GetString();
				std::vector<std::string> vecMaterialValue;
				CStringTools::StringSplit(strMaterial, " ", vecMaterialValue);
				if (vecMaterialValue.size() == 4)
				{
					m_matIns.Ambient = atof(vecMaterialValue[0].c_str());
					m_matIns.Diffuse = atof(vecMaterialValue[1].c_str());
					m_matIns.Specular = atof(vecMaterialValue[2].c_str());
					m_bHasMaterial = true;
				}
			}

			m_bHasPlacement = false;
			Value::MemberIterator matrixNodeIter = i_pJsonNode->FindMember(INSATTRIBUTE_NODE_NAME_MATIX);
			if (matrixNodeIter != i_pJsonNode->MemberEnd())
			{
				string strMatix = matrixNodeIter->value.GetString();
				std::vector<std::string> vecMatixValue;
				CStringTools::StringSplit(strMatix, " ", vecMatixValue);
				if (vecMatixValue.size() >= MTX_LINE_COUNT*MTX_COL_COUNT)
				{
					for (int ii = 0; ii < MTX_LINE_COUNT; ii++)
					{
						for (int jj = 0; jj < MTX_COL_COUNT; jj++)
						{
							m_mtxPlacement.PlcMatrix[ii][jj] = atof(vecMatixValue[ii * 4 + jj].c_str());
						}
					}
					m_bHasPlacement = true;
				}
			}

			return ret;
		}

		STK_STATUS Stk_InsAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			char cTemp[256];
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(INSATTRIBUTE_NODE_NAME_PLCPATH, m_idPlacement, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(INSATTRIBUTE_NODE_NAME_VISIBLE, m_eDisplayState, i_JsonDocP->GetAllocator());

			//i_pJsonNode->AddMember(INSATTRIBUTE_NODE_NAME_MATIX, m_eDisplayState, i_JsonDocP->GetAllocator());

			if (m_bHasPlacement)
			{
				string strMatirx = "";
				sprintf(cTemp, "%.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f %.8f",
					m_mtxPlacement.PlcMatrix[0][0], m_mtxPlacement.PlcMatrix[0][1], m_mtxPlacement.PlcMatrix[0][2], m_mtxPlacement.PlcMatrix[0][3],
					m_mtxPlacement.PlcMatrix[1][0], m_mtxPlacement.PlcMatrix[1][1], m_mtxPlacement.PlcMatrix[1][2], m_mtxPlacement.PlcMatrix[1][3],
					m_mtxPlacement.PlcMatrix[2][0], m_mtxPlacement.PlcMatrix[2][1], m_mtxPlacement.PlcMatrix[2][2], m_mtxPlacement.PlcMatrix[2][3]);
				strMatirx = cTemp;
				i_pJsonNode->AddMember(INSATTRIBUTE_NODE_NAME_MATIX, strMatirx, i_JsonDocP->GetAllocator());
			}

			if (m_bHasColor)
			{
				string strColor = "";
				sprintf(cTemp, "%.8f %.8f %.8f %.8f",
					m_rgbaIns.Red, m_rgbaIns.Green, m_rgbaIns.Blue, m_rgbaIns.Alpha );
				strColor = cTemp;
				i_pJsonNode->AddMember(INSATTRIBUTE_NODE_NAME_COLOR, strColor, i_JsonDocP->GetAllocator());
			}

			if (m_bHasMaterial)
			{
				string strMaterial = "";
				sprintf(cTemp, "%.8f %.8f %.8f",
					m_matIns.Ambient, m_matIns.Diffuse, m_matIns.Specular);
				strMaterial = cTemp;
				i_pJsonNode->AddMember(INSATTRIBUTE_NODE_NAME_MATERIAL, strMaterial, i_JsonDocP->GetAllocator());
			}
			
			return ret;
		}

		STK_IMPLEMENT_CLASS(Stk_InsAttribute, Stk_Object)
		/************************************************************************//**
		 *	@brief		Stk_InsAttribute构造函数
		 *	@param[in]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_InsAttribute::Stk_InsAttribute() :Stk_Object(new Stk_InsAttributeImpl())
		{
			
		}

		/************************************************************************//**
		 *	@brief		Stk_InsAttribute析构函数
		 *	@param[in]	无
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		Stk_InsAttribute::~Stk_InsAttribute()
		{

		}

		/*************************************************************************//**
		 *	@brief		设置实例显隐状态
		 *	@param[in]	i_eDisplayState		实例显隐状态
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@retval		其他				设置失败
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_InsAttribute::SetDisplayState(StkDisplayStateEnum i_eDisplayState)
		{
			((Stk_InsAttributeImpl*)m_pImpl)->m_eDisplayState = i_eDisplayState;
			return STK_SUCCESS;
		}

		/*************************************************************************//**
		 *	@brief		获取实例显隐状态
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		实例显隐状态
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		StkDisplayStateEnum Stk_InsAttribute::GetDisplayState()
		{
			return ((Stk_InsAttributeImpl*)m_pImpl)->m_eDisplayState;
		}

		/*************************************************************************//**
		 *	@brief		设置实例颜色
		 *	@param[in]	i_rgbaIns			实例颜色
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@retval		其他				设置失败
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_InsAttribute::SetColor(STK_RGBA32 i_rgbaIns)
		{
			((Stk_InsAttributeImpl*)m_pImpl)->m_rgbaIns = i_rgbaIns;
			((Stk_InsAttributeImpl*)m_pImpl)->m_bHasColor = true;
			return STK_SUCCESS;
		}

		/*************************************************************************//**
		 *	@brief		获取实例颜色
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		实例颜色
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_RGBA32& Stk_InsAttribute::GetColor()
		{
			return ((Stk_InsAttributeImpl*)m_pImpl)->m_rgbaIns;
		}

		/*************************************************************************//**
		 *	@brief		设置实例材质
		 *	@param[in]	i_matIns			实例材质
		 *	@param[out]	无
		 *	@retval		STK_SUCCESS			设置成功
		 *	@retval		其他				设置失败
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_STATUS Stk_InsAttribute::SetMaterial(STK_MAT32 i_matIns)
		{
			((Stk_InsAttributeImpl*)m_pImpl)->m_matIns = i_matIns;
			((Stk_InsAttributeImpl*)m_pImpl)->m_bHasMaterial = true;
			return STK_SUCCESS;
		}

		/*************************************************************************//**
		 *	@brief		获取实例材质
		 *	@param[in]	无
		 *	@param[out]	无
		 *	@retval		实例材质
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/
		STK_MAT32& Stk_InsAttribute::GetMaterial()
		{
			return ((Stk_InsAttributeImpl*)m_pImpl)->m_matIns;
		}

		/************************************************************************//**
		 *	@brief		设置相对于父级的配置ID，配置矩阵
		 *	@param[in]	i_idPlacement			配置ID
		 *	@param[in]	i_mtxPlcMatrix			配置矩阵
		 *	@retval		无
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		STK_STATUS Stk_InsAttribute::SetPlacement(STK_ID i_idPlacement, const STK_MTX32 &i_mtxPlcMatrix)
		{
			((Stk_InsAttributeImpl*)m_pImpl)->m_idPlacement = i_idPlacement;
			((Stk_InsAttributeImpl*)m_pImpl)->m_mtxPlacement = i_mtxPlcMatrix;
			((Stk_InsAttributeImpl*)m_pImpl)->m_bHasPlacement = true;

			return STK_SUCCESS;
		}

		/************************************************************************//**
		 *	@brief		获取相对于父级的配置ID，配置矩阵
		 *	@param[out]	o_idPlacement			配置ID
		 *	@param[out]	o_mtxPlcMatrix			配置矩阵
		 *	@retval		STK_SUCCESS				获取成功
		 *	@note		无
		 *	@attention	无
		 ****************************************************************************/

		STK_STATUS Stk_InsAttribute::GetPlacement(STK_ID &o_idPlacement, STK_MTX32 &o_mtxPlcMatrix)
		{
			o_idPlacement = ((Stk_InsAttributeImpl*)m_pImpl)->m_idPlacement;
			o_mtxPlcMatrix = ((Stk_InsAttributeImpl*)m_pImpl)->m_mtxPlacement;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_InsAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_InsAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS Stk_InsAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_InsAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

	}//SVLLib
}//HoteamSoft
