#ifndef	   M3D_GROUNDNODE_H_
#define    M3D_GROUNDNODE_H_

#include "m3d/base/Color.h"
#include "m3d/scene/SceneNode.h"

namespace M3D
{
	class Texture;
	class ResourceManager;
	/**
	* ���Ʊ�����ɫ��֧����ɫ���ƺ�ͼƬ�������ַ�ʽ
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
		* ���û��Ʊ����Ŀ�͸�
		* @param width
		* @param height
		*/
		void SetGroundSize(int size, int divisions, Vector3 center = Vector3::ZERO);

		/**
		* ���ñ���ɫ������ɫ
		* @param topColor
		*/
		void SetColor(const Color& color1, const Color& color2);


		/**
		* ��ȡ���ƶ�����ɫ
		* @param topColor
		*/
		Color* GetColors();

		/**
		* ��ȡ���Ʊ�������Ҫ�ĵ�
		* @return
		*/
		Vector3* GetVertexs();

		void SetResourceManager(ResourceManager * resMgr);

		int GetPointsNumber();

	protected:
		//	virtual void FinalRelease(void); // Container.h
		void Initial();

		vector<Vector3> points; //!<
		vector<Color> colors; //!<��������������������

	private:
		ResourceManager* m_resMgr;
		int m_size;
		int m_divisions;
		bool m_isDirty;
		Vector3 m_center;

	};
}
#endif // BACKGROUNDCOLORNODE_H
