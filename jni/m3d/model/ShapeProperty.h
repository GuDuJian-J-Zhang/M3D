/**@file
 *@brief
 *@author
 *@date		2013-7-10
 *@version	1.0
 *@ShapeProperty All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 *ShapeProperty类头文件
 *
 */

#ifndef M3D_SHAPE_PROPERTY_H_
#define M3D_SHAPE_PROPERTY_H_

#include "m3d/base/Object.h"

namespace M3D
{

/** @brief	自定义属性值类型	*/
enum ShapePropertyValueTypeEnum
{
	SHAPE_PROPERTY_TypeValueUnknown = 0,
	SHAPE_PROPERTY_VALUE_TYPE_BOOLEAN,						//!< 布尔
	SHAPE_PROPERTY_VALUE_TYPE_INTEGER,						//!< 整数
	SHAPE_PROPERTY_VALUE_TYPE_REAL,							//!< 实数
	SHAPE_PROPERTY_VALUE_TYPE_STRING						//!< 字符串
};

class M3D_API ShapePropertyItem{
public:
	string						m_strTitle;					//!< 自定义属性名
	ShapePropertyValueTypeEnum	m_eValueType;				//!< 自定义属性值的类型（枚举表示）
	string						m_strValue;					//!< 自定义属性值
	string						m_strUnits;					//!< 自定义属性单位
};

/**
 * @class ShapeProperty
 * @brief ShapeProperty类
 *
 * ShapeProperty用户自定义属性
 */
class M3D_API ShapeProperty: public Object
{
public:
	ShapeProperty();
	virtual ~ShapeProperty();

	/**
	 * 获取所有的用户自定义属性
	 * @return
	 */
	vector<ShapePropertyItem>& GetProperties();
	/**
	 * 一次性设置所有的用户自定义属性
	 * @param properties
	 */
	void SetProperties(vector<ShapePropertyItem>& properties);

private:
	vector<ShapePropertyItem> m_properties; //!< 存储所有的用户自定义属性
};

}

#endif /*M3D_SHAPE_PROPERTY_H_*/

