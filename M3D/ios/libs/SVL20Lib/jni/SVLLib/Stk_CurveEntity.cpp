#include "Stk_CurveEntity.h"
#include "Stk_Object.h"
#include "Stk_BaseDocumentImpl.h"
#include "StringTools.h"

#define CURVE_ID			"id"
#define CURVE_COLOR			"color"
#define	CURVE_TYPE			"type"
#define CURVE_USAGETYPE		"defType"
#define CURVE_DISPSTAT		"visible"
#define CURVE_DISPTYPE		"dispType"
#define CURVE_LINE_STARTPNT	"startPnt"
#define CURVE_LINE_ENDPNT	"endPnt"
//#define CURVE_POLYLINE_POINTNUM		"pointNum"
#define CURVE_POLYLINE_MIN			"min"
#define CURVE_POLYLINE_MAX			"max"
#define CURVE_POLYLINE_PNTPOINTS	"points"
#define CURVE_ELLIPSE_MIN			"min"
#define CURVE_ELLIPSE_MAX			"max"
#define CURVE_ELLIPSE_CENTERPNT		"center"
#define CURVE_ELLIPSE_XDIR			"xDir"
#define CURVE_ELLIPSE_YDIR			"yDir"
#define CURVE_ELLIPSE_ZDIR			"zDir"
#define CURVE_ELLIPSE_MAJOR_RADIUS	"majorRadius"
#define CURVE_ELLIPSE_MINOR_RADIUS	"minorRadius"
#define CURVE_ELLIPSE_START_POINT	"beginPnt"
#define CURVE_ELLIPSE_END_POINT		"endPnt"
#define CURVE_ELLIPSE_BEGIN_PARAMETER	"beginPar"
#define CURVE_ELLIPSE_END_PARAMETER		"endPar"
#define CURVE_ELLIPSE_NORMAL		"normal"
#define CURVE_ELLIPSE_ORIGIN_DIR	"originDir"

namespace HoteamSoft {
	namespace SVLLib {
		class Stk_CurveEntityImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_CurveEntity)
		public:
			Stk_CurveEntityImpl();
			virtual ~Stk_CurveEntityImpl(void);
		protected:
			STK_RGBA32				m_rgbaCurve;      //—’…´
			StkCurveTypeEnum		m_eCurveType;     //œﬂÃı¿‡–Õ
			StkCurveUsageEnum		m_eUsageType;     //œﬂÃıµƒ”¶”√¿‡–Õ
			STK_BOOL                m_bDispStat;   //œﬂÃı «∑Ò“˛≤ÿ
			StkCurveDispTypeEnum	m_eDispType;      //œﬂÃıµƒœ‘ æ¿‡–Õ
		};
		STK_CLASS_IMPL_IMPLEMENT(Stk_CurveEntity)
		Stk_CurveEntityImpl::Stk_CurveEntityImpl()
		{

		}

		Stk_CurveEntityImpl::~Stk_CurveEntityImpl(void)
		{

		}

		STK_IMPLEMENT_CLASS(Stk_CurveEntity, Stk_Object)

		Stk_CurveEntity::Stk_CurveEntity():Stk_Object(new Stk_CurveEntityImpl())
		{
		}


		Stk_CurveEntity::~Stk_CurveEntity()
		{
		}

		STK_RGBA32 Stk_CurveEntity::GetColor()
		{
			return ((Stk_CurveEntityImpl*)m_pImpl)->m_rgbaCurve;
		}

		STK_STATUS	Stk_CurveEntity::SetColor(const STK_RGBA32& i_rgbaCurve)
		{
			((Stk_CurveEntityImpl*)m_pImpl)->m_rgbaCurve = i_rgbaCurve;
			return STK_SUCCESS;
		}

		StkCurveTypeEnum Stk_CurveEntity::GetCurveType()
		{
			return ((Stk_CurveEntityImpl*)m_pImpl)->m_eCurveType;
		}

		void Stk_CurveEntity::SetCurveType(StkCurveTypeEnum i_eCurveType)
		{
			((Stk_CurveEntityImpl*)m_pImpl)->m_eCurveType = i_eCurveType;
		}

		StkCurveUsageEnum Stk_CurveEntity::GetUsageType()
		{
			return ((Stk_CurveEntityImpl*)m_pImpl)->m_eUsageType;
		}

		void Stk_CurveEntity::SetUsageType(StkCurveUsageEnum i_nuDefType)
		{
			((Stk_CurveEntityImpl*)m_pImpl)->m_eUsageType = i_nuDefType;
		}

		STK_STATUS	Stk_CurveEntity::SetCurveDispStat(STK_BOOL i_bDispStat)
		{
			((Stk_CurveEntityImpl*)m_pImpl)->m_bDispStat = i_bDispStat;
			return STK_SUCCESS;
		}

		STK_BOOL Stk_CurveEntity::GetCurveDispStat()
		{
			return ((Stk_CurveEntityImpl*)m_pImpl)->m_bDispStat;
		}

		StkCurveDispTypeEnum Stk_CurveEntity::GetCurveDispType()
		{
			return ((Stk_CurveEntityImpl*)m_pImpl)->m_eDispType;
		}

		STK_STATUS	Stk_CurveEntity::SetCurveDispType(StkCurveDispTypeEnum i_eDispType)
		{
			((Stk_CurveEntityImpl*)m_pImpl)->m_eDispType = i_eDispType;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_CurveEntity::FromJson(void* i_pJsonNode, void* i_JsonDocP)
		{
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_CurveEntity::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return STK_SUCCESS;
		}

		//Stk_LineImpl
		class Stk_LineImpl : public Stk_CurveEntityImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Line)
		public:
			Stk_LineImpl();
			virtual ~Stk_LineImpl(void);
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒStk_line ˝æ›
			*	@param[in]	i_pJsonNode			∏∏JsonΩ⁄µ„
			*	@param[in]	i_JsonDocP			JsonŒƒµµ∂‘œÛ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);
			/*****************************************************************************
			*	@brief		–¥Stk_line ˝æ›
			*	@param[in]	Value* i_pJsonNode			∏∏JsonΩ⁄µ„
			*	@param[in]	i_JsonDocP			JsonŒƒµµ∂‘œÛ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		private:
			STK_PNT32				m_pntStart;		//∆ ºµ„
			STK_PNT32				m_pntEnd;		//÷’÷πµ„
		};

		STK_CLASS_IMPL_IMPLEMENT(Stk_Line)
		Stk_LineImpl::Stk_LineImpl()
		{
			m_eCurveType = CURVE_TYPE_LINE;
			m_pntStart.X = 0.0f;
			m_pntStart.Y = 0.0f;
			m_pntStart.Z = 0.0f;
			m_pntEnd.X = 0.0f;
			m_pntEnd.Y = 0.0f;
			m_pntEnd.Z = 0.0f;
		}

		Stk_LineImpl::~Stk_LineImpl(void)
		{

		}

		HoteamSoft::SVLLib::STK_STATUS Stk_LineImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;

			m_ID = (*i_pJsonNode)[CURVE_ID].GetInt();
			string strColor = (*i_pJsonNode)[CURVE_COLOR].GetString();
			std::vector<std::string> vecColorValue;
			CStringTools::StringSplit(strColor, " ", vecColorValue);
			if (vecColorValue.size() == 4)
			{
				m_rgbaCurve.Red = atof(vecColorValue[0].c_str());
				m_rgbaCurve.Green = atof(vecColorValue[1].c_str());
				m_rgbaCurve.Blue = atof(vecColorValue[2].c_str());
				m_rgbaCurve.Alpha = atof(vecColorValue[3].c_str());
			}
			m_eCurveType = (StkCurveTypeEnum)(*i_pJsonNode)[CURVE_TYPE].GetInt();
			m_eUsageType = (StkCurveUsageEnum)(*i_pJsonNode)[CURVE_USAGETYPE].GetInt();
			m_bDispStat = (*i_pJsonNode)[CURVE_DISPSTAT].GetBool();
			m_eDispType = (StkCurveDispTypeEnum)(*i_pJsonNode)[CURVE_DISPTYPE].GetInt();
			string strStartPnt = (*i_pJsonNode)[CURVE_LINE_STARTPNT].GetString();
			vector<string> vecStartPnt;
			CStringTools::StringSplit(strStartPnt, " ", vecStartPnt);
			if (vecStartPnt.size() == 3)
			{
				m_pntStart.X = atof(vecStartPnt[0].c_str());
				m_pntStart.Y = atof(vecStartPnt[1].c_str());
				m_pntStart.Z = atof(vecStartPnt[2].c_str());
			}
			string strEndPnt = (*i_pJsonNode)[CURVE_LINE_ENDPNT].GetString();
			vector<string> vecEndPnt;
			CStringTools::StringSplit(strEndPnt, " ", vecEndPnt);
			if (vecEndPnt.size() == 3)
			{
				m_pntEnd.X = atof(vecEndPnt[0].c_str());
				m_pntEnd.Y = atof(vecEndPnt[1].c_str());
				m_pntEnd.Z = atof(vecEndPnt[2].c_str());
			}

			return ret;
		}

		STK_STATUS Stk_LineImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS ret = STK_SUCCESS;
			if (!i_pJsonNode || !i_JsonDocP)
				return STK_ERROR;
			string strValue;

			i_pJsonNode->SetObject();
			//ID
			i_pJsonNode->AddMember(CURVE_ID, m_ID, i_JsonDocP->GetAllocator());
			//—’…´
			strValue = "";
			char cColor[100] = { 0 };
			sprintf(cColor, "%.8f %.8f %.8f %.8f", m_rgbaCurve.Red, m_rgbaCurve.Green, m_rgbaCurve.Blue, m_rgbaCurve.Alpha);
			strValue = cColor;
			i_pJsonNode->AddMember(CURVE_COLOR, strValue, i_JsonDocP->GetAllocator());
			//∆‰À¸ Ù–‘
			i_pJsonNode->AddMember(CURVE_TYPE, m_eCurveType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_USAGETYPE, m_eUsageType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_DISPSTAT, m_bDispStat, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_DISPTYPE, m_eDispType, i_JsonDocP->GetAllocator());
			char cPntStart[100] = { 0 };
			sprintf(cPntStart, "%.8f %.8f %.8f", m_pntStart.X, m_pntStart.Y, m_pntStart.Z);
			strValue = cPntStart;
			i_pJsonNode->AddMember(CURVE_LINE_STARTPNT, strValue, i_JsonDocP->GetAllocator());
			char cPntEnd[100] = { 0 };
			sprintf(cPntEnd, "%.8f %.8f %.8f", m_pntEnd.X, m_pntEnd.Y, m_pntEnd.Z);
			strValue = cPntEnd;
			i_pJsonNode->AddMember(CURVE_LINE_ENDPNT, strValue, i_JsonDocP->GetAllocator());
			return ret;
		}

		STK_IMPLEMENT_CLASS(Stk_Line, Stk_CurveEntity)
		Stk_Line::Stk_Line(void) : Stk_CurveEntity(new Stk_LineImpl())
		{
			
		}

		Stk_Line::~Stk_Line(void)
		{

		}

		STK_PNT32	Stk_Line::GetStartPoint()
		{
			return ((Stk_LineImpl*)m_pImpl)->m_pntStart;
		}

		void Stk_Line::SetStartPoint(const STK_PNT32& i_pntStart)
		{
			((Stk_LineImpl*)m_pImpl)->m_pntStart = i_pntStart;
		}

		STK_PNT32 Stk_Line::GetEndPoint()
		{
			return ((Stk_LineImpl*)m_pImpl)->m_pntEnd;
		}

		void Stk_Line::SetEndPoint(const STK_PNT32& i_pntEnd)
		{
			((Stk_LineImpl*)m_pImpl)->m_pntEnd = i_pntEnd;
		}

		StkCurveTypeEnum	Stk_Line::GetCurveType()
		{
			return ((Stk_LineImpl*)m_pImpl)->m_eCurveType;
		}

		HoteamSoft::SVLLib::STK_STATUS Stk_Line::FromJson(void* i_pJsonNode, void* i_JsonDocP)
		{
			return ((Stk_LineImpl*)m_pImpl)->FromJson((Value*)i_pJsonNode, (Document*)i_JsonDocP);
		}

		STK_STATUS Stk_Line::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_LineImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}
		//Stk_PolyLineImpl
		class Stk_PolyLineImpl : public Stk_CurveEntityImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_PolyLine)
		public:
			Stk_PolyLineImpl();
			virtual ~Stk_PolyLineImpl(void);
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒStk_PolyLine ˝æ›
			*	@param[in]	i_pJsonNode			∏∏JsonΩ⁄µ„
			*	@param[in]	i_JsonDocP			JsonŒƒµµ∂‘œÛ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);
			/*****************************************************************************
			*	@brief		–¥Stk_PolyLine ˝æ›
			*	@param[in]	Value* i_pJsonNode			∏∏JsonΩ⁄µ„
			*	@param[in]	i_JsonDocP			JsonŒƒµµ∂‘œÛ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		private:
			STK_FLOAT32					m_fUMin;		//!< ∆ º∑∂Œß
			STK_FLOAT32					m_fUMax;		//!< ÷’÷π∑∂Œß
			vector<STK_PNT32>			m_pntPoints;	//!< µ„¡–
		};
		STK_CLASS_IMPL_IMPLEMENT(Stk_PolyLine)
		Stk_PolyLineImpl::Stk_PolyLineImpl()
		{
			m_eCurveType = CURVE_TYPE_POLYLINE;
			m_fUMin = 0.0f;
			m_fUMax = 0.0f;
			m_pntPoints.clear();
		}

		Stk_PolyLineImpl::~Stk_PolyLineImpl(void)
		{

		}

		HoteamSoft::SVLLib::STK_STATUS Stk_PolyLineImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;

			m_ID = (*i_pJsonNode)[CURVE_ID].GetInt();
			string strColor = (*i_pJsonNode)[CURVE_COLOR].GetString();
			std::vector<std::string> vecColorValue;
			CStringTools::StringSplit(strColor, " ", vecColorValue);
			if (vecColorValue.size() == 4)
			{
				m_rgbaCurve.Red = atof(vecColorValue[0].c_str());
				m_rgbaCurve.Green = atof(vecColorValue[1].c_str());
				m_rgbaCurve.Blue = atof(vecColorValue[2].c_str());
				m_rgbaCurve.Alpha = atof(vecColorValue[3].c_str());
			}
			m_eCurveType = (StkCurveTypeEnum)(*i_pJsonNode)[CURVE_TYPE].GetInt();
			m_eUsageType = (StkCurveUsageEnum)(*i_pJsonNode)[CURVE_USAGETYPE].GetInt();
			m_bDispStat = (*i_pJsonNode)[CURVE_DISPSTAT].GetBool();
			m_eDispType = (StkCurveDispTypeEnum)(*i_pJsonNode)[CURVE_DISPTYPE].GetInt();
			m_fUMin = (*i_pJsonNode)[CURVE_POLYLINE_MIN].GetFloat();
			m_fUMax = (*i_pJsonNode)[CURVE_POLYLINE_MAX].GetFloat();
			Value::MemberIterator pntItor = i_pJsonNode->FindMember(CURVE_POLYLINE_PNTPOINTS);
			if (pntItor != i_pJsonNode->MemberEnd())
			{
				Value valPoints = pntItor->value.GetArray();
				STK_PNT32 pnt;
				for (int i = 0; i < valPoints.Size(); i++)
				{
					Value valPoint;
					valPoint = valPoints[i];
					if (i % 3 == 0)
						pnt.X = valPoint.GetFloat();
					else if (i % 3 == 1)
						pnt.Y = valPoint.GetFloat();
					else if (i % 3 == 2)
					{
						pnt.Z = valPoint.GetFloat();
						m_pntPoints.push_back(pnt);
					}
				}
			}
			return ret;
		}

		HoteamSoft::SVLLib::STK_STATUS Stk_PolyLineImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS ret = STK_SUCCESS;
			if (!i_pJsonNode || !i_JsonDocP)
				return STK_ERROR;
			string strValue;

			i_pJsonNode->SetObject();
			//ID
			i_pJsonNode->AddMember(CURVE_ID, m_ID, i_JsonDocP->GetAllocator());
			//—’…´
			strValue = "";
			char cColor[100] = { 0 };
			sprintf(cColor, "%.8f %.8f %.8f %.8f", m_rgbaCurve.Red, m_rgbaCurve.Green, m_rgbaCurve.Blue, m_rgbaCurve.Alpha);
			strValue = cColor;
			i_pJsonNode->AddMember(CURVE_COLOR, strValue, i_JsonDocP->GetAllocator());
			//∆‰À¸ Ù–‘
			i_pJsonNode->AddMember(CURVE_TYPE, m_eCurveType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_USAGETYPE, m_eUsageType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_DISPSTAT, m_bDispStat, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_DISPTYPE, m_eDispType, i_JsonDocP->GetAllocator());
 			i_pJsonNode->AddMember(CURVE_POLYLINE_MIN, m_fUMin, i_JsonDocP->GetAllocator());
 			i_pJsonNode->AddMember(CURVE_POLYLINE_MAX, m_fUMax, i_JsonDocP->GetAllocator());
			int nPntSize = m_pntPoints.size();
			//i_pJsonNode->AddMember(CURVE_POLYLINE_POINTNUM, nPntSize, i_JsonDocP->GetAllocator());
			Value points;
			points.SetArray();
			for (int ii = 0; ii < nPntSize; ii++)
			{
				points.PushBack(m_pntPoints[ii].X, i_JsonDocP->GetAllocator());
				points.PushBack(m_pntPoints[ii].Y, i_JsonDocP->GetAllocator());
				points.PushBack(m_pntPoints[ii].Z, i_JsonDocP->GetAllocator());
			}
			i_pJsonNode->AddMember(CURVE_POLYLINE_PNTPOINTS, points, i_JsonDocP->GetAllocator());
			return ret;
		}

		STK_IMPLEMENT_CLASS(Stk_PolyLine, Stk_CurveEntity)
		Stk_PolyLine::Stk_PolyLine(void) : Stk_CurveEntity(new Stk_PolyLineImpl())
		{

		}

		Stk_PolyLine::~Stk_PolyLine(void)
		{

		}

		const vector<STK_PNT32>& Stk_PolyLine::GetPoints()
		{
			return ((Stk_PolyLineImpl*)m_pImpl)->m_pntPoints;
		}

		STK_STATUS Stk_PolyLine::SetPoints(const vector<STK_PNT32>& i_pntPoints)
		{
			((Stk_PolyLineImpl*)m_pImpl)->m_pntPoints = i_pntPoints;
			return STK_SUCCESS;
		}

		StkCurveTypeEnum	Stk_PolyLine::GetCurveType()
		{
			return ((Stk_PolyLineImpl*)m_pImpl)->m_eCurveType;
		}

		void Stk_PolyLine::GetRange(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax)
		{
			o_fUMin = ((Stk_PolyLineImpl*)m_pImpl)->m_fUMin;
			o_fUMax = ((Stk_PolyLineImpl*)m_pImpl)->m_fUMax;
		}

		STK_STATUS Stk_PolyLine::SetRange(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax)
		{
			((Stk_PolyLineImpl*)m_pImpl)->m_fUMin = i_fUMin;
			((Stk_PolyLineImpl*)m_pImpl)->m_fUMax = i_fUMax;
			return STK_SUCCESS;
		}

		HoteamSoft::SVLLib::STK_STATUS Stk_PolyLine::FromJson(void* i_pJsonNode, void* i_JsonDocP)
		{
			return ((Stk_PolyLineImpl*)m_pImpl)->FromJson((Value*)i_pJsonNode, (Document*)i_JsonDocP);
		}

		HoteamSoft::SVLLib::STK_STATUS Stk_PolyLine::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_PolyLineImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		//Stk_ArcImpl
		class Stk_ArcImpl : public Stk_CurveEntityImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Arc)
		public:
			Stk_ArcImpl();
			virtual ~Stk_ArcImpl(void);
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒStk_Arc ˝æ›
			*	@param[in]	i_pJsonNode			∏∏JsonΩ⁄µ„
			*	@param[in]	i_JsonDocP			JsonŒƒµµ∂‘œÛ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);
			/*****************************************************************************
			*	@brief		–¥Stk_Arc ˝æ›
			*	@param[in]	Value* i_pJsonNode			∏∏JsonΩ⁄µ„
			*	@param[in]	i_JsonDocP			JsonŒƒµµ∂‘œÛ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		private:
			STK_FLOAT32				m_fUMin;			//∆ º∑∂Œß
			STK_FLOAT32				m_fUMax;			//÷’÷π∑∂Œß
			STK_PNT32				m_pntCenter;		//÷––ƒµ„
			STK_DIR32				m_dirX;				//X∑ΩœÚœÚ¡ø
			STK_DIR32				m_dirY;				//Y∑ΩœÚœÚ¡ø
			STK_DIR32				m_dirZ;				//Z∑ΩœÚœÚ¡ø
			STK_FLOAT32				m_fMajorRadius;		//≥§∞Îæ∂
			STK_FLOAT32				m_fMinorRadius;		//∂Ã∞Îæ∂
			STK_PNT32				m_pntStart;			//∆µ„◊¯±Í
			STK_PNT32				m_pntEnd;			//÷’µ„◊¯±Í
			STK_FLOAT32				m_fStartPar;		//∆µ„≤Œ ˝Œª÷√
			STK_FLOAT32				m_fEndPar;			//÷’µ„≤Œ ˝Œª÷√
			STK_DIR32				m_dirNormal;		//‘≤ª°∑ΩœÚ
			STK_DIR32				m_dirOrigin;		//‘≠µ„∑ΩœÚ
		};
		STK_CLASS_IMPL_IMPLEMENT(Stk_Arc)
		Stk_ArcImpl::Stk_ArcImpl(void)
		{
			m_eCurveType = CURVE_TYPE_ELLIPSE;
			m_fUMin = 0.0f;
			m_fUMax = 0.0f;
			m_pntCenter.X = 0.0f;
			m_pntCenter.Y = 0.0f;
			m_pntCenter.Z = 0.0f;
			m_fMajorRadius = 0.0f;
			m_fMinorRadius = 0.0f;
			m_dirX.X = 0.0f;
			m_dirX.Y = 0.0f;
			m_dirX.Z = 0.0f;
			m_dirY.X = 0.0f;
			m_dirY.Y = 0.0f;
			m_dirY.Z = 0.0f;
			m_dirZ.X = 0.0f;
			m_dirZ.Y = 0.0f;
			m_dirZ.Z = 0.0f;
			m_dirNormal.X = 0.0f;
			m_dirNormal.Y = 0.0f;
			m_dirNormal.Z = 0.0f;
			m_dirOrigin.X = 0.0f;
			m_dirOrigin.Y = 0.0f;
			m_dirOrigin.Z = 0.0f;
		}

		Stk_ArcImpl::~Stk_ArcImpl(void)
		{

		}

		HoteamSoft::SVLLib::STK_STATUS Stk_ArcImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode)
				return STK_ERROR;

			m_ID = (*i_pJsonNode)[CURVE_ID].GetInt();
			string strColor = (*i_pJsonNode)[CURVE_COLOR].GetString();
			std::vector<std::string> vecColorValue;
			CStringTools::StringSplit(strColor, " ", vecColorValue);
			if (vecColorValue.size() == 4)
			{
				m_rgbaCurve.Red = atof(vecColorValue[0].c_str());
				m_rgbaCurve.Green = atof(vecColorValue[1].c_str());
				m_rgbaCurve.Blue = atof(vecColorValue[2].c_str());
				m_rgbaCurve.Alpha = atof(vecColorValue[3].c_str());
			}
			m_eCurveType = (StkCurveTypeEnum)(*i_pJsonNode)[CURVE_TYPE].GetInt();
			m_eUsageType = (StkCurveUsageEnum)(*i_pJsonNode)[CURVE_USAGETYPE].GetInt();
			m_bDispStat = (*i_pJsonNode)[CURVE_DISPSTAT].GetBool();
			m_eDispType = (StkCurveDispTypeEnum)(*i_pJsonNode)[CURVE_DISPTYPE].GetInt();
			m_fUMin = (*i_pJsonNode)[CURVE_ELLIPSE_MIN].GetFloat();
			m_fUMax = (*i_pJsonNode)[CURVE_ELLIPSE_MAX].GetFloat();
			string strValue = (*i_pJsonNode)[CURVE_ELLIPSE_CENTERPNT].GetString();
			vector<string> vecValue;
			CStringTools::StringSplit(strValue, " ", vecValue);
			if (vecValue.size() == 3)
			{
				m_pntCenter.X = atof(vecValue[0].c_str());
				m_pntCenter.Y = atof(vecValue[1].c_str());
				m_pntCenter.Z = atof(vecValue[2].c_str());
			}
			//xDir
			vecValue.clear();
			strValue = (*i_pJsonNode)[CURVE_ELLIPSE_XDIR].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			if (vecValue.size() == 3)
			{
				m_dirX.X = atof(vecValue[0].c_str());
				m_dirX.Y = atof(vecValue[1].c_str());
				m_dirX.Z = atof(vecValue[2].c_str());
			}
			//yDir
			vecValue.clear();
			strValue = (*i_pJsonNode)[CURVE_ELLIPSE_YDIR].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			if (vecValue.size() == 3)
			{
				m_dirY.X = atof(vecValue[0].c_str());
				m_dirY.Y = atof(vecValue[1].c_str());
				m_dirY.Z = atof(vecValue[2].c_str());
			}
			//zDir
			vecValue.clear();
			strValue = (*i_pJsonNode)[CURVE_ELLIPSE_ZDIR].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			if (vecValue.size() == 3)
			{
				m_dirZ.X = atof(vecValue[0].c_str());
				m_dirZ.Y = atof(vecValue[1].c_str());
				m_dirZ.Z = atof(vecValue[2].c_str());
			}
			//normal
			vecValue.clear();
			strValue = (*i_pJsonNode)[CURVE_ELLIPSE_NORMAL].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			if (vecValue.size() == 3)
			{
				m_dirNormal.X = atof(vecValue[0].c_str());
				m_dirNormal.Y = atof(vecValue[1].c_str());
				m_dirNormal.Z = atof(vecValue[2].c_str());
			}
			//originDir
			vecValue.clear();
			strValue = (*i_pJsonNode)[CURVE_ELLIPSE_ORIGIN_DIR].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			if (vecValue.size() == 3)
			{
				m_dirOrigin.X = atof(vecValue[0].c_str());
				m_dirOrigin.Y = atof(vecValue[1].c_str());
				m_dirOrigin.Z = atof(vecValue[2].c_str());
			}
			//majorRadius
			m_fMajorRadius = (*i_pJsonNode)[CURVE_ELLIPSE_MAJOR_RADIUS].GetFloat();
			//minorRadius
			m_fMinorRadius = (*i_pJsonNode)[CURVE_ELLIPSE_MINOR_RADIUS].GetFloat();
			//begin point
			vecValue.clear();
			strValue = (*i_pJsonNode)[CURVE_ELLIPSE_START_POINT].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			if (vecValue.size() == 3)
			{
				m_pntStart.X = atof(vecValue[0].c_str());
				m_pntStart.Y = atof(vecValue[1].c_str());
				m_pntStart.Z = atof(vecValue[2].c_str());
			}
			//end point
			vecValue.clear();
			strValue = (*i_pJsonNode)[CURVE_ELLIPSE_END_POINT].GetString();
			CStringTools::StringSplit(strValue, " ", vecValue);
			if (vecValue.size() == 3)
			{
				m_pntEnd.X = atof(vecValue[0].c_str());
				m_pntEnd.Y = atof(vecValue[1].c_str());
				m_pntEnd.Z = atof(vecValue[2].c_str());
			}
			//begin parameter
			m_fStartPar = (*i_pJsonNode)[CURVE_ELLIPSE_BEGIN_PARAMETER].GetFloat();
			//end parameter
			m_fEndPar = (*i_pJsonNode)[CURVE_ELLIPSE_END_PARAMETER].GetFloat();

			return ret;
		}

		HoteamSoft::SVLLib::STK_STATUS Stk_ArcImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS ret = STK_SUCCESS;
			if (!i_pJsonNode || !i_JsonDocP)
				return STK_ERROR;
			string strValue;

			i_pJsonNode->SetObject();
			//ID
			i_pJsonNode->AddMember(CURVE_ID, m_ID, i_JsonDocP->GetAllocator());
			//—’…´
			strValue = "";
			char cColor[100] = { 0 };
			sprintf(cColor, "%.8f %.8f %.8f %.8f", m_rgbaCurve.Red, m_rgbaCurve.Green, m_rgbaCurve.Blue, m_rgbaCurve.Alpha);
			strValue = cColor;
			i_pJsonNode->AddMember(CURVE_COLOR, strValue, i_JsonDocP->GetAllocator());
			//∆‰À¸ Ù–‘
			i_pJsonNode->AddMember(CURVE_TYPE, m_eCurveType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_USAGETYPE, m_eUsageType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_DISPSTAT, m_bDispStat, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_DISPTYPE, m_eDispType, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_ELLIPSE_MIN, m_fUMin, i_JsonDocP->GetAllocator());
			i_pJsonNode->AddMember(CURVE_ELLIPSE_MAX, m_fUMax, i_JsonDocP->GetAllocator());
			//centerPoint
			char cCenterPnt[100] = { 0 };
			sprintf(cCenterPnt, "%.8f %.8f %.8f", m_pntCenter.X, m_pntCenter.Y, m_pntCenter.Z);
			strValue = cCenterPnt;
			i_pJsonNode->AddMember(CURVE_ELLIPSE_CENTERPNT, strValue, i_JsonDocP->GetAllocator());
			//xDir
			char cXDir[100] = { 0 };
			sprintf(cXDir, "%.8f %.8f %.8f", m_dirX.X, m_dirX.Y, m_dirX.Z);
			strValue = cXDir;
			i_pJsonNode->AddMember(CURVE_ELLIPSE_XDIR, strValue, i_JsonDocP->GetAllocator());
			//yDir
			char cYDir[100] = { 0 };
			sprintf(cYDir, "%.8f %.8f %.8f", m_dirY.X, m_dirY.Y, m_dirY.Z);
			strValue = cYDir;
			i_pJsonNode->AddMember(CURVE_ELLIPSE_YDIR, strValue, i_JsonDocP->GetAllocator());
			//zDir
			char cZDir[100] = { 0 };
			sprintf(cZDir, "%.8f %.8f %.8f", m_dirZ.X, m_dirZ.Y, m_dirZ.Z);
			strValue = cZDir;
			i_pJsonNode->AddMember(CURVE_ELLIPSE_ZDIR, strValue, i_JsonDocP->GetAllocator());
			//normal
			char cNormal[100] = { 0 };
			sprintf(cNormal, "%.8f %.8f %.8f", m_dirNormal.X, m_dirNormal.Y, m_dirNormal.Z);
			strValue = cNormal;
			i_pJsonNode->AddMember(CURVE_ELLIPSE_NORMAL, strValue, i_JsonDocP->GetAllocator());
			//origin dir
			char cOriginDir[100] = { 0 };
			sprintf(cOriginDir, "%.8f %.8f %.8f", m_dirOrigin.X, m_dirOrigin.Y, m_dirOrigin.Z);
			strValue = cOriginDir;
			i_pJsonNode->AddMember(CURVE_ELLIPSE_ORIGIN_DIR, strValue, i_JsonDocP->GetAllocator());
			//major radius
			i_pJsonNode->AddMember(CURVE_ELLIPSE_MAJOR_RADIUS, m_fMajorRadius, i_JsonDocP->GetAllocator());
			//minor radius
			i_pJsonNode->AddMember(CURVE_ELLIPSE_MINOR_RADIUS, m_fMinorRadius, i_JsonDocP->GetAllocator());
			//begin point
			char cBeginPnt[100] = { 0 };
			sprintf(cBeginPnt, "%.8f %.8f %.8f", m_pntStart.X, m_pntStart.Y, m_pntStart.Z);
			strValue = cBeginPnt;
			i_pJsonNode->AddMember(CURVE_ELLIPSE_START_POINT, strValue, i_JsonDocP->GetAllocator());
			//end point
			char cEndPnt[100] = { 0 };
			sprintf(cEndPnt, "%.8f %.8f %.8f", m_pntEnd.X, m_pntEnd.Y, m_pntEnd.Z);
			strValue = cEndPnt;
			i_pJsonNode->AddMember(CURVE_ELLIPSE_END_POINT, strValue, i_JsonDocP->GetAllocator());
			//begin parameter
			i_pJsonNode->AddMember(CURVE_ELLIPSE_BEGIN_PARAMETER, m_fStartPar, i_JsonDocP->GetAllocator());
			//end parameter
			i_pJsonNode->AddMember(CURVE_ELLIPSE_END_PARAMETER, m_fEndPar, i_JsonDocP->GetAllocator());
			
			return ret;
		}

		STK_IMPLEMENT_CLASS(Stk_Arc, Stk_CurveEntity)
		Stk_Arc::Stk_Arc(void) : Stk_CurveEntity(new Stk_ArcImpl())
		{

		}

		Stk_Arc::~Stk_Arc(void)
		{

		}

		void Stk_Arc::GetRange(STK_FLOAT32& o_fUMin, STK_FLOAT32& o_fUMax)
		{
			o_fUMin = ((Stk_ArcImpl*)m_pImpl)->m_fUMin;
			o_fUMax = ((Stk_ArcImpl*)m_pImpl)->m_fUMax;
		}
		
		STK_STATUS	Stk_Arc::SetRange(STK_FLOAT32 i_fUMin, STK_FLOAT32 i_fUMax)
		{
			((Stk_ArcImpl*)m_pImpl)->m_fUMin = i_fUMin;
			((Stk_ArcImpl*)m_pImpl)->m_fUMax = i_fUMax;
			return STK_SUCCESS;
		}

		STK_PNT32 Stk_Arc::GetCenterPoint()
		{
			return ((Stk_ArcImpl*)m_pImpl)->m_pntCenter;
		}

		STK_STATUS	Stk_Arc::SetCenterPoint(const STK_PNT32& i_pntCenter)
		{
			((Stk_ArcImpl*)m_pImpl)->m_pntCenter = i_pntCenter;
			return STK_SUCCESS;
		}

		void Stk_Arc::GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ)
		{
			o_dirX = ((Stk_ArcImpl*)m_pImpl)->m_dirX;
			o_dirY = ((Stk_ArcImpl*)m_pImpl)->m_dirY;
			o_dirZ = ((Stk_ArcImpl*)m_pImpl)->m_dirZ;
		}

		STK_STATUS	Stk_Arc::SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ)
		{
			((Stk_ArcImpl*)m_pImpl)->m_dirX = i_dirX;
			((Stk_ArcImpl*)m_pImpl)->m_dirY = i_dirY;
			((Stk_ArcImpl*)m_pImpl)->m_dirZ = i_dirZ;
			return STK_SUCCESS;
		}

		void Stk_Arc::SetNormal(const STK_DIR32& i_dirNormal)
		{
			((Stk_ArcImpl*)m_pImpl)->m_dirNormal = i_dirNormal;
		}

		STK_DIR32 Stk_Arc::GetNormal()
		{
			return ((Stk_ArcImpl*)m_pImpl)->m_dirNormal;
		}

		void Stk_Arc::SetOriginDir(const STK_DIR32& i_dirOrigin)
		{
			((Stk_ArcImpl*)m_pImpl)->m_dirOrigin = i_dirOrigin;
		}

		STK_DIR32	Stk_Arc::GetOriginDir()
		{
			return ((Stk_ArcImpl*)m_pImpl)->m_dirOrigin;
		}

		void Stk_Arc::GetRadius(STK_FLOAT32& o_fMajorR, STK_FLOAT32& o_fMinorR)
		{
			o_fMajorR = ((Stk_ArcImpl*)m_pImpl)->m_fMajorRadius;
			o_fMinorR = ((Stk_ArcImpl*)m_pImpl)->m_fMinorRadius;
		}

		STK_STATUS Stk_Arc::SetRadius(STK_FLOAT32 i_fMajorR, STK_FLOAT32 i_fMinorR)
		{
			((Stk_ArcImpl*)m_pImpl)->m_fMajorRadius = i_fMajorR;
			((Stk_ArcImpl*)m_pImpl)->m_fMinorRadius = i_fMinorR;
			return STK_SUCCESS;
		}

		void Stk_Arc::GetCoordinatePnt(STK_PNT32& o_pntStart, STK_PNT32& o_pntEnd)
		{
			o_pntStart = ((Stk_ArcImpl*)m_pImpl)->m_pntStart;
			o_pntEnd = ((Stk_ArcImpl*)m_pImpl)->m_pntEnd;
		}

		StkCurveTypeEnum	Stk_Arc::GetCurveType()
		{
			return ((Stk_ArcImpl*)m_pImpl)->m_eCurveType;
		}

		STK_STATUS Stk_Arc::SetCoordiantePnt(const STK_PNT32& i_pntStart, const STK_PNT32& i_pntEnd)
		{
			((Stk_ArcImpl*)m_pImpl)->m_pntStart = i_pntStart;
			((Stk_ArcImpl*)m_pImpl)->m_pntEnd = i_pntEnd;
			return STK_SUCCESS;
		}

		void Stk_Arc::GetParameter(STK_FLOAT32& o_fStartPar, STK_FLOAT32& o_fEndPar)
		{
			o_fStartPar = ((Stk_ArcImpl*)m_pImpl)->m_fStartPar;
			o_fEndPar = ((Stk_ArcImpl*)m_pImpl)->m_fEndPar;
		}

		STK_STATUS	Stk_Arc::SetParameter(STK_FLOAT32 i_fStartPar, STK_FLOAT32 i_fEndPar)
		{
			((Stk_ArcImpl*)m_pImpl)->m_fStartPar = i_fStartPar;
			((Stk_ArcImpl*)m_pImpl)->m_fEndPar = i_fEndPar;
			return STK_SUCCESS;
		}

		HoteamSoft::SVLLib::STK_STATUS Stk_Arc::FromJson(void* i_pJsonNode, void* i_JsonDocP)
		{
			return ((Stk_ArcImpl*)m_pImpl)->FromJson((Value*)i_pJsonNode, (Document*)i_JsonDocP);
		}

		HoteamSoft::SVLLib::STK_STATUS Stk_Arc::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ArcImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}


		//Stk_NurbsCurveImpl
		class Stk_NurbsCurveImpl : public Stk_CurveEntityImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_NurbsCurve)
		public:
			Stk_NurbsCurveImpl();
			virtual ~Stk_NurbsCurveImpl(void);
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒStk_NurbsCurve ˝æ›
			*	@param[in]	i_strJson			Json∏Ò Ω◊÷∑˚¥Æ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		–¥Stk_NurbsCurve ˝æ›
			*	@param[in]	Œﬁ
			*	@param[out]	Œﬁ
			*	@retval		Json∏Ò Ω◊÷∑˚¥Æ
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			string					ToJson();
		private:
			STK_FLOAT32					m_fUMin;		//!< ∆ º∑∂Œß
			STK_FLOAT32					m_fUMax;		//!< ÷’÷π∑∂Œß
			STK_UINT32				m_nuDegree;			//!< degree÷µ
			vector<STK_UINT32>		m_vcKnotPlicity;	//!< ∂‡—˘–‘
			vector<STK_FLOAT32>		m_vcKnots;			//!< KnotsΩ⁄µ„
			vector<STK_PNT32>		m_vcPoints;			//!< øÿ÷∆µ„
			vector<STK_FLOAT32>		m_vcWeights;		//!< »®÷ÿ
		};
		STK_CLASS_IMPL_IMPLEMENT(Stk_NurbsCurve)
			Stk_NurbsCurveImpl::Stk_NurbsCurveImpl()
		{
			m_eCurveType = CURVE_TYPE_NURBSCURVE;
			m_fUMin = 0.0f;
			m_fUMax = 0.0f;
			m_nuDegree = 0;
		}

		Stk_NurbsCurveImpl::~Stk_NurbsCurveImpl(void)
		{

		}

		STK_STATUS Stk_NurbsCurveImpl::FromJson(string i_strJson)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (i_strJson.empty())
				return STK_ERROR;

			
			return ret;
		}

		string Stk_NurbsCurveImpl::ToJson()
		{
			string strJson = "";
			
			return strJson;
		}

		STK_IMPLEMENT_CLASS(Stk_NurbsCurve, Stk_CurveEntity)
			Stk_NurbsCurve::Stk_NurbsCurve(void) : Stk_CurveEntity(new Stk_NurbsCurveImpl())
		{

		}

		Stk_NurbsCurve::~Stk_NurbsCurve(void)
		{
			DELETE_STK_CLASS_IMPL
		}


		STK_STATUS Stk_NurbsCurve::GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax)
		{
			o_fMin = ((Stk_NurbsCurveImpl*)m_pImpl)->m_fUMin;
			o_fMax = ((Stk_NurbsCurveImpl*)m_pImpl)->m_fUMax;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_NurbsCurve::SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax)
		{
			((Stk_NurbsCurveImpl*)m_pImpl)->m_fUMin = i_fMin;
			((Stk_NurbsCurveImpl*)m_pImpl)->m_fUMax = i_fMax;

			return STK_SUCCESS;
		}

		STK_UINT32 Stk_NurbsCurve::GetDegree()
		{
			return ((Stk_NurbsCurveImpl*)m_pImpl)->m_nuDegree;
		}

		STK_STATUS Stk_NurbsCurve::SetDegree(STK_UINT32 i_nuDegree)
		{
			((Stk_NurbsCurveImpl*)m_pImpl)->m_nuDegree = i_nuDegree;

			return STK_SUCCESS;
		}

		vector<STK_UINT32> Stk_NurbsCurve::GetKnotMultiplicity()
		{
			return ((Stk_NurbsCurveImpl*)m_pImpl)->m_vcKnotPlicity;
		}

		STK_STATUS Stk_NurbsCurve::AddKnotMultiplicity(STK_UINT32 i_nuKnotPlicity)
		{
			((Stk_NurbsCurveImpl*)m_pImpl)->m_vcKnotPlicity.push_back(i_nuKnotPlicity);

			return STK_SUCCESS;
		}

		vector<STK_FLOAT32> Stk_NurbsCurve::GetKnots()
		{
			return ((Stk_NurbsCurveImpl*)m_pImpl)->m_vcKnots;
		}

		STK_STATUS Stk_NurbsCurve::AddKnot(STK_FLOAT32 i_fKnot)
		{
			((Stk_NurbsCurveImpl*)m_pImpl)->m_vcKnots.push_back(i_fKnot);

			return STK_SUCCESS;
		}

		vector<STK_PNT32> Stk_NurbsCurve::GetPoints()
		{
			return ((Stk_NurbsCurveImpl*)m_pImpl)->m_vcPoints;
		}

		STK_STATUS Stk_NurbsCurve::AddPoint(STK_PNT32 i_pntCtrl)
		{
			((Stk_NurbsCurveImpl*)m_pImpl)->m_vcPoints.push_back(i_pntCtrl);

			return STK_SUCCESS;
		}

		vector<STK_FLOAT32> Stk_NurbsCurve::GetWeights()
		{
			return ((Stk_NurbsCurveImpl*)m_pImpl)->m_vcWeights;
		}

		STK_STATUS Stk_NurbsCurve::AddWeight(STK_FLOAT32 i_fWeight)
		{
			((Stk_NurbsCurveImpl*)m_pImpl)->m_vcWeights.push_back(i_fWeight);

			return STK_SUCCESS;
		}

		STK_STATUS Stk_NurbsCurve::WriteVertex(STK_FLOAT32* o_fAtr, STK_PNT32 i_Point)
		{
			o_fAtr[0] = i_Point.X;
			o_fAtr[1] = i_Point.Y;
			o_fAtr[2] = i_Point.Z;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_NurbsCurve::FromJson(string i_strJson)
		{
			return ((Stk_NurbsCurveImpl*)m_pImpl)->FromJson(i_strJson);
		}

		string Stk_NurbsCurve::ToJson()
		{
			return ((Stk_NurbsCurveImpl*)m_pImpl)->ToJson();
		}


		//Stk_HyperbolaImpl
		class Stk_HyperbolaImpl : public Stk_CurveEntityImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Hyperbola)
		public:
			Stk_HyperbolaImpl();
			virtual ~Stk_HyperbolaImpl(void);
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒStk_Hyperbola ˝æ›
			*	@param[in]	i_strJson			Json∏Ò Ω◊÷∑˚¥Æ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		–¥Stk_Hyperbola ˝æ›
			*	@param[in]	Œﬁ
			*	@param[out]	Œﬁ
			*	@retval		Json∏Ò Ω◊÷∑˚¥Æ
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			string					ToJson();
		private:
			STK_FLOAT32				m_fUMin;			//!< ∆ º∑∂Œß
			STK_FLOAT32				m_fUMax;			//!< ÷’÷π∑∂Œß
			STK_PNT32				m_pntCenter;		//!< ÷––ƒµ„
			STK_DIR32				m_dirX;				//!< X∑ΩœÚœÚ¡ø
			STK_DIR32				m_dirY;				//!< Y∑ΩœÚœÚ¡ø
			STK_DIR32				m_dirZ;				//!< Z∑ΩœÚœÚ¡ø
			STK_FLOAT32				m_fSemiAxis;		//!< ∞Î÷·
			STK_FLOAT32				m_fSemiImgAxis;
			STK_PNT32				m_pntStart;			//!< ∆µ„◊¯±Í
			STK_PNT32				m_pntEnd;			//!< ÷’µ„◊¯±Í
		};
		STK_CLASS_IMPL_IMPLEMENT(Stk_Hyperbola)
			Stk_HyperbolaImpl::Stk_HyperbolaImpl()
		{
			m_eCurveType = CURVE_TYPE_HYPERBOLA;
			m_fUMin = 0.0f;
			m_fUMax = 0.0f;
			m_pntCenter.X = 0.0f;
			m_pntCenter.Y = 0.0f;
			m_pntCenter.Z = 0.0f;
			m_dirX.X = 0.0f;
			m_dirX.Y = 0.0f;
			m_dirX.Z = 0.0f;
			m_dirY.X = 0.0f;
			m_dirY.Y = 0.0f;
			m_dirY.Z = 0.0f;
			m_dirZ.X = 0.0f;
			m_dirZ.Y = 0.0f;
			m_dirZ.Z = 0.0f;
			m_fSemiAxis = 0.0f;
			m_fSemiImgAxis = 0.0f;
			m_pntStart.X = 0.0f;
			m_pntStart.Y = 0.0f;
			m_pntStart.Z = 0.0f;
			m_pntEnd.X = 0.0f;
			m_pntEnd.Y = 0.0f;
			m_pntEnd.Z = 0.0f;
		}

		Stk_HyperbolaImpl::~Stk_HyperbolaImpl(void)
		{

		}

		STK_STATUS Stk_HyperbolaImpl::FromJson(string i_strJson)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (i_strJson.empty())
				return STK_ERROR;


			return ret;
		}

		string Stk_HyperbolaImpl::ToJson()
		{
			string strJson = "";

			return strJson;
		}
		
		STK_IMPLEMENT_CLASS(Stk_Hyperbola, Stk_CurveEntity)
			Stk_Hyperbola::Stk_Hyperbola(void) : Stk_CurveEntity(new Stk_HyperbolaImpl())
		{

		}

		Stk_Hyperbola::~Stk_Hyperbola(void)
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_STATUS Stk_Hyperbola::GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax)
		{
			o_fMin = ((Stk_HyperbolaImpl*)m_pImpl)->m_fUMin;
			o_fMax = ((Stk_HyperbolaImpl*)m_pImpl)->m_fUMax;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Hyperbola::SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax)
		{
			((Stk_HyperbolaImpl*)m_pImpl)->m_fUMin = i_fMin;
			((Stk_HyperbolaImpl*)m_pImpl)->m_fUMax = i_fMax;

			return STK_SUCCESS;
		}

		STK_PNT32 Stk_Hyperbola::GetCenterPoint()
		{
			return ((Stk_HyperbolaImpl*)m_pImpl)->m_pntCenter;
		}

		STK_STATUS Stk_Hyperbola::SetCenterPoint(STK_PNT32& i_pntCenter)
		{
			((Stk_HyperbolaImpl*)m_pImpl)->m_pntCenter = i_pntCenter;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Hyperbola::GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ)
		{
			o_dirX = ((Stk_HyperbolaImpl*)m_pImpl)->m_dirX;
			o_dirY = ((Stk_HyperbolaImpl*)m_pImpl)->m_dirY;
			o_dirZ = ((Stk_HyperbolaImpl*)m_pImpl)->m_dirZ;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Hyperbola::SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ)
		{
			((Stk_HyperbolaImpl*)m_pImpl)->m_dirX = i_dirX;
			((Stk_HyperbolaImpl*)m_pImpl)->m_dirY = i_dirY;
			((Stk_HyperbolaImpl*)m_pImpl)->m_dirZ = i_dirZ;

			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Hyperbola::GetSemiAxis()
		{
			return ((Stk_HyperbolaImpl*)m_pImpl)->m_fSemiAxis;
		}

		STK_STATUS Stk_Hyperbola::SetSemiAxis(STK_FLOAT32 i_fSemiAxis)
		{
			((Stk_HyperbolaImpl*)m_pImpl)->m_fSemiAxis = i_fSemiAxis;

			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Hyperbola::GetSemiImgAxis()
		{
			return ((Stk_HyperbolaImpl*)m_pImpl)->m_fSemiImgAxis;
		}

		STK_STATUS Stk_Hyperbola::SetSemiImgAxis(STK_FLOAT32 i_fSemiImgAxis)
		{
			((Stk_HyperbolaImpl*)m_pImpl)->m_fSemiImgAxis = i_fSemiImgAxis;

			return STK_SUCCESS;
		}

		STK_PNT32 Stk_Hyperbola::GetStartPoint()
		{
			return ((Stk_HyperbolaImpl*)m_pImpl)->m_pntStart;
		}

		STK_STATUS Stk_Hyperbola::SetStartPoint(STK_PNT32 i_pntStart)
		{
			((Stk_HyperbolaImpl*)m_pImpl)->m_pntStart = i_pntStart;

			return STK_SUCCESS;
		}

		STK_PNT32 Stk_Hyperbola::GetEndPoint()
		{
			return ((Stk_HyperbolaImpl*)m_pImpl)->m_pntEnd;
		}

		STK_STATUS Stk_Hyperbola::SetEndPoint(STK_PNT32 i_pntEnd)
		{
			((Stk_HyperbolaImpl*)m_pImpl)->m_pntEnd = i_pntEnd;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Hyperbola::FromJson(string i_strJson)
		{
			return ((Stk_HyperbolaImpl*)m_pImpl)->FromJson(i_strJson);
		}

		string Stk_Hyperbola::ToJson()
		{
			return ((Stk_HyperbolaImpl*)m_pImpl)->ToJson();
		}


		//Stk_ParabolaImpl
		class Stk_ParabolaImpl : public Stk_CurveEntityImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_Parabola)
		public:
			Stk_ParabolaImpl();
			virtual ~Stk_ParabolaImpl(void);
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒStk_Parabola ˝æ›
			*	@param[in]	i_strJson			Json∏Ò Ω◊÷∑˚¥Æ
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(string i_strJson);
			/*****************************************************************************
			*	@brief		–¥Stk_Parabola ˝æ›
			*	@param[in]	Œﬁ
			*	@param[out]	Œﬁ
			*	@retval		Json∏Ò Ω◊÷∑˚¥Æ
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			string					ToJson();
		private:
			STK_FLOAT32				m_fUMin;			//!< ∆ º∑∂Œß
			STK_FLOAT32				m_fUMax;			//!< ÷’÷π∑∂Œß
			STK_PNT32				m_pntStart;			//!< ∆µ„◊¯±Í
			STK_PNT32				m_pntEnd;			//!< ÷’µ„◊¯±Í
			STK_PNT32				m_pntCenter;		//!< ÷––ƒµ„
			STK_DIR32				m_dirX;				//!< X∑ΩœÚœÚ¡ø
			STK_DIR32				m_dirY;				//!< Y∑ΩœÚœÚ¡ø
			STK_DIR32				m_dirZ;				//!< Z∑ΩœÚœÚ¡ø
			STK_FLOAT32				m_fFocalDis;		//!< Ωπµ„æ‡¿Î
		};
		STK_CLASS_IMPL_IMPLEMENT(Stk_Parabola)
			Stk_ParabolaImpl::Stk_ParabolaImpl()
		{
			m_eCurveType = CURVE_TYPE_PARABOLA;
			m_fUMin = 0.0f;
			m_fUMax = 0.0f;
			m_pntStart.X = 0.0f;
			m_pntStart.Y = 0.0f;
			m_pntStart.Z = 0.0f;
			m_pntEnd.X = 0.0f;
			m_pntEnd.Y = 0.0f;
			m_pntEnd.Z = 0.0f;
			m_pntCenter.X = 0.0f;
			m_pntCenter.Y = 0.0f;
			m_pntCenter.Z = 0.0f;
			m_dirX.X = 0.0f;
			m_dirX.Y = 0.0f;
			m_dirX.Z = 0.0f;
			m_dirY.X = 0.0f;
			m_dirY.Y = 0.0f;
			m_dirY.Z = 0.0f;
			m_dirZ.X = 0.0f;
			m_dirZ.Y = 0.0f;
			m_dirZ.Z = 0.0f;
			m_fFocalDis = 0.0f;
		}

		Stk_ParabolaImpl::~Stk_ParabolaImpl(void)
		{

		}

		STK_STATUS Stk_ParabolaImpl::FromJson(string i_strJson)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (i_strJson.empty())
				return STK_ERROR;


			return ret;
		}

		string Stk_ParabolaImpl::ToJson()
		{
			string strJson = "";

			return strJson;
		}

		STK_IMPLEMENT_CLASS(Stk_Parabola, Stk_CurveEntity)
			Stk_Parabola::Stk_Parabola(void) : Stk_CurveEntity(new Stk_ParabolaImpl())
		{

		}

		Stk_Parabola::~Stk_Parabola(void)
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_STATUS Stk_Parabola::GetDomain(STK_FLOAT32& o_fMin, STK_FLOAT32& o_fMax)
		{
			o_fMin = ((Stk_ParabolaImpl*)m_pImpl)->m_fUMin;
			o_fMax = ((Stk_ParabolaImpl*)m_pImpl)->m_fUMax;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Parabola::SetDomain(STK_FLOAT32 i_fMin, STK_FLOAT32 i_fMax)
		{
			((Stk_ParabolaImpl*)m_pImpl)->m_fUMin = i_fMin;
			((Stk_ParabolaImpl*)m_pImpl)->m_fUMax = i_fMax;

			return STK_SUCCESS;
		}

		STK_PNT32 Stk_Parabola::GetStartPoint()
		{
			return ((Stk_ParabolaImpl*)m_pImpl)->m_pntStart;
		}

		STK_STATUS Stk_Parabola::SetStartPoint(STK_PNT32 i_pntStart)
		{
			((Stk_ParabolaImpl*)m_pImpl)->m_pntStart = i_pntStart;

			return STK_SUCCESS;
		}

		STK_PNT32 Stk_Parabola::GetEndPoint()
		{
			return ((Stk_ParabolaImpl*)m_pImpl)->m_pntEnd;
		}

		STK_STATUS Stk_Parabola::SetEndPoint(STK_PNT32 i_pntEnd)
		{
			((Stk_ParabolaImpl*)m_pImpl)->m_pntEnd = i_pntEnd;

			return STK_SUCCESS;
		}

		STK_PNT32 Stk_Parabola::GetCenterPoint()
		{
			return ((Stk_ParabolaImpl*)m_pImpl)->m_pntCenter;
		}

		STK_STATUS Stk_Parabola::SetCenterPoint(STK_PNT32& i_pntCenter)
		{
			((Stk_ParabolaImpl*)m_pImpl)->m_pntCenter = i_pntCenter;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Parabola::GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ)
		{
			o_dirX = ((Stk_ParabolaImpl*)m_pImpl)->m_dirX;
			o_dirY = ((Stk_ParabolaImpl*)m_pImpl)->m_dirY;
			o_dirZ = ((Stk_ParabolaImpl*)m_pImpl)->m_dirZ;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Parabola::SetXYZDir(STK_DIR32& i_dirX, STK_DIR32& i_dirY, STK_DIR32& i_dirZ)
		{
			((Stk_ParabolaImpl*)m_pImpl)->m_dirX = i_dirX;
			((Stk_ParabolaImpl*)m_pImpl)->m_dirY = i_dirY;
			((Stk_ParabolaImpl*)m_pImpl)->m_dirZ = i_dirZ;

			return STK_SUCCESS;
		}

		STK_FLOAT32 Stk_Parabola::GetFocalDistance()
		{
			return ((Stk_ParabolaImpl*)m_pImpl)->m_fFocalDis;
		}

		STK_STATUS Stk_Parabola::SetFocalDistance(STK_FLOAT32 i_fDistance)
		{
			((Stk_ParabolaImpl*)m_pImpl)->m_fFocalDis = i_fDistance;

			return STK_SUCCESS;
		}

		STK_STATUS Stk_Parabola::FromJson(string i_strJson)
		{
			return ((Stk_ParabolaImpl*)m_pImpl)->FromJson(i_strJson);
		}

		string Stk_Parabola::ToJson()
		{
			return ((Stk_ParabolaImpl*)m_pImpl)->ToJson();
		}

	}
}
