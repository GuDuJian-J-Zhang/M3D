/**@file
 *@brief	测量对象显示数据生成工具类
 *@author
 *@date		2015-8-17
 *@version	1.0 
 *@Copyright All Rights Reserved. Copyright (C) 2016 HOTEAMSOFT, Ltd
 *
 */

#ifndef M3D_MEASURE_DISPLAY_HELPER_H_
#define M3D_MEASURE_DISPLAY_HELPER_H_

#include "m3d/M3D.h"

#include "m3d/base/Vector2.h"
#include "m3d/base/Vector3.h"
#include "m3d/base/Color.h"
#include "m3d/base/Rect.h" 
#include "m3d/extras/measure/tools/MeasureDisplayHelper.h"

namespace M3D
{
class MeasureAngle;
class MeasureDistance;
class MeasureProperty;
class SceneManager;
class ImageBoard;
class Measure;

/**
 * 图形类型枚举
 */
typedef enum Shape2DType
{
	SHAPE2D_BASE = 0, //!< 顶点
	SHAPE2D_POINT = 1, //!< 边
	SHAPE2D_LINE = 2, //!< 边
	SHAPE2D_RECT = 3, //!< 边
	SHAPE2D_TEXT = 4, //!< 文本
	SHAPE2D_ELLIPSE = 5 //椭圆
} SHAPE2DTYPE;

/**
 *	@brief 形状基类
 */
class M3D_API Shape2D
{
public:
	Shape2D();
	virtual ~Shape2D();

	/**
	 * @brief	转换为Json串
	 * @return:	string @see toJson定义
	 * @note
	 */
	virtual string toJson();
	/**
	 * @brief	Json串解析
	 * @param: 	const string & json
	 * @return:	bool
	 * @note
	 */
	virtual bool fromJson(const string& json);

	/**
	 * @brief	设置颜色
	 * @param: 	const Color & color
	 * @return:	void
	 * @note
	 */
	void SetColor(const Color& color);
	/**
	 * @brief	获取颜色
	 * @return:	M3D::Color&
	 * @note	颜色值
	 */
	Color& GetColor();

	/**
	 * @brief	获取类型
	 * @return:	M3D::SHAPE2DTYPE 类型
	 * @note
	 */
	SHAPE2DTYPE GetType()
	{
		return this->m_type;
	};

private:
	/**
	 * @brief	初始化
	 * @return:	void
	 * @note
	 */
	void Init();
public:
	Color m_color; //!< 颜色
	SHAPE2DTYPE m_type; //!<类型
};

/**
 *  @brief 2D文本类
 */
class M3D_API Texts2D: public Shape2D
{
public:
	Texts2D();
	virtual ~Texts2D();
	/**
	 * @brief 	转换为json串
	 * @return
	 */
	virtual string toJson();
	/**
	 * @brief 从Json串解析
	 * @param json
	 * @return
	 */
	virtual bool fromJson(const string& json);

public:

	/**
	 * @brief 初始化
	 */
	void Init();

	Vector2 m_start;//!<开始位置
	string m_texts;//!<文本内容

	string m_fontName;//!<字体名称
	float m_size;//!<字体大小
	bool m_alignCenter;//!<排列中心
};

/**
 *  @brief 2D线段类
 */
class M3D_API Line2D: public Shape2D
{
public:
	Line2D();
	virtual ~Line2D();

	/**
	 * @brief	转换为Json串
	 * @return	返回Json串
	 */
	virtual string toJson();
	/**
	 * @brief 从JSon串中解析
	 * @param json
	 * @return
	 */
	virtual bool fromJson(const string& json);

public:
	/**
	 * @brief 初始化
	 */
	void Init();

	Vector2 m_start;//!<开始位置
	Vector2 m_end;//!<结束位置
	int m_lineType; //!<线型
	float m_blod; //!<线的粗细
};

/**
 *  @brief 2D矩形类
 */
class M3D_API Rect2D: public Shape2D
{
public:
	Rect2D();
	virtual ~Rect2D();

	/**
	 * @brief 转换为Json串
	 * @return	返回Json字符串
	 */
	virtual string toJson();
	/**
	 * @brief 从Json串解析
	 * @param json
	 * @return
	 */
	virtual bool fromJson(const string& json);

public:
	/**
	 * @brief 初始化函数
	 */
	void Init();

	Vector2 m_start;//!< 开始位置
	Vector2 m_end; //!<结束位置

	int m_style;//!<格式
	Color m_background;//!<背景

	float m_strokeWidth; //!<线宽
	bool m_stroke; //!<是否空心
	bool m_round; //!<是否圆角矩形

	Vector2 m_radius; //!<圆角矩形
};

/**
 * @brief 椭圆类
 */
class M3D_API Ellipse2D: public Shape2D
{
public:
	Ellipse2D();
	~Ellipse2D();
	/**
	 * @brief 初始化函数
	 */
	void Init();
	/**
	 * @brief 转换为Json串
	 * @return	返回Json字符串
	 */
	virtual string toJson();
	/**
	 * @brief 从Json串解析
	 * @param json
	 * @return
	 */
	virtual bool fromJson(const string& json);
public:

	Vector2 m_start; //!<开始位置
	Vector2 m_end; //!<外接矩形 对角线端点
	Color m_background;//!<结束位置

	float m_strokeWidth; //!<线宽
	bool m_isFill; //!<是否填充

};

/**
 * @brief 按照行列方式显示数据
 */
class M3D_API Shape2DSet: public Shape2D
{
public:
	Shape2DSet();
	virtual ~Shape2DSet();
	/**
	 * @brief 添加shape实例
	 * @param shape2d
	 * @return 若成功 返回true 失败 false
	 */
	bool AddShape2D(Shape2D* shape2d);

	/**
	 * @brief 转换为Json串
	 * @return 返回Json串
	 */
	virtual string toJson();
	/**
	 * @brief 从Json串解析
	 * @param json
	 * @return
	 */
	virtual bool fromJson(const string& json);

	/**
	 * @brief 获取shape列表
	 * @return 返回shape列表
	 */
	vector<Shape2D*>& GetShapes();

	/**
	 * @brief 获取包围整个shape矩形
	 * @return 返回矩形
	 */
	Rect GetRect();

private:
	/**
	 * @brief 初始化
	 */
	void Init();

private:
	vector<Shape2D*> m_displayShapes;//!<shpe列表
};

/**
 * @brief 测量辅助类
 */
class M3D_API MeasureDisplayHelper
{
public:
	MeasureDisplayHelper();
	virtual ~MeasureDisplayHelper();
	/**
	 * @brief 创建矩形效果
	 * @param shape2DSet shape集合
	 * @param rect1Start 第一个矩形开始位置
	 * @param rect1End 第一个矩形结束位置
	 * @param rect2L 长度
	 * @param rect1Color 第一个矩形颜色
	 * @param rect2Color 第二个矩形颜色
	 * @param text1Color 左边文本颜色
	 * @param text2Color 右边文本颜色
	 * @param str1 左边文本
	 * @param str2 右边文本
	 * @param fontSize 字体大小
	 */

	static void createRectImage(Shape2DSet*shape2DSet, Vector2 & rect1Start, Vector2 & rect1End, float rect2L,
			Color& rect1Color, Color & rect2Color, Color& text1Color, Color & text2Color, string str1, string str2,
			float fontSize, bool drawD3);


	static void MeasureText(const string& measureText, float fontSize, const string& fontName, Vector2 & o_rect1Start, Vector2 & o_rect1End);

	/**
	 * @brief 创建矩形阴影
	 * @param shape2DSet shape集合
	 * @param rect1Start 矩形开始位置
	 * @param rect1End 矩形结束位置
	 * @param rect2L 长度
	 */
	static void createShadowRect(Shape2DSet*shape2DSet, Vector2 & rect1Start, Vector2 & rect1End, float rect2L);
	/**
	 * @brief 一定放在最后使用,内部已经把shape2DSet内存释放
	 * @param scene 场景指针
	 * @param imageboard 公告板
	 * @param shape2DSet shape集合
	 * @param pntInPlane 摄像机射线在投影平面的投影点
	 * @param wideFactor 宽度比例
	 * @param heightFactor 高度比例
	 */

	static void addImageToMemory(SceneManager* scene, ImageBoard* &imageboard, Shape2DSet * shape2DSet,
			Vector3 pntInPlane, float wideFactor, float heightFactor);

	/**
	 * @brief 获取相对位置
	 * @param positon 顶点vector
	 * @param count 数量
	 * @param rectangle2Wight 矩形宽度
	 * @param fontsize 字体大小
	 */
	static void GetRelativePosition(vector<Vector2>& positon, int count, float rectangle2Wight, float fontsize);

	/**
	 * @brief 计算中文字数
	 * @param str 字符串
	 * @param notZH 不是中文的个数
	 * @return 返回中文字符个数
	 */
	static int countChar(string &str, int &notZH);


	/**
	 * @brief 创建批注文字显示的图片，暂时在这里实现
	 * @param scene 场景指针
	 * @param textsArray 文本数组
	 * @param position 显示位置
	 * @return 返回生成的批注图片
	 */
	static ImageBoard* createNoteTextsImage(SceneManager* scene, vector<Texts2D*>& textsArray, Vector3 & position);

	/**
	 * @brief 创建文本图片
	 * @param scene 场景指针
	 * @param textsArray 文本数组
	 * @param position 显示位置
	 * @return 返回生成的文本图片
	 */
	static ImageBoard* createNoteTextsImageN(SceneManager* scene, vector<Texts2D*>& textsArray, Vector3 & position);

	/**
	 * @brief 创建批注矩形图片的参数
	 * @param shape2DSet shape集合
	 * @param rect1Start 矩形1开始位置
	 * @param rect1End 矩形1结束位置
	 * @param lineColor 线的颜色
	 * @param lineWidth 线的宽度
	 * @param backColor 背景颜色
	 * @param fontColor 字体颜色
	 * @param fontSize 字体大小
	 * @param str 文本
	 */
	static void CreateNoteRectangleImage(Shape2DSet*shape2DSet, Vector2 & rect1Start, Vector2 & rect1End,
			Color & lineColor, float lineWidth, Color & backColor, Color& fontColor, float fontSize, string str);

	/**
	 * @brief 创建序号批注
	 * @param scene 场景指针
	 * @param text 文本指针
	 * @param position 显示位置
	 * @return 返回文本生成的图片
	 */
	static ImageBoard* CreateSequenceNumberImage(SceneManager* scene, Texts2D* text, Vector3 & position);

	/**
	 * @brief 设置测量结果单位，一开始是按照mm来算
	 * @param unitFlag 单位标志
	 * @param value 数值
	 * @param unit 单位
	 */
	static void SetMeasureUnit(int unitFlag, vector<float>&value, string & unit);
    static float GetInnerUnitValue(float srcValue, string& unit);
	/**
	 * @brief 选择的面显示矩形
	 * @param lineArray line数组
	 * @param boxCenter 包围盒中心
	 * @param direction 方向
	 * @param faceNormal 面的法向量
	 */
	static void CreateSelectFaceMark(vector<Vector3> &pointArray, Vector3 & boxCenter, Vector3 & direction,
			Vector3 & faceNormal, float length);
	/**
	 * @brief 角度符号
	 * @param line1Start 第一条线起点
	 * @param line1End	第一条线结束点
	 * @param line2Start 第二条线起点
	 * @param line2End 第二条线结束点
	 * @param theta 角度
	 * @param lines 端点值集合
	 */

	static void CreateAngleMark(const Vector3 & line1Start, const Vector3 & line1End, const Vector3& line2Start,
			const Vector3 & line2End, float theta, vector<Vector3>& lines);

	static void CreateDesignerAngleMark(const Vector3 & line1Start, const Vector3 & line1End, 
		const Vector3& line2Start, const Vector3 & line2End, 
		const Vector3& center,  float radius, float theta, vector<Vector3>& lines);

	/**
	 * @brief 从字符串获取附加属性值 //TODO
	 * @param propertyStr 附加属性字符串
	 * @param property
	 * @return 成功 true  失败 false
	 */
	static bool GetPropertyFromString(string & propertyStr, string & property);
private:
	static Color m_textColor; //!<文本颜色
	static Color m_lineColor; //!<线的颜色

};

}

#endif /*M3D_MEASURE_DISPLAY_HELPER_H_*/

