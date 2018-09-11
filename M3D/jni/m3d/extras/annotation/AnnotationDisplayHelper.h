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
		* @brief ������ע�ı�ͼƬ
		* @param scene ����ָ��
		* @param textsArray �ı�����
		* @param position ��ʾλ��
		* @return �������ɵ��ı�ͼƬ
		*/
		static ImageBoard* createAnnotationImage(SceneManager* scene,
					string& annotationNote, float& fontSize, int& rows, Vector2& rectStart, Vector2& rectEnd,
					Color &frameColor, Color& fillColor, Vector3 & position);
		//static ImageBoard* createAnnotationImage(SceneManager* scene, vector<Texts2D*>& textsArray, Color &frameColor, Color& fillColor, Vector3 & position);

		/**
		* @brief ������ע�ı�
		* @param scene ����ָ��
		* @param textsArray �ı�����
		* @param position ��ʾλ��
		* @return �������ɵ��ı�ͼƬ
		*/
		static ImageBoard* createAnnotation(SceneManager* scene,
			string& annotationNote, float& fontSize, int& rows,
			Vector2& rectStart, Vector2& rectEnd, Vector3 & position);
		/**
		* @brief ������ע����ͼƬ�Ĳ���
		* @param shape2DSet shape����
		* @param rect1Start ����1��ʼλ��
		* @param rect1End ����1����λ��
		* @param lineColor �ߵ���ɫ
		* @param lineWidth �ߵĿ��
		* @param backColor ������ɫ
		* @param fontColor ������ɫ
		* @param fontSize �����С
		* @param str �ı�
		*/
		static void CreateAnnotationRectangleImage(Shape2DSet*shape2DSet, Vector2 & rect1Start, Vector2 & rect1End,
			Color & lineColor, float lineWidth, Color & backColor, Color& fontColor, float fontSize, int& rows, string str);
		static void CreateAnnotationRectangleImage(Shape2DSet*shape2DSet, Vector2 & rect1Start, Vector2 & rect1End,
			Color & backColor, Color& fontColor, float fontSize, int& rows, string str);

		static void GetAnnotationTextRectInfo(SceneManager* scene, vector<Texts2D*>& textsArray, string& annoNote, 
			float& fontSize, Vector2& rectStart, Vector2& rectEnd, int& rows);

		/**
		* @brief ������������
		* @param str �ַ���
		* @param notZH �������ĵĸ���
		* @return ���������ַ�����
		*/
		static string countChar(string &str, int &rowCount, int& clomnCount);
	};
}
#endif
