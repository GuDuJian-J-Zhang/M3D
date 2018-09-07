/**@file
 *@brief
 *@author	XL
 *@date		2015-7-7
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */
#ifndef     M3D_MEASURE_HELPER_H
#define     M3D_MEASURE_HELPER_H

#include 	"m3d/base/Object.h"

namespace M3D
{
class ImageBoard;
class Vector3;
class Vector2;
class Measure;
class SceneManager;
class Shape2DSet;
class Color;
class IShape;
/**
 * @brief 测量类。生成不同种类的测量对象又来呈现测量结果
 */
class M3D_API MeasureFactory: public Object
{
public:
	/**
	 * @brief 获取距离测量对象
	 * @param firstShapeID 第一个shape　ID
	 * @param secondShapID 第二个shape　ID
	 * @param type 类型
	 * @param screenPnt 拾取的屏幕坐标
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* CreateDistanceMeasure(int firstShapeID, int secondShapID,
			int type, Vector2& screenPnt, SceneManager* scene);

	static Measure* CreateTmpDistanceMeasure(int firstShapeID, int secondShapID,
		int type,  SceneManager* scene);

	static bool CompleteDistanceMeasure(Measure* measure, int firstPntId, int secondPntId, 
		Vector2 screenPnt, SceneManager* scene);

    static string MeasureToXMLElement(SceneManager* scene , Measure *measure);
    /**
     * @brief 根据XML信息创建测量
     * @param xmlElement
     * @param scene 场景指针
     * @return 返回测量对象，若未创建成功则返回NULL
     */
    static Measure* CreateMeasureFromXMLElement(SceneManager* scene, const string& xmlElement);	/**
	 * @brief 获取角度测量对象
	 * @param firstShapeID 第一个shape　ID
	 * @param secondShapID 第二个shape　ID
	 * @param type 类型
	 * @param screenPnt 拾取的屏幕坐标
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* CreateAngleMeasure(int firstShapeID, int secondShapID,
			int type, Vector2& screenPnt, SceneManager* scene);

	static Measure* CreateTmpAngleMeasure(int firstShapeID, int secondShapID,
		int thirdShapeID, int forthShapID,
		int type, SceneManager* scene);

	static bool CompleteAngleMeasure(Measure* measure, int firstShapeID, int secondShapID,
		int thirdShapeID, int forthShapID,
		Vector2 screenPnt, SceneManager* scene);

	/**
	 * @brief 获取测量属性
	 * @param shapeID shape ID
	 * @param type 类型
	 * @param screenPnt 拾取的屏幕坐标
	 * @param scene 场景指针
	 * @return 返回属性测量对象
	 */
	static Measure* CreatePropertyMeasure(int shapeID, int type,
			Vector2& screenPnt, SceneManager* scene);

	/**
	* @brief 获取直径测量对象
	* @param shapeID shape ID
	* @param type 类型
	* @param screenPnt 拾取的屏幕坐标
	* @param scene 场景指针
	* @return 返回属性测量对象
	*/
	static Measure* CreateTmpDiametreMeasure(int shapeID, int type, SceneManager* scene);
	static bool CompleteDiametreMeasure(Measure* measure, int shapeID, Vector2 screenPnt, SceneManager* scene);
	/**
	 * @brief 创建面之间的距离测量对象
	 * @param firstShapeID shape ID
	 * @param secondShapID shape ID
	 * @param thirdShapeID shape ID
	 * @param forthShapID shape ID 一般为选择点
	 * @param type 测量类型
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回面面之间的测量对象
	 */
	static Measure* CreateFaceDistanceMeasure(int firstShapeID, int secondShapID,
			int thirdShapeID, int forthShapID,
			int type, Vector2& screenPnt, SceneManager* scene);

	static Measure* CreateTmpFaceDistanceMeasure(int firstShapeID, int secondShapID,
		int thirdShapeID, int forthShapID,
		int type, SceneManager* scene);

	static bool CompleteFaceDistanceMeasure(Measure* measure, int firstPntId, int secondPntId,
		int thirdShapeID, int forthShapID,
		Vector2 screenPnt, SceneManager* scene);
	/**
	 * @brief 创建面面之间的角度测量对象
	 * @param firstShapeID shape ID
	 * @param secondShapID shape ID
	 * @param thirdShapeID shape ID
	 * @param forthShapID shape ID 一般为选择点
	 * @param type 测量类型
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回面面之间的测量对象
	 */
	static Measure* CreateFaceAngleMeasure(int firstShapeID, int secondShapID,
			int thirdShapeID, int forthShapID,
			int type, Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 获取测量附加属性
	 * @param shapeID shape ID
	 * @param type 类型
	 * @param scene 场景指针
	 * @param outString 传出的附加属性字符串
	 */
	static void GetMeasureProperty(int shapeID, int type,
			 SceneManager* scene,string & outString);

	/**
	 * @brief 将测量对象加入场景
	 * @param scene 场景指针
	 * @param measure 测量对象
	 * @return 若成功返回 true 失败 false
	 */
	static bool AddMeasureToScene(SceneManager* scene,Measure* measure);

	/**
	 * @brief 更新测量对象图片位置
	 * @param measure 测量对象
	 * @param screenPnt 屏幕点
	 * @param scene 场景指针
	 * @return 若成功返回 true 失败 false
	 */
	static bool  UpdateMeasureImagePos(Measure* measure,Vector2& screenPnt,SceneManager* scene);

	static Measure* CreateSingleMeasure(int type, SceneManager* scene);
	static bool CreateMeasureLine(Measure* measure, string& name, Vector3& start, Vector3& end, Color& color);
	static bool CreateMeasurePoint(Measure* measure, int type, float siez, Vector3& pnt);
	static bool CreateMeasureImageBoard(Measure* measure, string& text, Vector3& position, SceneManager* scene);
private:
	/**
	 * @brief 创建点点距离测量对象
	 * @param firstPntId 第一个点 shape ID
	 * @param secondPntId 第二个点 shape ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createPntToPntDistance(int firstPntId, int secondPntId,
			Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerPntToPntDistance(int firstPntId, int secondPntId, SceneManager* scene);
	static bool completeDesignerPntToPntDistance(Measure* measure, int firstPntId, int secondPntId, 
		Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 创建点到线距离测量对象
	 * @param firstPntId 点的shape ID
	 * @param secondLineId 线的shape ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createPntTOLineDistance(int firstPntId, int secondLineId,
			Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerPntToLineDistance(int lineId, int pntId, SceneManager* scene);
	static bool completeDesignerPntToLineDistance(Measure* measure, int lineId, int pntId,
		Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 创建线与线之间的距离测量对象
	 * @param firstLineId 第一条线的shape ID
	 * @param secondLineId 第二条线的shape ID
	 * @param screenPnt 拾取的屏幕坐标点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createLineToLineDistance(int firstLineId, int secondLineId,
			Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerLineToLineDistance(int firstLineId, int secondLineId, SceneManager* scene);
	static bool completeDesignerLineToLineDistance(Measure* measure, int firstLineId, int secondLineId,
		Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 创建线到面的距离测量对象
	 * @param firstLineId 线的shape ID
	 * @param secondFaceId 面的shape ID
	 * @param screenPnt 拾取的屏幕坐标
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createLineToFaceDistance(int firstLineId, int secondFaceId,
			Vector2& screenPnt, SceneManager* scene);
	static Measure* createDesignerLineToFaceDistance(int firstFaceId, int secondPntId,
		int thirdPntId, SceneManager* scene);

	static bool completeDesignerLineToFaceDistance(Measure* measure, int firstFaceId, int secondPntId,
		int thirdPntId,
		Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 创建点到面的测量对象
	 * @param firstPntId 点的shapeID
	 * @param secondFaceId 线的shape ID
	 * @param screenPnt 拾取的屏幕坐标
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createPntToFaceDistance(int firstPntId, int secondFaceId,
			Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerPntToFaceDistance(int firstFaceId, int secondPntId, 
		int thirdPntId, SceneManager* scene);
	static bool completeDesignerPntToFaceDistance(Measure* measure, int firstLineId, int secondPntId,
		int thirdPntId,
		Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 创建面到面距离测量对象
	 * @param firstFaceId 第一个面的shape ID
	 * @param secondFaceId 第二个面的shape ID
	 * @param screenPnt 拾取的屏幕坐标
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createFaceToFaceDistance(int firstFaceId, int secondFaceId,
			Vector2& screenPnt, SceneManager* scene);

	/**
	* @brief 创建圆心轴到圆心轴距离测量对象
	* @param firstFaceId 第一个圆心轴的shape ID
	* @param secondFaceId 第二个圆心轴的shape ID
	* @param scene 场景指针
	* @return 返回测量对象，若未创建成功则返回NULL
	*/
	static Measure* createDesignerShaftToShaftDistance(int firstLineId, int secondLineId, SceneManager* scene);
	static bool completeDesignerShaftToShaftDistance(Measure* measure, int firstLineId, int secondLineId,
		Vector2& screenPnt, SceneManager* scene);

	/**
	* @brief 创建圆心到圆心距离测量对象
	* @param firstFaceId 第一个圆心轴的shape ID
	* @param secondFaceId 第二个圆心轴的shape ID
	* @param scene 场景指针
	* @return 返回测量对象，若未创建成功则返回NULL
	*/
	static Measure* createDesignerCenterToCenterDistance(int firstLineId, int secondLineId, SceneManager* scene);
	static bool completeDesignerCenterToCenterDistance(Measure* measure, int firstLineId, int secondLineId,
		Vector2& screenPnt, SceneManager* scene);

	/**
	* @brief 创建直线距离测量对象
	* @param firstFaceId 直线的shape ID
	* @param scene 场景指针
	* @return 返回测量对象，若未创建成功则返回NULL
	*/
	static Measure* createDesignerLineDistance(int firstLineId, SceneManager* scene);
	static bool completeDesignerLineDistance(Measure* measure, int firstLineId,
		Vector2& screenPnt, SceneManager* scene);

	/**
	* @brief 创建弧长测量对象
	* @param firstFaceId 直线的shape ID
	* @param scene 场景指针
	* @return 返回测量对象，若未创建成功则返回NULL
	*/
	static Measure* createDesignerArcDistance(int firstLineId, SceneManager* scene);
	static bool completeDesignerArcDistance(Measure* measure, int firstLineId,
		Vector2& screenPnt, SceneManager* scene);
/*****************************************************************************************/

	static Measure* createLineToFaceDistance(int firstLineId, int secondFaceId,
			int thirdPntId, int forthPntId,
			Vector2& screenPnt, SceneManager* scene);

	///
	static Measure* createPntToFaceDistance(int firstPntId, int secondFaceId,
			int thirdPntId, int forthPntId,
			Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 创建面面之间的距离测量对象
	 * @param firstFaceId 第一个面的 ID
	 * @param secondFaceId 第二个面的 ID
	 * @param thirdPntId 第一个选择点
	 * @param forthPntId 第二个选择点
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createFaceToFaceDistance(int firstFaceId, int secondFaceId,
			int thirdPntId, int forthPntId,
			Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerFaceToFaceDistance(int firstFaceId, int secondFaceId,
		int thirdPntId, int forthPntId, SceneManager* scene);
	static bool completeDesignerFaceToFaceDistance(Measure* measure, int firstFaceId, int secondFaceId,
		int thirdPntId, int forthPntId,
		Vector2& screenPnt, SceneManager* scene);
	/*************************************************************************/
	//测量角度
	/**
	 * @brief 创建线与线之间的角度测量对象
	 * @param firstLineId 第一条线的shape ID
	 * @param secondLineId 第二条线的shape ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createLineToLineAngle(int firstLineId, int secondLineId,
			Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerLineToLineAngle(int firstPntId, int secondPntId, SceneManager* scene);
	static bool completeDesignerLineToLineAngle(Measure* measure, int firstLineId, int secondLineId,
		Vector2& screenPnt, SceneManager* scene);
	/**
	 * @brief 创建面面之间的角度测量对象
	 * @param firstFaceId 第一个面的shape ID
	 * @param secondFaceId 第二个面的shape ID
	 * @param screenPnt 拾取的屏幕中心点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createFaceToFaceAngle(int firstFaceId, int secondFaceId,
			Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerFaceToFaceAngle(int firstFaceId, int secondFaceId,
		int thirdPntId, int forthPntId, SceneManager* scene);

	static bool completeDesignerFaceToFaceAngle(Measure* measure, int firstFaceId, int secondFaceId,
		int thirdPntId, int forthPntId,
		Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 创建线和平面之间的角度测量对象
	 * @param firstLineId 线的shapeID
	 * @param secondFaceId 面的shape ID
	 * @param screenPnt 拾取的屏幕坐标
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createLineToFaceAngle(int firstLineId, int secondFaceId,
			Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerLineToFaceAngle(int firstFaceId, int secondLineId,
		int thirdPntId, SceneManager* scene);
	static bool completeDesignerLineToFaceAngle(Measure* measure, int firstFaceId, int secondLineId,
		int thirdPntId, Vector2& screenPnt, SceneManager* scene);

	/**
	* @brief 创建直径测量对象
	* @param lineId 直线的shape ID
	* @param screenPnt 拾取的屏幕坐标
	* @param scene 场景指针
	* @return 返回测量对象，若未创建成功则返回NULL
	*/
	static Measure* createDesignerDiametre(int lineId, SceneManager* scene);
	static bool completeDesignerDiametre(Measure* measure, int lineId, Vector2& screenPnt, SceneManager* scene);

	static Measure* createDesignerRadius(int lineId, SceneManager* scene);
	static bool completeDesignerRadius(Measure* measure, int lineId, Vector2& screenPnt, SceneManager* scene);
/*************************************************************************************/
	/// cdj
	/**
	 * @brief 创建面面之间的角度测量对象
	 * @param firstFaceId 线的shape ID
	 * @param secondFaceId 面的shape ID
	 * @param thirdPntId 选中的第一点
	 * @param forthPntId 选中的第二点
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createFaceToFaceAngle(int firstFaceId, int secondFaceId,
			int thirdPntId, int forthPntId,
			Vector2& screenPnt, SceneManager* scene);

	/**
	 * @brief 创建线面之间的角度测量对象
	 * @param firstLineId 线的shape ID
	 * @param secondFaceId 面的shape ID
	 * @param thirdPntId 选中的第一点
	 * @param forthPntId 选中的第二点
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createLineToFaceAngle(int firstLineId, int secondFaceId,
			int thirdPntId, int forthPntId,
			Vector2& screenPnt, SceneManager* scene);


	/*************************************************************************/
	//创建属性测量
	/**
	 * @brief 创建点属性测量对象
	 * @param shapeId 点的ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createPntProperty(int shapeId, Vector2& screenPnt,
			SceneManager* scene);

	/**
	 * @brief 创建线的属性测量对象
	 * @param shapeId 线的ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createLineProperty(int shapeId, Vector2& screenPnt,
			SceneManager* scene);
	/**
	 * @brief 创建点属性测量对象
	 * @param shapeId 点的ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createCircleProperty(int shapeId, Vector2& screenPnt,
			SceneManager* scene);
	/**
	 * @brief 创建面属性测量对象
	 * @param shapeId 面的ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createFaceProperty(int shapeId, Vector2& screenPnt,
			SceneManager* scene);

	/**
	 * @brief 创建孔属性测量对象
	 * @param shapeId 孔的ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createApertureProperty(int shapeId, Vector2& screenPnt,
			SceneManager* scene);

	/**
	 * @brief 创建模型属性测量对象
	 * @param shapeId 模型的ID
	 * @param screenPnt 拾取的屏幕点
	 * @param scene 场景指针
	 * @return 返回测量对象，若未创建成功则返回NULL
	 */
	static Measure* createModelProperty(int shapeId, Vector2& screenPnt,
			SceneManager* scene);

	/****************************************************************************/

	//获取测量对象属性

	/**
	 * @brief 获取点属性
	 * @param shapeId shape ID
	 * @param scene 场景指针
	 * @param propertyStr 属性字符串
	 */
	static void GetPntProperty(int shapeId,
			SceneManager* scene,string & propertyStr);

	/**
	 * @brief 获取线属性
	 * @param shapeId shape ID
	 * @param scene 场景指针
	 * @param propertyStr 属性字符串
	 */
	static void GetLineProperty(int shapeId,
			SceneManager* scene,string & propertyStr);

	/**
	 * @brief 获取圆弧属性
	 * @param shapeId shape ID
	 * @param scene 场景指针
	 * @param propertyStr 属性字符串
	 */
	static void GetCircleProperty(int shapeId,
			SceneManager* scene,string & propertyStr);

	/**
	 * @brief 获取面属性
	 * @param shapeId shape ID
	 * @param scene 场景指针
	 * @param propertyStr 属性字符串
	 */
	static void GetFaceProperty(int shapeId,
			SceneManager* scene,string & propertyStr);


	/**
	 * @brief 获取模型属性
	 * @param shapeId shape ID
	 * @param scene 场景指针
	 * @param propertyStr 属性字符串
	 */
	static void GetModelProperty(int shapeId,
			SceneManager* scene,string & propertyStr);

	/**
	* @brief 获取模型统计信息
	* @param shapeId shape ID
	* @param scene 场景指针
	* @param propertyStr 属性字符串
	*/
	static void GetModelStatistics(int shapeId,
		SceneManager* scene, string & propertyStr);
	
	/*****************************************************************************/

	static bool CreateCircular(Measure* measure, IShape* shape, Vector3& verticVector, Vector3& center, float& radius);
	static bool CreateCircularArc(Measure* measure, IShape* shape, Vector3& verticVector, Vector3& center, float& radius);
	static void DeleteMeasureLine(Measure* measure);

public:
	MeasureFactory();

	virtual ~MeasureFactory();
private:
	static Color m_leaderColor;//!<引线颜色
	static Color m_selectColor;//!<选择颜色
	static Color m_exLineColor;//!<延长线颜色
	static Color m_measureColor1;//!<第一个测量结果颜色
	static Color m_measureColor2;//!<第二个测量结果颜色
	static Color m_measureColor3;//!<第三个测量结果颜色

public:
	/*
	* 以以下为序列化和反序列化view文件的标签定义，详见view描述
	*/
	const static string Serializer_XML_Element_SVL;

	const static string Serializer_XML_Element_Model;
	const static string Serializer_XML_Element_Users;
	const static string Serializer_XML_Element_User;
	const static string Serializer_XML_Element_Measures;
	const static string Serializer_XML_Element_Color;

    const static string Serializer_XML_Element_ImageBoard ;
    const static string Serializer_XML_Element_Center ;
	const static string Serializer_XML_Element_Line3Ds;
	const static string Serializer_XML_Element_Line3D;
	const static string Serializer_XML_Element_Points;
	const static string Serializer_XML_Element_Point;
	const static string Serializer_XML_Element_Position;
	const static string Serializer_XML_Element_Direction;

	const static string Serializer_XML_Element_MeasureTexts;
	const static string Serializer_XML_Element_Texts;
	const static string Serializer_XML_Element_Text;
	const static string Serializer_XML_Element_Location;

	const static string Serializer_XML_Element_Attributes;
	const static string Serializer_XML_Element_Attribute;

	const static string Serializer_XML_Attribute_ID;
	const static string Serializer_XML_Attribute_MeasureType;
	const static string Serializer_XML_Attribute_UserID;
	const static string Serializer_XML_Attribute_Created;
	const static string Serializer_XML_Attribute_R;
	const static string Serializer_XML_Attribute_G;
	const static string Serializer_XML_Attribute_B;
	const static string Serializer_XML_Attribute_Color;
	const static string Serializer_XML_Attribute_X;
	const static string Serializer_XML_Attribute_Y;
	const static string Serializer_XML_Attribute_Z;
	const static string Serializer_XML_Attribute_Value;
	const static string Serializer_XML_Attribute_Length;
	const static string Serializer_XML_Attribute_Type;
	const static string Serializer_XML_Attribute_Key;
	const static string Serializer_XML_Attribute_URI;
	const static string Serializer_XML_Attribute_Width;
	const static string Serializer_XML_Attribute_Height;
	const static string Serializer_XML_Attribute_Name;
};

}
#endif

