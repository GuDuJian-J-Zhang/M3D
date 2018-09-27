#include "Stk_ObjectImpl.h"
#include "Stk_BaseDocumentImpl.h"
#include "Stk_GeometryAttribute.h"

#define  GEOMETRYATTRIBUTE_NODE_NAME_TYPE "type"
#define  GEOMETRYATTRIBUTE_NODE_NAME_NORMAL "normal"
#define  GEOMETRYATTRIBUTE_NODE_NAME_ORIGIN "origin"
#define  GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN "axisOrigin"
#define  GEOMETRYATTRIBUTE_NODE_NAME_AXISDIRECTION "axisDirection"
#define  GEOMETRYATTRIBUTE_NODE_NAME_ORIGINRADIUS "originRadius"
#define  GEOMETRYATTRIBUTE_NODE_NAME_MAJORRADIUS "originRadius"
#define  GEOMETRYATTRIBUTE_NODE_NAME_MINORRADIUS "originRadius"
#define  GEOMETRYATTRIBUTE_NODE_NAME_CENTERPOINT "centerPoint"
#define  GEOMETRYATTRIBUTE_NODE_NAME_DIRECTION "direction"
#define  GEOMETRYATTRIBUTE_NODE_NAME_STARTPOINT "startPoint"
#define  GEOMETRYATTRIBUTE_NODE_NAME_ENDPOINT "endPoint"
#define  GEOMETRYATTRIBUTE_NODE_NAME_LENGTH "length"
#define  GEOMETRYATTRIBUTE_NODE_NAME_XDIRECTION "xDirection"
#define  GEOMETRYATTRIBUTE_NODE_NAME_YDIRECTION "yDirection"
#define  GEOMETRYATTRIBUTE_NODE_NAME_ZDIRECTION "zDirection"

namespace HoteamSoft {
	namespace SVLLib {
        
        class Stk_GeometryAttributeImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_GeometryAttribute)
		public:
			Stk_GeometryAttributeImpl();
			virtual ~Stk_GeometryAttributeImpl(void);
		protected:
			StkGeoAttrTypeEnum     m_eGeoAttrType;   //º∏∫Œ Ù–‘¿‡–Õ
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_GeometryAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !i_JsonDocP)
				return STK_ERROR;
			return ret;
		}

		STK_STATUS	Stk_GeometryAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_SUCCESS;
			if (!i_pJsonNode || !i_JsonDocP)
				return STK_ERROR;
			return ret;
		}

		Stk_GeometryAttributeImpl::Stk_GeometryAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_UNKNOWN;
		}

		Stk_GeometryAttributeImpl::~Stk_GeometryAttributeImpl(void)
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_GeometryAttribute)
		Stk_GeometryAttribute::Stk_GeometryAttribute() : Stk_Object(new Stk_GeometryAttributeImpl())
		{

		}

		Stk_GeometryAttribute::~Stk_GeometryAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}
		STK_IMPLEMENT_CLASS(Stk_GeometryAttribute, Stk_Object)

		StkGeoAttrTypeEnum	Stk_GeometryAttribute::GetGeoAttrType()
		{
			return ((Stk_GeometryAttributeImpl*)m_pImpl)->m_eGeoAttrType;
		}

		STK_STATUS	Stk_GeometryAttribute::SetGeoAttrType(StkGeoAttrTypeEnum i_eAttrType)
		{
			((Stk_GeometryAttributeImpl*)m_pImpl)->m_eGeoAttrType = i_eAttrType;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_GeometryAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_GeometryAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_GeometryAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_GeometryAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_PlaneFaceAttributeImpl : public Stk_GeometryAttributeImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_PlaneFaceAttribute)
		public:
			Stk_PlaneFaceAttributeImpl();
			virtual ~Stk_PlaneFaceAttributeImpl(void);
		private:
			STK_DIR32			m_dirNormal;								//∆Ω√Ê∑®œÚ
			STK_PNT32			m_pntOrigin;								//∆Ω√Ê‘≠µ„
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_PlaneFaceAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_TYPE, m_eGeoAttrType, i_JsonDocP->GetAllocator());
			string strValue;
			char szValue[256] = { 0 };
			//normal
			sprintf(szValue, "%.8f", m_dirNormal.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirNormal.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirNormal.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_NORMAL, strValue, i_JsonDocP->GetAllocator());
			//origin
			strValue.clear();
			sprintf(szValue, "%.8f", m_pntOrigin.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntOrigin.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntOrigin.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ORIGIN, strValue, i_JsonDocP->GetAllocator());
			return ret;
		}

		STK_STATUS	Stk_PlaneFaceAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;

			return ret;
		}

		Stk_PlaneFaceAttributeImpl::Stk_PlaneFaceAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_PLANEFACE;
		}

		Stk_PlaneFaceAttributeImpl::~Stk_PlaneFaceAttributeImpl(void)
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_PlaneFaceAttribute)
		Stk_PlaneFaceAttribute::Stk_PlaneFaceAttribute() : Stk_GeometryAttribute(new Stk_PlaneFaceAttributeImpl())
		{
			
		}

		Stk_PlaneFaceAttribute::~Stk_PlaneFaceAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_IMPLEMENT_CLASS(Stk_PlaneFaceAttribute, Stk_GeometryAttribute)
		STK_STATUS	Stk_PlaneFaceAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_PlaneFaceAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_PlaneFaceAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_PlaneFaceAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_PlaneFaceAttribute::SetNormal(const STK_DIR32 &i_dirNormal)
		{
			((Stk_PlaneFaceAttributeImpl*)m_pImpl)->m_dirNormal = i_dirNormal;
			return STK_SUCCESS;
		}

		STK_DIR32	Stk_PlaneFaceAttribute::GetNormal()
		{
			return ((Stk_PlaneFaceAttributeImpl*)m_pImpl)->m_dirNormal;
		}

		STK_STATUS	Stk_PlaneFaceAttribute::SetOrigin(const STK_PNT32 &i_pntOrigin)
		{
			((Stk_PlaneFaceAttributeImpl*)m_pImpl)->m_pntOrigin = i_pntOrigin;
			return STK_SUCCESS;
		}

		STK_PNT32	Stk_PlaneFaceAttribute::GetOrigin()
		{
			return ((Stk_PlaneFaceAttributeImpl*)m_pImpl)->m_pntOrigin;
		}

		class Stk_RevolutionFaceAttributeImpl : public Stk_GeometryAttributeImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_RevolutionFaceAttribute)
		public:
			Stk_RevolutionFaceAttributeImpl();
			virtual ~Stk_RevolutionFaceAttributeImpl(void);
		protected:
			STK_DOUBLE64		m_dOriginRadius;									//‘≠µ„¥¶∞Îæ∂
			STK_DIR32			m_dirRevoAxis;								//–˝◊™÷·∑ΩœÚ
			STK_PNT32			m_pntAxisOrigin;							//–˝◊™÷·‘≠µ„
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_RevolutionFaceAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			return ret;
		}

		STK_STATUS	Stk_RevolutionFaceAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_TYPE, m_eGeoAttrType, i_JsonDocP->GetAllocator());
			//axisOrigin
			string strValue;
			char szValue[256] = { 0 };
			sprintf(szValue, "%.8f", m_pntAxisOrigin.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, strValue, i_JsonDocP->GetAllocator());
			//axisDirection
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirRevoAxis.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, m_dOriginRadius, i_JsonDocP->GetAllocator());
			//originRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ORIGINRADIUS, strValue, i_JsonDocP->GetAllocator());
			return ret;
		}

		Stk_RevolutionFaceAttributeImpl::Stk_RevolutionFaceAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_REVOLUTIONFACE;
		}

		Stk_RevolutionFaceAttributeImpl::~Stk_RevolutionFaceAttributeImpl()
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_RevolutionFaceAttribute)
		Stk_RevolutionFaceAttribute::Stk_RevolutionFaceAttribute() : Stk_GeometryAttribute(new Stk_RevolutionFaceAttributeImpl())
		{

		}

		Stk_RevolutionFaceAttribute::~Stk_RevolutionFaceAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}
		STK_IMPLEMENT_CLASS(Stk_RevolutionFaceAttribute, Stk_GeometryAttribute)

		STK_STATUS	Stk_RevolutionFaceAttribute::SetOriginRadius(STK_DOUBLE64 i_dRadius)
		{
			((Stk_RevolutionFaceAttributeImpl*)m_pImpl)->m_dOriginRadius = i_dRadius;
			return STK_SUCCESS;
		}

		STK_DOUBLE64	Stk_RevolutionFaceAttribute::GetOriginRadius()
		{
			return ((Stk_RevolutionFaceAttributeImpl*)m_pImpl)->m_dOriginRadius;
		}

		STK_STATUS	Stk_RevolutionFaceAttribute::SetRevoAxis(const STK_DIR32 &i_dirRevoAxis)
		{
			((Stk_RevolutionFaceAttributeImpl*)m_pImpl)->m_dirRevoAxis = i_dirRevoAxis;
			return STK_SUCCESS;
		}

		STK_DIR32	Stk_RevolutionFaceAttribute::GetRevoAxis()
		{
			return ((Stk_RevolutionFaceAttributeImpl*)m_pImpl)->m_dirRevoAxis;
		}

		STK_STATUS	Stk_RevolutionFaceAttribute::SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin)
		{
			((Stk_RevolutionFaceAttributeImpl*)m_pImpl)->m_pntAxisOrigin = i_pntAxisOrigin;
			return STK_SUCCESS;
		}

		STK_PNT32	Stk_RevolutionFaceAttribute::GetAxisOrigin()
		{
			return ((Stk_RevolutionFaceAttributeImpl*)m_pImpl)->m_pntAxisOrigin;
		}

		STK_STATUS	Stk_RevolutionFaceAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_RevolutionFaceAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_RevolutionFaceAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_RevolutionFaceAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_SphereFaceAttributeImpl : public Stk_GeometryAttributeImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_SphereFaceAttribute)
		public:
			Stk_SphereFaceAttributeImpl();
			virtual ~Stk_SphereFaceAttributeImpl(void);
		protected:
			STK_DOUBLE64		m_dOriginRadius;									//‘≠µ„¥¶∞Îæ∂
			STK_DIR32			m_dirRevoAxis;								//–˝◊™÷·∑ΩœÚ
			STK_PNT32			m_pntAxisOrigin;							//–˝◊™÷·‘≠µ„
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_SphereFaceAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			return ret;
		}

		STK_STATUS	Stk_SphereFaceAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_TYPE, m_eGeoAttrType, i_JsonDocP->GetAllocator());
			//axisOrigin
			string strValue;
			char szValue[256] = { 0 };
			sprintf(szValue, "%.8f", m_pntAxisOrigin.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, strValue, i_JsonDocP->GetAllocator());
			//axisDirection
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirRevoAxis.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, m_dOriginRadius, i_JsonDocP->GetAllocator());
			//originRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ORIGINRADIUS, strValue, i_JsonDocP->GetAllocator());
			return ret;
		}

		Stk_SphereFaceAttributeImpl::Stk_SphereFaceAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_SPHEREFACE;
		}

		Stk_SphereFaceAttributeImpl::~Stk_SphereFaceAttributeImpl()
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_SphereFaceAttribute)
			Stk_SphereFaceAttribute::Stk_SphereFaceAttribute() : Stk_GeometryAttribute(new Stk_SphereFaceAttributeImpl())
		{

		}

		Stk_SphereFaceAttribute::~Stk_SphereFaceAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}
		STK_IMPLEMENT_CLASS(Stk_SphereFaceAttribute, Stk_GeometryAttribute)

			STK_STATUS	Stk_SphereFaceAttribute::SetOriginRadius(STK_DOUBLE64 i_dRadius)
		{
			((Stk_SphereFaceAttributeImpl*)m_pImpl)->m_dOriginRadius = i_dRadius;
			return STK_SUCCESS;
		}

		STK_DOUBLE64	Stk_SphereFaceAttribute::GetOriginRadius()
		{
			return ((Stk_SphereFaceAttributeImpl*)m_pImpl)->m_dOriginRadius;
		}

		STK_STATUS	Stk_SphereFaceAttribute::SetRevoAxis(const STK_DIR32 &i_dirRevoAxis)
		{
			((Stk_SphereFaceAttributeImpl*)m_pImpl)->m_dirRevoAxis = i_dirRevoAxis;
			return STK_SUCCESS;
		}

		STK_DIR32	Stk_SphereFaceAttribute::GetRevoAxis()
		{
			return ((Stk_SphereFaceAttributeImpl*)m_pImpl)->m_dirRevoAxis;
		}

		STK_STATUS	Stk_SphereFaceAttribute::SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin)
		{
			((Stk_SphereFaceAttributeImpl*)m_pImpl)->m_pntAxisOrigin = i_pntAxisOrigin;
			return STK_SUCCESS;
		}

		STK_PNT32	Stk_SphereFaceAttribute::GetAxisOrigin()
		{
			return ((Stk_SphereFaceAttributeImpl*)m_pImpl)->m_pntAxisOrigin;
		}

		STK_STATUS	Stk_SphereFaceAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_SphereFaceAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_SphereFaceAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_SphereFaceAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_CylinderFaceAttributeImpl : public Stk_GeometryAttributeImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_CylinderFaceAttribute)
		public:
			Stk_CylinderFaceAttributeImpl();
			virtual ~Stk_CylinderFaceAttributeImpl(void);
		protected:
			STK_DOUBLE64		m_dOriginRadius;									//‘≠µ„¥¶∞Îæ∂
			STK_DIR32			m_dirRevoAxis;								//–˝◊™÷·∑ΩœÚ
			STK_PNT32			m_pntAxisOrigin;							//–˝◊™÷·‘≠µ„
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_CylinderFaceAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			return ret;
		}

		STK_STATUS	Stk_CylinderFaceAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_TYPE, m_eGeoAttrType, i_JsonDocP->GetAllocator());
			//axisOrigin
			string strValue;
			char szValue[256] = { 0 };
			sprintf(szValue, "%.8f", m_pntAxisOrigin.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, strValue, i_JsonDocP->GetAllocator());
			//axisDirection
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirRevoAxis.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, m_dOriginRadius, i_JsonDocP->GetAllocator());
			//originRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ORIGINRADIUS, strValue, i_JsonDocP->GetAllocator());
			return ret;
		}

		Stk_CylinderFaceAttributeImpl::Stk_CylinderFaceAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_CYLINDERFACE;
		}

		Stk_CylinderFaceAttributeImpl::~Stk_CylinderFaceAttributeImpl()
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_CylinderFaceAttribute)
			Stk_CylinderFaceAttribute::Stk_CylinderFaceAttribute() : Stk_GeometryAttribute(new Stk_CylinderFaceAttributeImpl())
		{

		}

		Stk_CylinderFaceAttribute::~Stk_CylinderFaceAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}
		STK_IMPLEMENT_CLASS(Stk_CylinderFaceAttribute, Stk_GeometryAttribute)

			STK_STATUS	Stk_CylinderFaceAttribute::SetOriginRadius(STK_DOUBLE64 i_dRadius)
		{
			((Stk_CylinderFaceAttributeImpl*)m_pImpl)->m_dOriginRadius = i_dRadius;
			return STK_SUCCESS;
		}

		STK_DOUBLE64	Stk_CylinderFaceAttribute::GetOriginRadius()
		{
			return ((Stk_CylinderFaceAttributeImpl*)m_pImpl)->m_dOriginRadius;
		}

		STK_STATUS	Stk_CylinderFaceAttribute::SetRevoAxis(const STK_DIR32 &i_dirRevoAxis)
		{
			((Stk_CylinderFaceAttributeImpl*)m_pImpl)->m_dirRevoAxis = i_dirRevoAxis;
			return STK_SUCCESS;
		}

		STK_DIR32	Stk_CylinderFaceAttribute::GetRevoAxis()
		{
			return ((Stk_CylinderFaceAttributeImpl*)m_pImpl)->m_dirRevoAxis;
		}

		STK_STATUS	Stk_CylinderFaceAttribute::SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin)
		{
			((Stk_CylinderFaceAttributeImpl*)m_pImpl)->m_pntAxisOrigin = i_pntAxisOrigin;
			return STK_SUCCESS;
		}

		STK_PNT32	Stk_CylinderFaceAttribute::GetAxisOrigin()
		{
			return ((Stk_CylinderFaceAttributeImpl*)m_pImpl)->m_pntAxisOrigin;
		}

		STK_STATUS	Stk_CylinderFaceAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_CylinderFaceAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_CylinderFaceAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_CylinderFaceAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_ConeFaceAttributeImpl : public Stk_GeometryAttributeImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_ConeFaceAttribute)
		public:
			Stk_ConeFaceAttributeImpl();
			virtual ~Stk_ConeFaceAttributeImpl(void);
		protected:
			STK_DOUBLE64		m_dOriginRadius;									//‘≠µ„¥¶∞Îæ∂
			STK_DIR32			m_dirRevoAxis;								//–˝◊™÷·∑ΩœÚ
			STK_PNT32			m_pntAxisOrigin;							//–˝◊™÷·‘≠µ„
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_ConeFaceAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			return ret;
		}

		STK_STATUS	Stk_ConeFaceAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_TYPE, m_eGeoAttrType, i_JsonDocP->GetAllocator());
			//axisOrigin
			string strValue;
			char szValue[256] = { 0 };
			sprintf(szValue, "%.8f", m_pntAxisOrigin.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, strValue, i_JsonDocP->GetAllocator());
			//axisDirection
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirRevoAxis.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, m_dOriginRadius, i_JsonDocP->GetAllocator());
			//originRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ORIGINRADIUS, strValue, i_JsonDocP->GetAllocator());
			return ret;
		}

		Stk_ConeFaceAttributeImpl::Stk_ConeFaceAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_CONICALFACE;
		}

		Stk_ConeFaceAttributeImpl::~Stk_ConeFaceAttributeImpl()
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_ConeFaceAttribute)
			Stk_ConeFaceAttribute::Stk_ConeFaceAttribute() : Stk_GeometryAttribute(new Stk_ConeFaceAttributeImpl())
		{

		}

		Stk_ConeFaceAttribute::~Stk_ConeFaceAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}
		STK_IMPLEMENT_CLASS(Stk_ConeFaceAttribute, Stk_GeometryAttribute)

			STK_STATUS	Stk_ConeFaceAttribute::SetOriginRadius(STK_DOUBLE64 i_dRadius)
		{
			((Stk_ConeFaceAttributeImpl*)m_pImpl)->m_dOriginRadius = i_dRadius;
			return STK_SUCCESS;
		}

		STK_DOUBLE64	Stk_ConeFaceAttribute::GetOriginRadius()
		{
			return ((Stk_ConeFaceAttributeImpl*)m_pImpl)->m_dOriginRadius;
		}

		STK_STATUS	Stk_ConeFaceAttribute::SetRevoAxis(const STK_DIR32 &i_dirRevoAxis)
		{
			((Stk_ConeFaceAttributeImpl*)m_pImpl)->m_dirRevoAxis = i_dirRevoAxis;
			return STK_SUCCESS;
		}

		STK_DIR32	Stk_ConeFaceAttribute::GetRevoAxis()
		{
			return ((Stk_ConeFaceAttributeImpl*)m_pImpl)->m_dirRevoAxis;
		}

		STK_STATUS	Stk_ConeFaceAttribute::SetAxisOrigin(const STK_PNT32 &i_pntAxisOrigin)
		{
			((Stk_ConeFaceAttributeImpl*)m_pImpl)->m_pntAxisOrigin = i_pntAxisOrigin;
			return STK_SUCCESS;
		}

		STK_PNT32	Stk_ConeFaceAttribute::GetAxisOrigin()
		{
			return ((Stk_ConeFaceAttributeImpl*)m_pImpl)->m_pntAxisOrigin;
		}

		STK_STATUS	Stk_ConeFaceAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ConeFaceAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_ConeFaceAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ConeFaceAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_ToroidalFaceAttributeImpl : public Stk_RevolutionFaceAttributeImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_ToroidalFaceAttribute)
		public:
			Stk_ToroidalFaceAttributeImpl();
			virtual ~Stk_ToroidalFaceAttributeImpl(void);
		private:
			STK_FLOAT32		m_fMajorRadius;			//¬÷Ã•√ÊµƒÕ‚æ∂
			STK_FLOAT32		m_fMinorRadius;			//¬÷Ã•√Êµƒƒ⁄æ∂
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_ToroidalFaceAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			return ret;
		}

		STK_STATUS	Stk_ToroidalFaceAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_TYPE, m_eGeoAttrType, i_JsonDocP->GetAllocator());
			//axisOrigin
			string strValue;
			char szValue[256] = { 0 };
			sprintf(szValue, "%.8f", m_pntAxisOrigin.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntAxisOrigin.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, strValue, i_JsonDocP->GetAllocator());
			//axisDirection
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirRevoAxis.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirRevoAxis.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_AXISORIGIN, strValue, i_JsonDocP->GetAllocator());
			//originRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ORIGINRADIUS, m_dOriginRadius, i_JsonDocP->GetAllocator());
			//majorRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_MAJORRADIUS, m_fMajorRadius, i_JsonDocP->GetAllocator());
			//minorRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_MINORRADIUS, m_fMinorRadius, i_JsonDocP->GetAllocator());
			return ret;
		}

		Stk_ToroidalFaceAttributeImpl::Stk_ToroidalFaceAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_TOROIDALFACE;
		}

		Stk_ToroidalFaceAttributeImpl::~Stk_ToroidalFaceAttributeImpl(void)
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_ToroidalFaceAttribute)
		Stk_ToroidalFaceAttribute::Stk_ToroidalFaceAttribute():Stk_RevolutionFaceAttribute(new Stk_ToroidalFaceAttributeImpl())
		{

		}

		Stk_ToroidalFaceAttribute::~Stk_ToroidalFaceAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_IMPLEMENT_CLASS(Stk_ToroidalFaceAttribute, Stk_RevolutionFaceAttribute)

		STK_FLOAT32		Stk_ToroidalFaceAttribute::GetMajorRadius()
		{
			return ((Stk_ToroidalFaceAttributeImpl*)m_pImpl)->m_fMajorRadius;
		}

		STK_STATUS		Stk_ToroidalFaceAttribute::SetMajorRaius(STK_FLOAT32 i_fRadius)
		{
			((Stk_ToroidalFaceAttributeImpl*)m_pImpl)->m_fMajorRadius = i_fRadius;
			return STK_SUCCESS;
		}

		STK_FLOAT32		Stk_ToroidalFaceAttribute::GetMinorRadius()
		{
			return ((Stk_ToroidalFaceAttributeImpl*)m_pImpl)->m_fMinorRadius;
		}

		STK_STATUS		Stk_ToroidalFaceAttribute::SetMinorRadius(STK_FLOAT32 i_fRdius)
		{
			((Stk_ToroidalFaceAttributeImpl*)m_pImpl)->m_fMinorRadius = i_fRdius;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_ToroidalFaceAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ToroidalFaceAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_ToroidalFaceAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_ToroidalFaceAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_LineAttributeImpl : public Stk_GeometryAttributeImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_LineAttribute)
		public:
			Stk_LineAttributeImpl();
			virtual ~Stk_LineAttributeImpl(void);
		private:
			STK_PNT32		m_pntCenterPoint;				//Lineµƒ÷–µ„
			STK_DIR32		m_dirDirection;					//Lineµƒ∑ΩœÚœÚ¡ø
			STK_PNT32		m_pntStartPoint;				//Line∆µ„
			STK_PNT32		m_pntEndPoint;					//Line÷’µ„
			STK_FLOAT32		m_fLength;						//Line≥§∂»
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_LineAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			return ret;
		}

		STK_STATUS	Stk_LineAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_TYPE, m_eGeoAttrType, i_JsonDocP->GetAllocator());
			string strValue;
			char szValue[256] = { 0 };
			//centerPoint
			sprintf(szValue, "%.8f", m_pntCenterPoint.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntCenterPoint.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntCenterPoint.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_CENTERPOINT, strValue, i_JsonDocP->GetAllocator());
			//direction
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirDirection.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirDirection.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirDirection.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_DIRECTION, strValue, i_JsonDocP->GetAllocator());
			//startPoint
			strValue.clear();
			sprintf(szValue, "%.8f", m_pntStartPoint.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntStartPoint.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntStartPoint.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_STARTPOINT, strValue, i_JsonDocP->GetAllocator());
			//endPoint
			strValue.clear();
			sprintf(szValue, "%.8f", m_pntEndPoint.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntEndPoint.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntEndPoint.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ENDPOINT, strValue, i_JsonDocP->GetAllocator());
			//length
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_LENGTH, m_fLength, i_JsonDocP->GetAllocator());
			return ret;
		}

		Stk_LineAttributeImpl::Stk_LineAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_LINE;
		}

		Stk_LineAttributeImpl::~Stk_LineAttributeImpl()
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_LineAttribute)
		Stk_LineAttribute::Stk_LineAttribute():Stk_GeometryAttribute(new Stk_LineAttributeImpl())
		{

		}

		Stk_LineAttribute::~Stk_LineAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_IMPLEMENT_CLASS(Stk_LineAttribute, Stk_GeometryAttribute)

		STK_PNT32	Stk_LineAttribute::GetCenterPoint()
		{
			return ((Stk_LineAttributeImpl*)m_pImpl)->m_pntCenterPoint;
		}

		STK_STATUS	Stk_LineAttribute::SetCenterPoint(STK_PNT32 &i_pntCenter)
		{
			((Stk_LineAttributeImpl*)m_pImpl)->m_pntCenterPoint = i_pntCenter;
			return STK_SUCCESS;
		}

		STK_DIR32	Stk_LineAttribute::GetDirection()
		{
			return ((Stk_LineAttributeImpl*)m_pImpl)->m_dirDirection;
		}

		STK_STATUS		Stk_LineAttribute::SetDirection(STK_DIR32 &i_dirDirect)
		{
			((Stk_LineAttributeImpl*)m_pImpl)->m_dirDirection = i_dirDirect;
			return STK_SUCCESS;
		}

		STK_PNT32	Stk_LineAttribute::GetStartPoint()
		{
			return ((Stk_LineAttributeImpl*)m_pImpl)->m_pntStartPoint;
		}

		STK_STATUS	Stk_LineAttribute::SetStartPoint(STK_PNT32 &i_pntStart)
		{
			((Stk_LineAttributeImpl*)m_pImpl)->m_pntStartPoint = i_pntStart;
			return STK_SUCCESS;
		}

		STK_PNT32	Stk_LineAttribute::GetEndPoint()
		{
			return ((Stk_LineAttributeImpl*)m_pImpl)->m_pntEndPoint;
		}

		STK_STATUS	Stk_LineAttribute::SetEndPoint(STK_PNT32 &i_pntEnd)
		{
			((Stk_LineAttributeImpl*)m_pImpl)->m_pntEndPoint = i_pntEnd;
			return STK_SUCCESS;
		}

		STK_FLOAT32	Stk_LineAttribute::GetLength()
		{
			return ((Stk_LineAttributeImpl*)m_pImpl)->m_fLength;
		}

		STK_STATUS	Stk_LineAttribute::SetLength(STK_FLOAT32 i_fLength)
		{
			((Stk_LineAttributeImpl*)m_pImpl)->m_fLength = i_fLength;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_LineAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_LineAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_LineAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_LineAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		class Stk_EllipseAttributeImpl : public Stk_GeometryAttributeImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_EllipseAttribute)
		public:
			Stk_EllipseAttributeImpl();
			virtual ~Stk_EllipseAttributeImpl(void);
		private:
			STK_PNT32		m_pntCenterPoint;				//Ellipseµƒ÷––ƒµ„
			STK_FLOAT32		m_fMajorRadius;					//Ellipseµƒ≥§÷·∞Îæ∂
			STK_FLOAT32		m_fMinorRadius;					//Ellipseµƒ∂Ã÷·∞Îæ∂
			STK_PNT32		m_pntStartPoint;				//±ﬂΩÁœﬂ∆µ„
			STK_PNT32		m_pntEndPoint;					//±ﬂΩÁœﬂ÷’µ„
			STK_DIR32		m_dirX;							//X∑ΩœÚœÚ¡ø
			STK_DIR32		m_dirY;							//Y∑ΩœÚœÚ¡ø
			STK_DIR32		m_dirZ;							//Z∑ΩœÚœÚ¡ø
		public:
			/****************************************************************************
			*	@brief		∂¡»°Œƒº˛÷–µƒBody ˝æ›
			*	@param[in]	i_pJsonNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			∂¡»°≥…π¶
			*	@retval		STK_ERROR			∂¡»° ß∞‹
			*	@retval		STK_ERROR_MEMORY	ƒ⁄¥Ê…Í«Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					FromJson(Value* i_pJsonNode, Document* i_JsonDocP);

			/*****************************************************************************
			*	@brief		–¥View ˝æ›
			*	@param[in]	Value* i_pJsonParentNode			View ∏∏JsonΩ⁄µ„
			*	@param[out]	Œﬁ
			*	@retval		STK_SUCCESS			–¥»Î≥…π¶
			*	@retval		∆‰À¸				–¥»Î ß∞‹
			*	@note		Œﬁ
			*	@attention	ƒ⁄≤øΩ”ø⁄
			****************************************************************************/
			STK_STATUS					ToJson(Value* i_pJsonNode, Document* i_JsonDocP);
		};

		STK_STATUS	Stk_EllipseAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			return ret;
		}

		STK_STATUS	Stk_EllipseAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_TYPE, m_eGeoAttrType, i_JsonDocP->GetAllocator());
			string strValue;
			char szValue[256] = { 0 };
			//centerPoint
			sprintf(szValue, "%.8f", m_pntCenterPoint.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntCenterPoint.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntCenterPoint.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_CENTERPOINT, strValue, i_JsonDocP->GetAllocator());
			//startPoint
			strValue.clear();
			sprintf(szValue, "%.8f", m_pntStartPoint.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntStartPoint.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntStartPoint.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_STARTPOINT, strValue, i_JsonDocP->GetAllocator());
			//endPoint
			strValue.clear();
			sprintf(szValue, "%.8f", m_pntEndPoint.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntEndPoint.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_pntEndPoint.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ENDPOINT, strValue, i_JsonDocP->GetAllocator());
			//majorRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_MAJORRADIUS, m_fMajorRadius, i_JsonDocP->GetAllocator());
			//minorRadius
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_MINORRADIUS, m_fMinorRadius, i_JsonDocP->GetAllocator());
			//xDirection
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirX.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirX.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirX.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_XDIRECTION, strValue, i_JsonDocP->GetAllocator());
			//yDirection
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirY.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirY.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirY.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_YDIRECTION, strValue, i_JsonDocP->GetAllocator());
			//zDirection
			strValue.clear();
			sprintf(szValue, "%.8f", m_dirZ.X);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirZ.Y);
			strValue += szValue;
			strValue += " ";
			sprintf(szValue, "%.8f", m_dirZ.Z);
			strValue += szValue;
			i_pJsonNode->AddMember(GEOMETRYATTRIBUTE_NODE_NAME_ZDIRECTION, strValue, i_JsonDocP->GetAllocator());
			return ret;
		}

		Stk_EllipseAttributeImpl::~Stk_EllipseAttributeImpl(void)
		{
			
		}

		Stk_EllipseAttributeImpl::Stk_EllipseAttributeImpl()
		{
			m_eGeoAttrType = GEOATTR_TYPE_ELLIPSE;
		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_EllipseAttribute)

		Stk_EllipseAttribute::Stk_EllipseAttribute():Stk_GeometryAttribute(new Stk_EllipseAttributeImpl())
		{

		}

		Stk_EllipseAttribute::~Stk_EllipseAttribute()
		{
			DELETE_STK_CLASS_IMPL
		}

		STK_IMPLEMENT_CLASS(Stk_EllipseAttribute, Stk_GeometryAttribute)

		STK_PNT32 Stk_EllipseAttribute::GetCenterPoint()
		{
			return ((Stk_EllipseAttributeImpl*)m_pImpl)->m_pntCenterPoint;
		}

		STK_STATUS	Stk_EllipseAttribute::SetCenterPoint(STK_PNT32 &i_pntCenter)
		{
			((Stk_EllipseAttributeImpl*)m_pImpl)->m_pntCenterPoint = i_pntCenter;
			return STK_SUCCESS;
		}

		STK_FLOAT32	Stk_EllipseAttribute::GetMajorRadius()
		{
			return ((Stk_EllipseAttributeImpl*)m_pImpl)->m_fMajorRadius;
		}

		STK_STATUS		Stk_EllipseAttribute::SetMajorRadius(STK_FLOAT32 i_fRadius)
		{
			((Stk_EllipseAttributeImpl*)m_pImpl)->m_fMajorRadius = i_fRadius;
			return STK_SUCCESS;
		}

		STK_FLOAT32	Stk_EllipseAttribute::GetMinorRadius()
		{
			return ((Stk_EllipseAttributeImpl*)m_pImpl)->m_fMinorRadius;
		}

		STK_STATUS		Stk_EllipseAttribute::SetMinorRadius(STK_FLOAT32 i_fRadius)
		{
			((Stk_EllipseAttributeImpl*)m_pImpl)->m_fMinorRadius = i_fRadius;
			return STK_SUCCESS;
		}

		STK_PNT32		Stk_EllipseAttribute::GetStartPoint()
		{
			return ((Stk_EllipseAttributeImpl*)m_pImpl)->m_pntStartPoint;
		}

		STK_STATUS		Stk_EllipseAttribute::SetStartPoint(STK_PNT32 &i_pntStart)
		{
			((Stk_EllipseAttributeImpl*)m_pImpl)->m_pntStartPoint = i_pntStart;
			return STK_SUCCESS;
		}

		STK_PNT32		Stk_EllipseAttribute::GetEndPoint()
		{
			return ((Stk_EllipseAttributeImpl*)m_pImpl)->m_pntEndPoint;
		}

		STK_STATUS		Stk_EllipseAttribute::SetEndPoint(STK_PNT32 &i_pntEnd)
		{
			((Stk_EllipseAttributeImpl*)m_pImpl)->m_pntEndPoint = i_pntEnd;
			return STK_SUCCESS;
		}

		void	Stk_EllipseAttribute::GetXYZDir(STK_DIR32& o_dirX, STK_DIR32& o_dirY, STK_DIR32& o_dirZ)
		{
			o_dirX = ((Stk_EllipseAttributeImpl*)m_pImpl)->m_dirX;
			o_dirY = ((Stk_EllipseAttributeImpl*)m_pImpl)->m_dirY;
			o_dirZ = ((Stk_EllipseAttributeImpl*)m_pImpl)->m_dirZ;
		}

		STK_STATUS		Stk_EllipseAttribute::SetXYZDir(const STK_DIR32& i_dirX, const STK_DIR32& i_dirY, const STK_DIR32& i_dirZ)
		{
			((Stk_EllipseAttributeImpl*)m_pImpl)->m_dirX = i_dirX;
			((Stk_EllipseAttributeImpl*)m_pImpl)->m_dirY = i_dirY;
			((Stk_EllipseAttributeImpl*)m_pImpl)->m_dirZ = i_dirZ;
			return STK_SUCCESS;
		}

		STK_STATUS	Stk_EllipseAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_EllipseAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_EllipseAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_EllipseAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}
	}
}
