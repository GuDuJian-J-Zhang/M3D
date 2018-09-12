#ifndef     M3D_ANNOTATION_FACTORY_H_
#define     M3D_ANNOTATION_FACTORY_H_

#include "m3d/base/Object.h"
/**
* @brief 创建标注类。用来创建批注
*/
namespace M3D
{
	class Vector2;
	class Vector3;
	class SceneManager;
	class Note;
	class Color;
	class Annotation;
	class RenderAction;

	class M3D_API AnnotationFactory : public Object
	{
	public:
		/**
		* @brief 创建批注
		* @param firstShapId shape ID
		* @param secondShapeId shape ID
		* @param firstPnt 拾取的屏幕点
		* @param secondPnt 拾取的屏幕点
		* @param thirdPnt 拾取的屏幕点
		* @param text 文本内容
		* @param scene 场景
		* @return 返回批注 失败 为NULL
		*/
		static Annotation* CreateAnnotation(int firstShapId,
			Vector2& firstPnt, Vector2& secondPnt,
			const string& text, Color& frameColor, Color& fillColor, 
			bool bLenderLine, bool bEnvelope, bool bStub, bool bFixed, 
			SceneManager* scene);
		static bool CreateAnnotation(Annotation* annotation, const string& text,
			Color& frameColor, Color& fillColor,
			bool bLenderLine, bool bEnvelope, bool bStub, bool bFixed,
			SceneManager* scene);
		static bool CreateAnnotation(Annotation* annotation, const string& text,
			Color& frameColor, Color& fillColor,
			SceneManager* scene);
		/**
		* @brief 将批注加入场景
		* @param scene 场景
		* @param note 批注
		* @return 成功 true 失败 false
		*/
		static bool AddNoteToScene(SceneManager* scene, Note* note);

		static void ModifyPntAnnotation(Annotation* annotation, RenderAction* renderAction);

		static float calculateGap(Annotation* annotation, float dragX, SceneManager* scene);
		/**
		* @brief 更改批注图片显示位置
		* @return 成功 true 失败 false
		*/
		static bool UpdateNoteImagePos(Note* note, Vector2& screenPnt, SceneManager* scene);

		/**
		* @brief 创建临时点
		* @param scene 场景
		* @return 成功 true 失败 false
		*/
		static IDTYPE CreateTmpPoint(Vector2& screenPnt, int ShapId, SceneManager* scene);
        
	public:
		AnnotationFactory();
		~AnnotationFactory();

	private:
		/**
		* @brief 创建批注
		* @param firstShapId shape ID
		* @param secondShapeId shape ID
		* @param locatePnt 拾取的屏幕点
		* @param displayPnt 拾取的屏幕点
		* @param text 文本
		* @param scene 场景
		* @return 返回批注 失败 NULL
		*/
		static Annotation* CreatePntAnnotation(int firstShapId,
			Vector2& firstPnt, Vector2& secondPnt,
			const string& text, Color& frameColor, Color& fillColor, 
			bool bLenderLine, bool bEnvelope, bool bStub, bool bFixed, 
			SceneManager* scene);
		static bool CreatePntAnnotation(Annotation* annotation, const string& text,
			Color& frameColor, Color& fillColor,
			bool bLenderLine, bool bEnvelope, bool bStub, bool bFixed,
			SceneManager* scene);
		static bool CreatePntAnnotation(Annotation* annotation, const string& text,
			Color& frameColor, Color& fillColor,
			SceneManager* scene);

		/**
		* @brief 更新测量对象图片位置
		* @param Annotation 批注对象
		* @param screenPnt 屏幕点
		* @param scene 场景指针
		* @return 若成功返回 true 失败 false
		*/
		static bool  UpdateAnnotationImagePos(Annotation* annotation, Vector2& screenPnt, SceneManager* scene);
	};
}

#endif
