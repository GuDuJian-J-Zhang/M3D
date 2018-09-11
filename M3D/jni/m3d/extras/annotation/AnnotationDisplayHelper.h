#ifndef M3D_ANNOTATION_DISPLAY_HELPER_H_
#define M3D_ANNOTATION_DISPLAY_HELPER_H_

#include "m3d/M3D.h"

namespace M3D {

	class SceneManager;
	class ImageBoard;
	class Texts2D;
	class Color;
	class Vector3;
	class Vector2;
	class Shape2DSet;

	class M3D_API AnnotationDisplayHelper
	{
	public:
		AnnotationDisplayHelper();
		virtual ~AnnotationDisplayHelper();

		/**
		* @brief 创建批注文本图片
		* @param scene 场景指针
		* @param textsArray 文本数组
		* @param position 显示位置
		* @return 返回生成的文本图片
		*/
		static ImageBoard* createAnnotationImage(SceneManager* scene,
					string& annotationNote, float& fontSize, int& rows, Vector2& rectStart, Vector2& rectEnd,
					Color &frameColor, Color& fillColor, Vector3 & position);
		//static ImageBoard* createAnnotationImage(SceneManager* scene, vector<Texts2D*>& textsArray, Color &frameColor, Color& fillColor, Vector3 & position);

		/**
		* @brief 创建批注文本
		* @param scene 场景指针
		* @param textsArray 文本数组
		* @param position 显示位置
		* @return 返回生成的文本图片
		*/
		static ImageBoard* createAnnotation(SceneManager* scene,
			string& annotationNote, float& fontSize, int& rows,
			Vector2& rectStart, Vector2& rectEnd, Vector3 & position);
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
		static void CreateAnnotationRectangleImage(Shape2DSet*shape2DSet, Vector2 & rect1Start, Vector2 & rect1End,
			Color & lineColor, float lineWidth, Color & backColor, Color& fontColor, float fontSize, int& rows, string str);
		static void CreateAnnotationRectangleImage(Shape2DSet*shape2DSet, Vector2 & rect1Start, Vector2 & rect1End,
			Color & backColor, Color& fontColor, float fontSize, int& rows, string str);

		static void GetAnnotationTextRectInfo(SceneManager* scene, vector<Texts2D*>& textsArray, string& annoNote, 
			float& fontSize, Vector2& rectStart, Vector2& rectEnd, int& rows);

		/**
		* @brief 计算中文字数
		* @param str 字符串
		* @param notZH 不是中文的个数
		* @return 返回中文字符个数
		*/
		static string countChar(string &str, int &rowCount, int& clomnCount);
	};
}
#endif
