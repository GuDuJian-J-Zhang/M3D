// All Rights Reserved. Copyright (C) 2016 Hoteamsoft. 
/**************************************************************************
	*	@class	Stk_DocumentManager
	*
	*			
**************************************************************************/
#ifndef _SVLLIB_STK_ACCESSOR_H_
#define _SVLLIB_STK_ACCESSOR_H_
#include "Stk_Object.h"
#include <string>
#include "Stk_DocumentImpl.h"
// #include <vector>
// #include <map>

namespace HoteamSoft {
	namespace SVLLib {

#define MESH_BYTEOFFSET "byteoffset"
#define MESH_INDEXLENGTH "indexByteLength"
#define MESH_ATTRIBUTE "attributeLength"
#define MESH_UVSLENGTH "UVsLength"
#define DATA_LENGTH "dataLength"
#define MESH_BIN_FILE "geomFile"

class Stk_Document;
class STK_TOOLKIT_EXPORT Stk_Accessor : public Stk_Object
        {
            STK_DECLARE_CLASS(Stk_Accessor)
protected:
	StkAccessorTypeEnum m_type;
	enum { _typeID = _SVLLIB_MESHPRIM };	//!< ¿‡±
public:
	Stk_Accessor(void);
	virtual ~Stk_Accessor(void);

	StkAccessorTypeEnum GetType() {
		return m_type;
	}

	void SetType(StkAccessorTypeEnum type) {
		this->m_type = type;
	}

	virtual STK_BOOL Encode(rapidjson::Value& accessor);
	virtual STK_BOOL Uncode(rapidjson::Value& accessor);
protected:

};

	}//svllib
}//hoteamsoft

#endif
