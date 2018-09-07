
#include "m3d/extras/measure/Measure.h"


namespace M3D
{
const  int Measure::MEASURE_TYPE_BASE = 0; ///基本测量对象

const  int Measure::MEASURE_TYPE_PNT_PNT_DISTANCE = 1;  ///点到点距离
const  int Measure::MEASURE_TYPE_PNT_LINE_DISTANCE = 2;  ///点到边线距离
const  int Measure::MEASURE_TYPE_PNT_FACE_DISTANCE = 3;  ///点到面距离
const  int Measure::MEASURE_TYPE_LINE_LINE_DISTANCE = 4;  ///线到线距离
const  int Measure::MEASURE_TYPE_LINE_FACE_DISTANCE = 5;  ///线到面距离
const  int Measure::MEASURE_TYPE_FACE_FACE_DISTANCE = 6;  ///面到面距离
const  int Measure::MEASURE_TYPE_SHAFT_SHAFT_DISTANCE = 9; ///圆心轴到圆心轴距离
const  int Measure::MEASURE_TYPE_CENTER_CENTER_DISTANCE = 10; ///圆心轴到圆心轴距离

const  int Measure::MEASURE_TYPE_LINE_LINE_ANGLE = 50; ///两条非平行线间的夹角
const  int Measure::MEASURE_TYPE_FACE_FACE_ANGLE = 51; ///两个面间的夹角
const  int Measure::MEASURE_TYPE_LINE_FACE_ANGLE = 52; ///直线与平面之间的夹角

const  int Measure::MEASURE_TYPE_CRICLE_DIAMETRE = 60; //!<圆直径
const  int Measure::MEASURE_TYPE_CRICLE_RADIUS = 61;  //!<圆半径

const   int Measure::MEASURE_TYPE_PNT_COORD = 100; ///点的坐标值
const   int Measure::MEASURE_TYPE_LINE_LENGTH = 101 ; ///边线的长度
const   int Measure::MEASURE_TYPE_CRICLE_PROPERTY = 102 ; ///圆或圆弧的弧长、直径（半径）、圆心
const   int Measure::MEASURE_TYPE_FACE_PROPERTY = 103; ///面的属性
const   int Measure::MEASURE_TYPE_APERTURE_PROPERTY = 104; ///孔的圆心、直径（半径）、深度
const	int Measure::MEASURE_TYPE_MODEL_PROPERTY = 105;///模型属性
const	int Measure::MEASURE_TYPE_MODEL_STATISTICS = 106;///模型统计

const  int Measure::MEASURE_TYPE_TEXT_NOTE = 150; ///文本批注
const  int Measure::MEASURE_TYPE_VOICE_NOTE = 151; ///语音批注
const int  Measure::MEASURE_TYPE_SEQUENCE_NUMBER_NOTE = 152;
const  int Measure::MEASURE_TYPE_THREED_GESTURE_NOTE = 153;//3D手势批注

Measure::Measure() :
		Note()
{
	this->SetType(SHAPE_MEASURE_BASE);
}

Measure::~Measure()
{

}

int Measure::GetMeasureType()
{
	return this->m_measureType;
}

void Measure::SetMeasureType(int measureType)
{
	this->m_measureType = measureType;
}

}
