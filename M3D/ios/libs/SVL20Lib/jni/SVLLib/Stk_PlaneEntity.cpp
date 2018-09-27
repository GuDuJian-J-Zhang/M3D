// All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
/****************************************************************************
*	@file	Stk_PlaneEntity.h
*
*	@brief	√Ê“™Àÿ¿‡
*
*	@par	¿˙ ∑:
*
****************************************************************************/
#include "Stk_PlaneEntity.h"
#include "Stk_BaseDocumentImpl.h"
#include "StringTools.h"

#define  CLIPPLANE_NODE_NAME_ID  "id"
#define  CLIPPLANE_NODE_NAME_ORIGINPNT "originPnt"
#define  CLIPPLANE_NODE_NAME_NORMAL "normal"
#define  CLIPPLANE_NODE_NAME_UDIR   "UDir"
#define  CLIPPLANE_NODE_NAME_VDIR  "VDir"
#define  CLIPPLANE_NODE_NAME_CLIPPINGCURVE "clippingCurve"
#define  CLIPPLANE_NODE_NAME_DISTANCE  "distance"

#define  CLIPPINGCURVE_NODE_NAME_ISDISPLAY "isDisplay"
#define  CLIPPINGCURVE_NODE_NAME_DRAWSTYLE "drawStyle"
#define  CLIPPINGCURVE_NODE_NAME_DRAWTYPE  "drawType"
#define  CLIPPINGCURVE_NODE_NAME_ANGLE     "angle"
#define  CLIPPINGCURVE_NODE_NAME_RATIO     "ratio"
#define  CLIPPINGCURVE_NODE_NAME_COLOR     "color"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_PlaneEntityImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_PlaneEntity)
		public:
			Stk_PlaneEntityImpl();
			virtual ~Stk_PlaneEntityImpl();
		protected:
			STK_RGBA32			m_rgbaPlane;	//!< —’…´
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒ ˝æ›
			*	@param[in]	i_pJsonNode		 ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document*i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥ ˝æ›
			*	@param[in]	Value* i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* pDoc);
		};
		
		STK_STATUS	Stk_PlaneEntityImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			return ret;
		}

		STK_STATUS	Stk_PlaneEntityImpl::FromJson(Value* i_pJsonNode, Document*i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			return ret;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_PlaneEntity)
		Stk_PlaneEntityImpl::Stk_PlaneEntityImpl()
		{

		}

		Stk_PlaneEntityImpl::~Stk_PlaneEntityImpl()
		{

		}
		STK_IMPLEMENT_CLASS(Stk_PlaneEntity, Stk_Object)
		
		Stk_PlaneEntity::Stk_PlaneEntity():Stk_Object(new Stk_PlaneEntityImpl())
		{

		}

		Stk_PlaneEntity::~Stk_PlaneEntity()
		{

		}

		StkPlaneTypeEnum Stk_PlaneEntity::GetType()
		{
			return m_ePlaneType;
		}

		STK_RGBA32	Stk_PlaneEntity::GetColor()
		{
			return ((Stk_PlaneEntityImpl*)m_pImpl)->m_rgbaPlane;
		}

		STK_STATUS	Stk_PlaneEntity::SetColor(const STK_RGBA32& i_rgbaSurface)
		{
			((Stk_PlaneEntityImpl*)m_pImpl)->m_rgbaPlane = i_rgbaSurface;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_PlaneEntity::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_PlaneEntityImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_PlaneEntity::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_PlaneEntityImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_ClipPlaneImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_ClipPlane)
		public:
			Stk_ClipPlaneImpl();
			virtual ~Stk_ClipPlaneImpl();
		private:
			//STK_ID              m_iID;              //∆ √ÊID
			STK_PNT32			m_pntOrigin;		//!< ∆ º∑∂Œß
			STK_DIR32			m_dirNormal;		//!< ∑®œﬂ
			STK_DIR32			m_dirU;				//!< U∑ΩœÚ
			STK_DIR32			m_dirV;				//!< V∑ΩœÚ
			STK_FLOAT32			m_fDistance;		//!< ∆ «–…Ó∂»£∫‘≠µ„”Î∆ «–√Êµƒæ‡¿Î…Ó∂»
			Stk_ClippingCurvePtr	m_stkClippingCurveP;//!< ∆ «–œﬂ
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒ ˝æ›
			*	@param[in]	i_pJsonNode		 ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document*i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥ ˝æ›
			*	@param[in]	Value* i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* pDoc);
		};

		STK_STATUS	Stk_ClipPlaneImpl::FromJson(Value* i_pJsonNode, Document*i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			i_pJsonNode->AddMember(CLIPPLANE_NODE_NAME_ID, m_ID, i_JsonDocP->GetAllocator());
			string strValue;
			std::vector<std::string> vecValues;
			//originPnt
			Value::MemberIterator originPntIter = i_pJsonNode->FindMember(CLIPPLANE_NODE_NAME_ORIGINPNT);
			if (originPntIter != i_pJsonNode->MemberEnd())
			{
				strValue = originPntIter->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValues);
				m_pntOrigin.X = atof(vecValues[0].c_str());
				m_pntOrigin.Y = atof(vecValues[1].c_str());
				m_pntOrigin.Z = atof(vecValues[2].c_str());
				vecValues.clear();
			}
			//normal
			Value::MemberIterator normalIter = i_pJsonNode->FindMember(CLIPPLANE_NODE_NAME_NORMAL);
			if (normalIter != i_pJsonNode->MemberEnd())
			{
				strValue = normalIter->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValues);
				m_dirNormal.X = atof(vecValues[0].c_str());
				m_dirNormal.Y = atof(vecValues[1].c_str());
				m_dirNormal.Z = atof(vecValues[2].c_str());
				vecValues.clear();
			}
			//UDir
			Value::MemberIterator UDirIter = i_pJsonNode->FindMember(CLIPPLANE_NODE_NAME_UDIR);
			if (UDirIter != i_pJsonNode->MemberEnd())
			{
				strValue = UDirIter->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValues);
				m_dirU.X = atof(vecValues[0].c_str());
				m_dirU.Y = atof(vecValues[1].c_str());
				m_dirU.Z = atof(vecValues[2].c_str());
			}
			//VDir
			Value::MemberIterator VDirIter = i_pJsonNode->FindMember(CLIPPLANE_NODE_NAME_VDIR);
			if (VDirIter != i_pJsonNode->MemberEnd())
			{
				strValue = VDirIter->value.GetString();
				CStringTools::StringSplit(strValue, " ", vecValues);
				m_dirV.X = atof(vecValues[0].c_str());
				m_dirV.Y = atof(vecValues[1].c_str());
				m_dirV.Z = atof(vecValues[2].c_str());
			}
			//clippingCurve
			Value::MemberIterator clippingCurveIter = i_pJsonNode->FindMember(CLIPPLANE_NODE_NAME_CLIPPINGCURVE);
			if (clippingCurveIter != i_pJsonNode->MemberEnd())
			{
				Value valClippingCurve;
				valClippingCurve.SetObject();
				m_stkClippingCurveP = Stk_ClippingCurvePtr::CreateObject();
				if(m_stkClippingCurveP != nullptr)
					m_stkClippingCurveP->FromJson(&valClippingCurve, i_JsonDocP);
			}

			Value::MemberIterator distanceIter = i_pJsonNode->FindMember(CLIPPLANE_NODE_NAME_DISTANCE);
			if (distanceIter != i_pJsonNode->MemberEnd())
			{
				m_fDistance = distanceIter->value.GetFloat();
			}
			return ret;
		}

		STK_STATUS	Stk_ClipPlaneImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(CLIPPLANE_NODE_NAME_ID, m_ID, pDoc->GetAllocator());
			string strValue = "";
			char szValue[256] = { 0 };
			//originPnt
			sprintf(szValue, "%.8f", m_pntOrigin.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntOrigin.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntOrigin.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(CLIPPLANE_NODE_NAME_ORIGINPNT, strValue, pDoc->GetAllocator());
			//normal
			strValue = "";
			sprintf(szValue, "%.8f", m_dirNormal.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirNormal.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirNormal.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(CLIPPLANE_NODE_NAME_NORMAL, strValue, pDoc->GetAllocator());
			//UDir
			strValue = "";
			sprintf(szValue, "%.8f", m_dirU.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirU.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirU.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(CLIPPLANE_NODE_NAME_UDIR, strValue, pDoc->GetAllocator());
			//VDir
			strValue = "";
			sprintf(szValue, "%.8f", m_dirV.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirV.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirV.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(CLIPPLANE_NODE_NAME_VDIR, strValue, pDoc->GetAllocator());

			i_pJsonNode->AddMember(CLIPPLANE_NODE_NAME_DISTANCE, m_fDistance, pDoc->GetAllocator());
			if (m_stkClippingCurveP != nullptr)
			{
				Value valClippingCurve;
				valClippingCurve.SetObject();
				m_stkClippingCurveP->ToJson(&valClippingCurve, pDoc);
				i_pJsonNode->AddMember(CLIPPLANE_NODE_NAME_CLIPPINGCURVE, valClippingCurve, pDoc->GetAllocator());
			}
			return ret;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_ClipPlane)
		Stk_ClipPlaneImpl::Stk_ClipPlaneImpl()
		{
			//m_iID = -1;
			m_stkClippingCurveP = nullptr;
		}

		Stk_ClipPlaneImpl::~Stk_ClipPlaneImpl()
		{

		}
		STK_IMPLEMENT_CLASS(Stk_ClipPlane, Stk_PlaneEntity)

		Stk_ClipPlane::Stk_ClipPlane() :Stk_PlaneEntity(new Stk_ClipPlaneImpl())
		{
			m_ePlaneType = SURFACE_TYPE_PLANE;
		}

		Stk_ClipPlane::~Stk_ClipPlane()
		{

		}

		STK_PNT32	Stk_ClipPlane::GetOrigin()
		{
			return ((Stk_ClipPlaneImpl*)m_pImpl)->m_pntOrigin;
		}

		void	Stk_ClipPlane::SetOrigin(const STK_PNT32 &i_pntOrigin)
		{
			((Stk_ClipPlaneImpl*)m_pImpl)->m_pntOrigin = i_pntOrigin;
		}

		STK_DIR32	Stk_ClipPlane::GetNormal()
		{
			return ((Stk_ClipPlaneImpl*)m_pImpl)->m_dirNormal;
		}

		void	Stk_ClipPlane::SetNormal(const STK_DIR32 &i_dirNormal)
		{
			((Stk_ClipPlaneImpl*)m_pImpl)->m_dirNormal = i_dirNormal;
		}

		STK_STATUS	Stk_ClipPlane::GetUVDir(STK_DIR32 &o_dirU, STK_DIR32 &o_dirV)
		{
			o_dirU = ((Stk_ClipPlaneImpl*)m_pImpl)->m_dirU;
			o_dirV = ((Stk_ClipPlaneImpl*)m_pImpl)->m_dirV;
			return STK_SUCCESS;
		}

		void	Stk_ClipPlane::SetUVDir(const STK_DIR32 &i_dirU, const STK_DIR32 &i_dirV)
		{
			((Stk_ClipPlaneImpl*)m_pImpl)->m_dirU = i_dirU;
			((Stk_ClipPlaneImpl*)m_pImpl)->m_dirV = i_dirV;
		}

		STK_FLOAT32	Stk_ClipPlane::GetDistance()
		{
			return ((Stk_ClipPlaneImpl*)m_pImpl)->m_fDistance;
		}

		STK_STATUS	Stk_ClipPlane::SetDistance(STK_FLOAT32 i_fDistance)
		{
			((Stk_ClipPlaneImpl*)m_pImpl)->m_fDistance = i_fDistance;
			return STK_SUCCESS;
		}

		Stk_ClippingCurvePtr Stk_ClipPlane::GetClippingCurve()
		{
			return ((Stk_ClipPlaneImpl*)m_pImpl)->m_stkClippingCurveP;
		}

		STK_STATUS	Stk_ClipPlane::SetClippingCurve(Stk_ClippingCurvePtr i_stkClippingCurveP)
		{
			((Stk_ClipPlaneImpl*)m_pImpl)->m_stkClippingCurveP = i_stkClippingCurveP;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_ClipPlane::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ClipPlaneImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_ClipPlane::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ClipPlaneImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_ClippingCurveImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_ClippingCurve)
		public:
			Stk_ClippingCurveImpl();
			virtual ~Stk_ClippingCurveImpl();
		private:
			STK_BOOL						m_bDisplay;		//!<  «∑Òœ‘ æ∆ «–œﬂ
			StkClippingCurveDrawStyleEnum	m_eDrawStyle;	//!< ªÊ÷∆œ‘ æ∑Ω Ω
			StkClippingCurveDrawTypeEnum	m_eDrawType;	//!< ªÊ÷∆œ‘ æ¿‡–Õ
			STK_FLOAT32						m_fAngle;		//!< ∆ «–œﬂΩ«∂»
			STK_FLOAT32						m_fRatio;		//!< ∆ «–œﬂœ‘ æ±»¿˝
			STK_RGBA32						m_rgbaCurve;	//!< ∆ «–œﬂ—’…´
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒ ˝æ›
			*	@param[in]	i_pJsonNode		 ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document*i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥ ˝æ›
			*	@param[in]	Value* i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* pDoc);
		};

		STK_STATUS	Stk_ClippingCurveImpl::FromJson(Value* i_pJsonNode, Document*i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;

			return ret;
		}

		STK_STATUS	Stk_ClippingCurveImpl::ToJson(Value* i_pJsonNode, Document* pDoc)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(CLIPPINGCURVE_NODE_NAME_ISDISPLAY, m_bDisplay, pDoc->GetAllocator());
			i_pJsonNode->AddMember(CLIPPINGCURVE_NODE_NAME_DRAWSTYLE, m_eDrawStyle, pDoc->GetAllocator());
			i_pJsonNode->AddMember(CLIPPINGCURVE_NODE_NAME_DRAWTYPE, m_eDrawType, pDoc->GetAllocator());
			i_pJsonNode->AddMember(CLIPPINGCURVE_NODE_NAME_ANGLE, m_fAngle, pDoc->GetAllocator());
			i_pJsonNode->AddMember(CLIPPINGCURVE_NODE_NAME_DRAWTYPE, m_fRatio, pDoc->GetAllocator());
			string strValue;
			char szValue[256] = { 0 };
			sprintf(szValue, "%.8f", m_rgbaCurve.Red);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaCurve.Green);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaCurve.Blue);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_rgbaCurve.Alpha);
			strValue += szValue;
			i_pJsonNode->AddMember(CLIPPINGCURVE_NODE_NAME_COLOR, strValue, pDoc->GetAllocator());
			return ret;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_ClippingCurve)
		Stk_ClippingCurveImpl::Stk_ClippingCurveImpl()
		{

		}

		Stk_ClippingCurveImpl::~Stk_ClippingCurveImpl()
		{

		}
		STK_IMPLEMENT_CLASS(Stk_ClippingCurve, Stk_Object)

		Stk_ClippingCurve::Stk_ClippingCurve() :  Stk_Object(new Stk_ClippingCurveImpl())
		{

		}

		Stk_ClippingCurve::~Stk_ClippingCurve()
		{

		}

		STK_STATUS	Stk_ClippingCurve::SetDisplay(STK_BOOL i_bDisplay)
		{
			((Stk_ClippingCurveImpl*)m_pImpl)->m_bDisplay = i_bDisplay;
			return STK_SUCCESS;
		}

		STK_BOOL	Stk_ClippingCurve::GetDisplay()
		{
			return ((Stk_ClippingCurveImpl*)m_pImpl)->m_bDisplay;
		}

		STK_STATUS	Stk_ClippingCurve::SetDrawStyle(StkClippingCurveDrawStyleEnum i_eDrawStyle)
		{
			((Stk_ClippingCurveImpl*)m_pImpl)->m_eDrawStyle = i_eDrawStyle;
			return STK_SUCCESS;
		}
		
		StkClippingCurveDrawStyleEnum	Stk_ClippingCurve::GetDrawStyle()
		{
			return ((Stk_ClippingCurveImpl*)m_pImpl)->m_eDrawStyle;
		}

		STK_STATUS	Stk_ClippingCurve::SetDrawType(StkClippingCurveDrawTypeEnum i_eDrawType)
		{
			((Stk_ClippingCurveImpl*)m_pImpl)->m_eDrawType = i_eDrawType;
			return STK_SUCCESS;
		}

		StkClippingCurveDrawTypeEnum Stk_ClippingCurve::GetDrawType()
		{
			return ((Stk_ClippingCurveImpl*)m_pImpl)->m_eDrawType;
		}

		STK_STATUS	Stk_ClippingCurve::SetAngle(STK_FLOAT32 i_fAngle)
		{
			((Stk_ClippingCurveImpl*)m_pImpl)->m_fAngle = i_fAngle;
			return STK_SUCCESS;
		}

		STK_FLOAT32	Stk_ClippingCurve::GetAngle()
		{
			return ((Stk_ClippingCurveImpl*)m_pImpl)->m_fAngle;
		}

		STK_STATUS	Stk_ClippingCurve::SetRatio(STK_FLOAT32 i_fRatio)
		{
			((Stk_ClippingCurveImpl*)m_pImpl)->m_fRatio = i_fRatio;
			return STK_SUCCESS;
		}

		STK_FLOAT32	Stk_ClippingCurve::GetRatio()
		{
			return ((Stk_ClippingCurveImpl*)m_pImpl)->m_fRatio;
		}

		STK_STATUS	Stk_ClippingCurve::SetColor(STK_RGBA32 i_rgbaCurve)
		{
			((Stk_ClippingCurveImpl*)m_pImpl)->m_rgbaCurve;
			return STK_SUCCESS;
		}

		STK_RGBA32	Stk_ClippingCurve::GetColor()
		{
			return ((Stk_ClippingCurveImpl*)m_pImpl)->m_rgbaCurve;
		}

		STK_STATUS	Stk_ClippingCurve::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ClippingCurveImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_ClippingCurve::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ClippingCurveImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}
	}
}
