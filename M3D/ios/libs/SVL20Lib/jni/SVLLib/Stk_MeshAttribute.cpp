#include "Stk_MeshAttribute.h"
#include "Stk_ObjectImpl.h"
#include "Stk_BaseDocumentImpl.h"
#include "Stk_GeometryAttribute.h"

#define  MESHATTRIBUTE_NODE_NAME_ID "id"
#define  MESHATTRIBUTE_NODE_NAME_GEOMETRYATTRIBUTE "geometryAttribute"

namespace HoteamSoft {
	namespace SVLLib {
		
		class Stk_MeshAttributeImpl : public Stk_ObjectImpl
		{
			STK_CLASS_IMPL_DECLARE(Stk_MeshAttribute)
		public:
			Stk_MeshAttributeImpl();
			virtual ~Stk_MeshAttributeImpl(void);
		private:
			Stk_GeometryAttributePtr       m_GeoAttrP;
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

		STK_STATUS	Stk_MeshAttributeImpl::ToJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			i_pJsonNode->SetObject();
			i_pJsonNode->AddMember(MESHATTRIBUTE_NODE_NAME_ID, m_ID, i_JsonDocP->GetAllocator());
			Value valGeoAttr;
			valGeoAttr.SetObject();
			if(m_GeoAttrP != nullptr)
				m_GeoAttrP->ToJson(&valGeoAttr, i_JsonDocP);
			i_pJsonNode->AddMember(MESHATTRIBUTE_NODE_NAME_GEOMETRYATTRIBUTE, valGeoAttr, i_JsonDocP->GetAllocator());
			return ret;
		}

		STK_STATUS	Stk_MeshAttributeImpl::FromJson(Value* i_pJsonNode, Document* i_JsonDocP)
		{
			STK_STATUS			ret = STK_ERROR;
			if (!i_pJsonNode || !i_JsonDocP)
				return ret;
			return ret;
		}

		Stk_MeshAttributeImpl::Stk_MeshAttributeImpl()
		{
			m_GeoAttrP = nullptr;
		}

		Stk_MeshAttributeImpl::~Stk_MeshAttributeImpl(void)
		{

		}

		STK_CLASS_IMPL_IMPLEMENT(Stk_MeshAttribute)
		Stk_MeshAttribute::Stk_MeshAttribute():Stk_Object(new Stk_MeshAttributeImpl())
		{

		}

		Stk_MeshAttribute::~Stk_MeshAttribute()
		{

		}
		STK_IMPLEMENT_CLASS(Stk_MeshAttribute, Stk_Object)

		STK_STATUS	Stk_MeshAttribute::ToJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_MeshAttributeImpl*)m_pImpl)->ToJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS	Stk_MeshAttribute::FromJson(void* i_DataP, void* i_JsonDocP)
		{
			return ((Stk_MeshAttributeImpl*)m_pImpl)->FromJson((Value*)i_DataP, (Document*)i_JsonDocP);
		}

		STK_STATUS Stk_MeshAttribute::SetGeometryAttribute(Stk_GeometryAttributePtr i_geoAttrP)
		{
			((Stk_MeshAttributeImpl*)m_pImpl)->m_GeoAttrP = i_geoAttrP;
			return STK_SUCCESS;
		}
	}
}
