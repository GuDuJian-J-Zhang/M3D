#ifndef	   M3D_GROUNDNODE_H_
#define    M3D_GROUNDNODE_H_

#include "m3d/base/Color.h"
#include "m3d/scene/SceneNode.h"

namespace M3D
{
	class Texture;
	class ResourceManager;
	/**
	* 绘制背景颜色，支持颜色绘制和图片绘制两种方式
	*/
	class M3D_API GroundNode : public SceneNode
	{
	public:
		GroundNode();
		virtual ~GroundNode();
		virtual int GetType(void);
		virtual void FindVisiableObject(RenderAction* renderAction);
	public:
		/**
		* 设置绘制背景的宽和高
		* @param width
		* @param height
		*/
		void SetGroundSize(int size, int divisions, Vector3 center = Vector3::ZERO);

		/**
		* 设置背景色顶部颜色
		* @param topColor
		*/
		void SetColor(const Color& color1, const Color& color2);


		/**
		* 获取绘制顶部颜色
		* @param topColor
		*/
		Color* GetColors();

		/**
		* 获取绘制背景所需要的点
		* @return
		*/
		Vector3* GetVertexs();

		void SetResourceManager(ResourceManager * resMgr);

		int GetPointsNumber();

	protected:
		//	virtual void FinalRelease(void); // Container.h
		void Initial();

		vector<Vector3> points; //!<
		vector<Color> colors; //!<背景绘制所需纹理坐标

	private:
		ResourceManager* m_resMgr;
		int m_size;
		int m_divisions;
		bool m_isDirty;
		Vector3 m_center;

	};
}
#endif // BACKGROUNDCOLORNODE_H
