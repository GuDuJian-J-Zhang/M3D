/**@file
 *@brief	测量note类头文件
 *@author   xuli
 *@date		2013-7-10
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_MEASURE_H_
#define M3D_MEASURE_H_

#include "m3d/model/Note.h"

namespace M3D
{

/**
 * @brief 测量对象类
 */
class M3D_API Measure: public Note
{
public:
	const static int MEASURE_TYPE_BASE; //!<基本测量对象
	///距离类型
	static const  int MEASURE_TYPE_PNT_PNT_DISTANCE ;  //!<点到点距离
	static const  int MEASURE_TYPE_PNT_LINE_DISTANCE ;  //!<点到边线距离
	static const  int MEASURE_TYPE_PNT_FACE_DISTANCE ;  //!<点到面距离
	static const  int MEASURE_TYPE_LINE_LINE_DISTANCE ;  //!<线到线距离
	static const  int MEASURE_TYPE_LINE_FACE_DISTANCE ;  //!<线到面距离
	static const  int MEASURE_TYPE_FACE_FACE_DISTANCE ;  //!<面到面距离
	static const  int MEASURE_TYPE_SHAFT_SHAFT_DISTANCE;  //!<圆心轴到圆心轴距离
	static const  int MEASURE_TYPE_CENTER_CENTER_DISTANCE;  //!<中心到中心距离

	///角度类型
	const static int MEASURE_TYPE_LINE_LINE_ANGLE ; //!<两条非平行线间的夹角
	const static int MEASURE_TYPE_FACE_FACE_ANGLE ; //!<两个面间的夹角
	const static int MEASURE_TYPE_LINE_FACE_ANGLE; //!<直线与平面之间的夹角

	const static int MEASURE_TYPE_CRICLE_DIAMETRE; //!<圆直径
	const static int MEASURE_TYPE_CRICLE_RADIUS; //!<圆半径

	const static int MEASURE_TYPE_PNT_COORD ; //!<点的坐标值
	const static int MEASURE_TYPE_LINE_LENGTH ; //!<边线的长度
	const static int MEASURE_TYPE_CRICLE_PROPERTY ; //!<圆或圆弧的弧长、直径（半径）、圆心
	const static int MEASURE_TYPE_FACE_PROPERTY ; //!<面的属性
	const static int MEASURE_TYPE_APERTURE_PROPERTY ; //!<孔的圆心、直径（半径）、深度
	const static int MEASURE_TYPE_MODEL_PROPERTY;//!<模型属性
	const static int MEASURE_TYPE_MODEL_STATISTICS;//!<模型统计

	const static int MEASURE_TYPE_TEXT_NOTE ; //!<文本批注
	const static int MEASURE_TYPE_VOICE_NOTE; //!<语音批注
	const static int MEASURE_TYPE_SEQUENCE_NUMBER_NOTE;//!<序号批注
	const static int MEASURE_TYPE_THREED_GESTURE_NOTE;//3D手势批注
//	const static int MEASURE_TYPE_PNT_COORD ; ///面的面积
public:
	Measure();
	virtual ~Measure();

	/**
	 * @brief 获取测量类型
	 * @return 获取测量类型
	 */
	int GetMeasureType();

	/**
	 * @brief 设置测量类型
	 * @param measureType 设置测量对象类型
	 */
	void SetMeasureType(int measureType);


private:
	/**
	 * @brief 初始化函数
	 */
	void Init();


private:
	int m_measureType; //!< 测量类型

};

}

#endif /*M3D_MEASURE_H_*/

