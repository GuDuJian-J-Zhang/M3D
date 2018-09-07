#ifndef	M3D_ANNOTATION_GROUP_H_
#define M3D_ANNOTATION_GROUP_H_

#include "m3d/scene/GroupNode.h"
#include "m3d/M3D.h"

namespace M3D
{
	class Annotation;
	/**
	* @class 批注组类，用来临时存放批注对象
	*/
	class M3D_API AnnotationGroup : public GroupNode
	{
	public:
		AnnotationGroup();
		virtual ~AnnotationGroup();
		/**
		* @brief 获取类型
		* @return 批注类型
		*/
		//virtual int GetType(void);
		/**
		* @brief 初始化
		*/
		void Initialize();
		/**
		* @brief 清空
		*/
		void Clear();

		/**
		* @brief 设置场景
		* @param scene 场景指针
		*/
		void SetScene(SceneManager* scene);
		/**
		* @brief 获取场景指针
		* @return 场景指针
		*/
		SceneManager* GetScene();

	private:
		SceneManager* m_scene;//!<当前场景指针
	};
}
#endif
