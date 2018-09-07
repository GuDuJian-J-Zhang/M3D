#ifndef	M3D_ANNOTATION_GROUP_H_
#define M3D_ANNOTATION_GROUP_H_

#include "m3d/scene/GroupNode.h"
#include "m3d/M3D.h"

namespace M3D
{
	class Annotation;
	/**
	* @class ��ע���࣬������ʱ�����ע����
	*/
	class M3D_API AnnotationGroup : public GroupNode
	{
	public:
		AnnotationGroup();
		virtual ~AnnotationGroup();
		/**
		* @brief ��ȡ����
		* @return ��ע����
		*/
		//virtual int GetType(void);
		/**
		* @brief ��ʼ��
		*/
		void Initialize();
		/**
		* @brief ���
		*/
		void Clear();

		/**
		* @brief ���ó���
		* @param scene ����ָ��
		*/
		void SetScene(SceneManager* scene);
		/**
		* @brief ��ȡ����ָ��
		* @return ����ָ��
		*/
		SceneManager* GetScene();

	private:
		SceneManager* m_scene;//!<��ǰ����ָ��
	};
}
#endif
