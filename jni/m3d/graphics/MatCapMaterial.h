#ifndef M3D_MATCAP_MATERIAL_H_
#define M3D_MATCAP_MATERIAL_H_
#include "m3d/graphics/Material.h"

namespace M3D
{
	class Texture;
	/************************************************************************/
	/* matcap���ʡ���phong�����е�matcao��ͼ�����ó�����Ϊһ�ֲ���������ʹ��                                                                     */
	/************************************************************************/
	class M3D_API MatCapMaterial :
		public Material
	{
	public:
		MatCapMaterial();
		~MatCapMaterial();
	//	Texture* MatCapMap() const;
	//	void MatCapMap(Texture* val);		
	//	Texture* NormalMap() const;
	//	void NormalMap(Texture* val);	
	//	Color& AlbedoColor();
	//	void AlbedoColor(Color val);
	//	virtual string Define() const;
	//	virtual void Define(string val);	
	//	Vector2& NormalMapScale();
	//	void NormalMapScale(Vector2 val);		
	//	float Opacity() const;
	//	void Opacity(float val);
	//private:

	//	Texture* m_albedoMap;//!<����ɫ��ͼ

	//	Texture* m_matCapMap;//!<������ͼ 

	//	Texture* m_normalMap;
	//	Color m_albedoColor;//!<albedo color

	//	Vector2 m_normalMapScale;
	//	float m_opacity;
	protected:
		void Init();
	};
}
#endif//M3D_MATCAP_MATERIAL_H_

