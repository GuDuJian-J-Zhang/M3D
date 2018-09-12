#ifndef     M3D_ANNOTATION_FACTORY_H_
#define     M3D_ANNOTATION_FACTORY_H_

#include "m3d/base/Object.h"
/**
* @brief ������ע�ࡣ����������ע
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
		* @brief ������ע
		* @param firstShapId shape ID
		* @param secondShapeId shape ID
		* @param firstPnt ʰȡ����Ļ��
		* @param secondPnt ʰȡ����Ļ��
		* @param thirdPnt ʰȡ����Ļ��
		* @param text �ı�����
		* @param scene ����
		* @return ������ע ʧ�� ΪNULL
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
		* @brief ����ע���볡��
		* @param scene ����
		* @param note ��ע
		* @return �ɹ� true ʧ�� false
		*/
		static bool AddNoteToScene(SceneManager* scene, Note* note);

		static void ModifyPntAnnotation(Annotation* annotation, RenderAction* renderAction);

		static float calculateGap(Annotation* annotation, float dragX, SceneManager* scene);
		/**
		* @brief ������עͼƬ��ʾλ��
		* @return �ɹ� true ʧ�� false
		*/
		static bool UpdateNoteImagePos(Note* note, Vector2& screenPnt, SceneManager* scene);

		/**
		* @brief ������ʱ��
		* @param scene ����
		* @return �ɹ� true ʧ�� false
		*/
		static IDTYPE CreateTmpPoint(Vector2& screenPnt, int ShapId, SceneManager* scene);
        
	public:
		AnnotationFactory();
		~AnnotationFactory();

	private:
		/**
		* @brief ������ע
		* @param firstShapId shape ID
		* @param secondShapeId shape ID
		* @param locatePnt ʰȡ����Ļ��
		* @param displayPnt ʰȡ����Ļ��
		* @param text �ı�
		* @param scene ����
		* @return ������ע ʧ�� NULL
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
		* @brief ���²�������ͼƬλ��
		* @param Annotation ��ע����
		* @param screenPnt ��Ļ��
		* @param scene ����ָ��
		* @return ���ɹ����� true ʧ�� false
		*/
		static bool  UpdateAnnotationImagePos(Annotation* annotation, Vector2& screenPnt, SceneManager* scene);
	};
}

#endif
